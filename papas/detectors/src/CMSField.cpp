/**
 * @file CMS.cc
 * @brief Implementation of the CMS detector
 */
#include "papas/detectors/CMSField.h"

namespace papas {

CMSField::CMSField(const VolumeCylinder& volume, double magnitude) : Field(volume, Material("void", 0, 0), magnitude) {}

CMSField::CMSField(const VolumeCylinder&& volume, double magnitude)
    : Field(volume, Material("void", 0, 0), magnitude) {}

}  // end namespace papas
