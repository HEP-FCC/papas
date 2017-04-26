// catch
#define CATCH_CONFIG_MAIN
#include "tests/catch.hpp"

// C++
#include <iostream>
#include <memory>
#include <numeric>
#include <string>
#include <unordered_map>
#include <vector>

// ROOT
#include "TApplication.h"
#include "TLorentzVector.h"
#include "TVector3.h"

#include "papas/datatypes/Event.h"
#include "papas/datatypes/Helix.h"
#include "papas/datatypes/HistoryHelper.h"
#include "papas/detectors/CMS.h"
#include "papas/detectors/CMSField.h"
#include "papas/detectors/Calorimeter.h"
#include "papas/display/Display.h"
#include "papas/display/GTrajectory.h"
#include "papas/display/ViewPane.h"
#include "papas/graphtools/Distance.h"
#include "papas/graphtools/EventRuler.h"
#include "papas/reconstruction/BuildPFBlocks.h"
#include "papas/reconstruction/MergeClusters.h"
#include "papas/reconstruction/PapasManagerTester.h"
#include "papas/reconstruction/SimplifyPFBlocks.h"
#include "papas/simulation/HelixPropagator.h"
#include "papas/simulation/Simulator.h"
#include "papas/simulation/StraightLinePropagator.h"
#include "papas/utility/TRandom.h"

using namespace papas;

TEST_CASE("IdCoder") {

  auto uid = IdCoder::makeId(1, IdCoder::ItemType::kTrack, 's', 1.23456);
  auto id1 = IdCoder::makeId(2, IdCoder::ItemType::kTrack, 's', 12.782);
  REQUIRE(IdCoder::pretty(id1) == "ts2");
  std::vector<Identifier> idvec;
  for (int i = -2; i < 2; i++) {
    uid = IdCoder::makeId(i + 6, IdCoder::ItemType::kTrack, 's', pow(2, i));
    idvec.push_back(uid);
  }
  std::sort(idvec.begin(), idvec.end(), std::greater<Identifier>());
  REQUIRE(IdCoder::pretty(idvec[0]) == "ts7");
  REQUIRE(IdCoder::value(idvec[0]) == 2.0);
  REQUIRE(IdCoder::pretty(idvec[3]) == "ts4");
  REQUIRE(IdCoder::value(idvec[3]) == 0.25);

  auto id = IdCoder::makeId(1, IdCoder::ItemType::kEcalCluster, 'g', 3.1);
  REQUIRE(IdCoder::subtype(id) == 'g');
  REQUIRE(IdCoder::type('e') == IdCoder::kEcalCluster);

  for (int j = 0; j < 6; j++) {
    IdCoder::ItemType e = IdCoder::ItemType::kEcalCluster;
    for (int i = 1; i < 21; i++) {
      int n = pow(2, i);
      // makeId is self testing
      id = IdCoder::makeId(i, e, 't', (float)(1. / n));
    }
  }
}

TEST_CASE("Helix") {  /// Helix path test
  TLorentzVector p4;
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
  SurfaceCylinder cyl1(papas::Position::kEcalIn, 1., 2.);
  SurfaceCylinder cyl2(papas::Position::kEcalOut, 2., 1.);
  std::shared_ptr<const Field> field = std::make_shared<Field>(CMSField(VolumeCylinder(Layer::kField, 2.9, 3.6), 3.8));
  Particle particle(211, -1, TLorentzVector{2., 0, 1, 5}, 1, 'r', TVector3{0, 0, 0}, 3.8);
  HelixPropagator helixprop(field);
  //(particle.p4(), {0,0,0}, 3.8, -1);
  helixprop.setPath(particle);
  helixprop.propagateOne(particle, cyl1);
  const auto& tvec = particle.path()->namedPoint(cyl1.layer());
  auto particle2 = Particle(211, -1, TLorentzVector{0., 2, 1, 5}, 2, 'r', TVector3{0, 0, 0}, 3.8);
  helixprop.setPath(particle2);
  helixprop.propagateOne(particle2, cyl1);
  const auto& tvec2 = particle2.path()->namedPoint(cyl1.layer());
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

  SurfaceCylinder cyl1(papas::Position::kEcalIn, 1, 2);
  SurfaceCylinder cyl2(papas::Position::kEcalIn, 0.7, 1.5);
  VolumeCylinder subcyl(papas::Layer::kEcal, 1, 2, 0.7, 1.5);
  REQUIRE(subcyl.inner().radius() == 0.7);
  REQUIRE(subcyl.inner().z() == 1.5);
  REQUIRE(subcyl.outer().radius() == 1.);
  REQUIRE(subcyl.outer().z() == 2.);
  REQUIRE_THROWS(VolumeCylinder(papas::Layer::kEcal, 1, 2, 1.1, 1.5));
}

