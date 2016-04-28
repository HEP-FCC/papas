//
//  Created by Alice Robson on 29/11/15.
//
//
#ifndef  DISPLAY_DISPLAYPFOBJECTS_H
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


class Cluster;
class PFParticle;


///Used to display Clusters on output graphs
class GBlob  {
public:
   GBlob(const Cluster& cluster);
   void Draw(const std::string& projection, const std::string& opt = "") const ;
private:
   ///which layer to plot at
   std::string m_layer;
   //outside circle of cluster
   std::unordered_map< std::string, std::unique_ptr<TEllipse>> m_contours;
   //inside circle of cluster
   std::unordered_map< std::string, std::unique_ptr<TEllipse>> m_inners;
};



///Used to display a track on output graphs ///AJRTODO implement helix
class GTrajectory {
public:
   GTrajectory(const std::vector<TVector3>& points, int linestyle = 1,
               int linecolor = 1);
   GTrajectory(const PFParticle& particle, int linestyle = 1,
               int linecolor = 1);
   GTrajectory(const Track& track, int linestyle = 1, int linecolor = 1);
   void setColor(int color);
   void Draw(const std::string& projection /*, const std::string& opt = ""*/) const ;
private:
   bool m_DrawSmearedClusters ;
   ///the tracks to be plotted
   std::unordered_map<std::string, std::unique_ptr<TGraph>> m_graphs;
};

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






#endif
