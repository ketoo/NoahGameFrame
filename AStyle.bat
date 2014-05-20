:: ===================
:: -A1 	ansi 代码风格
:: -s4 	4空格缩进
:: -C  	空各替换tabs
:: -S  	switch-case缩进
:: -w  	预处理缩进
:: -m0 	缩进min-0
:: -M80 缩进max -80
:: -j	if for while 加大括号
:: -H	if for while 后面加空格
:: -p	操作符左右添加空格
:: -U	去掉括号内部空格
:: -D	括号边缘添加空格
:: -Y	代码注释对其
:: -k2 	指针引用符号放中间；k1 靠近 type; k3 靠近 name
:: -W2 	1,2,3同上
:: -n	整理后不生成临时文件
:: 注意，参数均区分大小写!
:: ===================


set ASTYLE_PARAM=-A1 -s4 -c -S -w -m0 -M80 -j -U -H -p -Y -k1 -n
set ASTYLE_DIR=Tool/AStyle/bin/

cd %ASTYLE_DIR%

::a file
::set SRC_FILE=D:/Server/a.cpp
::AStyle %ASTYLE_PARAM% %SRC_FILE%

::a dir
set SRC_DIR=D:/NoahGameFrame/trunk/NFComm
AStyle %ASTYLE_PARAM% -r %SRC_DIR%/*.h %SRC_DIR%/*.cpp

set SRC_DIR=D:/NoahGameFrame/trunk/NFServer
AStyle %ASTYLE_PARAM% -r %SRC_DIR%/*.h %SRC_DIR%/*.cpp

pause