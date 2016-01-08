//
//  template_tree.cpp
//  polytree
//
//  Created by Alice Robson on 14/12/15.
//  Copyright © 2015 CERN. All rights reserved.
//

#include <boost/type_index.hpp>
#include <boost/any.hpp>
#include <algorithm>
#include "utility/identifier.h"
#include "DAG/directedacyclicgraph.h"



long makeNewRecCluster() {return Identifier::makeIdentifier(identifier::enumPFObjectType::CLUSTER,
                                                            identifier::enumLayer::TRACKER,
                                                            identifier::enumSubtype::SMEARED,
                                                            identifier::enumSource::RECONSTRUCTION);}

long makeNewSimTrack() {return Identifier::makeIdentifier(identifier::enumPFObjectType::TRACK,
                                                          identifier::enumLayer::ECAL,
                                                          identifier::enumSubtype::SMEARED,
                                                          identifier::enumSource::SIMULATION);;}


int idmain()
{
   //Construct a DirectedAcyclic Graph DAG
   //In which the nodes are identifeid by a (unique) long which
   // has been created vis the Identifier class and thus allows the user to check
   // various details of the associated object eg CLUSTER vs PARTICLE vs TRACK or ECAL vs HCAL etc
   //The details of these can be adjusted by creating a custom Identifier class
   
   /*  Construct an example polytree
    Here 0 and 8 are top level parents
    0 has children 1, 2, 3 etc
    
    
    ///   8
    ///    \
    ///     \
    ///      7
    ///       \
    ///        \
    ///         4
    ///        /
    ///       /
    ///      1--5
    ///     / \
    ///    /   \
    ///   0--2  6
    ///    \   /
    ///     \ /
    ///      3
    */
   
   //Now wrap each of these objects up into a Node and provide a label for each Node
   //Label is a template parameter so need not be a string
   
   
   
   typedef DAG::Node<const long> BNode; // the second argument is not used in this case
   typedef DAG::Visitor<BNode>::Nodeset BNodeset;
   typedef std::shared_ptr<BNode> spNode;
   
   // create a set of nodes
   // the nodes will be kept track of in the Node children and parent collections so
   // shared_ptr is used.
   spNode n0=std::make_shared<BNode>(makeNewSimTrack());
   spNode n1=std::make_shared<BNode>(makeNewSimTrack());
   spNode n2=std::make_shared<BNode>(makeNewSimTrack());
   spNode n3=std::make_shared<BNode>(makeNewRecCluster());
   spNode n4=std::make_shared<BNode>(makeNewRecCluster());
   spNode n5=std::make_shared<BNode>(makeNewRecCluster());
   spNode n6=std::make_shared<BNode>(makeNewSimTrack());
   spNode n7=std::make_shared<BNode>(makeNewSimTrack());
   spNode n8=std::make_shared<BNode>(makeNewSimTrack());
   
   // and now define the polytree
   // add the directed (parent -> child) branches of the polytree
   DAG::setLink<BNode>(n0,n1);
   DAG::setLink<BNode>(n0,n2);
   DAG::setLink<BNode>(n0,n3);
   DAG::setLink<BNode>(n1,n4);
   DAG::setLink<BNode>(n1,n5);
   DAG::setLink<BNode>(n1,n6);
   DAG::setLink<BNode>(n8,n7);
   DAG::setLink<BNode>(n7,n4);
   DAG::setLink<BNode>(n3,n6);
   
   
   //Start at node 0 for the following examples
   BNodeset m_root = BNodeset({n0});// make this just a single node (shared_ptr to Node)
   //Example 1
   //BFSVisitor uses an iterative method to traverse
   //output the PFObjecttype of all children
   
   std::cout<<std::endl<< "TRAVERSE CHILDREN ITERATIVE (starting from 0)  "<< std::endl;
   DAG::BFSVisitor<BNode> bfs;
   for (auto n : bfs.traverseChildren(m_root)) //vector shared_ptr Node
      std::cout<<  to_str(Identifier::getPFObjectType(n->getValue()) ) << std::endl;
   
   
   //Example 2
   //Find all the nodes connected (undirected) with node 0
   //output their ids, source and whether they are leaf or root in the graph
   
   std::cout<< std::endl<< "TRAVERSE UNDIRECTED (starting from 0)  "<< std::endl;
   for (auto n : bfs.traverseUndirected(m_root))
   {
      std::cout << Identifier::getUniqueID(n->getValue()) << ": " << to_str(Identifier::getSource(n->getValue()))  ;
      if (n->isLeaf())
         std::cout<<  " LEAF"   ;
      else if (n->isRoot())
         std::cout<<  " ROOT"   ;
      std::cout<< std::endl;
   }
   
   
   //Example 3
   //This is a recursive version which only outputs leaf nodes
   
   std::cout<< std::endl<< "TRAVERSE LEAF CHILDREN RECURSIVE   "<< std::endl;
   DAG::BFSRecurseVisitor<BNode> bfsrecursive;
   for (auto n : bfs.traverseUndirected(m_root))
   {
      if (n->isLeaf())
         std::cout<<  Identifier::getUniqueID(n->getValue())   << std::endl;
   }
   return 0;
}






int main()
{
   idmain();
   return 0;
}


