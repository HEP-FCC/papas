#include "directedacyclicgraph.h"

class idpair {
public:
  int itype;
  int ivalue;
  idpair(int t,int v) {itype=t; ivalue=v;};
};


int main() {
  
  typedef DAG::Node<const idpair> INode;

  // create a set of nodes
  // the nodes will be kept track of in the Node children and parent collections so
  // shared_ptr is used.
  INode n0(idpair(1,2));
  INode n1(idpair(1,3));
  INode n2(idpair(2,3));
  INode n3(idpair(2,2));
    // and now define the polytree
  // add the directed (parent -> child) branches of the polytree
  // each link requires an addChild and an addParent
  n0.addChild(n1);  // link between n0 and n1
  n0.addChild(n2);  // link between n0 and n2 etc
  n1.addChild(n3);
  
  
  // Start at node 0 for the following examples

  // Example 1
  // BFSVisitor uses an iterative method to traverse
  // output the Datatype of all children
  std::cout << std::endl << "TRAVERSE CHILDREN (start Node 0) " << std::endl;

  DAG::BFSVisitor<INode> bfs;
  for (auto n : bfs.traverseChildren(n0)) {
    std::cout << n->value().itype << ":" <<n->value().ivalue<<std::endl;
  }

    return 0;
}
