file(GLOB NFFileProcess_MiniExcelReader_Cpp ./MiniExcelReader/*.cpp)
file(GLOB NFFileProcess_MiniExcelReader_Hpp ./MiniExcelReader/*.h)
file(GLOB NFFileProcess_minizip_C ./minizip/ioapi.c ./minizip/ioapi_mem.c ./minizip/unzip.c)
file(GLOB NFFileProcess_minizip_Hpp ./minizip/*.h)
file(GLOB NFFileProcess_zlib_C ./zlib/*.c)
file(GLOB NFFileProcess_zlib_Hpp ./zlib/*.h)
file(GLOB NFFileProcess_ROOT_Cpp *.cpp)
file(GLOB NFFileProcess_ROOT_Hpp *.h)
file(GLOB NFFileProcess_zlib_Hpp ./Generator/*.hpp)

source_group("MiniExcelReader" FILES ${NFFileProcess_MiniExcelReader_Cpp})
source_group("MiniExcelReader" FILES ${NFFileProcess_MiniExcelReader_Hpp})

source_group("minizip" FILES ${NFFileProcess_minizip_C})
source_group("minizip" FILES ${NFFileProcess_minizip_Hpp})

source_group("tinyxml2" FILES ${NFFileProcess_tinyxml2_Cpp})
source_group("tinyxml2" FILES ${NFFileProcess_tinyxml2_Hpp})

source_group("zlib" FILES ${NFFileProcess_zlib_C})
source_group("zlib" FILES ${NFFileProcess_zlib_Hpp})

add_executable(NFFileProcess
	${NFFileProcess_MiniExcelReader_Cpp} 
	${NFFileProcess_MiniExcelReader_Hpp} 
	${NFFileProcess_minizip_C} 
	${NFFileProcess_minizip_Hpp}
	${NFFileProcess_tinyxml2_Cpp} 
	${NFFileProcess_tinyxml2_Hpp}
	${NFFileProcess_zlib_C} 
	${NFFileProcess_zlib_Hpp}
	${NFFileProcess_ROOT_Cpp}
	${NFFileProcess_ROOT_Hpp})

# NOTE: include_directories 转 target_include_directories
target_include_directories(NFFileProcess PRIVATE
	minizip
	tinyxml2
	zlib
	MiniExcelReader)
	
set_target_properties( NFFileProcess PROPERTIES
	FOLDER "NFTools"
	ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${SolutionDir}/_Out/NFDataCfg/Tool/
	RUNTIME_OUTPUT_DIRECTORY_DEBUG ${SolutionDir}/_Out/NFDataCfg/Tool/
	LIBRARY_OUTPUT_DIRECTORY_DEBUG ${SolutionDir}/_Out/NFDataCfg/Tool/
	ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${SolutionDir}/_Out/NFDataCfg/Tool/
	RUNTIME_OUTPUT_DIRECTORY_RELEASE ${SolutionDir}/_Out/NFDataCfg/Tool/
	LIBRARY_OUTPUT_DIRECTORY_RELEASE ${SolutionDir}/_Out/NFDataCfg/Tool/	)

# NOTE: add_definitions 转 target_compile_definitions
target_compile_definitions(NFFileProcess PRIVATE _CRT_SECURE_NO_WARNINGS _CRT_NONSTDC_NO_DEPRECATE)