//
//  particledata.h
//  fastsim
//
//  Created by Alice Robson on 13/01/16.
//
//

#ifndef particledata_h
#define particledata_h

#include <unordered_map>
#include <utility>

namespace papas {
/// @brief Class which provides pdgid, masss and charge data for particles
class ParticlePData {
public:
  static const double m_e;   // = 0.000511;
  static const double m_mu;  //= 0.105;
  static const double m_pi;  //= 0.139;
  static const double m_K0;  //= 0.498;
  static const double m_n;   //= 1.;
  static const double m_p;   //= 1.;

  /** @brief returns the corresponding Mass of particle with specified pdgid
   @param[in] pdgid particle type
  */
  static double particleMass(unsigned int pdgid) { return particleData(pdgid).first; };
  /** @brief returns the corresponding Charge of particle with specified pdgid
   @param[in] pdgid particle type
   */
  static double particleCharge(unsigned int pdgid) { return particleData(pdgid).second; };
  /** @brief returns a pair consisting of the Mass and Charge of a particle with specified pdgid
   @param[in] pdgid particle type
   */
  static const std::pair<double, unsigned int>& particleData(unsigned int pdgid) { return m_datamap[pdgid]; };
  /// Stores the particle id and associated Mass and Charge.
  static std::unordered_map<unsigned int, std::pair<double, unsigned int>> m_datamap;
};
}  // end namespace papas
#endif /* particledata_h */
