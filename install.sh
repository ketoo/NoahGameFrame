# compile dep libraries
cd ./Dependencies
chmod -R 755 build_dep.sh
./build_dep.sh

cd ../
pwd

# generate makefile
#chmod -R 755 Build4Makefile.sh
#./Build4Makefile.sh

#pwd
