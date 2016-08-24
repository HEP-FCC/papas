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
#include "Definitions.h"
#include "Distance.h"
#include "Edge.h"
#include "Helix.h"
#include "Id.h"
#include "Material.h"
#include "PParticle.h"
#include "Path.h"
#include "SimParticle.h"
#include "Simulator.h"
#include "SurfaceCylinder.h"
#include "VolumeCylinder.h"
#include "pTrack.h"

#include "BlockSplitter.h"
#include "PapasManager.h"
#include "Random.h"
#include "PFEvent.h"
#include "MergedClusterBuilder.h"
using namespace papas;

TEST_CASE("Id") {  /// ID test
  using namespace papas;
  IdType id(0);
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
}

TEST_CASE("Cylinder") {

  auto cyl1 = SurfaceCylinder(papas::Position::kEcalIn, 1, 2);
  auto cyl2 = SurfaceCylinder(papas::Position::kEcalIn, 0.7, 1.5);
  auto subcyl = VolumeCylinder(papas::Layer::kEcal, 1, 2, 0.7, 1.5);

  REQUIRE(subcyl.inner().radius() == 0.7);
  REQUIRE(subcyl.inner().z() == 1.5);
  REQUIRE(subcyl.outer().radius() == 1.);
  REQUIRE(subcyl.outer().z() == 2.);
  REQUIRE_THROWS(VolumeCylinder(papas::Layer::kEcal, 1, 2, 1.1, 1.5));
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

  REQUIRE(energies.size() == 10000);
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
  long uid = Id::makeId(Id::ItemType::kParticle);
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
  uid = Id::makeId(Id::ItemType::kParticle);
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
  randomgen::RandExponential rexpB(5.);
  double r4 = rexpB.next();

  REQUIRE(r3 != r4);
}

TEST_CASE("dummy") {
  bool success = true;
  REQUIRE(true == success);
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
  auto p3 = c1.position().Unit() * 100.;
  auto p4 = TLorentzVector();
  p4.SetVectM(p3, 1.);
  auto path = std::make_shared<Path>(StraightLine(p4, TVector3(0, 0, 0)));
  path->addPoint(papas::Position::kEcalIn, c1.position());
  path->addPoint(papas::Position::kHcalIn, c2.position());
  double charge = 1.;
  auto tr = Track(p3, charge, path);

  auto dist1 = Distance(c1, tr);
  REQUIRE(dist1.isLinked());
  auto dist2 = Distance(c2, c1);
  REQUIRE(dist2.isLinked());
}

TEST_CASE("Distance2") {
  auto c1 = Cluster(10, TVector3(1, 0, 0), 4., Id::ItemType::kEcalCluster);
  auto c2 = Cluster(20, TVector3(1, 0, 0), 4., Id::ItemType::kHcalCluster);
  auto dist1 = Distance(c1, c2);
  REQUIRE(dist1.isLinked());
  REQUIRE(dist1.distance() == 0);

  auto pos3 = TVector3(c1.position());
  pos3.RotateZ(0.059);
  auto c3 = Cluster(30, pos3, 5., Id::ItemType::kHcalCluster);
  auto dist2 = Distance(c1, c3);
  REQUIRE(dist2.isLinked());
  REQUIRE(dist2.distance() == 0.059);
  auto dist3 = Distance(c3, c1);
  REQUIRE(dist3.isLinked());
  REQUIRE(dist3.distance() == 0.059);
}

// TODO
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

TEST_CASE("Edges") {
  using namespace papas;
  IdType id1 = Id::makeEcalId();
  IdType id2 = Id::makeHcalId();
  IdType id3 = Id::makeTrackId();

  Edge edge = Edge(id1, id2, false, 0.0);
  Edge edge1 = Edge(id1, id3, true, 0.0);

  REQUIRE(edge1.isLinked() == TRUE);
  REQUIRE(edge.isLinked() == FALSE);
  // NB ids are ordered when stored so may be the opposite way around to the constructor
  REQUIRE(((edge1.id1() == id1 && edge1.id2() == id2) || (edge1.id2() == id1 && edge1.id1() == id2)) == FALSE);
  return;
}

