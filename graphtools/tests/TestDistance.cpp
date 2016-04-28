//
//  TestDistance.cpp
//  fastsim
//
//  Created by Alice Robson on 18/04/16.
//
//

#include "gtest/gtest.h"
#include "Distance.h"
#include "Identifier.h"
#include "Cluster.h"
#include "Track.h"
#include "TVector3.h"
#include "TLorentzVector.h"
#include "path.h"

TEST(Distance, distance) {
  
  Cluster c1 {10, TVector3(1, 0, 0), 1., Identifier::makeClusterId(fastsim::enumLayer::ECAL, fastsim::enumSubtype::RAW)};
  Cluster c2 {20, TVector3(1, 0, 0), 1., Identifier::makeClusterId(fastsim::enumLayer::HCAL, fastsim::enumSubtype::RAW)};
  
  
  TVector3 p3 = c1.position().Unit()*100.;
  TLorentzVector p4{};
  p4.SetVectM(p3, 1.);
  std::shared_ptr<Path> path =std::make_shared<Path>(p4, TVector3(0,0,0),20.);
  double charge = 1.;
  Track tr{p3, charge, path, 0};
  path->addPoint("_ECALin", c1.position());
  path->addPoint("_HCALin", c2.position());
  
  Distance distc1t(c1, tr);
  EXPECT_EQ(distc1t.distance(),0.);
  EXPECT_TRUE(distc1t.isLinked());
  
  Distance distc2t(c2, tr);
  EXPECT_EQ(distc2t.distance(),0.);
  EXPECT_TRUE(distc2t.isLinked());

  Distance distc2c1(c1, c2);
  EXPECT_TRUE(distc2t.isLinked());
  
  Distance disttt{};
  EXPECT_FALSE(disttt.isLinked());
  
}

