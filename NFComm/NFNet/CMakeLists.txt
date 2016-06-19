file(GLOB NFNet_ROOT_Cpp *.cpp)
file(GLOB NFNet_ROOT_Hpp *.h)

# 不要这个文件
file(GLOB RemoveItems_Cpp TestClient.cpp TestServer.cpp)
list(REMOVE_ITEM NFNet_ROOT_Cpp ${RemoveItems_Cpp})

add_library(NFNet
	${NFNet_ROOT_Cpp}
	${NFNet_ROOT_Hpp})
	
set_target_properties( NFNet PROPERTIES OUTPUT_NAME_DEBUG "NFNet_d")
set_target_properties( NFNet PROPERTIES
	FOLDER "NFSDK"
	ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/_Out/Comm/
	RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/_Out/Comm/
	LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/_Out/Comm/ )
if(UNIX)
	
else()
	target_link_libraries(NFNet anet_win64 libprotobuf lua Theron)
endif()


add_definitions("-D_LIB -DWIN32 -DWIN")