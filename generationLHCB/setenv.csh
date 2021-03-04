source /cvmfs/sft.cern.ch/lcg/views/LCG_99/x86_64-centos7-gcc10-opt/setup.csh
setenv MYPWD `pwd`
cd /cvmfs/geant4.cern.ch/geant4/10.7.ref02/x86_64-centos7-gcc10-optdeb/bin; source geant4.csh
cd $MYPWD
unsetenv MYPWD