TEST_CASE("PFBlocks") {
  using namespace papas;
  IdType id1 = Id::makeEcalId();
  IdType id2 = Id::makeHcalId();
  IdType id3 = Id::makeTrackId();

  IdType id4 = Id::makeEcalId();
  IdType id5 = Id::makeHcalId();
  IdType id6 = Id::makeTrackId();

  Ids ids{id1, id2, id3};
  Ids ids2{id4, id5, id6};

  Edge edge = Edge(id1, id2, false, 0.00023);
  Edge edge1 = Edge(id1, id3, true, 10030.0);
  Edge edge2 = Edge(id2, id3, true, 0.00005);

  Edge edge4 = Edge(id4, id5, false, 3.1234);
  Edge edge5 = Edge(id4, id6, true, 0.1234);
  Edge edge6 = Edge(id5, id6, true, 123.0);

  Edges edges;
  REQUIRE(edge1.distance() == 10030);
  REQUIRE(edge.isLinked() == FALSE);

  edges.emplace(edge.key(), std::move(edge));
  edges.emplace(edge1.key(), std::move(edge1));
  edges.emplace(edge2.key(), std::move(edge2));
  edges.emplace(edge4.key(), std::move(edge4));
  edges.emplace(edge5.key(), std::move(edge5));
  edges.emplace(edge6.key(), std::move(edge6));

  REQUIRE_NOTHROW(PFBlock(ids, edges));
  PFBlock block2(ids2, edges);
  REQUIRE(block2.countEcal() == 1);
  REQUIRE(block2.countHcal() == 1);
  REQUIRE(block2.countTracks() == 1);
  REQUIRE(block2.shortName() == "E1H1T1");
  REQUIRE(block2.isActive() == TRUE);
  REQUIRE(block2.elementIds() == ids2);
  REQUIRE(block2.size() == 3);
  REQUIRE(Id::isBlock(block2.uniqueId()) == TRUE);
  REQUIRE(block2.findEdge(edge4.key()).key() == edge4.key());
  REQUIRE_THROWS(block2.findEdge(edge1.key()).key());
  REQUIRE_THROWS(block2.findEdge(edge1.key()));
  REQUIRE(block2.edge(id4, id5).isLinked() == FALSE);
  REQUIRE(block2.edge(id4, id6).distance() == 0.1234);

  return;
}

TEST_CASE("BlockSplitter") {
  IdType id1 = Id::makeEcalId();
  IdType id2 = Id::makeHcalId();
  IdType id3 = Id::makeTrackId();

  std::vector<Id::Type> ids{id1, id2, id3};

  Edge edge = Edge(id1, id2, false, 0.00023);
  Edge edge1 = Edge(id1, id3, true, 10030.0);
  Edge edge2 = Edge(id2, id3, true, 0.00005);

  Edges edges;

  edges.emplace(edge.key(), std::move(edge));
  edges.emplace(edge1.key(), std::move(edge1));
  edges.emplace(edge2.key(), std::move(edge2));

  // create history nodes
  Nodes historyNodes;
  for (auto id : ids)
    historyNodes.emplace(id, std::move(PFNode(id)));

  Nodes emptyNodes;
  auto blockbuilder = BlockBuilder(ids, edges, historyNodes);
  REQUIRE(blockbuilder.subGraphs().size() == 1);

  Edges to_unlink;
  to_unlink[edge1.key()] = edge1;
  for (auto& block : blockbuilder.moveBlocks()) {
    auto blocksplitter = BlockSplitter(to_unlink, block.second, emptyNodes);
    REQUIRE(blocksplitter.blocks().size() == 2);
  }

  return;
}

TEST_CASE("Merge") {

  auto cluster1 = Cluster(10., TVector3(0., 1., 0.), 0.04, Id::kEcalCluster);
  auto cluster2 = Cluster(20., TVector3(0., 1., 0), 0.06, Id::kEcalCluster);
  Clusters eclusters;
  eclusters.emplace(cluster1.id(), cluster1);
  eclusters.emplace(cluster2.id(), cluster2);
  Clusters hclusters;
  Tracks tracks;
  Nodes nodes;
  auto testevent = papas::PFEvent (eclusters,hclusters,tracks,nodes);
  auto ruler = papas::EventRuler(testevent);
  auto builder = MergedClusterBuilder (eclusters, ruler, nodes);
  REQUIRE(builder.mergedClusters().size() ==1);
  for(auto mergedCluster : builder.mergedClusters() ) {
  REQUIRE_THROWS(mergedCluster.second.size());         // not valid for merged cluster
  REQUIRE_THROWS(mergedCluster.second.angularSize());  // not valid for merged cluster
  REQUIRE(mergedCluster.second.energy() == 30.);
  REQUIRE(mergedCluster.second.pt() == 30.);
  REQUIRE(mergedCluster.second.subClusters().size() == 2);
  REQUIRE(mergedCluster.second.theta() ==0);
  REQUIRE(mergedCluster.second.position().Z() == 0.);
  return;
}
}

TEST_CASE("merge_pair") {
  
  auto cluster1 = Cluster(20, TVector3(1, 0, 0), 0.1, Id::kHcalCluster);
  auto cluster2 = Cluster(20., TVector3(1, 0.05, 0.), 0.1, Id::kHcalCluster);
  Clusters hclusters;
  hclusters.emplace(cluster1.id(), cluster1);
  hclusters.emplace(cluster2.id(),cluster2);
  Clusters eclusters;
  Tracks tracks;
  Nodes nodes;
  auto testevent = papas::PFEvent (eclusters,hclusters,tracks,nodes);
  auto ruler = papas::EventRuler(testevent);
  auto builder = MergedClusterBuilder (hclusters, ruler, nodes);
  REQUIRE(builder.mergedClusters().size() ==1);
  return;
}