TEST_CASE("ClusterPT") {

  /// Test that pT is correctly set
  Cluster cluster(10., TVector3(1, 0, 0), 1., 1, IdCoder::ItemType::kEcalCluster, 't');
  REQUIRE(cluster.pt() == Approx(10.000));
  cluster.setEnergy(5.);
  REQUIRE(cluster.pt() == Approx(5.000));
}

TEST_CASE("ClusterSmear") {

  // Make a cluster
  double energy = 10.;
  Cluster cluster(energy, TVector3(1, 0, 0), 1., 2, IdCoder::kEcalCluster);
  CMS CMSDetector;
  auto ecal = CMSDetector.ecal();
  PapasManagerTester tester(CMSDetector);

  // smear it 10000 times and store the energy of the smeared cluster
  std::vector<double> energies;

  // run the simulator which will fill the above objects
  Particles particles;
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
  std::shared_ptr<const Field> field = std::make_shared<Field>(CMSField(VolumeCylinder(Layer::kField, 2.9, 3.6), 3.8));
  StraightLinePropagator propStraight(field);
  SurfaceCylinder cyl1(papas::Position::kEcalIn, 1, 2);
  SurfaceCylinder cyl2(papas::Position::kEcalOut, 2, 1);

  TLorentzVector tlv{1, 0, 1, 2.};
  Particle photon(22, 0, tlv, 0, 't');
  propStraight.setPath(photon);
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

  Particle photon2(22, 0, tlv, 1, 't');
  propStraight.setPath(photon2);
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
  Particle photon3(22, 0, tlv, 3, 's', {0, 0, 1.5}, 0.);
  propStraight.setPath(photon3);
  propStraight.propagateOne(photon3, cyl1);
  points = photon3.path()->points();
  REQUIRE(points[papas::Position::kEcalIn].Perp() == Approx(.5));

  // extrapolating from a vertex close to -endcap
  tlv = TLorentzVector(1, 0, -1, 2.);
  Particle photon4(22, 0, tlv, 4, 's', {0, 0, -1.5}, 0.);
  propStraight.setPath(photon4);
  propStraight.propagateOne(photon4, cyl1);
  points = photon4.path()->points();
  REQUIRE(points[papas::Position::kEcalIn].Perp() == Approx(.5));

  // extrapolating from a non-zero radius
  tlv = TLorentzVector(0, 0.5, 1, 2.);
  Particle photon5 = Particle(22, 0, tlv, 5, 's',
                              {
                                  0., 0.5, 0,
                              },
                              0.);
  propStraight.setPath(photon5);
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
  Cluster c1(1, TVector3(1, 0, 0), 1., 1, IdCoder::kEcalCluster, 't');
  Cluster c2(2, TVector3(1, 0, 0), 1., 2, IdCoder::kHcalCluster, 't');
  auto p3 = c1.position().Unit() * 100.;
  TLorentzVector p4;
  p4.SetVectM(p3, 1.);
  auto path = std::make_shared<Path>(StraightLine(p4, TVector3(0, 0, 0)));
  path->addPoint(papas::Position::kEcalIn, c1.position());
  path->addPoint(papas::Position::kHcalIn, c2.position());
  double charge = 1.;
  papas::Track tr(p3, charge, path, 't');
  Distance dist1(c1, tr);
  REQUIRE(dist1.isLinked());
  Distance dist2 = Distance(c2, c1);
  REQUIRE(dist2.isLinked());
}

