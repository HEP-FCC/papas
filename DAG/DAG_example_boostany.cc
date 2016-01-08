//
//  template_tree.cpp
//  polytree
//
//  Created by Alice Robson on 14/12/15.
//  Copyright © 2015 Alice Robson. All rights reserved.
//

#include "directedacyclicgraph.h"

#include <boost/type_index.hpp>
#include <boost/any.hpp>
#include <algorithm>
#include "utility/identifier.h"





int boostmain()
{
   typedef DAG::Node<boost::any&, std::string> BNode;
   typedef DAG::Visitor<BNode>::Nodeset BNodeset;
   typedef std::shared_ptr<BNode> spNode;
   BNodeset m_root;
   
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
   
   // Create some things to go inside the nodes
   // wrapping them into a boost::any
   boost::any ma0(0); //an integer inside a boost::any
   boost::any ma1(Middle(1));
   boost::any ma2(Middle(2));
   boost::any ma3(Middle(3));
   boost::any ma4(Middle(4));
   boost::any ma5(Middle(5));
   boost::any ma6(Middle(6));
   boost::any ma7(Middle(7));
   boost::any ma8(8);
   
   //Now wrap each of these objects up into a Node and provide a label for each Node
   spNode n0=std::make_shared<BNode>(ma0, "0");
   spNode n1=std::make_shared<BNode>(ma1, "1");
   spNode n2=std::make_shared<BNode>(ma2, "2");
   spNode n3=std::make_shared<BNode>(ma3, "3");
   spNode n4=std::make_shared<BNode>(ma4, "4");;
   spNode n5=std::make_shared<BNode>(ma5, "5");
   spNode n6=std::make_shared<BNode>(ma6, "6");
   spNode n7=std::make_shared<BNode>(ma7, "7");
   spNode n8=std::make_shared<BNode>(ma8, "8");
   
   // and now add the directed (parent -> child) branches of the polytree
   DAG::setLink<BNode>(n0,n1);
   DAG::setLink<BNode>(n0,n2);
   DAG::setLink<BNode>(n0,n3);
   DAG::setLink<BNode>(n1,n4);
   DAG::setLink<BNode>(n1,n5);
   DAG::setLink<BNode>(n1,n6);
   DAG::setLink<BNode>(n8,n7);
   DAG::setLink<BNode>(n7,n4);
   DAG::setLink<BNode>(n3,n6);
   
   //start from node 0
   m_root = BNodeset({n0});
   
   //choose a BFS visitor
   DAG::BFSVisitor<BNode> bfs;
   
   std::cout<< std::endl;
   std::cout<< "BOOST TRAVERSE UNDIRECTED (starting from 0)  "<< std::endl;

   //unravel the results
   for (auto n : bfs.traverseUndirected(m_root))
   {
      if (n->getValue().type()==typeid(int)) {
         std::cout<< "int " <<boost::any_cast<int> (n->getValue())  << std::endl;
      }
      if (n->getValue().type()==typeid(Middle)) {
         std::cout<< "Middle " << boost::any_cast<Middle>((n->getValue())).getValue() << std::endl;
      }
   }
   
   std::cout<<  "END"  << std::endl;
   return 0;
   
}




int main()
{
   lboostmain();
   
}