TEST_CASE("merge_pair_away") {
  
  auto cluster1 = Cluster(20, TVector3(1, 0, 0), 0.04, Id::kHcalCluster);
  auto cluster2 = Cluster(20., TVector3(1, 1.1, 0.), 0.04, Id::kHcalCluster);
  Clusters hclusters;
  hclusters.emplace(cluster1.id(), cluster1);
  hclusters.emplace(cluster2.id(),cluster2);
  Clusters eclusters;
  Tracks tracks;
  Nodes nodes;
  auto testevent = papas::PFEvent (eclusters,hclusters,tracks,nodes);
  auto ruler = papas::EventRuler(testevent);
  auto builder = MergedClusterBuilder (hclusters, ruler, nodes);
  REQUIRE(builder.mergedClusters().size() ==2);
  return;
}

TEST_CASE("merge_different_layers") {
  
  auto cluster1 = Cluster(20, TVector3(1, 0, 0), 0.04, Id::kEcalCluster);
  auto cluster2 = Cluster(20., TVector3(1, 1.1, 0.), 0.04, Id::kHcalCluster);
  Clusters hclusters;
  Clusters eclusters;
  hclusters.emplace(cluster1.id(), cluster1);
  hclusters.emplace(cluster2.id(),cluster2);
  
  Tracks tracks;
  Nodes nodes;
  auto testevent = papas::PFEvent (eclusters,hclusters,tracks,nodes);
  auto ruler = papas::EventRuler(testevent);
  auto builder = MergedClusterBuilder (hclusters, ruler, nodes);
  REQUIRE(builder.mergedClusters().size() ==2);
 
  return;
}

/*
TEST_CASE("merge_inside") {
  
  auto cluster1 = Cluster(20, TVector3(1, 0, 0), 0.055, Id::kEcalCluster);
  auto cluster2 = Cluster(20., TVector3(1.,0.1, 0.0), 0.055, Id::kEcalCluster);
  Clusters hclusters;
  Clusters eclusters;
  hclusters.emplace(cluster1.id(), cluster1);
  hclusters.emplace(cluster2.id(),cluster2);
  
  Tracks tracks;
  Nodes nodes;
  auto testevent = papas::PFEvent (eclusters,hclusters,tracks,nodes);
  auto ruler = papas::EventRuler(testevent);
  auto builder = MergedClusterBuilder (hclusters, ruler, nodes);
  REQUIRE(builder.mergedClusters().size() ==1);
  for (auto c : builder.mergedClusters())
    REQUIRE(c.second.isInside(TVector3(1, 0.06, 0));
  
  return;
}*/

/*TEST_CASE("merge_pair_away"
  auto cluster1 = Cluster(20, TVector3(1,0,0), 0.04, Id::kHcalCluster);
  auto cluster2 = Cluster(20, TVector3(1,1.1,0.0), 0.04, Id::kHcalCluster);
  auto id = cluster1.id();
  auto mergedCluster = Cluster(cluster1,Id::makeId(Id::itemType(id)));

clusters = [ Cluster(20, TVector3(1,0,0), 0.04, 'hcal_in'),
            Cluster(20, TVector3(1,1.1,0.0), 0.04, 'hcal_in')]
merge_clusters(clusters, 'hcal_in')
self.assertEqual( len(clusters), 2 )
self.assertEqual( len(clusters[0].subclusters), 1)
self.assertEqual( len(clusters[1].subclusters), 1)

def test_merge_different_layers(self):
clusters = [ Cluster(20, TVector3(1,0,0), 0.04, 'ecal_in'),
            Cluster(20, TVector3(1,0,0), 0.04, 'hcal_in')]
merge_clusters(clusters, 'hcal_in')
self.assertEqual( len(clusters), 2)

def test_inside(self):
clusters = [ Cluster(20, TVector3(1, 0, 0), 0.055, 'hcal_in'),
            Cluster(20, TVector3(1.,0.1, 0.0), 0.055, 'hcal_in')]
merged_clusters = merge_clusters(clusters, 'hcal_in')
self.assertEqual( len(merged_clusters), 1 )
cluster = merged_clusters[0]
self.assertEqual( (True, 0.), cluster.is_inside(TVector3(1, 0 , 0)) )
self.assertEqual( (True, 0.), cluster.is_inside(TVector3(1, 0.1, 0)) )
in_the_middle = cluster.is_inside(TVector3(1, 0.06, 0))
self.assertTrue(in_the_middle[0])
self.assertAlmostEqual(in_the_middle[1], 0.04000)
self.assertFalse( cluster.is_inside(TVector3(1, 0.156, 0))[0]  )
*/