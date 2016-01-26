
//
//  main.cpp
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
#include "TApplication.h"


//SSD libs
//#include "fastsim/my_utilities.h"
#include "material.h"
#include "geometry.h"
#include "CMS.h"
#include "particle.h"
#include "datatypes.h"
#include "simulator.h"
#include "path.h"
#include "displaygeometry.h"
#include "displaycore.h"
#include "displaypfobjects.h"
#include "TVector3.h"

#include "random.h"

void testCylinder();
void testClusterPT();
void testHelix();
void testCMS();
void testStraightLine();
void testRandomNorm();
void testRandomExp();

//std::default_random_engine fastsim::RandNormal::engine(0);

int main(int argc, char* argv[]){
   //testRandom();
testRandomNorm();
   testRandomExp();
    testStraightLine();
   testCylinder();
   testHelix();
   testClusterPT();
   testCMS();
   //FASTSIMRandom::m_generator(1101966);
 
   
   // ROOT App to allow graphs to be plotted
   TApplication theApp("App", &argc, argv);
   if (gROOT->IsBatch()) {
      fprintf(stderr, "%s: cannot run in batch mode\n", argv[0]);
      return 1;
   }
   
 
   
   //theApp.Run();
   
   return EXIT_SUCCESS;
}



void testHelix()
{//Helix path test
   TLorentzVector p4 = TLorentzVector();
   p4.SetPtEtaPhiM(1, 0, 0, 5.11e-4);
   Helix helix(3.8, 1, p4,TVector3(0,0,0));
   double length = helix.getPathLength(1.0e-9);
   TVector3 junk = helix.getPointAtTime(1e-9);
   std::cout<<"Helix point: " <<junk.X() << " " << junk.Y() << " " << junk.Z()<<" ";
   std::cout<< "\nlength"<<length<< " " << helix.getDeltaT(length)<<std::endl;
   
}
void testStructures()
{
   //testing cylinders etc
   std::cout << "Try base classes\n";
   Material M(fastsim::enumLayer::ECAL,1,1);
   SurfaceCylinder S("empty");
   VolumeCylinder V("new", 4, 6, 3, 6);
}

void testGraphs()
{//Testing graphics
   Display display = Display({Projection::xy,Projection::yz});
   //Display display = Display({Projection::xy,Projection::yz,Projection::ECAL_thetaphi ,Projection::HCAL_thetaphi });
   
   TVector3 vpos(1.,.5,.3);
   Cluster cluster=  Cluster(10., vpos, 1.,Identifier::makeECALClusterID() );
   std::vector<TVector3> tvec;
   tvec.push_back(TVector3(0.,0.,0.));
   tvec.push_back(TVector3(1.,1.,1.));
   tvec.push_back(TVector3(2.,2.,2.));
   
   
   /*std::shared_ptr<GTrajectories> gtrajectories (new GTrajectories(tvec)) ;// simulator.ptcs)
   std::shared_ptr<GTrajectories> gcluster (new GTrajectories(cluster)) ;
   display.Register(gtrajectories,1);
   display.Register(gcluster,2);
   display.Draw();*/
   
   //Testing graphics
   /* TVector3 vpos(1.,.5,.3);
    Cluster cluster=  Cluster(10., vpos, 1.,Identifier::makeECALClusterID() );
    std::vector<TVector3> tvec;
    
    std::cout <<"cluster "<< cluster.getPt()<<"\n";
    
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
    display.Draw();*/
   
   
}

void write_failed(std::string name)
{std::cout<< name << " FAILED" <<std::endl;
}

void test_almost(std::string name,double val, double expected)
{
   if( val>=0 & val < expected *1.0000001 & val >expected *.99999999)
      return ;
   if( val>0 & val > expected *1.0000001 & val >expected *.99999999)
      return ;
   return write_failed(name);
}


void testCylinder(){

   auto cyl1 = SurfaceCylinder("cyl1", 1, 2);
   auto cyl2 = SurfaceCylinder("cyl2", 0.7, 1.5);
   auto subcyl = VolumeCylinder("subcyl", 1 ,2, 0.7, 1.5 );
   
   if( subcyl.Inner().getRadius()!=0.7 )
      write_failed("subcyl.inner.rad");
   if( subcyl.Inner().Z()!=1.5)
      write_failed("subcyl.inner.rad");
   if( subcyl.Outer().getRadius()!=1. )
      write_failed("subcyl outer");
   if( subcyl.Outer().Z()!=2.)
      write_failed("subcyl outer Z");
   std::cout<<"Tested Cylinders" <<std::endl;
   
 /*
# inner cylinder larger than the outer one
self.assertRaises(ValueError,
                  VolumeCylinder, 'test', 1, 2, 1.1, 1.5 )
# signature does not exist anymore
self.assertRaises(TypeError,
                  VolumeCylinder, cyl2, cyl1 )
# forgot name
self.assertRaises(ValueError,
                  VolumeCylinder, 1, 2, 0.9, 1.9)
*/
}

void testCMS()
{
   CMS cms = CMS();
     std::cout<<"TODO Tested CMS" <<std::endl;
 /*  radii = [cyl.rad for cyl in cms.cylinders()];
self.assertEqual( radii, sorted(radii))
zs = [cyl.z for cyl in cms.cylinders()]
self.assertEqual( zs, sorted(zs))*/
   
}

