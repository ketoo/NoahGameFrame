set(ProjectName "NFPluginLoader")
file(GLOB NFPluginLoader_ROOT_Cpp *.cpp)
file(GLOB NFPluginLoader_ROOT_Hpp *.h)
#Exclude this file
#file(GLOB RemoveItems_Cpp NFCPluginActor.cpp)
#list(REMOVE_ITEM NFPluginLoader_ROOT_Cpp ${RemoveItems_Cpp})

#file(GLOB RemoveItems_Hpp NFCPluginActor.h)
#list(REMOVE_ITEM NFPluginLoader_ROOT_Hpp ${RemoveItems_Hpp})

add_executable(NFPluginLoader ${NFPluginLoader_ROOT_Cpp} ${NFPluginLoader_ROOT_Hpp})
link_NFSDK("NFPluginLoader")
if(UNIX)
	add_dependencies(NFPluginLoader NFCore)
	if (CMAKE_BUILD_TYPE MATCHES "Release")
		target_link_libraries(NFPluginLoader  pthread dl)
	else()
		target_link_libraries(NFPluginLoader  pthread dl)
	endif()
	
	
else()
	add_dependencies(NFPluginLoader NFCore )
	target_link_libraries(NFPluginLoader )
	
endif()

set_target_properties( NFPluginLoader PROPERTIES OUTPUT_NAME_DEBUG "NFPluginLoader_d")
set_target_properties( NFPluginLoader PROPERTIES
	FOLDER "NFSDK"
	ARCHIVE_OUTPUT_DIRECTORY ${NFOutPutDir}
	RUNTIME_OUTPUT_DIRECTORY ${NFOutPutDir}
	LIBRARY_OUTPUT_DIRECTORY ${NFOutPutDir} )

add_definitions(-D_X64 -D_CONSOLE -DNF_DYNAMIC_PLUGIN)