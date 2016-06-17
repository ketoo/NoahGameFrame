file(GLOB NFMessageDefine_ROOT_Cpp *.cpp)
file(GLOB NFMessageDefine_ROOT_CC *.cc)
file(GLOB NFMessageDefine_ROOT_Hpp *.h)

add_library(NFMessageDefine SHARED
	${NFMessageDefine_ROOT_Cpp}
	${NFMessageDefine_ROOT_CC}
	${NFMessageDefine_ROOT_Hpp})
	
set_target_properties( NFMessageDefine PROPERTIES OUTPUT_NAME_DEBUG "NFMessageDefine_d")
set_target_properties( NFMessageDefine PROPERTIES
	FOLDER "NFSDK"
	ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/_Out/Comm/
	RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/_Out/Comm/
	LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/_Out/Comm/ )
target_link_libraries(NFMessageDefine anet_win64 libprotobuf lua Theron)

add_definitions(
-D_USRDLL
-D_LIB
-DPROTOBUF_USE_DLLS
-DLIBPROTOC_EXPORTS
)
