function CreatEnumTable(tbl, index) 
    local enumtbl = {} 
    local enumindex = index or 0 
    for i, v in ipairs(tbl) do 
        enumtbl[v] = enumindex + i 
    end 
    return enumtbl 
end

RecordOptype =
{
	"Add",
	"Del",
	"Swap",
	"Create",
	"Update",
	"Cleared",
	"Sort",
	"Cover",
	"UNKNOW",
}
RecordOptype = CreatEnumTable(RecordOptype, -1)

NF_SERVER_TYPES =
{
    "NF_ST_NONE",
    "NF_ST_REDIS",
    "NF_ST_MYSQL",
    "NF_ST_MASTER",
    "NF_ST_LOGIN",
    "NF_ST_PROXY",
    "NF_ST_GAME",
	"NF_ST_WORLD",
	"NF_ST_DB",
	"NF_ST_MAX",
};
NF_SERVER_TYPES = CreatEnumTable(NF_SERVER_TYPES, -1)