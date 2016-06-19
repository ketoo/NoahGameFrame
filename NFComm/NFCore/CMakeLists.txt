
file(GLOB TEMP_SRC *.cpp)
# 不要这个文件
file(GLOB RemoveItems Example.cpp)
list(REMOVE_ITEM TEMP_SRC ${RemoveItems})
add_library(NFCore ${TEMP_SRC})
set_target_properties( NFCore PROPERTIES OUTPUT_NAME_DEBUG "NFCore_d")
set_target_properties( NFCore PROPERTIES 
	FOLDER "NFSDK"
	ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/_Out/Comm/
	RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/_Out/Comm/
	LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/_Out/Comm/ )
	
if(UNIX)

else()
	target_link_libraries(NFCore Theron)
endif()

