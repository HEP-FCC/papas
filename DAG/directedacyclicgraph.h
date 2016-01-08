//
//  template_tree.h
//
//
//  Created by Alice Robson on 14/12/15.
//  Copyright © 2015 CERN. All rights reserved.
//

/**
 * @file polytree.h
 * @brief Implementation of Polytree (Driected Acyclic Graph)
 */
/**
    Supports traversal of a DirectedAcyclicGraph (also know here as DAG or a polytree) 
    implemented using a templated Node alongside a  visitor pattern

    Directed Acyclic Graph
    Nodes may have multiple children and /or multiple parents
    A search may be for children, parents or undirected links.
    An undirected search finds all nodes that are connected to the start node(s).
 
    Visitor Pattern
    The visitor pattern is nicely described on WIKI https://en.wikipedia.org/wiki/Visitor_pattern
 
    The key things are that the Node class (or Element) class has an Accept function which calls the
    Visitor class visit function.
    This structuring allows for possible double dispatch ie both polymorphism for both
    the node and the visitor class. It also allows the visitor class to work cleanly
    without interference in the Node class.
    New visitor patterns can be created by the user by deriving from the Visitor class interface (BFSVisitor
    is an example of this.
 
    The Node class is templated Node<T, L>
    where T is intended to be eitehr an identifier or the item of interest and L an optional additional label or id
 
    Examples are provided for the following cases
    (i) Long identifier
        T is set to a long identifier which contains encoding information about the items of interest.
        The actual item of interest is not contained within nodes and therefore a separate set of collection structures
        such as unordered_maps will be required to link between the long identifier and the various collections
        For example if the nodes were either tracks or clusters then
            we need:- stl::unordered_map<id,cluster>
                      stl::unordered_map<id,track>
        And the user must generate a unique id for each cluster and track
        The identifier class provides an example of the encoding and decoding of node information
 
    (ii) Polymorphic classes
         T is set to be &Base and the nodes include Middle (derives from Base) and Base instances.
         Note that &Base is used for T in order to avoid copying the classes and in order that polymorphism is available
 
     (iii) Boost:Any
         Allows direct insertion into the Nodes of any mixed set of class items.
         T=Boost::Any&
 
     Other implementation details
     The visitor patterns have been implemented using shared_ptr<Node> because each Node also contains an unordered_set of Nodes. The user must therefore provide the Nodes to the visitor pattern as shared_ptr to Node.
     The visitor pattern accept has been slightly modified to allow the shared_ptr to Node to be returned to the Visitor.
 
 */

#ifndef DirectedAcyclicGraph_h
#define DirectedAcyclicGraph_h


#include <iostream>
#include <unordered_set>
#include <list>
#include <queue>
#include  <boost/enable_shared_from_this.hpp>



///DirectedAcyclicGraph Namespace
namespace DAG {
   
   template <typename T, typename L=int> class Node;
   
   template<typename T>
   void setLink(std::shared_ptr<T> p,std::shared_ptr<T> c)
   {
      p->addChild(c);
      c->addParent(p);
   }
   
   ///Visitor interface
   /**Defines the visitor class interface for the DirectedAcyclicGraph     */
   template <typename T>
   class Visitor //Base Class/Interface
   {
   public:
      typedef std::shared_ptr<T> spT; //
      typedef std::unordered_set<spT> Nodeset; //Allows find, no repeat nodes
      typedef std::vector<spT> Nodevector;
      Visitor();
      virtual void visit(spT node) = 0; // Key function
      virtual const Nodevector&  traverseChildren(const Nodeset& nodes)=0;
      virtual const Nodevector&  traverseParents(const Nodeset& nodes)=0;
      virtual const Nodevector&  traverseUndirected(const Nodeset& nodes)=0;
   protected:
   };
   
   
   template <typename T, typename L> //AJRQUERY implementation of label L, is this too complex?
   class Node
   {
   public:
      typedef Node<T,L> TNode;
      typedef std::shared_ptr<TNode> spNode;
      typedef std::unordered_set<spNode> Nodeset; //Allows find with no repeat nodes
      
