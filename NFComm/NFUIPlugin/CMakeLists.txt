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

file(GLOB KHR_ROOT_Cpp
	./KHR/*.cpp)
	
file(GLOB KHR_ROOT_Hpp 
	./KHR/*.h)

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
	${KHR_ROOT_Cpp}
	${KHR_ROOT_Hpp}
	${IMGUI_ROOT_Cpp}
	${IMGUI_ROOT_Hpp}
	${GLAD_ROOT_Cpp}
	${GLAD_ROOT_Hpp})
	
set_target_properties( NFUIPlugin PROPERTIES OUTPUT_NAME_DEBUG "NFUIPlugin")
set_target_properties( NFUIPlugin PROPERTIES PREFIX "")
set_target_properties( NFUIPlugin PROPERTIES 
	FOLDER "NFMidWare/NFUIPlugin"
	ARCHIVE_OUTPUT_DIRECTORY ${NFOutPutDir}
	RUNTIME_OUTPUT_DIRECTORY ${NFOutPutDir}
	LIBRARY_OUTPUT_DIRECTORY ${NFOutPutDir} )
link_NFSDK("NFUIPlugin")
if(UNIX)
	if (CMAKE_BUILD_TYPE MATCHES "Release")
		target_link_libraries(NFUIPlugin libSDL2)
	else()
		target_link_libraries(NFUIPlugin libSDL2d)
	endif()

	target_link_libraries(NFUIPlugin -static-libstdc++)

	add_definitions(
		-D_USRDLL
		-DELPP_NO_DEFAULT_LOG_FILE
	)
	
else()
	if (CMAKE_BUILD_TYPE MATCHES "Release")
		target_link_libraries(NFUIPlugin libSDL2)
	else()
		target_link_libraries(NFUIPlugin libSDL2d)
	endif()

	target_link_libraries(NFUIPlugin -static-libstdc++)

	add_definitions(
		-DWIN
		-D_USRDLL
		-DELPP_NO_DEFAULT_LOG_FILE
	)
endif()
