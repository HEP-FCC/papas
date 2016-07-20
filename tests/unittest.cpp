// STL
#include <iostream>
#include <vector>

// catch
#define CATCH_CONFIG_MAIN
#include "Id.h"
#include "catch.hpp"

// C++
#include <cstdlib>
#include <functional>
#include <iostream>
#include <numeric>
#include <string>
#include <unordered_map>

// ROOT
#include "TApplication.h"
#include "TFile.h"
#include "TLorentzVector.h"
#include "TROOT.h"
#include "TVector3.h"

#include "Display.h"
#include "GTrajectory.h"
#include "ViewPane.h"

// SSD libs
//#include "fastsim/my_utilities.h"
#include "CMS.h"
#include "Cluster.h"
#include "SimParticle.h"

#include "Definitions.h"
#include "Edge.h"
#include "Helix.h"
#include "Id.h"
#include "PParticle.h"
#include "Path.h"
#include "Simulator.h"
#include "SurfaceCylinder.h"
#include "VolumeCylinder.h"
#include "material.h"
#include "pTrack.h"
#include "Distance.h"

#include "PapasManager.h"

// include "random.h"
#include "random.h"
using namespace papas;

TEST_CASE("Id") {  /// ID test
  using namespace papas;
  Id::Type id(0);
  auto uid = Id::uniqueId(id);
  auto type = Id::itemType(id);

  for (int j = 0; j < 6; j++) {

    Id::ItemType e = Id::ItemType(0);
    for (int i = 1; i < 30; i++) {
      int n = pow(2, i);
      id = Id::makeId(e, n);
      uid = Id::uniqueId(id);
      type = Id::itemType(id);
      REQUIRE(type == e);
      REQUIRE(uid == n);
    }
  }
}

TEST_CASE("Helix") {  /// Helix path test
  TLorentzVector p4 = TLorentzVector();
  p4.SetPtEtaPhiM(1, 0, 0, 5.11e-4);
  Helix helix(p4, TVector3(0, 0, 0), 3.8, 1);
  double length = helix.pathLength(1.0e-9);
  TVector3 junk = helix.pointAtTime(1e-9);

  REQUIRE(junk.Z() == 0);
  REQUIRE(junk.X() == Approx(0.2939983));
  REQUIRE(junk.Y() == Approx(-0.050697917));
  REQUIRE(length == Approx(0.299792));
  REQUIRE(helix.deltaT(length) == Approx(1e-9));
}

TEST_CASE("Helixpath") {  /// Helix path test
  auto cyl1 = SurfaceCylinder(papas::Position::kEcalIn, 1., 2.);
  auto cyl2 = SurfaceCylinder(papas::Position::kEcalOut, 2., 1.);
  double field = 3.8;
  auto particle = SimParticle(0, 211, -1, TLorentzVector{2., 0, 1, 5}, TVector3{0, 0, 0}, field);
  auto helixprop = HelixPropagator(3.8);
  //(particle.p4(), {0,0,0}, 3.8, -1);
  helixprop.propagateOne(particle, cyl1);
  auto tvec = particle.pathPosition(cyl1.layer());
  auto particle2 = SimParticle(0, 211, -1, TLorentzVector{0., 2, 1, 5}, TVector3{0, 0, 0}, field);
  helixprop.propagateOne(particle2, cyl1);
  auto tvec2 = particle2.pathPosition(cyl1.layer());
  REQUIRE(fabs(tvec.X()) == Approx(fabs(tvec2.Y())));
  REQUIRE(tvec2.Z() == Approx(0.50701872));
}
TEST_CASE("Structures") {
  // testing cylinders etc
  // Try base classes ;
  Material M(1, 1);
  SurfaceCylinder S(papas::Position::kEcalIn);
  VolumeCylinder V(papas::Layer::kEcal, 4, 6, 3, 6);
  // TODO return SUCCEED();
}