      Node(T v,L label);
      Node(T v); //for case when L is not used try partial 
      void accept(Visitor<TNode>& visitor, spNode me); //Key function, modified to deal with shared pointer
      
      const T& getValue() const                     { return m_val;};          // return the thing
      const L& getLabel() const                     { return m_label;};        // return an optional label
      
      void setChildren(const Nodeset& nodes) { m_children=nodes; }
      void setParents(const Nodeset& nodes)  { m_parents=nodes; }
      void addChild( spNode spnode)          { m_children.insert(spnode); }
      void addParent( spNode spnode)         { m_parents.insert(spnode); }
      Nodeset getChildren() const            { return m_children; }
      Nodeset getParents() const             { return m_parents; }
      bool isLeaf() const                    { return m_children.size()==0;}
      bool isRoot() const                    { return m_parents.size()==0;}
      
   private:
      //T& m_val; was not easy with a T=long
      T m_val; // thing that the node is encapsulating (eg a cluster, track or particle)
      L m_label;// an identifier of some sort, allowing various types of node to be selected
      Nodeset m_children;
      Nodeset m_parents;
   };
   
   
   //Breadth First Search implementation of BFSVisitor (iterative)
   template <typename T>
   class BFSVisitor : public Visitor<T>
   {
   public:
      typedef std::shared_ptr<T> spT;
      typedef std::unordered_set<spT> Nodeset; // shared pointer
      typedef std::vector<spT> Nodevector ; // TODO check list vs vector
      
      BFSVisitor();
      void visit(spT node) override;
      
      const  Nodevector& traverseChildren(const Nodeset& nodes)   override;
      const  Nodevector& traverseParents(const Nodeset& nodes)    override;
      const  Nodevector& traverseUndirected(const Nodeset& nodes) override;
      
   protected:
      Nodeset    m_visited; //which nodes have been visited (reset each time a traversal is made)
      Nodevector m_result;  //the list of nodes that are linked and that will be returned
      enum class enumVisitType {CHILDREN,PARENTS,UNDIRECTED};
      virtual void traverse(const Nodeset& nodes, BFSVisitor<T>::enumVisitType visittype) ; //the iterative method
      
   };
   
   
   //Breadth First Search alternative implementation using recursion
   template <typename T>
   class BFSRecurseVisitor : public BFSVisitor<T>
   {
   public:
   private:
      virtual void traverse(const typename BFSVisitor<T>::Nodeset & nodes,
                            typename BFSVisitor<T>::enumVisitType visittype) override;
   };
   
   
   template<typename T, typename L>
   Node<T,L>::Node(T v, L label)
   :m_val(v), m_label(label)
   {
      
   }
   
   template<typename T, typename L>
   Node<T,L>::Node(T v)
   :m_val(v)
   {
      
   }
   
   template<typename T, typename L>
   void Node<T,L>::accept(Visitor<TNode>& visitor, spNode me)
   { visitor.visit(me); //don't know how to get *this back to the shared_ptr
   } ;
   
   template<typename T>
   Visitor<T>::Visitor()
   {
   }
   
   template<typename T>
   BFSVisitor<T>::BFSVisitor()
   : Visitor<T>(),m_visited()
   {
   }
   
