# Writing a new Detector for Papas

To implement a new detector it is recommended to copy an existing detector and modify appropriately.
New detectors must implement the interfaces found here:-

https://github.com/HEP-FCC/papas/blob/master/papas/detectors

Examples of the implementation of the CMS and Clic detectors can be found in papas/detectors/CMS and
papas/detectors/Clic

For example for CMS the files are
 * CMS.h / CMS.cpp
 * CMSEcal.h / CMSEcal.cpp
 * CMSHcal.h / CMSHcal.cpp
 * CMSTracker.h / CMSTracker.cpp
 * CMSField.h / CMSField.cpp


## Calorimeters

The detector calorimeter implementations of HCAL and ECAL inherit from the PAPAS Calorimeter class.
They should each implement detector specific versions of

 * clusterSize - minimum detectable cluster size
 * acceptance - whether or not a cluster will be recorded by the calorimeter
 * energyResolution - granularity of the energy detected
 * energyResponse 

For example:
https://github.com/HEP-FCC/papas/blob/master/papas/detectors/cms/CMSEcal.h
https://github.com/HEP-FCC/papas/blob/master/papas/detectors/cms/src/CMSEcal.cpp


## Tracker

This should be defined in a similar way to the two Calorimeters.

The following functions must be provided by the user:

 * ptResolution - momentum detection resolution of the tracker
 * acceptance - whether a track will be detected

eg.

https://github.com/HEP-FCC/papas/blob/master/papas/detectors/cms/CMSTracker.h
https://github.com/HEP-FCC/papas/blob/master/papas/detectors/cms/src/CMSTracker.cpp

## Field

See CMSField.h / CMSField.cpp

Allows the magnitude of the field to be defined (assumed constant). At present no other functions need to be defined.

## Main detector definition

Once the detector elements are written, the detector class can created. The new detector class inherits from the provided PAPAS Detector class. The Detector constructor defines each of the detector elements (ECAL, HCAL, tracker, field). For example


https://github.com/HEP-FCC/papas/blob/master/papas/detectors/cms/CMS.h
https://github.com/HEP-FCC/papas/blob/master/papas/detectors/cms/src/CMS.cpp


## Using the new detector in FCCSW

It is necessary to implement an interface to each detector. The easiest approach is to copy and modify the interfaces for an existing detector, eg

https://github.com/HEP-FCC/FCCSW/tree/master/Sim/SimPapas/src/CMSDetector
https://github.com/HEP-FCC/FCCSW/tree/master/Sim/SimPapas/src/ClicDetector

A python configuration file for a new detector should also be created, examples are

https://github.com/HEP-FCC/FCCSW/tree/master/Sim/SimPapas/python
 


