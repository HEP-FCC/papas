//
//  ParticleData.cpp
//  papas
//
//  Created by Alice Robson on 11/05/16.
//
//


#include "ParticleData.h"
namespace papas {
  
const double ParticleData::m_e = 0.000511;
const double ParticleData::m_mu = 0.105;
const double ParticleData::m_pi = 0.139;
const double ParticleData::m_K0 = 0.498;
const double ParticleData::m_n = 1.;
const double ParticleData::m_p = 1.;

std::unordered_map<int, std::pair<double, int>> ParticleData::m_datamap =  {
  {11,  {ParticleData::m_e,   1}},
  {-11, {ParticleData::m_e,  -1}},
  {13,  {ParticleData::m_mu,  1}},
  {-13, {ParticleData::m_mu, -1}},
  {22,  {0,                   0}},
  {130, {ParticleData::m_K0,  0}},
  {211, {ParticleData::m_pi,  1}},
  {-211,{ParticleData::m_pi, -1}}
}  ;
  
}//end namesapce