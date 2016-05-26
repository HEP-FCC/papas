
//
//  main.cpp
//
//  Created by Alice Robson on 14/01/16.
//
//
//C++
#include <iostream>
#include <cstdlib>
#include <unordered_map>

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
#include "Particle.h"
#include "Simulator.h"
#include "Path.h"
#include "Helix.h"
#include "displaygeometry.h"
#include "displaycore.h"
#include "displaypfobjects.h"
#include "TVector3.h"
#include "Cluster.h"
#include "Track.h"
#include "PFParticle.h"
#include "Id.h"
#include "Definitions.h"
#include "Edge.h"

//include "random.h"
#include "random.h"

//gtest test
#include "gtest/gtest.h"
//#include <boost/python.hpp>



using namespace std;
using namespace papas;
extern int test_edges();
extern int test_blocks();
extern int test_BlockBuilder();
extern int test_BlockSplitter();
extern int test_Distance();

//extern void tryMapMoveObject();




int main(int argc, char* argv[]){
  
  //test_RandomNew();
  
  //TODO convert to Gtest
  test_edges();
  test_blocks();
  test_BlockBuilder();
  //tryMapMoveObject();
    
    
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

TEST(fastsim, ID)
{  ///ID test
  
  Id::Type id(0);
  auto uid= Id::uniqueId(id);
  auto type =Id::itemType(id);
  //auto idtype =Id::itemType(id);
  for (int j=0; j<6 ; j++ ) {
    
    //Id::ItemType e=Id::ItemType(j);
    Id::ItemType e=Id::ItemType(0);
    for (int i =1 ; i<30 ; i++) {
      int n=pow(2,i);
      id=Id::makeId(e, n);
      uid= Id::uniqueId(id);
      type =Id::itemType(id);
      EXPECT_EQ(type, e);
      EXPECT_EQ(uid, n);
      
    }
  }
  
}


TEST(fastsim, Helix)
{  ///Helix path test
   TLorentzVector p4 = TLorentzVector();
   p4.SetPtEtaPhiM(1, 0, 0, 5.11e-4);
   Helix helix( p4,TVector3(0,0,0),3.8, 1);
   double length = helix.pathLength(1.0e-9);
   TVector3 junk = helix.pointAtTime(1e-9);
   
   EXPECT_EQ(junk.Z(), 0);
   EXPECT_NEAR(junk.X(), 0.2939983, 1e-6);
   EXPECT_NEAR(junk.Y(), -0.050697917, 1e-6);
   EXPECT_NEAR(length,    0.299792,1e-5);
   EXPECT_NEAR(helix.deltaT(length),1e-9,1e-14);
}

TEST(fastsim,Structures){
   //testing cylinders etc
   // Try base classes ;
   Material M(1, 1);
  SurfaceCylinder S(papas::Position::kEcalIn);
  VolumeCylinder V(papas::Layer::kEcal, 4, 6, 3, 6);
   return SUCCEED();
}

void testGraphs()
{//Testing graphics
  Display display = Display({papas::ViewPane::Projection::xy,papas::ViewPane::Projection::yz});
   //Display display = Display({Projection::xy,Projection::yz,Projection::ECAL_thetaphi ,Projection::HCAL_thetaphi });
   
   TVector3 vpos(1.,.5,.3);
   Cluster cluster =  Cluster(10., vpos, 1.,Id::makeEcalId() );
   std::vector<TVector3> tvec;
   tvec.push_back(TVector3(0.,0.,0.));
   tvec.push_back(TVector3(1.,1.,1.));
   tvec.push_back(TVector3(2.,2.,2.));
   
   
   /*std::shared_ptr<GTrajectories> gtrajectories (new GTrajectories(tvec)) ;// simulator.ptcs)
   std::shared_ptr<GTrajectories> gcluster (new GTrajectories(cluster)) ;
   display.addToRegister(gtrajectories,1);
   display.addToRegister(gcluster,2);
   display.Draw();*/
   
   //Testing graphics
   /*
    TVector3 vpos(1.,.5,.3);
    Cluster cluster=  Cluster(10., vpos, 1.,Id::makeEcalId() );
    std::vector<TVector3> tvec;
    
    std::cout <<"cluster "<< cluster.pt()<<"\n";
    
    std::vector<TVector3> tvec;
    tvec.push_back(TVector3(0.,0.,0.));
    tvec.push_back(TVector3(1.,1.,1.));
    tvec.push_back(TVector3(2.,2.,2.));
    
    
    Display display = Display({enumProjection::xy,enumProjection::yz});
    //Display display = Display({Projection::xy,Projection::yz,Projection::ECAL_thetaphi ,Projection::HCAL_thetaphi });
    
    std::shared_ptr<GDetector> gdetector (new GDetector(CMSDetector));
    display.addToRegister(gdetector, 0);
    
    std::shared_ptr<GTrajectories> gtrajectories (new GTrajectories(tvec)) ;// simulator.ptcs)
    std::shared_ptr<GTrajectories> gcluster (new GTrajectories(cluster)) ;
    display.addToRegister(gtrajectories,1);
    display.addToRegister(gcluster,2);
    display.Draw();*/
   
   
}


TEST(fastsim,Cylinder){

   auto cyl1 = SurfaceCylinder(papas::Position::kEcalIn, 1, 2);
   auto cyl2 = SurfaceCylinder(papas::Position::kEcalIn, 0.7, 1.5);
   auto subcyl = VolumeCylinder(papas::Layer::kEcal, 1 ,2, 0.7, 1.5 );
   
   EXPECT_EQ(subcyl.inner().getRadius(),0.7 );
   EXPECT_EQ(subcyl.inner().Z(),1.5);
   EXPECT_EQ(subcyl.outer().getRadius(),1. );
   EXPECT_EQ(subcyl.outer().Z(),2.);
   
 /* TODO
# inner cylinder larger than the outer one
self.assertRaises(ValueError,
                  VolumeCylinder, 'test', 1, 2, 1.1, 1.5 )
# signature does not exist anymore
self.assertRaises(TypeError,
                  VolumeCylinder, cyl2, cyl1 )
# forgot name
self.assertRaises(ValueError,
                  VolumeCylinder, 1, 2, 0.9, 1.9*/

}

TEST(fastsim, CMS)
{
   CMS cms = CMS();
     std::cout<<"TODO Tested CMS" <<std::endl;
 /*  radii = [cyl.rad for cyl in cms.cylinders()];
self.assertEqual( radii, sorted(radii))
zs = [cyl.z for cyl in cms.cylinders()]
self.assertEqual( zs, sorted(zs))*/
   
}

TEST(fastsim, ClusterPT){
   
   ///Test that pT is correctly set
   Cluster    cluster = Cluster(10., TVector3(1,0,0), 1, 1);
   EXPECT_NEAR(cluster.pt(),10.000, 1e-6);
   
   cluster.setEnergy(5.);
   EXPECT_NEAR(cluster.pt(),5.000, 1e-6);
   
}



TEST(fastsim, Canvas) { //change to concrete object or unique pointer is there is an issue
   TCanvas *c1 = new TCanvas("c1","A Simple Graph Example",200,10,700,500);
   c1->SetFillColor(42);
   c1->SetGrid();
   
   const int n = 20;
   double x[n], y[n];
   for (int i=0;i<n;i++) {
      x[i] = i;
      y[i] = 2*i;
      //std::cout<<x[i]<<"\t"<<y[i]<<std::endl;
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
   return SUCCEED();
}


TEST(fastsim, StraightLine){
   TVector3 origin {0,0,0};
   StraightLinePropagator propStraight;
   auto cyl1 = SurfaceCylinder(papas::Position::kEcalIn, 1, 2);
   auto cyl2 = SurfaceCylinder(papas::Position::kEcalOut, 2, 1);
   
   TLorentzVector tlv{1, 0, 1, 2.};
   long uid=Id::makeParticleId(papas::enumSource::SIMULATION);
   PFParticle photon = PFParticle(uid,22,tlv ) ;
   propStraight.propagateOne(photon, cyl1);
   propStraight.propagateOne(photon, cyl2);
   auto points=photon.path()->points();
   
   // test extrapolation to barrel
   EXPECT_EQ(points.size(),3UL); 
  EXPECT_NEAR(points[papas::Position::kEcalIn].Perp(), 1.,1e-6 );
   EXPECT_NEAR(points[papas::Position::kEcalIn].Z(), 1. ,1e-6);
   // test extrapolation to endcap
   EXPECT_NEAR(points[papas::Position::kEcalOut].Z(), 1. ,1e-6);
   
   //testing extrapolation to -z
   tlv=TLorentzVector(1, 0, -1, 2.);
   uid=Id::makeParticleId(papas::enumSource::SIMULATION);
   photon = PFParticle(uid,22,tlv ) ;
   propStraight.propagateOne(photon, cyl1);
   propStraight.propagateOne(photon, cyl2);
   points=photon.path()->points();
   EXPECT_EQ(points.size(),3UL);
   EXPECT_NEAR(points[papas::Position::kEcalIn].Perp(), 1.,1e-6 );
   EXPECT_NEAR(points[papas::Position::kEcalIn].Z(), -1. ,1e-6);
   // test extrapolation to endcap
   EXPECT_NEAR(points[papas::Position::kEcalOut].Z(), -1. ,1e-6);

   
   // extrapolating from a vertex close to +endcap
   tlv=TLorentzVector(1, 0, 1, 2.);
   photon = PFParticle(uid,22,tlv , {0,0,1.5}, 0.);
   propStraight.propagateOne(photon, cyl1);
   points=photon.path()->points();
   EXPECT_NEAR(points[papas::Position::kEcalIn].Perp(), .5,1e-6 );

   // extrapolating from a vertex close to -endcap
   tlv=TLorentzVector(1, 0, -1, 2.);
   photon = PFParticle(uid,22,tlv, {0,0,-1.5}, 0.);
   propStraight.propagateOne(photon, cyl1);
   points=photon.path()->points();
   EXPECT_NEAR(points[papas::Position::kEcalIn].Perp(), .5,1e-6 );
   
   // extrapolating from a non-zero radius
   tlv=TLorentzVector(0, 0.5, 1, 2.);
   photon = PFParticle(uid,22,tlv, {0.,0.5,0,}, 0.);
   propStraight.propagateOne(photon, cyl1);
   points=photon.path()->points();
   EXPECT_NEAR(points[papas::Position::kEcalIn].Perp(), 1.,1e-6 );
   EXPECT_NEAR(points[papas::Position::kEcalIn].Z(), 1.,1e-6 );
   
}






TEST(utility, RandomExp)
{
   //seed it to have known start point
   randomgen::setEngineSeed(100);
   randomgen::RandExponential rexp(5.);
   double r1 = rexp.next();
  randomgen::setEngineSeed(100);
   randomgen::RandExponential rexp3(5.);
   double r2 = rexp3.next();
   EXPECT_EQ(r1, r2);
   
   //use a random start point so should not give same answers
    randomgen::setEngineSeed();
   randomgen::RandExponential rexpA(5.);
   double r3 = rexpA.next();
  //std::cout << rexpA()<<", "<< rexpA()<<", "<< rexpA()<<", "<< rexpA();
   randomgen::RandExponential rexpB(5.);
   double r4 = rexpB.next();
  
  
   EXPECT_NE(r3, r4);
  
}

TEST(fastsim, dummy){
  bool success = true;
  EXPECT_EQ(true, success);
}




void test_helix()
{//Helix path test
  TLorentzVector p4 = TLorentzVector();
  p4.SetPtEtaPhiM(1, 0, 0, 5.11e-4);
  Helix helix(p4,TVector3(0,0,0),3.8, 1);
  double length = helix.pathLength(1.0e-9);
  TVector3 junk = helix.pointAtTime(1e-9);
  std::cout<<"Helix point: " <<junk.X() << " " << junk.Y() << " " << junk.Z()<<" ";
  std::cout<< "\nlength"<<length<< " " << helix.deltaT(length)<<std::endl;
  
}
void test_Structures()
{
  //testing cylinders etc
  std::cout << "Try base classes\n";
  Material M(1, 1);
  SurfaceCylinder S(papas::Position::kEcalIn);
  VolumeCylinder V(papas::Layer::kEcal, 4, 6, 3, 6);
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


/*using boost::any_cast;
 typedef std::list<boost::any> many;
 
 
 void append_int(many & values, int value)
 {
 boost::any to_append = value;
 values.push_back(to_append);
 }*/


int old_main_stuff(int argc, char* argv[])
{
  
  B b;
  b.foo();
  
  
  std::unordered_map<long,TVector3> rootvec;
  TVector3 tv1{0,0,0};
  TVector3 tv2{0,0,0};
  rootvec[0]=std::move(tv1);
  rootvec[2]=std::move(tv2);
  rootvec.emplace(3,TVector3{0,0,0});
  
  // all new
  TApplication theApp("App", &argc, argv);
  if (gROOT->IsBatch()) {
    fprintf(stderr, "%s: cannot run in batch mode\n", argv[0]);
    return 1;
  }
  //testing();
  //theApp.Run();
  return 0;
}

void test_graphs()
{//Testing graphics
  Display display = Display({papas::ViewPane::Projection::xy,papas::ViewPane::Projection::yz});
  //Display display = Display({papas::ViewPane::Projection::xy,papas::ViewPane::Projection::yz,papas::ViewPane::Projection::ECAL_thetaphi ,papas::ViewPane::Projection::HCAL_thetaphi });
  
  TVector3 vpos(1.,.5,.3);
  Cluster cluster=  Cluster(10., vpos, 1.,Id::makeEcalId() );
  std::vector<TVector3> tvec;
  tvec.push_back(TVector3(0.,0.,0.));
  tvec.push_back(TVector3(1.,1.,1.));
  tvec.push_back(TVector3(2.,2.,2.));
  
  
  /*std::shared_ptr<GTrajectories> gtrajectories (new GTrajectories(tvec)) ;// simulator.ptcs)
   std::shared_ptr<GTrajectories> gcluster (new GTrajectories(cluster)) ;
   display.addToRegister(gtrajectories,1);
   display.addToRegister(gcluster,2);
   display.Draw();*/
  
  //Testing graphics
  /* TVector3 vpos(1.,.5,.3);
   Cluster cluster=  Cluster(10., vpos, 1.,Id::makeEcalId() );
   std::vector<TVector3> tvec;
   
   std::cout <<"cluster "<< cluster.pt()<<"\n";
   
   std::vector<TVector3> tvec;
   tvec.push_back(TVector3(0.,0.,0.));
   tvec.push_back(TVector3(1.,1.,1.));
   tvec.push_back(TVector3(2.,2.,2.));
   
   
   Display display = Display({enumProjection::xy,enumProjection::yz});
   //Display display = Display({Projection::xy,Projection::yz,Projection::ECAL_thetaphi ,Projection::HCAL_thetaphi });
   
   std::shared_ptr<GDetector> gdetector (new GDetector(CMSDetector));
   display.addToRegister(gdetector, 0);
   
   std::shared_ptr<GTrajectories> gtrajectories (new GTrajectories(tvec)) ;// simulator.ptcs)
   std::shared_ptr<GTrajectories> gcluster (new GTrajectories(cluster)) ;
   display.addToRegister(gtrajectories,1);
   display.addToRegister(gcluster,2);
   display.Draw();*/
  
  
}


void mytesting() { //change to concrete object or unique pointer is there is an issue
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




class MyClass{
public:
  MyClass(std::string);
  MyClass(MyClass &other);
  MyClass(const MyClass &other);
  MyClass(const MyClass &&other);
  MyClass(MyClass &&other);
  MyClass someFunction();
  
  std::string m_str;
};

MyClass::MyClass(std::string str)
:m_str(str)
{
  
}

MyClass::MyClass(const MyClass &other)
{
  std::cout << "Copy constructor was called" << m_str << std::endl;
  m_str=other.m_str;
}


MyClass::MyClass(MyClass &&other)
{
  m_str=std::move(other.m_str);
  std::cout << "Move constructor was called" << m_str << std::endl;
}
MyClass::MyClass(const MyClass &&other)
{
  m_str=std::move(other.m_str);
  
  std::cout << "const Move constructor was called" << m_str << std::endl;
}

MyClass someFunction()
{
  MyClass dummy("dummy");
  return dummy;
}


void test_RandomNew()
{
  
  auto rgen = randomgen::RandUniform(1,2);
  for (auto i =1 ; i <20; i++) {
  std::cout<<rgen.next()<<",";
  }
  std::cout<<std::endl <<std::endl;
  auto rgen1 = randomgen::RandUniform(1,2);
  for (auto i =1 ; i <20; i++) {
    std::cout<<rgen1.next()<<",";
  }
  std::cout<<std::endl <<std::endl;
  auto rgen2 = randomgen::RandUniform(1,2);
  for (auto i =1 ; i <20; i++) {
    std::cout<<rgen2.next()<<",";
  }
  randomgen::setEngineSeed();
  std::cout<<std::endl<<std::endl <<std::endl;
  std::cout<<std::endl <<std::endl;
  auto rgen3 = randomgen::RandUniform(1,2);
  for (auto i =1 ; i <20; i++) {
    std::cout<<rgen3.next()<<",";
  }
  std::cout<<std::endl<<std::endl <<std::endl;
}

TEST(utility, randomgen)
{
  //seed it to have known start point
  randomgen::setEngineSeed(100);
  randomgen::RandNormal rnorm(5., 1.);
  double r1 = rnorm.next();

  //reseed at same point
  randomgen::setEngineSeed(100);
  randomgen::RandNormal rnorm3(5., 1.);
  double r2 = rnorm3.next();
  //test random normal seeded comes out same
  EXPECT_EQ(r1, r2);

  //seed randomly
  randomgen::setEngineSeed();
  randomgen::RandNormal rnormA(5., 1.);
  double r3 = rnormA.next();
  double r5 = rnormA.next();
  randomgen::RandNormal rnormB(5., 1.);
  double r4 = rnormB.next();

  EXPECT_NE(r3, r4);
}

//Different sort of test
/*void tryMapMoveObject()
{//illustrates moving items from one map to another
  std::unordered_map<int,const TClass> map;
  map.reserve(10);
  map.emplace(1,MyClass("one"));
  map.emplace(2,MyClass("two"));
  map.emplace(3,MyClass("three"));
  std::unordered_map<int,const MyClass> map2;
  std::unordered_map<int,const MyClass> map1;
  
  for ( auto & m : map)
  {
    if (m.first==1 || m.first==3)
      map2.emplace(m.first, std::move(m.second));
    else
      map1.emplace(m.first, std::move(m.second));
  }
  std::cout << map.size()<<map2.size()<<map1.size();
  
  return;
  
  
}*/

