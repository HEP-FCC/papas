//
//  Created by Alice Robson on 09/11/15.
//
//
#include "GBlob.h"
#include "Cluster.h"
#include <memory>

namespace papas {

GBlob::GBlob(const Cluster& cluster) {
  TVector3 pos = cluster.position();
  double radius = cluster.size();
  double thetaphiradius = cluster.angularSize();
  double max_energy = Cluster::s_maxEnergy;
  double iradius = radius * cluster.energy() / max_energy;
  double ithetaphiradius = thetaphiradius * cluster.energy() / max_energy;

  int color = 1;

  m_contours["xy"] = std::unique_ptr<TEllipse>{new TEllipse(pos.X(), pos.Y(), radius)};
  m_inners["xy"] = std::unique_ptr<TEllipse>{new TEllipse(pos.X(), pos.Y(), iradius)};
  m_contours["yz"] = std::unique_ptr<TEllipse>{new TEllipse(pos.Z(), pos.Y(), radius)};
  m_inners["yz"] = std::unique_ptr<TEllipse>{new TEllipse(pos.Z(), pos.Y(), iradius)};
  m_contours["xz"] = std::unique_ptr<TEllipse>{new TEllipse(pos.Z(), pos.X(), radius)};
  m_inners["xz"] = std::unique_ptr<TEllipse>{new TEllipse(pos.Z(), pos.X(), iradius)};
  m_contours["ECAL_thetaphi"] =
      std::unique_ptr<TEllipse>{new TEllipse(M_PI_2 - pos.Theta(), pos.Phi(), thetaphiradius)};
  m_inners["ECAL_thetaphi"] = std::unique_ptr<TEllipse>{new TEllipse(M_PI_2 - pos.Theta(), pos.Phi(), ithetaphiradius)};
  m_contours["HCAL_thetaphi"] =
      std::unique_ptr<TEllipse>{new TEllipse(M_PI_2 - pos.Theta(), pos.Phi(), thetaphiradius)};
  m_inners["HCAL_thetaphi"] = std::unique_ptr<TEllipse>{new TEllipse(M_PI_2 - pos.Theta(), pos.Phi(), ithetaphiradius)};

  // Loop thorugh inners and outers applying settings
  for (auto const& contour : m_contours) {
    contour.second->SetLineColor(4);
    contour.second->SetFillStyle(0);
  }
  for (auto const& inner : m_inners) {
    inner.second->SetFillColor(color);
    inner.second->SetFillStyle(3002);
  }
}

void GBlob::Draw(const std::string& projection, const std::string& opt) const {

  // some things are not sensible so skip these
  if ((m_layer == papas::Layer::kEcal && projection == "HCAL_thetaphi") ||
      (m_layer == papas::Layer::kHcal && projection == "ECAL_thetaphi"))
    return;

  std::string useopt = opt + "psame";

  // find the approprite projection and plot it
  if (m_contours.find(projection) != m_contours.end()) {
    m_contours.at(projection)->Draw(useopt.c_str());  // "at" instead of  []; otherwise fails because of const ness
  }

  // TODOAJR else
  // raise ValueError('implement drawing for projection ' + projection )
  if (m_inners.find(projection) != m_inners.end()) {
    m_inners.at(projection)->Draw(useopt.c_str());
  }
}

}  // end namespace papas
