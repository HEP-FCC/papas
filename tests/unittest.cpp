// STL
#include <iostream>
#include <vector>

// catch
#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "Id.h"

TEST_CASE("fastsim") {  /// ID test
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
      REQUIRE(type== e);
      REQUIRE(uid ==n);
    }
  }
}
