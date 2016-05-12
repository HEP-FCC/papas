//
//  Created by Alice Robson on 09/11/15.
//
//

#include "displaypfobjects.h"
#include <iostream>
#include "Id.h"
#include "Cluster.h"
#include "Track.h"
#include "PFParticle.h"
#include "path.h"
//#include "Id.h"

GBlob::GBlob(const Cluster& cluster)
{
   TVector3 pos = cluster.position();
   double radius = cluster.size();
   double thetaphiradius = cluster.angularSize();
   double max_energy = Cluster::s_maxEnergy;
   double iradius = radius * cluster.energy() / max_energy;
   double ithetaphiradius = thetaphiradius * cluster.energy() / max_energy;

   //set the layer
   //m_layer = Id::layer(cluster.id());

   //set the color according to particle type
   int color = 1;
  
   /*if (cluster.pdgid() == 22 or cluster.pdgid() == 11)
      color = 2;
   else if (cluster.pdgid() > 0)
      color = 4;*/
  
   //AJRTODO implement the other projections
   //
   //TEllipse m_contourYz = TEllipse(pos.Z(), pos.Y(), radius);
   //TEllipse m_contourXz = TEllipse(pos.Z(), pos.X(), radius);
   //TEllipse m_contourThetaphi = TEllipse(math.pi/2. - pos.Theta(), pos.Phi(),
   //                                thetaphiradius);
   //self.inner_xy = TEllipse(pos.X(), pos.Y(), iradius)
   //self.inner_yz = TEllipse(pos.Z(), pos.Y(), iradius)
   //self.inner_xz = TEllipse(pos.Z(), pos.X(), iradius)
   //self.inner_thetaphi = TEllipse(math.pi/2. - pos.Theta(), pos.Phi(),
   //                              ithetaphiradius)
   //inners = [self.inner_xy, self.inner_yz, self.inner_xz,
   //          self.inner_thetaphi]

   m_contours["xy"] = std::unique_ptr<TEllipse> {new TEllipse(pos.X(), pos.Y(), radius)};
   m_inners["xy"] = std::unique_ptr<TEllipse> {new TEllipse(pos.X(), pos.Y(), iradius)};
   m_contours["yz"] = std::unique_ptr<TEllipse> {new TEllipse(pos.Z(), pos.Y(), radius)};
   m_inners["yz"] = std::unique_ptr<TEllipse> {new TEllipse(pos.Z(), pos.Y(), iradius)};
   m_contours["xz"] = std::unique_ptr<TEllipse> {new TEllipse(pos.Z(), pos.X(), radius)};
   m_inners["xz"] = std::unique_ptr<TEllipse> {new TEllipse(pos.Z(), pos.X(), iradius)};
   m_contours["ECAL_thetaphi"] = std::unique_ptr<TEllipse> {new TEllipse(M_PI_2 - pos.Theta(), pos.Phi(), thetaphiradius)};
   m_inners["ECAL_thetaphi"] = std::unique_ptr<TEllipse> {new TEllipse(M_PI_2 - pos.Theta(), pos.Phi(), ithetaphiradius)};
   m_contours["HCAL_thetaphi"] = std::unique_ptr<TEllipse> {new TEllipse(M_PI_2 - pos.Theta(), pos.Phi(), thetaphiradius)};
   m_inners["HCAL_thetaphi"] = std::unique_ptr<TEllipse> {new TEllipse(M_PI_2 - pos.Theta(), pos.Phi(), ithetaphiradius)};
   //                              ithetaphiradius))};

   //Loop thorugh inners and outers applying settings
   for (auto const& contour : m_contours) {
      contour.second->SetLineColor(color);
      contour.second->SetFillStyle(0);
   }
   for (auto const& inner : m_inners) {
      inner.second->SetFillColor(color);
      inner.second->SetFillStyle(3002);
   }
}

