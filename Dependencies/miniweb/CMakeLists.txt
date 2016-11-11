set(ProjectName "miniweb")
set(miniwebFile
    ./http.c
    ./httpauth.c
    ./httphandler.c
    ./httppil.c
    ./httppost.c
    ./httpxml.c
    ./miniweb.c
    ./processpil.c
    ./httpapi.h
    ./httpint.h
    ./httppil.h
    ./httpvod.h
    ./httpxml.h
    ./processpil.h
    ./revision.h
)

set(SolutionDir ${PROJECT_SOURCE_DIR})
include_directories(${SolutionDir}/Dependencies/miniweb/libctb/include/)

add_executable(miniweb ${miniwebFile})

if(UNIX)

else()
	add_definitions( -DWIN32 -D_DEBUG -D_CONSOLE -DHTTP_DEBUG -D_CRT_SECURE_NO_DEPRECATE)
endif()

set_target_properties( miniweb PROPERTIES
	FOLDER "NFTools"
	ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${SolutionDir}/Tool
	RUNTIME_OUTPUT_DIRECTORY_DEBUG ${SolutionDir}/Tool
	LIBRARY_OUTPUT_DIRECTORY_DEBUG ${SolutionDir}/Tool
	ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${SolutionDir}/Tool
	RUNTIME_OUTPUT_DIRECTORY_RELEASE ${SolutionDir}/Tool
	LIBRARY_OUTPUT_DIRECTORY_RELEASE ${SolutionDir}/Tool )
