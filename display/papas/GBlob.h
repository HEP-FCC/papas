//
//  Created by Alice Robson on 29/11/15.
//
//
#ifndef PAPAS_GBLOB_H
#define PAPAS_GBLOB_H

#include <string>
#include <unordered_map>
#include <utility>
#include <memory>

#include "TEllipse.h"

namespace papas {

class Cluster;

/// Used to display Clusters on output graphs
class GBlob {
public:
  GBlob(const Cluster& cluster);
  void Draw(const std::string& projection, const std::string& opt = "") const;

private:
  std::string m_layer;  ///<which layer to plot at
  // outside circle of cluster
  std::unordered_map<std::string, std::unique_ptr<TEllipse>> m_contours;
  // inside circle of cluster
  std::unordered_map<std::string, std::unique_ptr<TEllipse>> m_inners;
};

}  // end namespace papas

#endif