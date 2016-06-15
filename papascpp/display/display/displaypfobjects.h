//
//  Created by Alice Robson on 29/11/15.
//
//
#ifndef DISPLAY_DISPLAYPFOBJECTS_H
#define DISPLAY_DISPLAYPFOBJECTS_H

#include <unordered_map>
#include <string>
#include <utility>
#include "TPolyLine.h"
#include "TGraph.h"
#include "TArc.h"
#include "TEllipse.h"
#include "TColor.h"
#include "TVector3.h"
#include "displaygeometry.h"
#include "Path.h"

namespace papas {

//class Cluster;
class PFParticle;

///Used to display Clusters on output graphs
class GBlob  {
public:
   GBlob(const Cluster& cluster);
   void Draw(const std::string& projection, const std::string& opt = "") const ;
private:
  
   std::string m_layer;  ///<which layer to plot at
   //outside circle of cluster
   std::unordered_map< std::string, std::unique_ptr<TEllipse>> m_contours;
   //inside circle of cluster
   std::unordered_map< std::string, std::unique_ptr<TEllipse>> m_inners;
};


///Used to display a track on output graphs ///AJRTODO implement helix
class GTrajectory {
public:
   GTrajectory(const Path::Points& points, TVector3 tvec, int linestyle = 1, int linecolor = 1);
  //GTrajectory(const PFParticle& particle, int linestyle = 1, int linecolor = 1);
  // GTrajectory(const Track& track, int linestyle = 1, int linecolor = 1);
   void setColor(int color);
   void Draw(const std::string& projection /*, const std::string& opt = ""*/) const ;
private:
  void initGraphs(const  std::vector<double>& X, const  std::vector<double>& Y,
             const  std::vector<double>& Z,const  std::vector<double>& tX,
             const  std::vector<double>& tY, int linestyle,
                  int linecolor);
   bool m_DrawSmearedClusters ;
   ///the tracks to be plotted
   std::unordered_map<std::string, std::unique_ptr<TGraph>> m_graphs;
};
  /*
  class GStraightTrajectory: public GTrajectory {
public:
  def __init__(self, description):
  super(GStraightTrajectory, self).__init__(description,
                                            linestyle=2, linecolor=1)
  
  def draw(self, projection):
  super(GStraightTrajectory, self).draw(projection, 'l')
    }*/


///Used to display the tracks and clusters on output graphs
class GTrajectories : public Drawable  {
public:
   /// Initial implementation for a line (should accept list of particles eventually (or equiv))
   GTrajectories(const std::vector<TVector3>& points);
   /// Initial implementation for a cluster (should accept list of particles eventually (or equiv))
   GTrajectories(const Cluster& cluster);
   GTrajectories(const Track& track);
   GTrajectories(const PFParticle& particle);
   void Draw(const std::string& projection) const override;
private:
   ///all the tracks
   std::list<GTrajectory> m_gTrajectories;
   ///All the clusters
   std::list<GBlob> m_gBlobs;
};

} // end namespace papas




#endif
