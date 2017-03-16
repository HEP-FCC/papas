// STL
#include <iostream>
#include <vector>

// catch
#define CATCH_CONFIG_MAIN

#include "tests/catch.hpp"

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

#include "papas/display/Display.h"
#include "papas/display/GTrajectory.h"
#include "papas/display/ViewPane.h"

// SSD libs
#include "papas/datatypes/Cluster.h"
#include "papas/datatypes/Definitions.h"
#include "papas/datatypes/Event.h"
#include "papas/datatypes/Helix.h"
#include "papas/datatypes/HistoryHelper.h"
#include "papas/datatypes/Identifier.h"
#include "papas/datatypes/Particle.h"
#include "papas/datatypes/Particle.h"
#include "papas/datatypes/Path.h"
#include "papas/datatypes/Track.h"
#include "papas/detectors/CMS.h"
#include "papas/detectors/Material.h"
#include "papas/detectors/SurfaceCylinder.h"
#include "papas/detectors/VolumeCylinder.h"
#include "papas/graphtools/Distance.h"
#include "papas/graphtools/Edge.h"
#include "papas/graphtools/EventRuler.h"
#include "papas/reconstruction/BlockBuilder.h"
#include "papas/reconstruction/MergedClusterBuilder.h"
#include "papas/reconstruction/PFBlockSplitter.h"
#include "papas/reconstruction/PapasManagerTester.h"
#include "papas/simulation/Simulator.h"
#include "papas/utility/TRandom.h"

using namespace papas;

TEST_CASE("Identifier") {

  auto uid = Identifier::makeId(1, Identifier::ItemType::kTrack, 's', 1.23456);
  auto id1 = Identifier::makeId(2, Identifier::ItemType::kTrack, 's', 12.782);
  REQUIRE(Identifier::pretty(id1) == "ts2");
  std::vector<IdType> idvec;
  for (int i = -2; i < 2; i++) {
    uid = Identifier::makeId(i + 6, Identifier::ItemType::kTrack, 's', pow(2, i));
    idvec.push_back(uid);
  }
  std::sort(idvec.begin(), idvec.end(), std::greater<IdType>());
  REQUIRE(Identifier::pretty(idvec[0]) == "ts7");
  REQUIRE(Identifier::value(idvec[0]) == 2.0);
  REQUIRE(Identifier::pretty(idvec[3]) == "ts4");
  REQUIRE(Identifier::value(idvec[3]) == 0.25);

  auto id = Identifier::makeId(1, Identifier::ItemType::kEcalCluster, 'g', 3.1);
  REQUIRE(Identifier::itemType('e') == Identifier::kEcalCluster);

  for (int j = 0; j < 6; j++) {
    Identifier::ItemType e = Identifier::ItemType::kEcalCluster;
    for (int i = 1; i < 21; i++) {
      int n = pow(2, i);
      // makeId is self testing
      id = Identifier::makeId(i, e, 't', (float)(1. / n));
    }
  }
}

TEST_CASE("Helix") {  /// Helix path test
  TLorentzVector p4 = TLorentzVector();
  p4.SetPtEtaPhiM(1, 0, 0, 5.11e-4);
  Helix helix(p4, TVector3(0, 0, 0), 1, 3.8);
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
  auto particle = PFParticle(211, -1, TLorentzVector{2., 0, 1, 5}, 1, 'r', TVector3{0, 0, 0}, field);
  auto helixprop = HelixPropagator(3.8);
  //(particle.p4(), {0,0,0}, 3.8, -1);
  helixprop.propagateOne(particle, cyl1);
  auto tvec = particle.path()->namedPoint(cyl1.layer());
  auto particle2 = PFParticle(211, -1, TLorentzVector{0., 2, 1, 5}, 2, 'r', TVector3{0, 0, 0}, field);
  helixprop.propagateOne(particle2, cyl1);
  auto tvec2 = particle2.path()->namedPoint(cyl1.layer());
  REQUIRE(fabs(tvec.X()) == Approx(fabs(tvec2.Y())));
  REQUIRE(tvec2.Z() == Approx(0.50701872));
}