TEST_CASE("Distance2") {

  Cluster c1(10, TVector3(1, 0, 0), 4., 1, IdCoder::ItemType::kEcalCluster, 't');
  Cluster c2(20, TVector3(1, 0, 0), 4., 2, IdCoder::ItemType::kHcalCluster, 't');
  Distance dist1(c1, c2);
  REQUIRE(dist1.isLinked());
  REQUIRE(dist1.distance() == 0);
  TVector3 pos3(c1.position());
  pos3.RotateZ(0.059);
  Cluster c3(30, pos3, 5., 3, IdCoder::ItemType::kHcalCluster, 't');
  Distance dist2(c1, c3);
  REQUIRE(dist2.isLinked());
  REQUIRE(dist2.distance() == 0.059);
  Distance dist3(c3, c1);
  REQUIRE(dist3.isLinked());
  REQUIRE(dist3.distance() == 0.059);
}

// TODO
void test_graphs() {  // Testing graphics
  Display display({papas::ViewPane::Projection::xy, papas::ViewPane::Projection::yz});
  // Display display =
  // Display({papas::ViewPane::Projection::xy,papas::ViewPane::Projection::yz,papas::ViewPane::Projection::ECAL_thetaphi
  // ,papas::ViewPane::Projection::HCAL_thetaphi });

  TVector3 vpos(1., .5, .3);
  Cluster cluster(10., vpos, 1., 1, IdCoder::ItemType::kEcalCluster, 't');
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
  Identifier id1 = IdCoder::makeId(1, IdCoder::kEcalCluster, 't');
  Identifier id2 = IdCoder::makeId(2, IdCoder::kHcalCluster, 't');
  Identifier id3 = IdCoder::makeId(3, IdCoder::kTrack, 't');

  Edge edge(id1, id2, false, 0.0);
  Edge edge1(id1, id3, true, 0.0);

  REQUIRE(edge1.isLinked() == true);
  REQUIRE(edge.isLinked() == false);
  // NB ids are ordered when stored so may be the opposite way around to the constructor
  REQUIRE(edge.otherId(id1) == id2);
  return;
}

TEST_CASE("PFBlocks") {
  using namespace papas;
  Identifier id1 = IdCoder::makeId(1, IdCoder::kEcalCluster, 't');
  Identifier id2 = IdCoder::makeId(2, IdCoder::kHcalCluster, 't');
  Identifier id3 = IdCoder::makeId(3, IdCoder::kTrack, 't');

  Identifier id4 = IdCoder::makeId(4, IdCoder::kEcalCluster, 't');
  Identifier id5 = IdCoder::makeId(5, IdCoder::kHcalCluster, 't');
  Identifier id6 = IdCoder::makeId(6, IdCoder::kTrack, 't');

  Ids ids({id1, id2, id3});
  Ids ids2({id4, id5, id6});

  Edge edge(id1, id2, false, 0.00023);
  Edge edge1(id1, id3, true, 10030.0);
  Edge edge2(id2, id3, true, 0.00005);

  Edge edge4(id4, id5, false, 3.1234);
  Edge edge5(id4, id6, true, 0.1234);
  Edge edge6(id5, id6, true, 123.0);

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
  REQUIRE(IdCoder::isBlock(block2.id()) == true);
  REQUIRE(block2.edge(edge4.key()).key() == edge4.key());
  REQUIRE_THROWS(block2.edge(edge1.key()).key());
  REQUIRE_THROWS(block2.edge(edge1.key()));
  REQUIRE(block2.edge(id4, id5).isLinked() == false);
  REQUIRE(block2.edge(id4, id6).distance() == 0.1234);

  return;
}

