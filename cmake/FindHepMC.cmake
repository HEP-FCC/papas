#Author: Kamil Sobol
# This module tries to find the HepMC installation on your system.

MESSAGE(STATUS "Looking for HepMC...")


# try to find HepMC in user defined path
FIND_LIBRARY(HEPMC_LIB
NAMES
HepMC
PATHS
${HEPMC_PREFIX}/lib
)


# if not try to find HepMC in standard instalation paths
IF(${HEPMC_LIB} MATCHES "HEPMC_LIB-NOTFOUND")
FIND_LIBRARY(HEPMC_LIB
NAMES
HepMC
PATHS
/usr/lib
/usr/local/lib
)
ENDIF(${HEPMC_LIB} MATCHES "HEPMC_LIB-NOTFOUND")

IF(NOT ${HEPMC_LIB} MATCHES "HEPMC_LIB-NOTFOUND")
FIND_PATH(HEPMC_INCLUDE
HepMC/GenEvent.h
/usr/include
/usr/local/include
${HEPMC_PREFIX}/include
)
ENDIF(NOT ${HEPMC_LIB} MATCHES "HEPMC_LIB-NOTFOUND")

# final printout.
IF((${HEPMC_LIB} MATCHES "HEPMC_LIB-NOTFOUND") OR (${HEPMC_INCLUDE} MATCHES "HEPMC_INCLUDE-NOTFOUND"))
SET(HEPMC_FOUND FALSE)
MESSAGE( STATUS "\n"
"!!!!! HepMC !!!!!\n"
"!!!!! shared library or includes !!!!!\n"
"!!!!! not found. !!!!!\n"
"!!!!! Event interface to HepMC !!!!!\n"
"!!!!! will not be built !!!!!\n"
"!!!!! If you have it installed !!!!!\n"
"!!!!! in custom localisation please edit !!!!!\n"
"!!!!! config/build.properties file !!!!!")
ELSE((${HEPMC_LIB} MATCHES "HEPMC_LIB-NOTFOUND") OR (${HEPMC_INCLUDE} MATCHES "HEPMC_INCLUDE-NOTFOUND"))
SET(HEPMC_FOUND TRUE)
MESSAGE(STATUS "Looking for HepMC... - found " ${HEPMC_LIB} )
MESSAGE(STATUS "Looking for HepMC... - found " ${HEPMC_INCLUDE} )
ENDIF((${HEPMC_LIB} MATCHES "HEPMC_LIB-NOTFOUND") OR (${HEPMC_INCLUDE} MATCHES "HEPMC_INCLUDE-NOTFOUND"))