   template<typename T>
   void BFSVisitor<T>::visit( std::shared_ptr<T> spnode)
   {
      m_result.push_back(spnode); //add to result
      m_visited.insert(spnode);   //mark it as visited
   }
   
   
   template<typename T>
   void BFSVisitor<T>::traverse(const Nodeset& nodes,  typename BFSVisitor<T>::enumVisitType visittype)
   {
      typedef typename BFSVisitor<T>::enumVisitType pt;
      
      // Create a queue for the Breadth First Search
      std::queue<spT> nodeQueue;
      
      // Mark the current node as visited and enqueue it
      for (auto node : nodes)
      {
         if (m_visited.find(node)==m_visited.end()) //if node is not listed as already being visited
         {
            node->accept(*this, node); //mark as visited and add to results
            nodeQueue.push(node); // put into the queue
         }
      }
      
      while(!nodeQueue.empty())
      {
         
         // Get head Node from Queue and iterate its children and/or parents
         // each of the parents and children that are not already visited
         // get put onto the end of the queue
         // One this is done the head Node can be removed (popped)
         // from the queue and processing proceeds to the
         // next item in the queue
         if (visittype==pt::CHILDREN| visittype==pt::UNDIRECTED) //use the children
         {
            for(auto node : nodeQueue.front()->getChildren())
            {
               if (m_visited.find(node)==m_visited.end()) //check node is not already being visited
               {
                  node->accept(*this, node);
                  nodeQueue.push(node);
               }
            }
         }
         if (visittype==pt::PARENTS| visittype==pt::UNDIRECTED) //use the parents
         {
            for(auto node : nodeQueue.front()->getParents())
            {
               if (m_visited.find(node)==m_visited.end()) //check node is not already being visited
               {
                  node->accept(*this, node);
                  nodeQueue.push(node);
               }
            }
         }
         nodeQueue.pop();
      }
   }
   
   
   template<typename T>
   void BFSRecurseVisitor<T>::traverse(const typename BFSVisitor<T>::Nodeset& nodes, typename BFSVisitor<T>::enumVisitType visittype)
   {
      //For a recursive  breadth first traversal we gather all nodes at the same depth
      //the
      typedef typename BFSVisitor<T>::enumVisitType pt;
      //typedef typename BFSVisitor<T>::enumVisitType pt;
      typename BFSVisitor<T>::Nodeset visitnextnodes; //this collects all the nodes at the next "depth"
      
      if (nodes.empty()) {
         return; //end of the recursion
      }
      
      for ( auto node : nodes) {
         
         //Only process a node if not already visited
         if (BFSVisitor<T>::m_visited.find(node)==BFSVisitor<T>::m_visited.end())
         {
            //this will add the node to the "result" and mark the node as visited
            node->accept(*this, node);
            
            //Now add in all the children/parent/undirected links for the next depth
            //and store these into visitnextnodes
            if (visittype==pt::CHILDREN| visittype==pt::UNDIRECTED)
               visitnextnodes.insert(node->getChildren().begin(), node->getChildren().end());
            
            if (visittype==pt::PARENTS| visittype==pt::UNDIRECTED) //Add in parents to layer to visit
               visitnextnodes.insert(node->getParents().begin(), node->getParents().end());
         }
      }
      traverse(visitnextnodes,visittype);
   }
   
   
   
   template<typename T>
   const std::vector<std::shared_ptr<T>>& BFSVisitor<T>::traverseChildren(const Nodeset& nodes)
   {
      m_result={}; //reset the list of results:
      traverse(nodes, BFSVisitor<T>::enumVisitType::CHILDREN);
      m_visited={};  //reset the list of visited nodes
      return m_result;
   }
   
   
   template<typename T>
   const std::vector<std::shared_ptr<T>>& BFSVisitor<T>::traverseParents(const Nodeset& nodes)
   {
      m_result={};//reset the list of results
      traverse(nodes,BFSVisitor<T>::enumVisitType::PARENTS);
      m_visited={}; //reset the list of visited nodes
      return m_result;
   }
   
   template<typename T>
   const std::vector<std::shared_ptr<T>>& BFSVisitor<T>::traverseUndirected(const Nodeset& nodes)
   {
      m_result={}; //reset the list of results
      traverse(nodes,BFSVisitor<T>::enumVisitType::UNDIRECTED);
      m_visited={}; //reset the list of visited nodes
      return m_result;
   }
   
}



#endif /* DirectedAcyclicGraph */
