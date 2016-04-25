# fastsim_cc

Documentation for FastSim C++

Instructions:

Download from GitHub


To run in Xcode

cd xbuild
cmake -G Xcode ..

Open Xcode: need to set the dyld_library_path manually (at least for now)
Go to target main
drop down to
edit scheme
Set the DYLD_LIBRARY_PATH = /Users/alice/local/lib/root