void testGraphs() {  // Testing graphics
                     // Display display = Display({papas::ViewPane::Projection::xy, papas::ViewPane::Projection::yz});
  // Display display = Display({Projection::xy,Projection::yz,Projection::ECAL_thetaphi ,Projection::HCAL_thetaphi });

  TVector3 vpos(1., .5, .3);
  Cluster cluster = Cluster(10., vpos, 1., Id::ItemType::kEcalCluster);
  std::vector<TVector3> tvec;
  tvec.push_back(TVector3(0., 0., 0.));
  tvec.push_back(TVector3(1., 1., 1.));
  tvec.push_back(TVector3(2., 2., 2.));

  /*std::shared_ptr<GTrajectories> gtrajectories (new GTrajectories(tvec)) ;// simulator.ptcs)
   std::shared_ptr<GTrajectories> gcluster (new GTrajectories(cluster)) ;
   display.addToRegister(gtrajectories,1);
   display.addToRegister(gcluster,2);
   display.Draw();*/

  // Testing graphics
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

TEST_CASE("Cylinder") {

  auto cyl1 = SurfaceCylinder(papas::Position::kEcalIn, 1, 2);
  auto cyl2 = SurfaceCylinder(papas::Position::kEcalIn, 0.7, 1.5);
  auto subcyl = VolumeCylinder(papas::Layer::kEcal, 1, 2, 0.7, 1.5);

  REQUIRE(subcyl.inner().getRadius() == 0.7);
  REQUIRE(subcyl.inner().z() == 1.5);
  REQUIRE(subcyl.outer().getRadius() == 1.);
  REQUIRE(subcyl.outer().z() == 2.);

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

TEST_CASE("CMS") {
  CMS cms = CMS();
  std::cout << "TODO Tested CMS" << std::endl;
  /*  radii = [cyl.rad for cyl in cms.cylinders()];
   self.assertEqual( radii, sorted(radii))
   zs = [cyl.z for cyl in cms.cylinders()]
   self.assertEqual( zs, sorted(zs))*/
}

TEST_CASE("ClusterPT") {

  /// Test that pT is correctly set
  Cluster cluster = Cluster(10., TVector3(1, 0, 0), 1, Id::ItemType::kEcalCluster);
  REQUIRE(cluster.pt() == Approx(10.000));

  cluster.setEnergy(5.);
  REQUIRE(cluster.pt() == Approx(5.000));
}

TEST_CASE("ClusterSmear") {

  // Make a cluster
  double energy = 10.;
  auto cluster = Cluster(energy, TVector3(1, 0, 0), 1., Id::kEcalCluster);  // #alice made this use default layer
  auto CMSDetector = CMS();
  auto ecal = CMSDetector.ecal();
  PapasManager papasManager{CMSDetector};

  // smear it 10000 times and store the energy of the smeared cluster
  std::vector<double> energies;
  for (int i = 0; i < 10000; i++) {
    auto smeared = papasManager.simulator().smearCluster(cluster);
    energies.push_back(smeared.energy());
  }

  // check mean and stddev
  double sum = std::accumulate(energies.begin(), energies.end(), 0.0);
  std::cout << energies.size();
  double m = sum / energies.size();
  double accum = 0.0;
  std::for_each(std::begin(energies), std::end(energies), [&](const double d) { accum += (d - m) * (d - m); });

  double stdev = sqrt(accum / (energies.size() - 1.));
  double eres = ecal->energyResolution(cluster.energy(), cluster.eta());

  REQUIRE(m == Approx(energy).epsilon(0.01));
  REQUIRE(stdev == Approx(eres * energy).epsilon(0.1));
}

TEST_CASE("Canvas") {  // change to concrete object or unique pointer is there is an issue
  TCanvas* c1 = new TCanvas("c1", "A Simple Graph Example", 200, 10, 700, 500);
  c1->SetFillColor(42);
  c1->SetGrid();

  const int n = 20;
  double x[n], y[n];
  for (int i = 0; i < n; i++) {
    x[i] = i;
    y[i] = 2 * i;
    // std::cout<<x[i]<<"\t"<<y[i]<<std::endl;
  }

  TGraph* gr = new TGraph(n, x, y);
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
  c1->Connect("Closed()", "TApplication", gApplication, "Terminate()");  // new
  return;
}

TEST_CASE("StraightLine") {
  TVector3 origin{0, 0, 0};
  StraightLinePropagator propStraight;
  auto cyl1 = SurfaceCylinder(papas::Position::kEcalIn, 1, 2);
  auto cyl2 = SurfaceCylinder(papas::Position::kEcalOut, 2, 1);

  TLorentzVector tlv{1, 0, 1, 2.};
  long uid = Id::makeParticleId(papas::enumSource::SIMULATION);
  SimParticle photon = SimParticle(uid, 22, 0, tlv);
  propStraight.propagateOne(photon, cyl1);
  propStraight.propagateOne(photon, cyl2);
  auto points = photon.path()->points();

  // test extrapolation to barrel
  REQUIRE(points.size() == 3UL);
  REQUIRE(points[papas::Position::kEcalIn].Perp() == Approx(1.));
  REQUIRE(points[papas::Position::kEcalIn].Z() == Approx(1.));
  // test extrapolation to endcap
  REQUIRE(points[papas::Position::kEcalOut].Z() == Approx(1.));

  // testing extrapolation to -z
  tlv = TLorentzVector(1, 0, -1, 2.);
  uid = Id::makeParticleId(papas::enumSource::SIMULATION);
  SimParticle photon2 = SimParticle(uid, 22, 0, tlv);
  propStraight.propagateOne(photon2, cyl1);
  propStraight.propagateOne(photon2, cyl2);
  points = photon2.path()->points();
  REQUIRE(points.size() == 3UL);
  REQUIRE(points[papas::Position::kEcalIn].Perp() == Approx(1.));
  REQUIRE(points[papas::Position::kEcalIn].Z() == Approx(-1.));
  // test extrapolation to endcap
  REQUIRE(points[papas::Position::kEcalOut].Z() == Approx(-1.));

  // extrapolating from a vertex close to +endcap
  tlv = TLorentzVector(1, 0, 1, 2.);
  SimParticle photon3 = SimParticle(uid, 22, 0, tlv, {0, 0, 1.5}, 0.);
  propStraight.propagateOne(photon3, cyl1);
  points = photon3.path()->points();
  REQUIRE(points[papas::Position::kEcalIn].Perp() == Approx(.5));

  // extrapolating from a vertex close to -endcap
  tlv = TLorentzVector(1, 0, -1, 2.);
  SimParticle photon4 = SimParticle(uid, 22, 0, tlv, {0, 0, -1.5}, 0.);
  propStraight.propagateOne(photon4, cyl1);
  points = photon4.path()->points();
  REQUIRE(points[papas::Position::kEcalIn].Perp() == Approx(.5));

  // extrapolating from a non-zero radius
  tlv = TLorentzVector(0, 0.5, 1, 2.);
  SimParticle photon5 = SimParticle(uid, 22, 0, tlv,
                                    {
                                        0., 0.5, 0,
                                    },
                                    0.);
  propStraight.propagateOne(photon5, cyl1);
  points = photon5.path()->points();
  REQUIRE(points[papas::Position::kEcalIn].Perp() == Approx(1.));
  REQUIRE(points[papas::Position::kEcalIn].Z() == Approx(1.));
}

TEST_CASE("RandomExp") {
  // seed it to have known start point
  randomgen::setEngineSeed(100);
  randomgen::RandExponential rexp(5.);
  double r1 = rexp.next();
  randomgen::setEngineSeed(100);
  randomgen::RandExponential rexp3(5.);
  double r2 = rexp3.next();
  REQUIRE(r1 == r2);

  // use a random start point so should not give same answers
  randomgen::setEngineSeed();
  randomgen::RandExponential rexpA(5.);
  double r3 = rexpA.next();
  // std::cout << rexpA()<<", "<< rexpA()<<", "<< rexpA()<<", "<< rexpA();
  randomgen::RandExponential rexpB(5.);
  double r4 = rexpB.next();

  REQUIRE(r3 != r4);
}

TEST_CASE("dummy") {
  bool success = true;
  REQUIRE(true == success);
}

void test_helix() {  // Helix path test
  TLorentzVector p4 = TLorentzVector();
  p4.SetPtEtaPhiM(1, 0, 0, 5.11e-4);
  Helix helix(p4, TVector3(0, 0, 0), 3.8, 1);
  double length = helix.pathLength(1.0e-9);
  TVector3 junk = helix.pointAtTime(1e-9);
  std::cout << "Helix point: " << junk.X() << " " << junk.Y() << " " << junk.Z() << " ";
  std::cout << "\nlength" << length << " " << helix.deltaT(length) << std::endl;
}
void test_Structures() {
  // testing cylinders etc
  std::cout << "Try base classes\n";
  Material M(1, 1);
  SurfaceCylinder S(papas::Position::kEcalIn);
  VolumeCylinder V(papas::Layer::kEcal, 4, 6, 3, 6);
}

struct A {
  virtual void foo() const = 0;
  void bar();
};

struct B : A {
  // void foo()  override; // Error: B::foo does not override A::foo
  // (signature mismatch)
  void foo() const override;  // OK: B::foo overrides A::foo
                              // void bar() override; // Error: A::bar is not virtual
};

void B::foo() const { std::cout << "foo"; }

/*using boost::any_cast;
 typedef std::list<boost::any> many;


 void append_int(many & values, int value)
 {
 boost::any to_append = value;
 values.push_back(to_append);
 }*/

int old_main_stuff(int argc, char* argv[]) {

  B b;
  b.foo();

  std::unordered_map<long, TVector3> rootvec;
  TVector3 tv1{0, 0, 0};
  TVector3 tv2{0, 0, 0};
  rootvec[0] = std::move(tv1);
  rootvec[2] = std::move(tv2);
  rootvec.emplace(3, TVector3{0, 0, 0});

  // all new
  TApplication theApp("App", &argc, argv);
  if (gROOT->IsBatch()) {
    fprintf(stderr, "%s: cannot run in batch mode\n", argv[0]);
    return 1;
  }
  // testing();
  // theApp.Run();
  return 0;
}

void test_graphs() {  // Testing graphics
  Display display = Display({papas::ViewPane::Projection::xy, papas::ViewPane::Projection::yz});
  // Display display =
  // Display({papas::ViewPane::Projection::xy,papas::ViewPane::Projection::yz,papas::ViewPane::Projection::ECAL_thetaphi
  // ,papas::ViewPane::Projection::HCAL_thetaphi });

  TVector3 vpos(1., .5, .3);
  Cluster cluster = Cluster(10., vpos, 1., Id::ItemType::kEcalCluster);
  std::vector<TVector3> tvec;
  tvec.push_back(TVector3(0., 0., 0.));
  tvec.push_back(TVector3(1., 1., 1.));
  tvec.push_back(TVector3(2., 2., 2.));

  /*std::shared_ptr<GTrajectories> gtrajectories (new GTrajectories(tvec)) ;// simulator.ptcs)
   std::shared_ptr<GTrajectories> gcluster (new GTrajectories(cluster)) ;
   display.addToRegister(gtrajectories,1);
   display.addToRegister(gcluster,2);
   display.Draw();*/

  // Testing graphics
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

void mytesting() {  // change to concrete object or unique pointer is there is an issue
  TCanvas* c1 = new TCanvas("c1", "A Simple Graph Example", 200, 10, 700, 500);
  c1->SetFillColor(42);
  c1->SetGrid();

  const int n = 20;
  double x[n], y[n];
  for (int i = 0; i < n; i++) {
    x[i] = i;
    y[i] = 2 * i;
    std::cout << x[i] << "\t" << y[i] << std::endl;
  }

  TGraph* gr = new TGraph(n, x, y);
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
  c1->Connect("Closed()", "TApplication", gApplication, "Terminate()");  // new
}

class MyClass {
public:
  MyClass(std::string);
  MyClass(MyClass& other);
  MyClass(const MyClass& other);
  MyClass(const MyClass&& other);
  MyClass(MyClass&& other);
  MyClass someFunction();

  std::string m_str;
};

MyClass::MyClass(std::string str) : m_str(str) {}

MyClass::MyClass(const MyClass& other) {
  std::cout << "Copy constructor was called" << m_str << std::endl;
  m_str = other.m_str;
}

MyClass::MyClass(MyClass&& other) {
  m_str = std::move(other.m_str);
  std::cout << "Move constructor was called" << m_str << std::endl;
}
MyClass::MyClass(const MyClass&& other) {
  m_str = std::move(other.m_str);

  std::cout << "const Move constructor was called" << m_str << std::endl;
}

MyClass someFunction() {
  MyClass dummy("dummy");
  return dummy;
}

void test_RandomNew() {

  auto rgen = randomgen::RandUniform(1, 2);
  for (auto i = 1; i < 20; i++) {
    std::cout << rgen.next() << ",";
  }
  std::cout << std::endl << std::endl;
  auto rgen1 = randomgen::RandUniform(1, 2);
  for (auto i = 1; i < 20; i++) {
    std::cout << rgen1.next() << ",";
  }
  std::cout << std::endl << std::endl;
  auto rgen2 = randomgen::RandUniform(1, 2);
  for (auto i = 1; i < 20; i++) {
    std::cout << rgen2.next() << ",";
  }
  randomgen::setEngineSeed();
  std::cout << std::endl << std::endl << std::endl;
  std::cout << std::endl << std::endl;
  auto rgen3 = randomgen::RandUniform(1, 2);
  for (auto i = 1; i < 20; i++) {
    std::cout << rgen3.next() << ",";
  }
  std::cout << std::endl << std::endl << std::endl;
}

TEST_CASE("randomgen") {
  // seed it to have known start point
  randomgen::setEngineSeed(100);
  randomgen::RandNormal rnorm(5., 1.);
  double r1 = rnorm.next();

  // reseed at same point
  randomgen::setEngineSeed(100);
  randomgen::RandNormal rnorm3(5., 1.);
  double r2 = rnorm3.next();
  // test random normal seeded comes out same
  REQUIRE(r1 == r2);

  // seed randomly
  randomgen::setEngineSeed();
  randomgen::RandNormal rnormA(5., 1.);
  double r3 = rnormA.next();
  double r5 = rnormA.next();
  randomgen::RandNormal rnormB(5., 1.);
  double r4 = rnormB.next();

  REQUIRE(r3 != r4);
}


TEST_CASE("Distance") {
auto c1 = Cluster(1, TVector3(1, 0, 0), 1., Id::ItemType::kEcalCluster);
auto c2 = Cluster(2, TVector3(1, 0, 0), 1., Id::ItemType::kHcalCluster);
 auto p3 = c1.position().Unit()*100.;
  auto p4 = TLorentzVector();
  p4.SetVectM(p3, 1.);
  auto path = std::make_shared<Path>(StraightLine(p4, TVector3(0,0,0)));
  path->addPoint(papas::Position::kEcalIn,c1.position());
  path->addPoint(papas::Position::kHcalIn,c2.position());
  double charge = 1.;
  auto tr = Track(p3, charge, path);

  auto dist1 = Distance( c1, tr);
  REQUIRE(dist1.isLinked());
  auto dist2 = Distance(c2, c1);
  REQUIRE(dist2.isLinked());
}
/*elems = [c1, c2, tr]
for ele in elems:
link_type, link_ok, distance = ruler(ele, ele)
if ele!=tr:
self.assertTrue(link_ok)
elif ele==tr:
self.assertFalse(link_ok)
for ele1, ele2 in itertools.combinations(elems, 2):
link_type, link_ok, distance = ruler(ele1, ele2)
self.assertTrue(link_ok)
link_type, link_ok, distance = ruler(c2, c1)
self.assertEqual(link_type, ('ecal_in','hcal_in'))*/