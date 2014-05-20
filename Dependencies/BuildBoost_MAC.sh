Mac上编译boost
首先编译Boost.Build
        修改sh的文件属性
         
 chmod +x bootstrap.sh
 chmod +x ./tools/build/v2/engine/build.sh
 
执行 bootstrap.sh
./bootstrap.sh

生成b2可执行程序

编译Boost库
//用Clang编译boost file sytem
./b2 toolset=clang cxxflags="-arch x86_64" linkflags="-arch x86_64" --with-filesystem      

The Boost C++ Libraries were successfully built!
The following directory should be added to compiler include paths:
    /Users/liwei/DEV/Project/boost_1_50_0
The following directory should be added to linker library paths:
    /Users/liwei/DEV/Project/boost_1_50_0/stage/lib


最后生成的文件保存在./stage/lib目录

 
默认编译出来的是Release版本，编译debug版本：
./b2 toolset=clang cxxflags="-arch x86_64" linkflags="-arch x86_64" --with-filesystem variant=debug --stagedir=./stage/x64/debug





速度像飞一样！