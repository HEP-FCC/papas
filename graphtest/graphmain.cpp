//
//  main.cpp
//  graphtest
//
//  Created by Alice Robson on 14/01/16.
//
//
//C++
#include <iostream>
#include <cstdlib>

//ROOT
#include "TROOT.h"
#include "TVector3.h"
#include "TLorentzVector.h"

//Boost
#include <boost/any.hpp>


//SSD libs
#include "../fastsim/my_utilities.h"
#include "../detectors/material.h"
#include "../detectors/geometry.h"
#include "../detectors/CMS.h"
#include "../pfobjects/particle.h"
#include "../pfobjects/pfobjects.h"
#include "../simulation/simulator.h"
//#include "supertree.h"
#include "../pfobjects/path.h"
#include "../display/displaygeometry.h"
#include "../display/displaycore.h"
#include "../display/displaypfobjects.h"

#include "TAxis.h"
#include "TGraph.h"
#include "TMultiGraph.h"
#include "TCanvas.h"
#include "TApplication.h"
#include "TStyle.h"
#include "TPad.h"
#include "TROOT.h"
#include "TColor.h"
#include "TGFrame.h"
#include "TVirtualPad.h"


#include "../utility/identifier.h"
int main(int argc, const char * argv[]) {
   // insert code here...
   std::cout << "Hello, World!\n";
   
   CMS CMSDetector;
   //Testing graphics
   TVector3 vpos(1.,.5,.3);
   Cluster cluster=  Cluster(10., vpos, 1.,Identifier::makeECALClusterID() );
   std::vector<TVector3> tvec;
   tvec.push_back(TVector3(0.,0.,0.));
   tvec.push_back(TVector3(1.,1.,1.));
   tvec.push_back(TVector3(2.,2.,2.));
   
   
   Display display = Display({enumProjection::XY,enumProjection::YZ});
   //Display display = Display({Projection::xy,Projection::yz,Projection::ECAL_thetaphi ,Projection::HCAL_thetaphi });
   
   std::shared_ptr<GDetector> gdetector (new GDetector(CMSDetector));
   
   display.Register(gdetector, 0);
   
   std::shared_ptr<GTrajectories> gtrajectories (new GTrajectories(tvec)) ;// simulator.ptcs)
   std::shared_ptr<GTrajectories> gcluster (new GTrajectories(cluster)) ;
   display.Register(gtrajectories,1);
   display.Register(gcluster,2);
   display.Draw();
    return 0;
}
