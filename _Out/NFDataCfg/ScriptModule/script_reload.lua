io.write("Start reload!\n");
ScriptReloadList={
	{tbl=nil, tblName="TestModule"},
	{tbl=nil, tblName="TestModule2"},
}

reload_script_table(ScriptReloadList)

--clear the data of this file
