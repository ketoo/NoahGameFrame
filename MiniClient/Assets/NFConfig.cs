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


    public void Load()
    {
        xmldoc = new XmlDocument();

        xmldoc.Load("Config.xml");
        root = xmldoc.SelectSingleNode("XML");
    }

    public int GetServerID()
    {
        if(0 == mnServerID)
        {
            XmlNode node = root.SelectSingleNode("ServerID");
            XmlAttribute id = node.Attributes["ID"];
            mnServerID = int.Parse(id.Value);
        }



        return mnServerID;
    }

    public ArrayList GetServerList()
    {
        if (null == aWorldList)
        {
            aWorldList = new ArrayList();

            XmlNode node = root.SelectSingleNode("Server");

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

    public int GetRobotCloneID()
    {
        if(0 == mnCloneID)
        {
            XmlNode node = root.SelectSingleNode("Robot");
            XmlNode nodeClone = node.SelectSingleNode("CloneScene");
            XmlAttribute strID = nodeClone.Attributes["ID"];
            mnCloneID = int.Parse(strID.Value);

        }

        return mnCloneID;

    }

    public int GetCityID()
    {

        if (0 == mnCityID)
        {
            XmlNode node = root.SelectSingleNode("Robot");
            XmlNode nodeClone = node.SelectSingleNode("City");
            XmlAttribute strID = nodeClone.Attributes["ID"];

            mnCityID = int.Parse(strID.Value);
        }
        return mnCityID;
    }

    public int GetCityCloneMinTime()
    {
        if (0 == mnCloneMinTime)
        {
            XmlNode node = root.SelectSingleNode("Robot");
            XmlNode nodeCity = node.SelectSingleNode("City");
            XmlAttribute strID = nodeCity.Attributes["CloneMinTime"];

            mnCloneMinTime = int.Parse(strID.Value);
        }
        return mnCloneMinTime;

    }

    public int GetCityCloneMaxTime()
    {
        if (0 == mnCloneMaxTime)
        {
            XmlNode node = root.SelectSingleNode("Robot");
            XmlNode nodeCity = node.SelectSingleNode("City");
            XmlAttribute strID = nodeCity.Attributes["CloneMaxTime"];

            mnCloneMaxTime = int.Parse(strID.Value);
        }

        return mnCloneMaxTime;
    }

    public int GetCityWalkMinTime()
    {
        if (0 == mnWalkMinTime)
        {
            XmlNode node = root.SelectSingleNode("Robot");
            XmlNode nodeCity = node.SelectSingleNode("City");
            XmlAttribute strID = nodeCity.Attributes["WalkMinTime"];

            mnWalkMinTime = int.Parse(strID.Value);
        }
        return mnWalkMinTime;
    }

    public void GetCityRndPos(ref float fX, ref float fZ)
    {
        if (mstrArray[0].Length <= 0)
        {
            XmlNode node = root.SelectSingleNode("Robot");
            XmlNode nodeCity = node.SelectSingleNode("City");
            XmlAttribute strID = nodeCity.Attributes["Pos"];
            mstrArray = strID.Value.Split(';');
            for (int i = 0; i < mstrArray.Count(); ++i)
            {
                string strPos = mstrArray[i];
                string[] strPosArray = strPos.Split(',');
                int nPosX = int.Parse(strPosArray[0]);
                int nPosZ = int.Parse(strPosArray[1]);
            }
        }

        System.Random ro = new System.Random();
        string strPosRnd = mstrArray[0];
        string[] strPosArrayRnd = strPosRnd.Split(',');
        fX = float.Parse(strPosArrayRnd[0]);
        fZ = float.Parse(strPosArrayRnd[1]);

        float fRndPosOffestX = ro.Next(-10, 10) / 10.0f;
        float fRndPosOffestZ = ro.Next(-10, 10) / 10.0f;

        fX += fRndPosOffestX;
        fZ += fRndPosOffestZ;

    }

    public int GetCityWalkMaxTime()
    {
        if (0 == mnWalkMaxTime)
        {
            XmlNode node = root.SelectSingleNode("Robot");
            XmlNode nodeCity = node.SelectSingleNode("City");
            XmlAttribute strID = nodeCity.Attributes["WalkMaxTime"];

            mnWalkMaxTime = int.Parse(strID.Value);
        }

        return mnWalkMaxTime;
    }

    public int GetRobotCloneQuitMinTime()
    {
        if (0 == mnQuitMinTime)
        {
            XmlNode node = root.SelectSingleNode("Robot");
            XmlNode nodeClone = node.SelectSingleNode("CloneScene");
            XmlAttribute strID = nodeClone.Attributes["QuitMinTime"];

            mnQuitMinTime = int.Parse(strID.Value);
        }

        return mnQuitMinTime;
    }

    public int GetRobotCloneQuitMaxTime()
    {
        if (0 == mnQuitMaxTime)
        {
            XmlNode node = root.SelectSingleNode("Robot");
            XmlNode nodeClone = node.SelectSingleNode("CloneScene");
            XmlAttribute strID = nodeClone.Attributes["QuitMaxTime"];

            mnQuitMaxTime = int.Parse(strID.Value);
        }

        return mnQuitMaxTime;
    }

    public int GetRobotCloneMonsterCount()
    {
        if (0 == mnMonsterCount)
        {
            XmlNode node = root.SelectSingleNode("Robot");
            XmlNode nodeClone = node.SelectSingleNode("CloneScene");
            XmlAttribute strID = nodeClone.Attributes["MonsterCount"];
            mnMonsterCount = int.Parse(strID.Value);
        }

        return mnMonsterCount;
    }

    public string GetRobotChatData()
    {
        if (null == mnChatData)
        {

            XmlNode node = root.SelectSingleNode("Robot");
            XmlNode nodeChat = node.SelectSingleNode("Chat");
            XmlAttribute strID = nodeChat.Attributes["ID"];

            mnChatData = strID.Value;
        }

        return mnChatData;
    }

    public int GetRobotChatMinTime()
    {
        if (0 == mnChatMinTime)
        {
            XmlNode node = root.SelectSingleNode("Robot");
            XmlNode nodeChat = node.SelectSingleNode("Chat");
            XmlAttribute strID = nodeChat.Attributes["MinTime"];

            mnChatMinTime = int.Parse(strID.Value);
        }

        return mnChatMinTime;
    }

    public int GetRobotChatMaxTime()
    {
        if (0 == mnChatMaxTime)
        {
            XmlNode node = root.SelectSingleNode("Robot");
            XmlNode nodeChat = node.SelectSingleNode("Chat");
            XmlAttribute strID = nodeChat.Attributes["MaxTime"];

            mnChatMaxTime = int.Parse(strID.Value);
        }

        return mnChatMaxTime;
    }
}