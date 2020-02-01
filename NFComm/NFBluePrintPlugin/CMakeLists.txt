set(ProjectName "NFBluePrintPlugin")
file(GLOB NFBluePrintPlugin_ROOT_Cpp
	*.cpp)
	
file(GLOB NFBluePrintPlugin_ROOT_Hpp 
	*.h)

file(GLOB NF_BLUEPRINT_NODE_Cpp ./BluePrintNode/*.cpp)
file(GLOB NF_BLUEPRINT_NODE_Hpp ./BluePrintNode/*.h)


file(GLOB NF_BLUEPRINT_VM_Cpp ./BluePrintVM/*.cpp)
file(GLOB NF_BLUEPRINT_VM_Hpp ./BluePrintVM/*.h)

add_library(NFBluePrintPlugin STATIC
	${NFBluePrintPlugin_ROOT_Cpp}
	${NFBluePrintPlugin_ROOT_Hpp}
	${NF_BLUEPRINT_NODE_Cpp}
	${NF_BLUEPRINT_NODE_Hpp}
	${NF_BLUEPRINT_VM_Cpp}
	${NF_BLUEPRINT_VM_Hpp})
	
set_target_properties( NFBluePrintPlugin PROPERTIES OUTPUT_NAME_DEBUG "NFBluePrintPlugin")
set_target_properties( NFBluePrintPlugin PROPERTIES PREFIX "")
set_target_properties( NFBluePrintPlugin PROPERTIES 
	FOLDER "NFComm/NFBluePrintPlugin"
	ARCHIVE_OUTPUT_DIRECTORY ${NFOutPutDir}
	RUNTIME_OUTPUT_DIRECTORY ${NFOutPutDir}
	LIBRARY_OUTPUT_DIRECTORY ${NFOutPutDir} )
link_NFSDK("NFBluePrintPlugin")
if(UNIX)
	target_include_directories(NFBluePrintPlugin PUBLIC
			${SolutionDir}/Dependencies/googletest-release-1.8.0/googletest/include/)


	target_link_libraries(NFBluePrintPlugin -static-libstdc++)

	add_definitions(
		-DELPP_NO_DEFAULT_LOG_FILE
	)
	
else()

	target_link_libraries(NFBluePrintPlugin -static-libstdc++)
	
	add_definitions(
		-DWIN
		-DELPP_NO_DEFAULT_LOG_FILE
	)
endif()
