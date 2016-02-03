

 Installation

download from github
the build directory is designed to be separate from the code

src .... contains the GIThub files
build (same level as build)

mkdir build
cd build
cmake ../../src/
make
./mainexe

For Xcode project use: cmake -G Xcode ../../src/

Should install even without Boost but this has not been tested