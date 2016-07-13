fcc-edm
=======

The FCC Event Data Model.

The Data Model uses [PODIO](https://github.com/HEP-FCC/podio). On lxplus the FCC PODIO installation is picked up by the `init.sh` script. If you are unable to use this central installation you'll have to compile, install and setup the environment for PODIO.

Prerequisites
--

If you are on lxplus, all the necessary software is preinstalled.

On Mac OS or Ubuntu, you need to install the following software.

### podio

https://github.com/HEP-FCC/podio


Setting up
--

Before compiling or using:

1- make sure that podio's environment is set up

2- set up the fcc-edm environment:

    source ./init.sh


Compilation
--

    mkdir build
    cd build
    cmake -DCMAKE_INSTALL_PREFIX=../install ..
    make -j 4 install
    cd ..


Test
--
Rudimentary tests that do write and read can be run with

    make test


Running
--
Make sure that the ROOT dictionaries are in the LD_LIBRARY_PATH. This is automatically done when you install, otherwise you may need to update the path accordingly.
