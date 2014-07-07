using System;
using System.Collections;
using System.Collections.Generic;
using System.IO;
using System.Net;
using System.Net.Sockets;
using System.Threading;
using System.Linq;
using System.Text;

namespace NFTCPClient
{
    public enum NFTCPClientState
    {
        Connecting,
        Connected,
        Disconnected
    }
	
	public enum NFTCPEventType
    {
        None,
        Connected,
        Disconnected,
        ConnectionRefused,
        DataReceived
    }

    public class NFSocketPacket
    {
        public byte[] bytes = null;
        public int bytesCount = 0;

        public NFSocketPacket(byte[] bytes, int bytesCount)
        {
            this.bytes = bytes;
            this.bytesCount = bytesCount;
        }

    }

    public class NFTCPEventParams
    {
        public NFClientNet client = null;
        public int clientID = 0;
        public TcpClient socket = null;
        public NFTCPEventType eventType = NFTCPEventType.None;
        public string message = "";
        public NFSocketPacket packet = null;

    }
	
    public class NFClientNet
    {
        public NFNet net = null;

        public NFClientNet(NFNet clientNet)
        {
            net = clientNet;
            Init();
        }

        void Init()
        {

            binListener = new NFBinaryClientListener(this);

            mxState = NFTCPClientState.Disconnected;
            mxEvents = new Queue<NFTCPEventType>();
            mxMessages = new Queue<string>();
            mxPackets = new Queue<NFSocketPacket>();
        }
        // MonoBehaviour
        private int bufferSize = 65536;

        private NFTCPClientState mxState;
        private NetworkStream mxStream;
        private StreamWriter mxWriter;
        private StreamReader mxReader;
        private Thread mxReadThread;
        private TcpClient mxClient;
        private Queue<NFTCPEventType> mxEvents;
        private Queue<string> mxMessages;
        private Queue<NFSocketPacket> mxPackets;

        private NFBinaryClientListener binListener;


        public bool IsConnected()
        {
            return mxState == NFTCPClientState.Connected;
        }

        public NFTCPClientState GetState()
        {
            return mxState;
        }

        public NFBinaryClientListener GetBinListener()
        {
            return binListener;
        }

        public void Update()
        {
			
            while (mxEvents.Count > 0)
            {
                lock (mxEvents)
                {
                    NFTCPEventType eventType = mxEvents.Dequeue();

                    NFTCPEventParams eventParams = new NFTCPEventParams();
                    eventParams.eventType = eventType;
                    eventParams.client = this;
                    eventParams.socket = mxClient;

                    if (eventType == NFTCPEventType.Connected)
                    {
                        binListener.OnClientConnect(eventParams);
                    }
                    else if (eventType == NFTCPEventType.Disconnected)
                    {
                        binListener.OnClientDisconnect(eventParams);

                        mxReader.Close();
                        mxWriter.Close();
                        mxClient.Close();

                    }
                    else if (eventType == NFTCPEventType.DataReceived)
                    {
                        lock (mxPackets)
                        {
                            eventParams.packet = mxPackets.Dequeue();
                        
                            binListener.OnDataReceived(eventParams);
                        }
                    }
                    else if (eventType == NFTCPEventType.ConnectionRefused)
                    {

                    }

                }
                
            }

        }

        private void OnApplicationQuit()
        {
            Disconnect();
        }

        // Private

        private void ConnectCallback(IAsyncResult ar)
        {

            try
            {

                TcpClient tcpClient = (TcpClient)ar.AsyncState;
                tcpClient.EndConnect(ar);

                SetTcpClient(tcpClient);

            }
            catch (Exception e)
            {
                lock (mxEvents)
                {
                    mxEvents.Enqueue(NFTCPEventType.ConnectionRefused);
                }

            }

        }

        private void ReadData()
        {
            bool endOfStream = false;

            while (!endOfStream)
            {
               int bytesRead = 0;
               byte[] bytes = new byte[bufferSize];
               try
               {
                   bytesRead = mxStream.Read(bytes, 0, bufferSize);
               }
               catch (Exception e)
               {
                   e.ToString();
               }

               if (bytesRead == 0)
               {

                   endOfStream = true;

               }
               else
               {
                   lock (mxEvents)
                   {

                       mxEvents.Enqueue(NFTCPEventType.DataReceived);
                   }
                   lock (mxPackets)
                   {
                       mxPackets.Enqueue(new NFSocketPacket(bytes, bytesRead));
                   }

               }
            }

            mxState = NFTCPClientState.Disconnected;

            mxClient.Close();
            lock (mxEvents)
            {
                mxEvents.Enqueue(NFTCPEventType.Disconnected);
            }

        }

        // Public
        public void Connect(string hostname, int port)
        {
            if (mxState == NFTCPClientState.Connected)
            {
                return;
            }

            mxState = NFTCPClientState.Connecting;

            mxMessages.Clear();
            mxEvents.Clear();

            mxClient = new TcpClient();

            mxClient.BeginConnect(hostname,
                                 port,
                                 new AsyncCallback(ConnectCallback),
                                 mxClient);

        }

        public void Disconnect()
        {

            mxState = NFTCPClientState.Disconnected;

            try { if (mxReader != null) mxReader.Close(); }
            catch (Exception e) { e.ToString(); }
            try { if (mxWriter != null) mxWriter.Close(); }
            catch (Exception e) { e.ToString(); }
            try { if (mxClient != null) mxClient.Close(); }
            catch (Exception e) { e.ToString(); }

        }

        public void SendBytes(byte[] bytes)
        {
            SendBytes(bytes, 0, bytes.Length);
        }

        public void SendBytes(byte[] bytes, int offset, int size)
        {

            if (!IsConnected())
                return;

            mxStream.Write(bytes, offset, size);
            mxStream.Flush();

        }

        public void SetTcpClient(TcpClient tcpClient)
        {

            mxClient = tcpClient;

            if (mxClient.Connected)
            {

                mxStream = mxClient.GetStream();
                mxReader = new StreamReader(mxStream);
                mxWriter = new StreamWriter(mxStream);

                mxState = NFTCPClientState.Connected;

                mxEvents.Enqueue(NFTCPEventType.Connected);

                mxReadThread = new Thread(ReadData);
                mxReadThread.IsBackground = true;
                mxReadThread.Start();
            }
            else
            {
                mxState = NFTCPClientState.Disconnected;
            }

        }

    }
}