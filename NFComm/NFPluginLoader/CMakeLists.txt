file(GLOB NFPluginLoader_ROOT_Cpp *.cpp)
file(GLOB NFPluginLoader_ROOT_Hpp *.h)
# 不要这个文件
file(GLOB RemoveItems_Cpp NFCPluginActor.cpp)
list(REMOVE_ITEM NFPluginLoader_ROOT_Cpp ${RemoveItems_Cpp})

file(GLOB RemoveItems_Hpp NFCPluginActor.h)
list(REMOVE_ITEM NFPluginLoader_ROOT_Hpp ${RemoveItems_Hpp})

add_executable(NFPluginLoader ${NFPluginLoader_ROOT_Cpp} ${NFPluginLoader_ROOT_Hpp})

if(UNIX)
	add_dependencies(NFPluginLoader NFCore)
else()
	add_dependencies(NFPluginLoader NFCore Theron)
endif()

set_target_properties( NFPluginLoader PROPERTIES OUTPUT_NAME_DEBUG "NFPluginLoader_d")
set_target_properties( NFPluginLoader PROPERTIES
	FOLDER "NFSDK"
	ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/_Out/Comm/
	RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/_Out/Comm/
	LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/_Out/Comm/ )
target_link_libraries(NFPluginLoader Theron)

add_definitions(-D_X64 -D_CONSOLE -DNF_DYNAMIC_PLUGIN)