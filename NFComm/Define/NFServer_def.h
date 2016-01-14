// -------------------------------------------------------------------------
//    文件名      :    NFServer_def.h
//    创建者      :    LvSheng.Huang
//    创建时间    :    2010-10-5 12:05
//    功能描述    :    
//
// -------------------------------------------------------------------------
#ifndef NFC_SERVERDEF_H
#define NFC_SERVERDEF_H

#pragma pack(push,1)

enum NF_SERVER_TYPES
{
	NF_ST_CLIENT		= 0,    // client
	NF_ST_REDIS			= 1,    //
	NF_ST_MYSQL			= 2,    //
	NF_ST_MASTER		= 3,    //
	NF_ST_LOGIN			= 4,    //
	NF_ST_PROXY			= 5,    //
	NF_ST_GAME			= 6,    //
	NF_ST_WORLD			= 7,    //

};

#pragma pack(pop)

#endif