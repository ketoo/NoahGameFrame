Mac�ϱ���boost
���ȱ���Boost.Build
        �޸�sh���ļ�����
         
 chmod +x bootstrap.sh
 chmod +x ./tools/build/v2/engine/build.sh
 
ִ�� bootstrap.sh
./bootstrap.sh

����b2��ִ�г���

����Boost��
//��Clang����boost file sytem
./b2 toolset=clang cxxflags="-arch x86_64" linkflags="-arch x86_64" --with-filesystem      

The Boost C++ Libraries were successfully built!
The following directory should be added to compiler include paths:
    /Users/liwei/DEV/Project/boost_1_50_0
The following directory should be added to linker library paths:
    /Users/liwei/DEV/Project/boost_1_50_0/stage/lib


������ɵ��ļ�������./stage/libĿ¼

 
Ĭ�ϱ����������Release�汾������debug�汾��
./b2 toolset=clang cxxflags="-arch x86_64" linkflags="-arch x86_64" --with-filesystem variant=debug --stagedir=./stage/x64/debug





�ٶ����һ����