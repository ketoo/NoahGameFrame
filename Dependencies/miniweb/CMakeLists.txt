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
	add_definitions( -DNOCONSOLE )
    add_library(miniweb ${miniwebFile} ${miniwebFile2})
else()
	add_definitions( -DWIN32 -D_CONSOLE -D_CRT_SECURE_NO_DEPRECATE -DNOCONSOLE )
    add_library(miniweb ${miniwebFile})
endif()
target_link_libraries( miniweb Threads::Threads)
set_target_properties( miniweb PROPERTIES OUTPUT_NAME_DEBUG "miniweb_d")
set_target_properties( miniweb PROPERTIES
	FOLDER "NFDeps"
	ARCHIVE_OUTPUT_DIRECTORY ${NFOutPutDir}
	RUNTIME_OUTPUT_DIRECTORY ${NFOutPutDir}
	LIBRARY_OUTPUT_DIRECTORY ${NFOutPutDir})
