using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml;
using System.Collections;
using NFTCPClient;
using System.IO;
using System.Net;
using System.Net.Sockets;
using System.Threading;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;

public class NFConfig
{
    XmlDocument xmldoc = null;
    XmlNode root = null;

    public void Load()
    {
        xmldoc = new XmlDocument();

        xmldoc.Load("Config.xml");
        root = xmldoc.SelectSingleNode("XML");
    }

    public int GetServerID()
    {
        XmlNode node = root.SelectSingleNode("ServerID");
        XmlAttribute id = node.Attributes["ID"];
        return int.Parse(id.Value);
    }

    public ArrayList GetServerList()
    {
        ArrayList aServerList = new ArrayList();

        XmlNode node = root.SelectSingleNode("Server");

        XmlNodeList nodeList = node.SelectNodes("Server");
        for (int i = 0; i < nodeList.Count; ++i )
        {
            XmlNode nodeServer = nodeList.Item(i);
            XmlAttribute strIP = nodeServer.Attributes["IP"];
            XmlAttribute strPort = nodeServer.Attributes["Port"];

            string strServer = strIP.Value + ":" +strPort.Value;
            aServerList.Add(strServer);
        }

        return aServerList;
    }

}