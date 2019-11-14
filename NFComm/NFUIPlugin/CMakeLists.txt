include_directories(
	./)

set(ProjectName "NFUIPlugin")

file(GLOB NFUIPlugin_ROOT_Cpp
	*.cpp)
	
file(GLOB NFUIPlugin_ROOT_Hpp 
	*.h)

file(GLOB NFUIVIEW_ROOT_Cpp
	./UI/*.cpp)
	
file(GLOB NFUIVIEW_ROOT_Hpp 
	./UI/*.h)

file(GLOB GL_ROOT_Cpp
	./GL/*.cpp)
	
file(GLOB GL_ROOT_Hpp 
	./GL/*.h)

file(GLOB IMGUI_ROOT_Cpp
	./imgui/*.cpp)
	
file(GLOB IMGUI_ROOT_Hpp 
	./imgui/*.h)

file(GLOB GLAD_ROOT_Cpp
	./glad/*.cpp)
	
file(GLOB GLAD_ROOT_Hpp 
	./glad/*.h)

add_library(NFUIPlugin STATIC
	${NFUIPlugin_ROOT_Cpp}
	${NFUIPlugin_ROOT_Hpp}
	${NFUIVIEW_ROOT_Cpp}
	${NFUIVIEW_ROOT_Hpp}
	${GL_ROOT_Cpp}
	${GL_ROOT_Hpp}
	${IMGUI_ROOT_Cpp}
	${IMGUI_ROOT_Hpp})
	
set_target_properties( NFUIPlugin PROPERTIES OUTPUT_NAME_DEBUG "NFUIPlugin")
set_target_properties( NFUIPlugin PROPERTIES PREFIX "")
set_target_properties( NFUIPlugin PROPERTIES 
	FOLDER "NFComm/NFUIPlugin"
	ARCHIVE_OUTPUT_DIRECTORY ${NFOutPutDir}
	RUNTIME_OUTPUT_DIRECTORY ${NFOutPutDir}
	LIBRARY_OUTPUT_DIRECTORY ${NFOutPutDir} )
	
link_NFSDK("NFUIPlugin")

if(UNIX)
	if (CMAKE_BUILD_TYPE MATCHES "Release")
		target_link_libraries(NFUIPlugin libSDL2.a)
	else()
		target_link_libraries(NFUIPlugin libSDL2d.a)
	endif()

	target_link_libraries(NFUIPlugin -static-libstdc++)

	add_definitions(
		-DELPP_NO_DEFAULT_LOG_FILE
	)
	
else()

endif()
