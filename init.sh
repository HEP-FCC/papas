
platform='unknown'
sw_afs=0
unamestr=`uname`

export LCGPATH=/afs/cern.ch/sw/lcg/views/LCG_83/x86_64-slc6-gcc49-opt
if [ -z ${FCCPAPAS+x} ]; then
    export FCCPAPAS=$PWD/install
    echo "FCCPAPAS is unset, setting to $FCCPAPAS";
fi

if [[ "$unamestr" == 'Linux' ]]; then
    platform='Linux'
    if [[ -d /afs/cern.ch/sw/lcg ]] && [[ `dnsdomainname` = 'cern.ch' ]] ; then
	#should check domain to make sure we're at CERN
	#or is this software available somewhere in Lyon? 
	sw_afs=1
        source $LCGPATH/setup.sh
        if [ -z "$PODIO" ]; then
            export PODIO=/afs/cern.ch/exp/fcc/sw/0.7/podio/0.4/x86_64-slc6-gcc49-opt/
            export FCCEDM=/afs/cern.ch/exp/fcc/sw/0.7/fcc-edm/0.4/x86_64-slc6-gcc49-opt/
        fi
	export PYTHIA8_DIR=/afs/cern.ch/exp/fcc/sw/0.6/LCG_80/MCGenerators/pythia8/212/x86_64-slc6-gcc49-opt
	export HEPMC_PREFIX=$LCGPATH

	echo software taken from /afs/cern.ch/sw/lcg
    fi
    export LD_LIBRARY_PATH=$PYTHIA8_DIR/lib:$FCCPAPAS/lib:$FCCEDM/lib:$PODIO/lib:$HEPMC_PREFIX/lib:$FASTJET_ROOT_DIR/lib:$LD_LIBRARY_PATH
elif [[ "$unamestr" == 'Darwin' ]]; then
    platform='Darwin'
    export DYLD_LIBRARY_PATH=$FCCPAPAS/lib:$HEPMC_PREFIX/lib:$FASTJET_ROOT_DIR/lib:$PYTHIA8_DIR/lib:$DYLD_LIBRARY_PATH
fi
echo platform detected: $platform
export ROOT_INCLUDE_PATH=$ROOT_INCLUDE_PATH:$FCCEDM/include/datamodel
export PATH=$FCCPAPAS/bin:$FASTJET_ROOT_DIR/bin:$PATH
export PYTHIA8DATA=${PYTHIA8_DIR}/share/Pythia8/xmldoc
export PYTHONPATH=$PODIO/python:$PYTHONPATH
