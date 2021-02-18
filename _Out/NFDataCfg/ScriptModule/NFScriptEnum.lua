function CreatEnumTable(tbl, index) 
    local enumtbl = {} 
    local enumindex = index or 0 
    for i, v in ipairs(tbl) do 
        enumtbl[v] = enumindex + i 
    end 
    return enumtbl 
end

ObjectEvent =
{
    "COE_CREATE_NODATA",
    "COE_CREATE_BEFORE_ATTACHDATA",
    "COE_CREATE_LOADDATA",
    "COE_CREATE_AFTER_ATTACHDATA",
    "COE_CREATE_BEFORE_EFFECT",
    "COE_CREATE_EFFECTDATA",
    "COE_CREATE_AFTER_EFFECT",
    "COE_CREATE_READY",
    "COE_CREATE_HASDATA",
    "COE_CREATE_FINISH",
    "COE_CREATE_CLIENT_FINISH",
    "COE_BEFOREDESTROY",
    "COE_DESTROY",
}
ObjectEvent = CreatEnumTable(ObjectEvent, -1)

RecordOpType =
{
	"Add",
	"Del",
    "AfterDel",
	"Swap",
	"Create",
	"Update",
	"Cleared",
	"Sort",
	"Cover",
	"UNKNOW",
}
RecordOpType = CreatEnumTable(RecordOpType, -1)

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