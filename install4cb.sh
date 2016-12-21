# compile dep libraries
cd ./Dependencies
chmod -R 755 build_dep.sh
./build_dep.sh

cd ..
# generate makefile
chmod -R 755 GenerateConfigXML.sh
./GenerateConfigXML.sh

#chmod -R 755 convertvs2cb.sh
#./convertvs2cb.sh
