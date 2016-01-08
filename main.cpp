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
#include "fastsim/my_utilities.h"
#include "detectors/material.h"
#include "detectors/geometry.h"
#include "detectors/CMS.h"
#include "pfobjects/particle.h"
#include "pfobjects/pfobjects.h"
#include "simulation/simulator.h"
//#include "supertree.h"
#include "pfobjects/path.h"
#include "display/displaygeometry.h"
#include "display/displaycore.h"
#include "display/displaypfobjects.h"

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


#include "utility/identifier.h"

//ENUM_WITH_STRING(Proj, (xy)(yz)(xz)(ECAL_thetaphi))


using namespace std;

void testing() { //change to concrete object or unique pointer is there is an issue
   TCanvas *c1 = new TCanvas("c1","A Simple Graph Example",200,10,700,500);
   c1->SetFillColor(42);
   c1->SetGrid();
   
   const int n = 20;
   double x[n], y[n];
   for (int i=0;i<n;i++) {
      x[i] = i;
      y[i] = 2*i;
      cout<<x[i]<<"\t"<<y[i]<<endl;
   }
   
   TGraph *gr = new TGraph(n,x,y);
   gr->SetLineColor(2);
   gr->SetLineWidth(4);
   gr->SetMarkerColor(4);
   gr->SetMarkerStyle(21);
   gr->SetTitle("a simple graph");
   gr->GetXaxis()->SetTitle("X title");
   gr->GetYaxis()->SetTitle("Y title");
   gr->Draw("ACP");
   
   c1->Update();
   c1->Modified();
   c1->Connect("Closed()", "TApplication", gApplication, "Terminate()"); //new
}



struct A
{
    virtual void foo() const = 0;
    void bar();
};

struct B : A
{
    //void foo()  override; // Error: B::foo does not override A::foo
    // (signature mismatch)
    void foo() const override; // OK: B::foo overrides A::foo
    //void bar() override; // Error: A::bar is not virtual
};

void B::foo() const
{
    std::cout <<"foo";
}


using boost::any_cast;
typedef std::list<boost::any> many;


void append_int(many & values, int value)
{
   boost::any to_append = value;
   values.push_back(to_append);
}

int main(int argc, char* argv[]){
   //identifier
   
   
   long id =Identifier::makeAnotherIdentifier(0);
   long id1 =Identifier::makeAnotherIdentifier(0);
   many values;
   int x=3;
   values.push_back(x);

   
  B b;
    b.foo();
   
   // all new
   TApplication theApp("App", &argc, argv);
   if (gROOT->IsBatch()) {
      fprintf(stderr, "%s: cannot run in batch mode\n", argv[0]);
      return 1;
   }
   //testing();
   //theApp.Run();
   
   
   
    std::cout << "Try base classes\n";
    
    Material M("test",1,1);
    SurfaceCylinder S("empty");
    VolumeCylinder V("new", 4, 6, 3, 6);
    
    CMS CMSDetector;
    TLorentzVector tlv(1.,1. , 1., .7);
    Particle ppp(22,.5,tlv,1);
    
    std::cout<<ppp.StringDescription()<<"\n";
    
   
   //dispay tests
   
   
  /*
   Projection A=Projection::xy;
   cout<<A;
   cout<<to_str(A);
   */
   
   
   
   TLorentzVector p4 = TLorentzVector();
   p4.SetPtEtaPhiM(1, 0, 0, 5.11e-4);
   //std::shared_ptr<TVector3> tvect = std::shared_ptr<TVector3>{new TVector3(0,0,0)};
   //Helix helix(3.8, 1, p4,tvect );
   Helix helix(3.8, 1, p4,TVector3(0,0,0));
   double length = helix.getPathLength(1.0e-9);
   
   
   TVector3 junk = helix.getPointAtTime(1e-9);
   std::cout<<"HElix point: " <<junk.X() << " " << junk.Y() << " " << junk.Z()<<" ";
   std::cout<< "\nlength"<<length<< " " << helix.getDeltaT(length)<<std::endl;
   
       // test from pfobjects
   
    TVector3 vpos(1.,.5,.3);
    Cluster cluster=  Cluster(10., vpos, 1., "ecaltest");
    std::cout <<"cluster "<< cluster.getPt()<<"\n";
    
    //cluster.setEnergy(5.);
    //std::cout << cluster.getPt()<<"\n";
    
   //Simulator sim{CMSDetector};
    //SimParticle photon(22,  M_PI/2., M_PI/2.+0.0, 10.);
    
    //ptc = pfsimparticle()
    TLorentzVector tlvphoton=MakeParticleLorentzVector(22,  M_PI/2., M_PI/2.+0.0, 10.);
   // SimParticle photon(22, tlvphoton);
   // sim.simulatePhoton(photon);
   
   
   std::vector<TVector3> tvec;
   tvec.push_back(TVector3(0.,0.,0.));
   tvec.push_back(TVector3(1.,1.,1.));
   tvec.push_back(TVector3(2.,2.,2.));
   

   Display display = Display({enumProjection::xy,enumProjection::yz});
   //Display display = Display({Projection::xy,Projection::yz,Projection::ECAL_thetaphi ,Projection::HCAL_thetaphi });
   
   std::shared_ptr<GDetector> gdetector (new GDetector(CMSDetector));
   
   display.Register(gdetector, 0);

   std::shared_ptr<GTrajectories> gtrajectories (new GTrajectories(tvec)) ;// simulator.ptcs)
   std::shared_ptr<GTrajectories> gcluster (new GTrajectories(cluster)) ;
   display.Register(gtrajectories,1);
   display.Register(gcluster,2);
   display.Draw();
  

   
  return EXIT_SUCCESS;
}
