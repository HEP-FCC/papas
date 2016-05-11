#include "gtest/gtest.h"
#include "GraphBuilder.h"
#include "Id.h"
#include "Edge.h"

TEST(GraphBuilder, one) {
  
  longId id1 = Id::makeECALClusterId();
  longId id2 = Id::makeHCALClusterId();
  longId id3 = Id::makeTrackId();
  
  longId id4 = Id::makeECALClusterId();
  longId id5 = Id::makeHCALClusterId();
  longId id6 = Id::makeTrackId();
  
  std::vector<longId> ids {id1,id2,id3, id4,id5,id6};
  
  Edge edge =  Edge(id1, id2, false, 0.00023);
  Edge edge1 = Edge(id1, id3, true, 10030.0);
  Edge edge2 = Edge(id2, id3, true, 0.00005);
  
  Edge edge4 = Edge(id4, id5, false, 3.1234);
  Edge edge5 = Edge(id4, id6, true, 0.1234);
  Edge edge6 = Edge(id5, id6, true, 123.0);
  
  std::unordered_map<long long, class Edge> edges;
  
  edges.emplace(edge.key(),  std::move(edge));
  edges.emplace(edge1.key(), std::move(edge1));
  edges.emplace(edge2.key(), std::move(edge2));
  edges.emplace(edge4.key(), std::move(edge4));
  edges.emplace(edge5.key(), std::move(edge5));
  edges.emplace(edge6.key(), std::move(edge6));
  
  ASSERT_EQ(edges.size() , 6UL);
  
  
  
  auto graphbuilder = GraphBuilder(ids, edges);
  
  ASSERT_EQ(graphbuilder.subGraphs().size() , 2UL);
  ASSERT_EQ(graphbuilder.subGraphs()[0].size(),3UL);
  
  //std::cout<<graphbuilder;

}
