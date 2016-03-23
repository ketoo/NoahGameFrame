the Dependencies libs for NF are in folder tars

1、extract 
	curl-7.47.1.zip 					>>>>>>> ..\curl
	gperftools-2.4.zip 					>>>>>>> ..\gperftools
	libevent-2.0.22-stable.tar.gz 		>>>>>>> ..\libevent
	mysql++-3.2.2.tar.gz 				>>>>>>> ..\mysqlpp
	protobuf-2.6.0.tar.gz 				>>>>>>> ..\protobuf
	Theron-6.00.02.zip 					>>>>>>> ..\Theron
	mysql-connector-c-6.1.6-winx64.zip	>>>>>>> "C:\Program Files\MySQL\MySQL Connector C 6.1"

2、modified files
	a、for convenience, we modified the gperftools\vsprojects\libtcmalloc_minimal\libtcmalloc_minimal.vcproj file
   		modified the output lib file name from libtcmalloc_minimal-debug.dll、libtcmalloc_minimal-debug.lib to libtcmalloc_minimal.dll and libtcmalloc_minimal.lib

   	b、Theron.vcxproj add <AdditionalIncludeDirectories>$(ProjectDir)..\Include\;$(ProjectDir)..\Include\External\;</AdditionalIncludeDirectories> for avoid compile error
   	
   	c、remove the sample projects from mysql++ lib for avoid compile samples error

