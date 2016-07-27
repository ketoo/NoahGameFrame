add_subdirectory(protobuf)
# Extract libevent
message("Extracting libevent...")
if(UNIX)

  execute_process(
	COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/build_dep.sh
	WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
	)
  
else()

  execute_process(
	COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/build_dep.bat
	WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
	)
	
endif()
