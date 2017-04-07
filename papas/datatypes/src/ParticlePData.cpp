#include "papas/datatypes/ParticlePData.h"

namespace papas {

const double ParticlePData::m_e = 0.000511;
const double ParticlePData::m_mu = 0.105;
const double ParticlePData::m_pi = 0.139;
const double ParticlePData::m_K0 = 0.498;
const double ParticlePData::m_n = 1.;
const double ParticlePData::m_p = 1.;

std::unordered_map<unsigned int, std::pair<double, unsigned int>> ParticlePData::m_datamap = {
    {11, {ParticlePData::m_e, 1}},
    {-11, {ParticlePData::m_e, -1}},
    {13, {ParticlePData::m_mu, 1}},
    {-13, {ParticlePData::m_mu, -1}},
    {22, {0, 0}},
    {130, {ParticlePData::m_K0, 0}},
    {211, {ParticlePData::m_pi, 1}},
    {-211, {ParticlePData::m_pi, -1}}};

}  // end namesapce