TEST_CASE("BlockSplitter") {
  Identifier id1 = IdCoder::makeId(1, IdCoder::kHcalCluster, 't');
  Identifier id2 = IdCoder::makeId(2, IdCoder::kHcalCluster, 't');
  Identifier id3 = IdCoder::makeId(3, IdCoder::kTrack, 't');
  Ids ids{id1, id2, id3};

  Edge edge(id1, id2, false, 0.00023);
  Edge edge1(id1, id3, true, 10030.0);
  Edge edge2(id2, id3, true, 0.00005);

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
  buildPFBlocks(ids, std::move(edges), 'r', blocks, historyNodes);
  REQUIRE(blocks.size() == 1);

  Edges to_unlink;
  to_unlink[edge1.key()] = edge1;
  Event event(emptyNodes);
  event.addCollectionToFolder(blocks);
  Blocks simplifiedBlocks;
  simplifyPFBlocks(event, 'r', simplifiedBlocks, emptyNodes);
  REQUIRE(simplifiedBlocks.size() == 2);
  return;
}

TEST_CASE("Merge") {
  Cluster cluster1(10., TVector3(0., 1., 0.), 0.04, 1, IdCoder::kEcalCluster, 't');
  Cluster cluster2(20., TVector3(0., 1., 0), 0.06, 2, IdCoder::kEcalCluster, 't');
  Clusters eclusters;
  eclusters.emplace(cluster1.id(), cluster1);
  eclusters.emplace(cluster2.id(), cluster2);
  Clusters mergedClusters;
  Nodes nodes;
  Event event(nodes);
  event.addCollectionToFolder(eclusters);

  papas::EventRuler ruler(event);
  mergeClusters(event, "et", ruler, mergedClusters, nodes);

  REQUIRE(mergedClusters.size() == 1);
  for (auto& mergedCluster : mergedClusters) {
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
  Cluster cluster1(20, TVector3(1, 0, 0), 0.1, 1, IdCoder::kHcalCluster, 't');
  Cluster cluster2(20., TVector3(1, 0.05, 0.), 0.1, 2, IdCoder::kHcalCluster, 't');
  Clusters hclusters;
  hclusters.emplace(cluster1.id(), cluster1);
  hclusters.emplace(cluster2.id(), cluster2);
  Clusters mergedClusters;
  Nodes nodes;
  Event event(nodes);
  event.addCollectionToFolder(hclusters);

  papas::EventRuler ruler(event);
  mergeClusters(event, "ht", ruler, mergedClusters, nodes);

  REQUIRE(mergedClusters.size() == 1);
  return;
}

TEST_CASE("merge_pair_away") {

  Cluster cluster1(20, TVector3(1, 0, 0), 0.04, 1, IdCoder::kHcalCluster, 't');
  Cluster cluster2(20., TVector3(1, 1.1, 0.), 0.04, 2, IdCoder::kHcalCluster, 't');
  Clusters hclusters;
  hclusters.emplace(cluster1.id(), cluster1);
  hclusters.emplace(cluster2.id(), cluster2);
  Clusters mergedClusters;

  Nodes nodes;
  Event event(nodes);
  event.addCollectionToFolder(hclusters);

  papas::EventRuler ruler(event);
  mergeClusters(event, "ht", ruler, mergedClusters, nodes);

  REQUIRE(mergedClusters.size() == 2);
  return;
}

TEST_CASE("merge_different_layers") {

  Cluster cluster1(20, TVector3(1, 0, 0), 0.04, 1, IdCoder::kEcalCluster, 't');
  Cluster cluster2(20., TVector3(1, 1.1, 0.), 0.04, 2, IdCoder::kHcalCluster, 't');
  Clusters hclusters;
  Clusters eclusters;
  hclusters.emplace(cluster1.id(), cluster1);
  hclusters.emplace(cluster2.id(), cluster2);

  Clusters mergedClusters;

  Nodes nodes;
  Event event(nodes);
  REQUIRE_THROWS(event.addCollectionToFolder(hclusters));

  return;
}

TEST_CASE("test_papasevent") {
  Nodes nodes;
  Event event(nodes);
  Clusters ecals;
  Tracks tracks;
  Identifier lastid = 0;
  Identifier lastcluster = 0;

  for (int i = 0; i < 2; i++) {
    Cluster cluster(10., TVector3(0, 0, 1), 2., i, IdCoder::kEcalCluster, 't');
    ecals.emplace(cluster.id(), std::move(cluster));
    lastcluster = cluster.id();
    papas::Track track(TVector3(0, 0, 0), i, std::make_shared<Path>(), 't');
    tracks.emplace(track.id(), std::move(track));
    lastid = track.id();
  }
  event.addCollectionToFolder(ecals);
  event.addCollectionToFolder(tracks);
  // check that adding the same collection twice fails
  REQUIRE_THROWS(event.addCollectionToFolder(ecals));

  // check we can get back collections OK
  REQUIRE(event.clusters(IdCoder::type('e'), 't').size() == 2);
  REQUIRE(event.hasCollection(499) == false);
  REQUIRE(event.hasCollection(IdCoder::kEcalCluster, 't') == true);
  REQUIRE(event.hasCollection(lastid) == true);
  REQUIRE_NOTHROW(event.track(lastid));
  REQUIRE_NOTHROW(event.cluster(lastcluster));
  REQUIRE_THROWS(event.track(500));
  REQUIRE(event.hasObject(499) == false);
  REQUIRE(event.hasObject(lastid) == true);
}

TEST_CASE("test_history") {

  Nodes history;
  Event event(history);
  Clusters ecals;
  Particles particles;
  Identifier lastid = 0;
  Identifier lastcluster = 0;
  // make a dummy papasevent including some history
  for (int i = 0; i < 2; i++) {
    Cluster cluster(10., TVector3(0, 0, 1), 2., 1, IdCoder::kEcalCluster, 't');
    ecals.emplace(cluster.id(), std::move(cluster));
    lastcluster = cluster.id();
    PFNode cnode(lastcluster);
    history.emplace(lastcluster, std::move(cnode));
    Particle particle(22, -1, TLorentzVector(1, 1, 1, 1), 1, 'r', TVector3(0., 0., 0.), 0.7);
    particles.emplace(particle.id(), std::move(particle));
    lastid = particle.id();
    PFNode pnode(lastid);
    history.emplace(lastid, std::move(pnode));
    history.at(lastid).addChild(history.at(lastcluster));
  }
  event.addCollectionToFolder(ecals);
  event.addCollectionToFolder(particles);
  event.extendHistory(history);
  HistoryHelper hhelper(event);
  // find what is connected to the last particle created
  auto ids = hhelper.linkedIds(lastid);
  // filter the ecals from the linked ids
  REQUIRE(ids.size() == 2);
  auto fids = hhelper.filteredIds(ids, IdCoder::kEcalCluster, 't');
  REQUIRE(fids.size() == 1);
  REQUIRE(*fids.begin() == lastcluster);
  // filter the particle from the linked ids
  fids = hhelper.filteredIds(ids, IdCoder::kParticle, 'r');
  REQUIRE(fids.size() == 1);
  REQUIRE(*fids.begin() == lastid);
}

TEST_CASE("merge_inside") {

  Cluster cluster1(20, TVector3(1, 0, 0), 0.055, 1, IdCoder::kHcalCluster, 't');
  Cluster cluster2(20., TVector3(1., 0.1, 0.0), 0.055, 2, IdCoder::kHcalCluster, 't');
  Clusters hclusters;
  hclusters.emplace(cluster1.id(), cluster1);
  hclusters.emplace(cluster2.id(), cluster2);

  Tracks tracks;
  Nodes nodes;
  Event testevent(nodes);
  Clusters mergedClusters;
  testevent.addCollectionToFolder(hclusters);
  papas::EventRuler ruler(testevent);
  mergeClusters(testevent, "ht", ruler, mergedClusters, nodes);
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