TEST_CASE("Structures") {
  // testing cylinders etc
  // Try base classes ;
  Material M("test", 1, 1);
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
  auto cluster = Cluster(10., TVector3(1, 0, 0), 1., 1, Identifier::ItemType::kEcalCluster, 't');
  REQUIRE(cluster.pt() == Approx(10.000));
  cluster.setEnergy(5.);
  REQUIRE(cluster.pt() == Approx(5.000));
}

TEST_CASE("ClusterSmear") {

  // Make a cluster
  double energy = 10.;
  auto cluster = Cluster(energy, TVector3(1, 0, 0), 1., 2, Identifier::kEcalCluster);
  auto CMSDetector = CMS();
  auto ecal = CMSDetector.ecal();
  auto tester = PapasManagerTester(CMSDetector);

  // smear it 10000 times and store the energy of the smeared cluster
  std::vector<double> energies;

  // run the simulator which will fill the above objects
  ListParticles particles;
  auto simulator = tester.setSimulator(particles);

  for (int i = 0; i < 10000; i++) {
    auto smeared = simulator.smearCluster(cluster);
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
  PFParticle photon = PFParticle(22, 0, tlv, 1);
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

  PFParticle photon2 = PFParticle(22, 0, tlv, 1);
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
  PFParticle photon3 = PFParticle(22, 0, tlv, 3, 's', {0, 0, 1.5}, 0.);
  propStraight.propagateOne(photon3, cyl1);
  points = photon3.path()->points();
  REQUIRE(points[papas::Position::kEcalIn].Perp() == Approx(.5));

  // extrapolating from a vertex close to -endcap
  tlv = TLorentzVector(1, 0, -1, 2.);
  PFParticle photon4 = PFParticle(22, 0, tlv, 4, 's', {0, 0, -1.5}, 0.);
  propStraight.propagateOne(photon4, cyl1);
  points = photon4.path()->points();
  REQUIRE(points[papas::Position::kEcalIn].Perp() == Approx(.5));

  // extrapolating from a non-zero radius
  tlv = TLorentzVector(0, 0.5, 1, 2.);
  PFParticle photon5 = PFParticle(22, 0, tlv, 5, 's',
                                  {
                                      0., 0.5, 0,
                                  },
                                  0.);
  propStraight.propagateOne(photon5, cyl1);
  points = photon5.path()->points();
  REQUIRE(points[papas::Position::kEcalIn].Perp() == Approx(1.));
  REQUIRE(points[papas::Position::kEcalIn].Z() == Approx(1.));
}

TEST_CASE("TRandomExp") {
  // seed it to have known start point
  rootrandom::Random::seed(100);
  ;
  double r1 = rootrandom::Random::exponential(5.);
  rootrandom::Random::seed(100);
  double r2 = rootrandom::Random::exponential(5.);
  REQUIRE(r1 == r2);

  // use a random start point so should not give same answers
  rootrandom::Random::seed();
  double r3 = rootrandom::Random::exponential(5.);
  double r4 = rootrandom::Random::exponential(5.);

  REQUIRE(r3 != r4);
}

TEST_CASE("dummy") {
  bool success = true;
  REQUIRE(true == success);
}

TEST_CASE("Distance") {
  auto c1 = Cluster(1, TVector3(1, 0, 0), 1., 1, Identifier::kEcalCluster, 't');
  auto c2 = Cluster(2, TVector3(1, 0, 0), 1., 2, Identifier::kHcalCluster, 't');
  auto p3 = c1.position().Unit() * 100.;
  auto p4 = TLorentzVector();
  p4.SetVectM(p3, 1.);
  auto path = std::make_shared<Path>(StraightLine(p4, TVector3(0, 0, 0)));
  path->addPoint(papas::Position::kEcalIn, c1.position());
  path->addPoint(papas::Position::kHcalIn, c2.position());
  double charge = 1.;
  auto tr = Track(p3, charge, path, 't');
  auto dist1 = Distance(c1, tr);
  REQUIRE(dist1.isLinked());
  auto dist2 = Distance(c2, c1);
  REQUIRE(dist2.isLinked());
}

TEST_CASE("Distance2") {
  auto c1 = Cluster(10, TVector3(1, 0, 0), 4., 1, Identifier::ItemType::kEcalCluster, 't');
  auto c2 = Cluster(20, TVector3(1, 0, 0), 4., 2, Identifier::ItemType::kHcalCluster, 't');
  auto dist1 = Distance(c1, c2);
  REQUIRE(dist1.isLinked());
  REQUIRE(dist1.distance() == 0);

  auto pos3 = TVector3(c1.position());
  pos3.RotateZ(0.059);
  auto c3 = Cluster(30, pos3, 5., 3, Identifier::ItemType::kHcalCluster, 't');
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
  Cluster cluster = Cluster(10., vpos, 1., 1, Identifier::ItemType::kEcalCluster, 't');
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
  IdType id1 = Identifier::makeId(1, Identifier::kEcalCluster, 't');
  IdType id2 = Identifier::makeId(2, Identifier::kHcalCluster, 't');
  IdType id3 = Identifier::makeId(3, Identifier::kTrack, 't');

  Edge edge = Edge(id1, id2, false, 0.0);
  Edge edge1 = Edge(id1, id3, true, 0.0);

  REQUIRE(edge1.isLinked() == true);
  REQUIRE(edge.isLinked() == false);
  // NB ids are ordered when stored so may be the opposite way around to the constructor
  REQUIRE(edge.otherId(id1) == id2);
  return;
}

TEST_CASE("PFBlocks") {
  using namespace papas;
  IdType id1 = Identifier::makeId(1, Identifier::kEcalCluster, 't');
  IdType id2 = Identifier::makeId(2, Identifier::kHcalCluster, 't');
  IdType id3 = Identifier::makeId(3, Identifier::kTrack, 't');

  IdType id4 = Identifier::makeId(4, Identifier::kEcalCluster, 't');
  IdType id5 = Identifier::makeId(5, Identifier::kHcalCluster, 't');
  IdType id6 = Identifier::makeId(6, Identifier::kTrack, 't');

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
  REQUIRE(edge.isLinked() == false);

  edges.emplace(edge.key(), std::move(edge));
  edges.emplace(edge1.key(), std::move(edge1));
  edges.emplace(edge2.key(), std::move(edge2));
  edges.emplace(edge4.key(), std::move(edge4));
  edges.emplace(edge5.key(), std::move(edge5));
  edges.emplace(edge6.key(), std::move(edge6));

  REQUIRE_NOTHROW(PFBlock(ids, edges, 'r'));
  PFBlock block2(ids2, edges, 'r');
  REQUIRE(block2.countEcal() == 1);
  REQUIRE(block2.countHcal() == 1);
  REQUIRE(block2.countTracks() == 1);
  REQUIRE(block2.shortName() == "E1H1T1");
  REQUIRE(block2.elementIds() == ids2);
  REQUIRE(block2.size() == 3);
  REQUIRE(Identifier::isBlock(block2.id()) == true);
  REQUIRE(block2.findEdge(edge4.key()).key() == edge4.key());
  REQUIRE_THROWS(block2.findEdge(edge1.key()).key());
  REQUIRE_THROWS(block2.findEdge(edge1.key()));
  REQUIRE(block2.edge(id4, id5).isLinked() == false);
  REQUIRE(block2.edge(id4, id6).distance() == 0.1234);

  return;
}

TEST_CASE("BlockSplitter") {
  IdType id1 = Identifier::makeId(1, Identifier::kHcalCluster, 't');
  IdType id2 = Identifier::makeId(2, Identifier::kHcalCluster, 't');
  IdType id3 = Identifier::makeId(3, Identifier::kTrack, 't');
  Ids ids{id1, id2, id3};

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
  Blocks blocks;
  auto blockbuilder = BlockBuilder(ids, std::move(edges), historyNodes, blocks, 'r');
  REQUIRE(blockbuilder.subGraphs().size() == 1);

  Edges to_unlink;
  to_unlink[edge1.key()] = edge1;
  auto event = Event();
  event.addCollection(blocks);
  Blocks simplifiedBlocks = Blocks();
  auto splitter = PFBlockSplitter(event, 'r', simplifiedBlocks, emptyNodes);
  REQUIRE(simplifiedBlocks.size() == 2);
  return;
}

TEST_CASE("Merge") {
  auto cluster1 = Cluster(10., TVector3(0., 1., 0.), 0.04, 1, Identifier::kEcalCluster, 't');
  auto cluster2 = Cluster(20., TVector3(0., 1., 0), 0.06, 2, Identifier::kEcalCluster, 't');
  Clusters eclusters;
  eclusters.emplace(cluster1.id(), cluster1);
  eclusters.emplace(cluster2.id(), cluster2);
  Clusters mergedClusters;
  Nodes nodes;
  auto event = Event();
  event.addCollection(eclusters);

  auto ruler = papas::EventRuler(event);
  auto builder = MergedClusterBuilder(event, "et", ruler, mergedClusters, nodes);

  REQUIRE(mergedClusters.size() == 1);
  for (auto mergedCluster : mergedClusters) {
    REQUIRE_THROWS(mergedCluster.second.size());         // not valid for merged cluster
    REQUIRE_THROWS(mergedCluster.second.angularSize());  // not valid for merged cluster
    REQUIRE(mergedCluster.second.energy() == 30.);
    REQUIRE(mergedCluster.second.pt() == 30.);
    REQUIRE(mergedCluster.second.subClusters().size() == 2);
    REQUIRE(mergedCluster.second.theta() == 0);
    REQUIRE(mergedCluster.second.position().Z() == 0.);
    return;
  }
}

TEST_CASE("merge_pair") {

  auto cluster1 = Cluster(20, TVector3(1, 0, 0), 0.1, 1, Identifier::kHcalCluster, 't');
  auto cluster2 = Cluster(20., TVector3(1, 0.05, 0.), 0.1, 2, Identifier::kHcalCluster, 't');
  Clusters hclusters;
  hclusters.emplace(cluster1.id(), cluster1);
  hclusters.emplace(cluster2.id(), cluster2);
  Clusters mergedClusters;
  ;
  Nodes nodes;
  auto event = Event();
  event.addCollection(hclusters);

  auto ruler = papas::EventRuler(event);
  auto builder = MergedClusterBuilder(event, "ht", ruler, mergedClusters, nodes);

  REQUIRE(mergedClusters.size() == 1);
  return;
}

TEST_CASE("merge_pair_away") {

  auto cluster1 = Cluster(20, TVector3(1, 0, 0), 0.04, 1, Identifier::kHcalCluster, 't');
  auto cluster2 = Cluster(20., TVector3(1, 1.1, 0.), 0.04, 2, Identifier::kHcalCluster, 't');
  Clusters hclusters;
  hclusters.emplace(cluster1.id(), cluster1);
  hclusters.emplace(cluster2.id(), cluster2);
  Clusters mergedClusters;

  Nodes nodes;
  auto event = Event();
  event.addCollection(hclusters);

  auto ruler = papas::EventRuler(event);
  auto builder = MergedClusterBuilder(event, "ht", ruler, mergedClusters, nodes);

  REQUIRE(mergedClusters.size() == 2);
  return;
}

TEST_CASE("merge_different_layers") {

  auto cluster1 = Cluster(20, TVector3(1, 0, 0), 0.04, 1, Identifier::kEcalCluster, 't');
  auto cluster2 = Cluster(20., TVector3(1, 1.1, 0.), 0.04, 2, Identifier::kHcalCluster, 't');
  Clusters hclusters;
  Clusters eclusters;
  hclusters.emplace(cluster1.id(), cluster1);
  hclusters.emplace(cluster2.id(), cluster2);

  Clusters mergedClusters;

  Nodes nodes;
  auto event = Event();
  REQUIRE_THROWS(event.addCollection(hclusters));

  return;
}

TEST_CASE("test_papasevent") {
  auto event = Event();
  auto ecals = Clusters();
  auto tracks = Tracks();
  IdType lastid = 0;
  IdType lastcluster = 0;

  for (int i = 0; i < 2; i++) {
    auto cluster = Cluster(10., TVector3(0, 0, 1), 2., i, Identifier::kEcalCluster, 't');
    ecals.emplace(cluster.id(), std::move(cluster));
    lastcluster = cluster.id();
    auto track = Track(TVector3(0, 0, 0), i, std::make_shared<Path>(), 't');
    tracks.emplace(track.id(), std::move(track));
    lastid = track.id();
  }
  event.addCollection(ecals);
  event.addCollection(tracks);
  // check that adding the same collection twice fails
  REQUIRE_THROWS(event.addCollection(ecals));

  // check we can get back collections OK
  REQUIRE(event.clusters("et").size() == 2);
  REQUIRE(event.hasCollection(499) == false);
  REQUIRE(event.hasCollection(Identifier::kEcalCluster, 't') == true);
  REQUIRE(event.hasCollection(lastid) == true);
  REQUIRE_NOTHROW(event.track(lastid));
  REQUIRE_NOTHROW(event.cluster(lastcluster));
  REQUIRE_THROWS(event.track(500));
  REQUIRE(event.hasObject(499) == false);
  REQUIRE(event.hasObject(lastid) == true);
}

TEST_CASE("test_history") {
  auto event = Event();
  Nodes history;
  auto ecals = Clusters();
  auto particles = Particles();
  IdType lastid = 0;
  IdType lastcluster = 0;

  // make a dummy papasevent including some history
  for (int i = 0; i < 2; i++) {
    auto cluster = Cluster(10., TVector3(0, 0, 1), 2., 1, Identifier::kEcalCluster, 't');
    ecals.emplace(cluster.id(), std::move(cluster));
    lastcluster = cluster.id();
    auto cnode = PFNode(lastcluster);
    history.emplace(lastcluster, std::move(cnode));
    auto particle = PFParticle(22, -1, TLorentzVector(1, 1, 1, 1), 1, 'r', TVector3(0., 0., 0.), 0.7);
    particles.emplace(particle.id(), std::move(particle));
    lastid = particle.id();
    auto pnode = PFNode(lastid);
    history.emplace(lastid, std::move(pnode));
    history.at(lastid).addChild(history.at(lastcluster));
  }
  event.addCollection(ecals);
  event.addCollection(particles);
  event.extendHistory(history);
  auto hhelper = HistoryHelper(event);
  // find what is connected to the last particle created
  auto ids = hhelper.linkedIds(lastid);
  // filter the ecals from the linked ids
  REQUIRE(ids.size() == 2);
  auto fids = hhelper.filteredIds(ids, Identifier::kEcalCluster, 't');
  REQUIRE(fids.size() == 1);
  REQUIRE(*fids.begin() == lastcluster);
  // filter the particle from the linked ids
  fids = hhelper.filteredIds(ids, Identifier::kParticle, 'r');
  REQUIRE(fids.size() == 1);
  REQUIRE(*fids.begin() == lastid);
}

TEST_CASE("merge_inside") {

  auto cluster1 = Cluster(20, TVector3(1, 0, 0), 0.055, 1, Identifier::kHcalCluster, 't');
  auto cluster2 = Cluster(20., TVector3(1., 0.1, 0.0), 0.055, 2, Identifier::kHcalCluster, 't');
  Clusters hclusters;
  hclusters.emplace(cluster1.id(), cluster1);
  hclusters.emplace(cluster2.id(), cluster2);

  Tracks tracks;
  Nodes nodes;
  Event testevent;
  Clusters mergedClusters;
  testevent.addCollection(hclusters);
  auto ruler = papas::EventRuler(testevent);
  auto builder = MergedClusterBuilder(testevent, "ht", ruler, mergedClusters,nodes);
  REQUIRE(mergedClusters.size() == 1);
  /*for (auto c : mergedClusters) {
  //  REQUIRE(c.second.isInside(TVector3(1, 0.06, 0))); }
  */
 return;
}

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