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

        //public ORTCPServer server = null;
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

            _state = NFTCPClientState.Disconnected;
            _events = new Queue<NFTCPEventType>();
            _messages = new Queue<string>();
            _packets = new Queue<NFSocketPacket>();
        }
        // MonoBehaviour

        private string hostname = "localhost";
        private int port = 1998;
        private int bufferSize = 65536;

        private NFTCPClientState _state;
        private NetworkStream _stream;
        private StreamWriter _writer;
        private StreamReader _reader;
        private Thread _readThread;
        private TcpClient _client;
        private Queue<NFTCPEventType> _events;
        private Queue<string> _messages;
        private Queue<NFSocketPacket> _packets;

        private NFBinaryClientListener binListener;


        public bool IsConnected()
        {
            return _state == NFTCPClientState.Connected;
        }

        public NFTCPClientState GetState()
        {
            return _state;
        }

        public NFBinaryClientListener GetBinListener()
        {
            return binListener;
        }

        public void Update()
        {
			
            while (_events.Count > 0)
            {
                lock (_events)
                {
                    NFTCPEventType eventType = _events.Dequeue();

                    NFTCPEventParams eventParams = new NFTCPEventParams();
                    eventParams.eventType = eventType;
                    eventParams.client = this;
                    eventParams.socket = _client;

                    if (eventType == NFTCPEventType.Connected)
                    {
                        binListener.OnClientConnect(eventParams);
                    }
                    else if (eventType == NFTCPEventType.Disconnected)
                    {
                        binListener.OnClientDisconnect(eventParams);

                        _reader.Close();
                        _writer.Close();
                        _client.Close();

                    }
                    else if (eventType == NFTCPEventType.DataReceived)
                    {
                        lock (_packets)
                        {
                            eventParams.packet = _packets.Dequeue();
                        
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
                lock (_events)
                {
                    _events.Enqueue(NFTCPEventType.ConnectionRefused);
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
                   bytesRead = _stream.Read(bytes, 0, bufferSize);
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
                   lock (_events)
                   {

                       _events.Enqueue(NFTCPEventType.DataReceived);
                   }
                   lock (_packets)
                   {
                       _packets.Enqueue(new NFSocketPacket(bytes, bytesRead));
                   }

               }
            }

            _state = NFTCPClientState.Disconnected;

            _client.Close();
            lock (_events)
            {
                _events.Enqueue(NFTCPEventType.Disconnected);
            }

        }

        // Public
        public void Connect(string hostname, int port)
        {
            if (_state == NFTCPClientState.Connected)
            {
                return;
            }

            this.hostname = hostname;
            this.port = port;

            _state = NFTCPClientState.Connecting;

            _messages.Clear();
            _events.Clear();

            _client = new TcpClient();

            _client.BeginConnect(hostname,
                                 port,
                                 new AsyncCallback(ConnectCallback),
                                 _client);

        }

        public void Disconnect()
        {

            _state = NFTCPClientState.Disconnected;

            try { if (_reader != null) _reader.Close(); }
            catch (Exception e) { e.ToString(); }
            try { if (_writer != null) _writer.Close(); }
            catch (Exception e) { e.ToString(); }
            try { if (_client != null) _client.Close(); }
            catch (Exception e) { e.ToString(); }

        }

        public void Send(string message)
        {

            if (!IsConnected())
                return;
            //添加包长在前面4位长度
            int nLen = message.Length;
            //string strLen = nLen.ToString("D4");
            string strLen = nLen.ToString().PadLeft(4, '0');

            StringBuilder strInfo = new StringBuilder();
            strInfo.Append(strLen);
            strInfo.Append(message);

            _writer.WriteLine(strInfo.ToString());
            _writer.Flush();

        }

        public void SendBytes(byte[] bytes)
        {
            SendBytes(bytes, 0, bytes.Length);
        }

        public void SendBytes(byte[] bytes, int offset, int size)
        {

            if (!IsConnected())
                return;

            _stream.Write(bytes, offset, size);
            _stream.Flush();

        }

        public void SetTcpClient(TcpClient tcpClient)
        {

            _client = tcpClient;

            if (_client.Connected)
            {

                _stream = _client.GetStream();
                _reader = new StreamReader(_stream);
                _writer = new StreamWriter(_stream);

                _state = NFTCPClientState.Connected;

                _events.Enqueue(NFTCPEventType.Connected);

                _readThread = new Thread(ReadData);
                _readThread.IsBackground = true;
                _readThread.Start();
            }
            else
            {
                _state = NFTCPClientState.Disconnected;
            }

        }

    }
}