void GBlob::Draw(const std::string&   projection, const std::string& opt) const
{

   //some things are not sensible so skip these
  if ( (m_layer == papas::Layer::kEcal && projection == "HCAL_thetaphi") ||
         (m_layer == papas::Layer::kHcal && projection == "ECAL_thetaphi"))
      return;

   std::string useopt = opt + "psame";

   //find the approprite projection and plot it
   if (m_contours.find(projection) != m_contours.end()) {
      m_contours.at(projection)->Draw(
         useopt.c_str()); // "at" instead of  []; otherwise fails because of const ness
   }

   //TODOAJR else
   // raise ValueError('implement drawing for projection ' + projection )
   if (m_inners.find(projection) != m_inners.end()) {
      m_inners.at(projection)->Draw(useopt.c_str());
   }
}
/*if projection == 'xy':
 self.contour_xy.Draw(opt+"psame")
 self.inner_xy.Draw(opt+"psame")
 elif projection == 'yz':
 self.contour_yz.Draw(opt+"psame")
 self.inner_yz.Draw(opt+"psame")
 elif projection == 'xz':
 self.contour_xz.Draw(opt+"psame")
 self.inner_xz.Draw(opt+"psame")
 elif projection == 'ECAL_thetaphi':
 if self.cluster.layer == 'ecal_in':
 self.contour_thetaphi.Draw(opt+"psame")
 self.inner_thetaphi.Draw(opt+"psame")
 elif projection == 'HCAL_thetaphi':
 if self.cluster.layer == 'hcal_in':
 self.contour_thetaphi.Draw(opt+"psame")
 self.inner_thetaphi.Draw(opt+"psame")
 else:
 raise ValueError('implement drawing for projection ' + projection )
 */


GTrajectory::GTrajectory(const  std::vector<TVector3>& points, int linestyle,
                         int linecolor) //AJRTODo generalise argumtnet to be a list of things with points
:  m_DrawSmearedClusters(true)
{
   int npoints = points.size();
   std::vector<double> X;
   std::vector<double> Y;
   std::vector<double> Z;
   std::vector<double> tX; // for thetaphi graphs
   std::vector<double> tY; // for thetaphi graphs
   
   //Extract vectors of x, y and z values
   for (int i = 0 ; i < npoints; i++) {
      X.push_back(points[i].X());
      Y.push_back(points[i].Y());
      Z.push_back(points[i].Z());
      
      //first point is wrong and should be tppoint = description.p4().Vect()
      tX.push_back(M_PI_2 - points[i].Theta());
      tY.push_back(points[i].Phi());
      //std::cout << "X " << X[i] << "Y " << Y[i]<< "Z " << Z[i];
   }
   
   //pass the vectors to the various projections
   m_graphs["xy"] = std::unique_ptr<TGraph> {new TGraph(npoints, &X[0], &Y[0])};
   m_graphs["yz"] = std::unique_ptr<TGraph> {new TGraph(npoints, &Z[0], &Y[0])};
   m_graphs["xz"] = std::unique_ptr<TGraph> {new TGraph(npoints, &Z[0], &X[0])};
   
   m_graphs["ECAL_thetaphi"] = std::unique_ptr<TGraph> {new TGraph(npoints, &tX[0], &tY[0])};
   m_graphs["HCAL_thetaphi"] = std::unique_ptr<TGraph> {new TGraph(npoints, &tX[0], &tY[0])};
   
   
   
   //AJRTODO add in other projections
   
   //set graph styles
   for (auto const& graph : m_graphs) {
      graph.second->SetMarkerStyle(2);
      graph.second->SetMarkerSize(0.7);
      graph.second->SetLineStyle(linestyle);
      graph.second->SetLineColor(linecolor);
   }
}


GTrajectory::GTrajectory(const  PFParticle& particle, int linestyle,
                         int linecolor) //AJRTODo generalise argumtnet to be a list of things with points
{
  const Path::Points& points = particle.path()->points();
   int npoints = points.size();
   std::vector<double> X;
   std::vector<double> Y;
   std::vector<double> Z;
   std::vector<double> tX; // for thetaphi graphs
   std::vector<double> tY; // for thetaphi graphs

   //Extract vectors of x, y and z values
  int i=0;
      for (auto p : points) {
        
         X.push_back(p.second.X());
         Y.push_back(p.second.Y());
         Z.push_back(p.second.Z());
        if (i==1) {
         tX.push_back(M_PI_2 - p.second.Theta());
         tY.push_back(p.second.Phi());
        }
        else
        {
          TVector3 vec = particle.p4().Vect();
          tX.push_back(M_PI_2 - vec.Theta());
          tY.push_back(vec.Phi());

        }
        i += 1;
      //first point is wrong and should be tppoint = description.p4().Vect()
      
        //std::cout << "X " << X[i] << "Y " << Y[i]<< "Z " << Z[i];
   }

   //pass the vectors to the various projections
   m_graphs["xy"] = std::unique_ptr<TGraph> {new TGraph(npoints, &X[0], &Y[0])};
   m_graphs["yz"] = std::unique_ptr<TGraph> {new TGraph(npoints, &Z[0], &Y[0])};
   m_graphs["xz"] = std::unique_ptr<TGraph> {new TGraph(npoints, &Z[0], &X[0])};

   m_graphs["ECAL_thetaphi"] = std::unique_ptr<TGraph> {new TGraph(npoints, &tX[0], &tY[0])};
   m_graphs["HCAL_thetaphi"] = std::unique_ptr<TGraph> {new TGraph(npoints, &tX[0], &tY[0])};



   //AJRTODO add in other projections

   //set graph styles
   for (auto const& graph : m_graphs) {
      graph.second->SetMarkerStyle(2);
      graph.second->SetMarkerSize(0.7);
      graph.second->SetLineStyle(linestyle);
      graph.second->SetLineColor(linecolor);
   }
}

