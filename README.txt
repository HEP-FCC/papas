

Installation of PAPAS

Instructions still under development.

(1) Download from github


Lxplus:

(2) Need to set up EDM/PODIO 
      
      source ./init_stack_fcc



(3) Build it
		mkdir build
		cd build
		cmake ../
		cmake -DCMAKE_INSTALL_PREFIX=../install ../
		make -j 4
		make install

(4) Run example
   		./mainexe "filepath/filename of pythia.root file"


For Mac Xcode project use: 

	mkdir xbuild
	cd xbuild
	cmake -G Xcode ../
	make



