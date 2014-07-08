NoahGameFrame NFDataCfg introduce:
=============
Copyright © http://www.yowoyo.com

We put logic and data config in this dir, support to edit with excel(xls or xlsx), then use Tool/xls2xml.exe, translate xls to xml file, it is recommended not to use Chinese

### Dir introduce:
<p>1.Excel_Struct, excel logic configuration</p>
<p>2.Excel_Ini, excel data configuration</p>
<p>3.Ini, xml data configuration</p>
<p>4.Struct, xml logic configuration</p>
<p>5.Struct/LogicClass.xml, the main logic configuration, every excel in dir Excel_struct can generates a <Class/> element</p>
<p>6.Struct/Class, using Tool/xls2xml.exe will generate xml file for every excel in dir Excel_Struct</p>
<p>7.Ini/NPC, using Tool/xls2xml.exe will generate xml file for every excel in dir Excel_Ini</p>

###How to use xls2xml.exe?
xls2xml.exe is writed by python, because py2exe don't support python3, so we use python.cxfreeze to build exe file
cxxfreeze configuration is in main.py

click xls2xml.exe to generate xml file


#####More info will be added!

