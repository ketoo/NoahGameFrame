set(ProjectName "miniweb")
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
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



if(UNIX)
    set(miniwebFile2
        ./libctb/src/fifo.cpp
        ./libctb/src/getopt.cpp
        ./libctb/src/iobase.cpp
        ./libctb/src/kbhit.cpp
        ./libctb/src/portscan.cpp
        ./libctb/src/serportx.cpp
        )
        add_executable(miniweb ${miniwebFile} ${miniwebFile2})
else()
	add_definitions( -DWIN32 -D_DEBUG -D_CONSOLE -DHTTP_DEBUG -D_CRT_SECURE_NO_DEPRECATE)
    add_executable(miniweb ${miniwebFile})
endif()
target_link_libraries( miniweb Threads::Threads)
set_target_properties( miniweb PROPERTIES
	FOLDER "NFTools"
	ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${SolutionDir}/Tool
	RUNTIME_OUTPUT_DIRECTORY_DEBUG ${SolutionDir}/Tool
	LIBRARY_OUTPUT_DIRECTORY_DEBUG ${SolutionDir}/Tool
	ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${SolutionDir}/Tool
	RUNTIME_OUTPUT_DIRECTORY_RELEASE ${SolutionDir}/Tool
	LIBRARY_OUTPUT_DIRECTORY_RELEASE ${SolutionDir}/Tool )
