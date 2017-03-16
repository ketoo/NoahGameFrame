////////////////////////////////////////////
//  Filename ProtocolManager.h
//  Copyright (C) 2017  Stonexin
//  CreateTime 2017/03/13
//
////////////////////////////////////////////

#include <string>
#include <vector>
#include <memory>

#include <google/protobuf/descriptor.h>
#include <google/protobuf/dynamic_message.h>
#include <google/protobuf/io/zero_copy_stream_impl.h>
#include <google/protobuf/io/tokenizer.h>
#include <google/protobuf/compiler/parser.h>
#include <google/protobuf/descriptor.pb.h>
#include <google/protobuf/compiler/importer.h>

#include "NFComm/NFCore/NFSingleton.hpp"

using namespace std;

using namespace  google::protobuf;
using namespace google::protobuf::compiler;

typedef std::shared_ptr<google::protobuf::Message> MessagePtr;


struct  MsgField
{
	std::string name;	//field name
	int nFieldType;			//the type like the double\int\string\message\bytes
	int label_type;		//optional=1 // required=2 // repeated=3
	std::string strFieldType;
	std::string strLabelType;

};


struct MsgInfo
{
	std::string msg_name;			//msg full name
	std::vector<MsgField> fields;
};

typedef std::shared_ptr<MsgInfo> MsgInfoPtr;

class ProtocolManager : public NFSingleton<ProtocolManager>
{
public:
	ProtocolManager();
	~ProtocolManager();
		
	bool InitProtocol( const std::string& strProtocolDir );

	bool HandleProtocolMsg( const char * buffer, size_t size );

	MessagePtr MakeProtocolMsg( const std::string& proto_full_filename,	const std::string& strMsgName);

	MessagePtr MakeProtocolMsg(const std::string& strMsgName);

	MessagePtr FillProtoMsg(const std::string& proto_full_filename,
		const std::string& strMsgName,
		const std::vector<std::string>& fieldVals);

	MessagePtr FillProtoMsg(const std::string& strMsgName,const std::vector<std::string>& fieldVals);
	
	std::vector<std::string> GetProtocolMessageList( const std::string& proto_full_filename="" );	

	bool GetFieldsFromProtoFile(const std::string& msgFullName, std::vector<MsgField>& fields);
	MsgInfo GetProtoMessageInfo(const std::string& msgFullName);

	//bool InitMsgIDNameMap( const wxString& strXmlFile );
	//bool UpdateMsgIDNameMap();
	//unsigned int GetMsgIDFromName( const std::string& msgName );
private:

	bool MakeInputsBeProtoPathRelative(DiskSourceTree* source_tree, std::string& protoFile);
	
	bool GetDescFromProtoFile(const std::string& proto_full_filename, FileDescriptorProto* file_desc_proto);

	bool GetMsgListFromProtoFile(const std::string& proto_full_filename, std::vector<std::string>& msgNameList);

	bool GetFieldsFromProtoFile(const std::string& proto_full_filename, const std::string& msgName, std::vector<MsgField>& fields);

	wxString GetProtoFullNameFromMsgName(const std::string& strMsgName);
	std::vector<std::string> GetProtocolsFromDir(const std::string& dirName);
	MsgInfo GetProtoMessageInfo(const std::string& proto_full_filename, const std::string& strMsgName);
	bool GetProtoFullFileName(const std::string& protoFn, std::string& proto_full_filename);

private:
	std::map<wxString,std::vector<MsgInfoPtr> >  m_xMessageMap;

	DiskSourceTree	m_DiskSourceTree;
	std::shared_ptr<Importer>		m_pImporter;
	DynamicMessageFactory m_DMsgFactory;

	//std::map<unsigned int, wxString> m_MsgIDNameMap;

	//wxString m_strMsgIDNameMapXmlFile;

	static const char * const PBLabelName[4];
};
