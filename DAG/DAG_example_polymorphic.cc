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





//Example base classs
class Base
{
public:
   Base(int i): m_val(i) {};
   
   virtual void Write() const { std::cout << "base: " + std::to_string(m_val) <<std::endl;};
   int m_val;
};

//Example derived class
class Middle: public Base
{
public:
   using Base::Base;
   
   void Write() const override { std::cout << "middle: " + std::to_string(m_val)<<std::endl; };
      };
      
      
      
      //some functions to extract parts of the polytree
      bool is_middle(DAG::Node<const Base&, std::string > node)
      {
         //checks if this is a Middle class instance
         Base& b(const_cast<Base&>((node.getValue())));  //work around const ness
         if (dynamic_cast<Middle*>(&b))
            return true;
         return false;
      }
      
      
      bool match_label(const DAG::Node<Base&, std::string > node,const std::string& match)
      {
         //checks if the Node label contains the match
         int found =node.getLabel().find(match);
         return (found!=std::string::npos);
      }
      
      
      
      int middlemain()
      {
         typedef DAG::Node<const Base&, std::string  > BNode;
         typedef DAG::Visitor<BNode>::Nodeset BNodeset;
         typedef std::shared_ptr<BNode> spNode;
         
         
         
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
         
         // Create some things to go inside each of the 9 nodes
         // Here they are all derived from Base
         Base b0(0);
         Middle m1(1);
         Middle m2(2);
         Middle m3(3);
         Middle m4(4);
         Middle m5(5);
         Middle m6(6);
         Middle m7(7);
         Base b8(8);
         
         //Now wrap each of these objects up into a Node and provide a label for each Node
         //Label is a template parameter so need not be a string
         //
         spNode n0=std::make_shared<BNode>(b0,"particle 0");
         spNode n1=std::make_shared<BNode>(m1,"cluster 1");
         spNode n2=std::make_shared<BNode>(m2,"cluster 2");
         spNode n3=std::make_shared<BNode>(m3,"cluster 3");
         spNode n4=std::make_shared<BNode>(m4,"merged cluster 4");
         spNode n5=std::make_shared<BNode>(m5,"merged cluster 6");
         spNode n6=std::make_shared<BNode>(m6,"smeared 7");
         spNode n7=std::make_shared<BNode>(m7,"smeared 8");
         spNode n8=std::make_shared<BNode>(b8,"particle 8");
         
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
         
         
         //USE NODE 0 AS THE START NODE
         BNodeset m_root = BNodeset({n0});
         
         //BFSVisitor uses an iterative method to traverse
         std::cout<< std::endl<< "TRAVERSE MIDDLE LEAF CHILDREN (starting from 0)  "<< std::endl;
         DAG::BFSVisitor<BNode> bfs;
         for (auto y : bfs.traverseChildren(m_root))
         {
            BNode& bn=*y.get();
            if (y->isLeaf() & is_middle(bn))
               std::cout<< y->getLabel()  << std::endl;
         }
         
         //Find all the node connected (undirected) with node 0
         std::cout<< std::endl<< "POLYMORPHIC TRAVERSE UNDIRECTED (starting from 0)  "<< std::endl;
         for (const auto& n : bfs.traverseUndirected(m_root))
            n->getValue().Write();
         
         
         return 0;
      }
      
      
      
      
      int main()
      {
         middlemain();
         
      }
      
      
