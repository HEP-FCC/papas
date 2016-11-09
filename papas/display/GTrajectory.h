//
//  Created by Alice Robson on 29/11/15.
//
//
#ifndef PAPAS_GTRAJECTORY_H
#define PAPAS_GTRAJECTORY_H

#include <string>
#include <unordered_map>
#include <utility>

#include "TGraph.h"

namespace papas {

/// Used to display a track on output graphs
class GTrajectory {
public:
  GTrajectory(const std::vector<double>& X, const std::vector<double>& Y, const std::vector<double>& Z,
              const std::vector<double>& tX, const std::vector<double>& tY, int markerstyle, double markersize,
              int linestyle, int linecolor, int linewidth);
  void setColor(int color);
  void Draw(const std::string& projection) const;

private:
  std::unordered_map<std::string, std::unique_ptr<TGraph>> m_graphs;  ///< the line/curves for a track on each proj
};

}  // end namespace papas

#endif
