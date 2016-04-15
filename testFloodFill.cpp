//
//  testFloodFill.cpp
//  fastsim
//
//  Created by Alice Robson on 15/04/16.
//
//

#include "directedacyclicgraph.h"
#include "FloodFill.h"
#include <unordered_map>

int test_FloodFill()
{
  typedef DAG::Node<long> PFNode;
  typedef std::unordered_map<long, PFNode> Nodes;
  
  Nodes myNodes;
  long id1, id2;
  id1=1;
  id2=2;
  DAG::FloodFill<long> FFill;
  myNodes.emplace(id1, PFNode(id1));
  myNodes.emplace(id2, PFNode(id2));
  myNodes[id1].addChild(myNodes[id2]);
  
  for (const auto & nodevector : FFill.traverse(myNodes)) {
    std::cout << "Block: ";
    for (auto &node : nodevector) {
      std::cout<< node->getValue()<< ", " ;
    }
  }
  std::cout<< std::endl;
  return 0;
}
