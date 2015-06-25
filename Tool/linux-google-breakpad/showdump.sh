ExeName="NFServer_d"
DumpName="36929c83-a6ba-ad53-59e61da1-xxxxxx.dmp"

echo "Server: ${ExeName}, DumpName:${DumpName}"

./dump_syms ./${ExeName} > ${ExeName}.sym
#head -n1 ${ExeName}.sym | awk '{print $4}' > $DumpNO

DumpNO=`head -n1 ${ExeName}.sym | awk '{print $4}'`

rm ./symbols -rf

mkdir -p ./symbols/${ExeName}/${DumpNO}

mv ${ExeName}.sym ./symbols/${ExeName}/${DumpNO}

./minidump_stackwalk ${DumpName} ./symbols

echo "Server: ${ExeName}, DumpName:${DumpName}, DumpNo:${DumpNO}"