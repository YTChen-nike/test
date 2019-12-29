if [ -d "build"]; then
	rm -rf build
fi
mkdir build 
cd build
cmke .
make
cd ..