GTrajectory::GTrajectory(const  Track& track, int linestyle,
                         int linecolor) //AJRTODo generalise argumtnet to be a list of things with points
{
  
  //TODO implement a helix curve here somewhere
  const Path::Points& points=track.path()->points();
  int npoints = points.size();
  std::vector<double> X;
  std::vector<double> Y;
  std::vector<double> Z;
  std::vector<double> tX; // for thetaphi graphs
  std::vector<double> tY; // for thetaphi graphs
  
  //Extract vectors of x, y and z values
  int i=0;
  for (auto p : points) {
    
    X.push_back(p.second.X());
    Y.push_back(p.second.Y());
    Z.push_back(p.second.Z());
    if (i==1) {
      tX.push_back(M_PI_2 - p.second.Theta());
      tY.push_back(p.second.Phi());
    }
    else
    {
      TVector3 vec = track.p3();
      tX.push_back(M_PI_2 - vec.Theta());
      tY.push_back(vec.Phi());
      
    }
    i += 1;
    //first point is wrong and should be tppoint = description.p4().Vect()
    
    //std::cout << "X " << X[i] << "Y " << Y[i]<< "Z " << Z[i];
  }
  
  //pass the vectors to the various projections
  m_graphs["xy"] = std::unique_ptr<TGraph> {new TGraph(npoints, &X[0], &Y[0])};
  m_graphs["yz"] = std::unique_ptr<TGraph> {new TGraph(npoints, &Z[0], &Y[0])};
  m_graphs["xz"] = std::unique_ptr<TGraph> {new TGraph(npoints, &Z[0], &X[0])};
  
  m_graphs["ECAL_thetaphi"] = std::unique_ptr<TGraph> {new TGraph(npoints, &tX[0], &tY[0])};
  m_graphs["HCAL_thetaphi"] = std::unique_ptr<TGraph> {new TGraph(npoints, &tX[0], &tY[0])};
  
  
  
  //AJRTODO add in other projections
  
  //set graph styles
  for (auto const& graph : m_graphs) {
    graph.second->SetMarkerStyle(2);
    graph.second->SetMarkerSize(0.7);
    graph.second->SetLineStyle(linestyle);
    graph.second->SetLineColor(linecolor);
  }
}



void GTrajectory::setColor(int color)
{
   for (auto const& graph : m_graphs) {
      graph.second->SetMarkerColor(color);
   }
}

void GTrajectory::Draw(const std::string&   projection /*,
                       const std::string&  opt*/) const
{
   std::string usedopt = "lpsame"; //AJRTODO

   m_graphs.at(projection) ->Draw(
      usedopt.c_str()); // "at" instead of  [] otherwise fails because of const ness

   // raise ValueError('implement drawing for projection ' + projection )

};


///Constructor for showing tracks
GTrajectories::GTrajectories(const std::vector<TVector3>& points)
//AJRTODO const std::list<Particle>& particles)
{
   //TrajClass = GTrajectory ; //AJRTODO GStraightTrajectoryif is_neutral else GHelixTrajectory
   m_gTrajectories.push_back(GTrajectory(points));
}

GTrajectories::GTrajectories(const PFParticle& particle)
//AJRTODO const std::list<Particle>& particles)
{
   //Path& path= sp.path();
   
   //std::vector<TVector3>& points= track.get
   //TrajClass = GTrajectory ; //AJRTODO GStraightTrajectoryif is_neutral else GHelixTrajectory
   m_gTrajectories.push_back(GTrajectory(particle));
}

GTrajectories::GTrajectories(const Track& track)
//AJRTODO const std::list<Particle>& particles)
{
  //Path& path= sp.path();
  
  //std::vector<TVector3>& points= track.get
  //TrajClass = GTrajectory ; //AJRTODO GStraightTrajectoryif is_neutral else GHelixTrajectory
  m_gTrajectories.push_back(GTrajectory(track));
}

///Constructor for showing clusters
GTrajectories::GTrajectories(const Cluster& cluster)
{
   m_gBlobs.push_back(GBlob(cluster));
}


void GTrajectories::Draw(const std::string& projection) const
{
   //draw tracks
   for (auto const& traj : m_gTrajectories) {
      traj.Draw(projection);
   }
   //draw clusters
   for (auto const& traj : m_gBlobs) {
      traj.Draw(projection);
   }
}

