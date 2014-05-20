A little tweek on official redis-cpp-client sources to make it 
possible to build & run on both linux and windows machines.

To build on linux, use the Makefile of the original package.

To build on windows, use the redis_cpp_msvc.sln Visual Studio
solution file which should be found at the same folder of this
readme txt file. Please be informed that you should change both
of "Additional Include Directories" and "Additional Library 
Directories" to point to the proper boost library path on your 
machine.

To run the tests on Windows machine, you need a windows-port of
redis server running on localhost. Prebuilt binaries are avaliable
online: 
    https://github.com/dmajkic/redis/downloads

Click on the 'redis-server.exe' ro run the redis server, after that
run the redis_cpp_tests and everything should work fine.

                                    hklo.tw@gmail.com