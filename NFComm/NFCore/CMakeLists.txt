file(GLOB NFCore_ROOT_Cpp
	*.cpp)
file(GLOB NFCore_ROOT_Hpp
	*.cc)
file(GLOB NFCore_ROOT_H
	*.h)

add_library(NFCore STATIC ${NFCore_ROOT_Cpp} ${NFCore_ROOT_H} ${NFCore_ROOT_Hpp})

# NOTE: 使用 target_compile_definitions 替代 add_definitions
target_compile_definitions(NFCore PRIVATE NF_NONCLIENT_BUILD)
set_target_properties( NFCore PROPERTIES OUTPUT_NAME_DEBUG "NFCore")
set_target_properties( NFCore PROPERTIES PREFIX "")
set_target_properties( NFCore PROPERTIES 
	FOLDER "NFComm/NFCore"
	ARCHIVE_OUTPUT_DIRECTORY ${NFOutPutDir}
	RUNTIME_OUTPUT_DIRECTORY ${NFOutPutDir}
	LIBRARY_OUTPUT_DIRECTORY ${NFOutPutDir} )

# NOTE: target_link_libraries 添加 PRIVATE 关键字
if(UNIX)
	target_link_libraries(NFCore PRIVATE -static-libstdc++)
else()
	target_link_libraries(NFCore PRIVATE -static-libstdc++)
endif()
