file(GLOB NFUrlPlugin_ROOT_Cpp *.cpp)
file(GLOB NFUrlPlugin_ROOT_Hpp *.h)

remove_file(NFUrlPlugin_ROOT_Cpp "Plugin.cpp")

add_library(NFUrlPlugin SHARED
	${NFUrlPlugin_ROOT_Cpp}
	${NFUrlPlugin_ROOT_Hpp})
	
set_target_properties( NFUrlPlugin PROPERTIES OUTPUT_NAME_DEBUG "NFUrlPlugin_d")
set_target_properties( NFUrlPlugin PROPERTIES
	FOLDER "NFSDK"
	ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/_Out/Comm/
	RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/_Out/Comm/
	LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/_Out/Comm/ )
target_link_libraries(NFUrlPlugin anet_win64 libprotobuf lua Theron NFMessageDefine)

add_definitions("-DWIN -D_USRDLL -DCURL_STATICLIB -DHTTP_ONLY")