void testClusterPT(){
   
   ///Test that pT is correctly set.'''
   Cluster    cluster = Cluster(10., TVector3(1,0,0), 1, 1);
   if (!( 9.9999 <cluster.getPt() && cluster.getPt()<10.0001))
      write_failed("CLUSTER ENERGY 10 ");
   cluster.setEnergy(5.);
   if (!( 4.9999 < cluster.getPt()  && cluster.getPt()<5.0001))
      write_failed("CLUSTER ENERGY 5");
   std::cout<<"Tested CLUSTERPT" <<std::endl;

}



void testing() { //change to concrete object or unique pointer is there is an issue
   TCanvas *c1 = new TCanvas("c1","A Simple Graph Example",200,10,700,500);
   c1->SetFillColor(42);
   c1->SetGrid();
   
   const int n = 20;
   double x[n], y[n];
   for (int i=0;i<n;i++) {
      x[i] = i;
      y[i] = 2*i;
      std::cout<<x[i]<<"\t"<<y[i]<<std::endl;
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


void testStraightLine(){
   TVector3 origin {0,0,0};
   StraightLinePropagator propStraight;
   auto cyl1 = SurfaceCylinder("cyl1", 1, 2);
   auto cyl2 = SurfaceCylinder("cyl2", 2, 1);
   
   TLorentzVector tlv{1, 0, 1, 2.};
   SimParticle photon = SimParticle(22,tlv ,{0,0,0}) ;
   propStraight.propagateOne(photon, cyl1);
   propStraight.propagateOne(photon, cyl2);
   auto points=photon.getPath().getPoints();
 /*  for (auto p : points)
      std::cout<< "  X:"<<p.second.X()<<"  Y:"<< p.second.Y()<<"  Z:"<< p.second.Z()<<"  P:"<< p.second.Perp()<<std::endl;;*/
   if (points.size()!=3)
      write_failed("Particle POints");
   test_almost( "SL1",points["cyl1"].Perp(), 1. );
   test_almost( "SL2",points["cyl1"].Z(), 1. );
   test_almost( "SL3",points["cyl2"].Z(), 1. );
   
   tlv=TLorentzVector(1, 0, -1, 2.);
   photon = SimParticle(22,tlv ,{0,0,0}) ;
   propStraight.propagateOne(photon, cyl1);
   propStraight.propagateOne(photon, cyl2);
   points=photon.getPath().getPoints();
    if (points.size()!=3)
      write_failed("Particle POints");
   test_almost( "SL4",points["cyl1"].Perp(), 1. );
   test_almost( "SL5",points["cyl1"].Z(), -1. );
   test_almost( "SL6",points["cyl2"].Z(), -1. );
   
   
   tlv=TLorentzVector(1, 0, 1, 2.);
   photon = SimParticle(22,tlv , {0,0,1.5});
   propStraight.propagateOne(photon, cyl1);
   points=photon.getPath().getPoints();
   test_almost( "SL7",points["cyl1"].Perp(), 0.5 );

   
   
   tlv=TLorentzVector(1, 0, -1, 2.);
   photon = SimParticle(22,tlv , {0,0,-1.5});
   propStraight.propagateOne(photon, cyl1);
   points=photon.getPath().getPoints();
   test_almost( "SL8",points["cyl1"].Perp(), 0.5 );

   
   std::cout<<"Tested STRAIGHTLINE" <<std::endl;

   
}




void testRandomNorm()
{
   //double v=fastsim::rd();
   //seed it to have known start point
   fastsim::RandNormal rnorm(5.,1.,100);
   double r1= rnorm();
   
   
   fastsim::RandNormal rnorm3(5.,1.,100);
   double r2=rnorm3();
   
   if( r1!=r2 )
      write_failed("random normal seeded");
   
   //use a rnadom start point so should not give same answers
   //fastsim::engine.seed(100);
   std::cout<<fastsim::makeseed()<<std::endl;
   std::cout<<fastsim::makeseed()<<std::endl;

   std::cout<<fastsim::makeseed()<<std::endl;

   
   
   fastsim::RandNormal rnormA(5.,1.,fastsim::makeseed());
   double r3= rnormA();
   //r3=rnormA();
   //fastsim::engine.seed(101);
   
   fastsim::RandNormal rnormB(5.,1.);
   //r3=rnormA();
   double r4=rnormB();
   
   if( r3==r4 )
      write_failed("random normal");

   std::cout<<rnormA()<<std::endl;
   
}

void testRandomExp()
{
   
   //seed it to have known start point
   fastsim::RandExponential rexp(5.,100);
   double r1= rexp();
   
   fastsim::RandExponential rexp3(5.,100);
   double r2=rexp3();
   
   if( r1!=r2 )
      write_failed("random exp seeded");
   
   //use a rnadom start point so should not give same answers
   fastsim::RandExponential rexpA(5.);
   double r3= rexpA();
   
   fastsim::RandExponential rexpB(5.);
   double r4=rexpB();
   
   if( r3==r4 )
      write_failed("random exp");
   
   
   
}
