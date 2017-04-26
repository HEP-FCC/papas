platform='unknown'
sw_afs=0
unamestr=`uname`
source /cvmfs/fcc.cern.ch/sw/0.8/init_fcc_stack.sh $1
#export LCGPATH=/afs/cern.ch/sw/lcg/views/LCG_83/x86_64-slc6-gcc49-opt
#if [ -z ${FCCPAPASCPP+x} ]; then
    unset FCCPAPASCPP
    echo "FCCPAPASCPP is unset, setting to $PWD/install";
    export FCCPAPASCPP=$PWD/install
    export PATH=$FCCPAPASCPP/bin:$PATH    
#fi

if [[ "$unamestr" == 'Linux' ]]; then
    platform='Linux'
    export LD_LIBRARY_PATH=$FCCPAPASCPP/lib:$LD_LIBRARY_PATH
elif [[ "$unamestr" == 'Darwin' ]]; then
    platform='Darwin'
    export DYLD_LIBRARY_PATH=$FCCPAPASCPP/lib:$DYLD_LIBRARY_PATH
fi
echo platform detected: $platform
export PYTHONPATH=$FCCPAPASCPP/python:$PYTHONPATH
