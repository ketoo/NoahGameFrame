﻿//-----------------------------------------------------------------------
// <copyright file="NFConfig.cs">
//     Copyright (C) 2015-2015 lvsheng.huang <https://github.com/ketoo/NFrame>
// </copyright>
//-----------------------------------------------------------------------
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
using System.Runtime.InteropServices;
using UnityEngine;

public class NFConfig
{
    private XmlDocument xmldoc = null;
    private XmlNode root = null;

    private int mnServerID = 0;
    private ArrayList aWorldList = null;
    private int mnCloneID = 0;
    private int mnCloneMinTime = 0;
    private int mnCloneMaxTime = 0;
    private int mnWalkMinTime = 0;
    private int mnWalkMaxTime = 0;
    private int mnCityID = 0;
    private string[] mstrArray = new string[100];

    private int mnQuitMinTime = 0;
    private int mnQuitMaxTime = 0;
    private int mnMonsterCount = 0;

    private string mnChatData = null;
    private int mnChatMinTime = 0;
    private int mnChatMaxTime = 0;
    private String mstrRootPath = null;


    public void Load()
    {
        //TextAsset textAsset = (TextAsset)Resources.Load("Config", typeof(TextAsset));
        string textAsset = Resources.Load("Config").ToString();
        xmldoc = new XmlDocument();

        xmldoc.LoadXml(textAsset);
        root = xmldoc.SelectSingleNode("XML");



        //xmldoc = new XmlDocument();
        //xmldoc.Load("Config.xml");
        //root = xmldoc.SelectSingleNode("XML");
    }

    public ArrayList GetServerList()
    {
        if (null == aWorldList)
        {
            aWorldList = new ArrayList();

            XmlNode node = root.SelectSingleNode("Servers");

            XmlNodeList nodeList = node.SelectNodes("Server");
            for (int i = 0; i < nodeList.Count; ++i)
            {
                XmlNode nodeServer = nodeList.Item(i);
                XmlAttribute strIP = nodeServer.Attributes["IP"];
                XmlAttribute strPort = nodeServer.Attributes["Port"];

                string strServer = strIP.Value + ":" + strPort.Value;
                aWorldList.Add(strServer);
            }
        }

        return aWorldList;
    }

    public bool GetSelectServer(ref string strIP, ref int nPort)
    {
        ArrayList serverList = GetServerList();
        if (null != serverList && serverList.Count > 0)
        {
            string strData = (string)serverList[0];
            string[] sArray = strData.Split(':');
            if (sArray.Length == 2)
            {
                strIP = sArray[0];
                nPort = int.Parse(sArray[1]);

                return true;
            }
        }

        return false;
    }

    public string GetDataPath()
    {
        XmlNode node = root.SelectSingleNode("Evironment");

        XmlNode nodeDataPath = node.SelectSingleNode("DataPath");
        return nodeDataPath.Attributes["ID"].Value;
    }
}