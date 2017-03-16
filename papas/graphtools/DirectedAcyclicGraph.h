//
//  DirectedAcyclicGraph.h
//
//
//  Created by Alice Robson on 14/12/15.
//  Copyright © 2015 CERN. All rights reserved.
//

#ifndef DirectedAcyclicGraph_h
#define DirectedAcyclicGraph_h

#include <iostream>
#include <list>
#include <queue>
#include <unordered_set>

/// Directed Acyclic Graph
/**
 * @brief Implementation of Directed Acyclic Graph
 *
 * Supports traversal of a DirectedAcyclicGraph (also known here as DAG or a polytree)
 * implemented using a templated Node alongside algorithms to visit linked nodes
 *
 *  Directed Acyclic Graph:
 *  Nodes may have multiple children and /or multiple parents.
 *  A search may be for children, parents or undirected links.
 *  An undirected search finds all nodes that are connected to the start node(s).
 *
 *  Visitor Pattern:
 *  The visitor pattern is nicely described on WIKI https://en.wikipedia.org/wiki/Visitor_pattern
 *
 *  The key things are that the Node class (or Element) class has an Accept function which calls the
 *  Visitor class visit function.
 *  This structuring allows for possible double dispatch ie polymorphism for both
 *  the node and the visitor class. It also allows the visitor class to work cleanly
 *  without interference in the Node class.
 *  New visitor patterns can be created by the user by deriving from the Visitor class interface (BFSVisitor
 *  is an example of this.
 *
 *  The Node class is templated Node<T>
 *  where T is intended to be either an identifier or the item of interest.
 *  The Node class may not be const, but the thing it contains (T) may be set to be a const object
 *
 *  Examples are provided for the following cases
 *  (i) Long identifier
 *  T is set to a const long identifier which contains encoding information about the items of interest.
 *  The actual item of interest is not contained within nodes and therefore a separate set of collection structures
 *  such as unordered_maps will be required to link between the long identifier and the various collections
 *  For example if the nodes were either tracks or clusters then
 *  we need:- stl::unordered_map<id,cluster>
 *  stl::unordered_map<id,track>
 *  And the user must generate a unique id for each cluster and track
 *  The identifier class provides an example of the encoding and decoding of node information
 *
 *  (ii) Polymorphic classes
 *  T is set to be const &Base and the nodes include Base and Middle (derives from Base) instances.
 *  Note that &Base is used for T in order to avoid copying the classes and in order that polymorphism is available
 *
 * (iii) Boost:Any
 *  Allows direct insertion into the Nodes of any mixed set of class items
 *  T= const Boost::Any&
 *
 */

namespace DAG {

// note internal use of pointer to N (supports the Nodes being concrete objects)
template <typename N>
using Nodeset = std::unordered_set<const N*>;  ///<set of Nodes which allows find and just one of each node
template <typename N>
using Nodevector = std::vector<const N*>;  ///<vector of Nodes typically used to return results
enum class enumVisitType { CHILDREN, PARENTS, UNDIRECTED };

/// Visitor class interface for the DirectedAcyclicGraph
/**Defines the visitor class interface for the DirectedAcyclicGraph
 */
/// @tparam  N is a Node
template <typename N>  // N is the templated Node
class Visitor {
public:
  Visitor();                              ///<Constructor
  virtual void visit(const N* node) = 0;  ///< Key function for visitor pattern

  /// returns vector of all child nodes (including the start node and all children of children)
  virtual const Nodevector<N>& traverseChildren(const N& startnode, int depth) = 0;
  /// returns vector of all parent nodes (including the start node and all parents of parents)
  virtual const Nodevector<N>& traverseParents(const N& startnode, int depth) = 0;
  /// returns everything linked to the start node
  virtual const Nodevector<N>& traverseUndirected(const N& startnode, int depth) = 0;
  virtual const Nodevector<N>& traverseNodes(const N& startnode, DAG::enumVisitType visittype, int depth) = 0;

protected:
};

/// Node class for visitor pattern templated on T
/// @tparam T item of interest
template <typename T>
class Node {
public:
  typedef Node<T> TNode;
  Node(const T& v);  ///< Constructor
  Node();            ///< Needed for putting a Node inside a unordered_set
  // ideally no copying because it means nodes are no longer unique
  TNode& operator=(TNode&) = delete;
  TNode& operator=(const TNode&) = delete;
  // unordered_set requires these to be available
  Node(TNode&) = default;
  Node(const TNode&) = default;
  // Move is good
  TNode& operator=(TNode&& other) = default;
  Node(TNode&& other) = default;

  void accept(Visitor<TNode>& visitor) const;
  void addChild(Node& node);  ///< Add in a link to node (also set the reverse parent link in the other node)
  const T& value() const { return m_val; };  ///< return the node item
  const Nodeset<TNode>& children() const { return m_children; }
  const Nodeset<TNode>& parents() const { return m_parents; }

protected:
  T m_val;                                                 ///< thing that the node is encapsulating (eg identifier )
  Nodeset<TNode> m_children;                               ///< direct child nodes
  Nodeset<TNode> m_parents;                                ///< direct parent nodes
  void addParent(Node& node) { m_parents.insert(&node); }  // private as only available via addChild
};

/// Breadth First Search implementation of BFSVisitor (iterative)
/// @tparam N the Node
template <typename N>
class BFSVisitor : public Visitor<N> {
public:
  BFSVisitor();
  void visit(const N* node) override;  ///< key to visitor pattern
  const Nodevector<N>& traverseChildren(const N& node, int depth = -1) override;
  const Nodevector<N>& traverseParents(const N& node, int depth = -1) override;
  const Nodevector<N>& traverseUndirected(const N& node, int depth = -1) override;
  const Nodevector<N>& traverseNodes(const N& node, DAG::enumVisitType visittype, int depth = -1) override;

protected:
  Nodeset<N> m_visited;    ///< which nodes have been visited (reset each time a traversal is made)
  Nodevector<N> m_result;  ///< the list of nodes that are linked and that will be returned

  /// @enum Whether to seek for linked Children , Parents or both
  // enum class enumVisitType { CHILDREN, PARENTS, UNDIRECTED };  ///< internal enumeration

  virtual void traverse(const DAG::Nodeset<N>& nodes, DAG::enumVisitType visittype, int depth);
  bool alreadyVisited(const N* node) const;
};

/// Breadth First Search alternative implementation using recursion
template <typename N>
class BFSRecurseVisitor : public BFSVisitor<N> {
public:
private:
  /// core traversal code uses by all of the public traversals
  virtual void traverse(const DAG::Nodeset<N>& nodes, typename DAG::enumVisitType visittype, int depth) override;
};

/// Constructor
template <typename T>
Node<T>::Node(const T& v) : m_val(v) {}

/// Constructor
template <typename T>
Node<T>::Node() : m_val(T()) {}

template <typename T>
void Node<T>::addChild(Node& node) {
  // AddChild automatically adds in the parent link - this should be a safer route and avoid
  // missing links
  m_children.insert(&node);
  node.addParent(*this);
}

/**
 accept the visitor
 @param Visitor<TNode>& visitor
 @return void
 */
template <typename T>
void Node<T>::accept(Visitor<TNode>& visitor) const {
  visitor.visit(this);
}

/// Constructor
template <typename N>
Visitor<N>::Visitor() {}

/// Constructor
template <typename N>
BFSVisitor<N>::BFSVisitor() : Visitor<N>(), m_visited() {}

/**
 visit a node - add the node to the results and mark as "visited"
 @param N* node - the node that is to be visited
 @return void
 */
template <typename N>
void BFSVisitor<N>::visit(const N* node) {
  m_result.push_back(node);  // add to result
  m_visited.insert(node);    // mark it as visited
}

template <typename N>
bool BFSVisitor<N>::alreadyVisited(const N* node) const {
  if (m_visited.find(node) == m_visited.end()) return false;
  return true;
}

/**
 traverse the nodes using Breadth First Search implemented using a Queue
 @param Nodeset& nodes - the start node(s)
 @param typename DAG::enumVisitType visittype - CHILDREN/PARENTS/UNDIRECTED
 @param int depth - how many levels to visit (-1 = everything, 0 = start node(s), 2= start node plus 2 levels)
 @return void
 */
template <typename N>
void BFSVisitor<N>::traverse(const Nodeset<N>& nodes, typename DAG::enumVisitType visittype, int depth) {
  typedef typename DAG::enumVisitType pt;

  // Create a queue for the Breadth First Search
  std::queue<const N*> nodeQueue;
  std::queue<int> nodeDepth;  // keeps track of the node depths so we can limit how deep we go if we wish

  // Mark the current node as visited and enqueue it
  for (auto const& node : nodes) {
    if (m_visited.find(node) == m_visited.end()) {  // if node is not listed as already being visited
      node->accept(*this);                          // mark as visited and add to results
      nodeQueue.push(node);                         // put into the queue
      nodeDepth.push(0);
    }
  }

  while (!nodeQueue.empty()) {

    // Get head Node from Queue and iterate its children and/or parents
    // each of the parents and children that are not already visited
    // get put onto the end of the queue
    // One this is done the head Node can be removed (popped)
    // from the queue and processing proceeds to the
    // next item in the queue
    int curdepth = nodeDepth.front();

    if ((depth < 0 || curdepth < depth) &&  // NB depth=-1 means we are visiting everything
        ((visittype == pt::CHILDREN) | (visittype == pt::UNDIRECTED))) {  // use the children
      for (auto node : nodeQueue.front()->children()) {
        if (m_visited.find(node) == m_visited.end()) {  // check node is not already being visited
          node->accept(*this);
          nodeQueue.push(node);
          nodeDepth.push(curdepth + 1);
        }
      }
    }
    if ((depth < 0 || curdepth < depth) &&                               // NB depth=-1 means we are visiting everything
        ((visittype == pt::PARENTS) | (visittype == pt::UNDIRECTED))) {  // use the parents
      for (auto node : nodeQueue.front()->parents()) {

        if (m_visited.find(node) == m_visited.end()) {  // check node is not already being visited
          node->accept(*this);
          nodeQueue.push(node);
          nodeDepth.push(curdepth + 1);
        }
      }
    }
    nodeQueue.pop();
    nodeDepth.pop();
  }
}

/**
 traverse the nodes using Breadth First Search implemented using a recursion
 @param Nodeset<N>& nodes - the start node(s)
 @param typename DAG::enumVisitType visittype - CHILDREN/PARENTS/UNDIRECTED
 @param int depth - how many levels to visit (-1 = everything, 0 = start node(s), 2= start node plus 2 levels)
 @return void
 */
template <typename N>
void BFSRecurseVisitor<N>::traverse(const Nodeset<N>& nodes, typename DAG::enumVisitType visittype, int depth) {
  // For a recursive  breadth first traversal we gather all nodes at the same depth
  typedef typename DAG::enumVisitType pt;
  Nodeset<N> visitnextnodes;  // this collects all the nodes at the next "depth"

  if (nodes.empty()) {
    return;  // end of the recursion
  }

  for (auto node : nodes) {

    // Only process a node if not already visited
    if (BFSVisitor<N>::m_visited.find(node) == BFSVisitor<N>::m_visited.end()) {
      // this will add the node to the "result" and mark the node as visited
      node->accept(*this);

      // Now add in all the children/parent/undirected links for the next depth
      // and store these into visitnextnodes
      // NB depth=-1 means we are visiting everything
      if (depth != 0 && (visittype == pt::CHILDREN | visittype == pt::UNDIRECTED))
        for (const auto child : node->children()) {
          if (!this->alreadyVisited(child)) visitnextnodes.insert(child);
        }
      if (depth != 0 && (visittype == pt::PARENTS | visittype == pt::UNDIRECTED))
        for (const auto parent : node->parents()) {
          if (!this->alreadyVisited(parent)) visitnextnodes.insert(parent);
        }
    }
  }
  depth--;
  traverse(visitnextnodes, visittype, depth);
}

/**
 traverse the children using Breadth First Search
 @param N& startnode
 @param int depth - how many levels to visit (-1 = everything, 0 = start node(s), 2= start node plus 2 levels)
 @return const std::vector<N*>&  results vector of Nodes
 */
template <typename N>
const std::vector<const N*>& BFSVisitor<N>::traverseChildren(const N& startnode, int depth) {
  m_result = {};                // reset the list of results:
  Nodeset<N> root{&startnode};  // create an initial nodeset containing the root node
  traverse(root, DAG::enumVisitType::CHILDREN, depth);
  m_visited = {};  // reset the list of visited nodes
  return m_result;
}

/**
 traverse the parents using Breadth First Search
 @param N& startnode
 @param int depth - how many levels to visit (-1 = everything, 0 = start node(s), 2= start node plus 2 levels)
 @return const std::vector<N*>&  results vector of Nodes
 */
template <typename N>
const std::vector<const N*>& BFSVisitor<N>::traverseParents(const N& startnode, int depth) {
  m_result = {};                // reset the list of results
  Nodeset<N> root{&startnode};  // create an initial nodeset containing the root node
  traverse(root, DAG::enumVisitType::PARENTS, depth);
  m_visited = {};  // reset the list of visited nodes
  return m_result;
}

/**
 traverse all nodes linked to the start node using Breadth First Search
 @param N& startnode
 @param int depth - how many levels to visit (-1 = everything, 0 = start node(s), 2= start node plus 2 levels)
 @return const std::vector<N*>&  results vector of Nodes
 */
template <typename N>
const std::vector<const N*>& BFSVisitor<N>::traverseUndirected(const N& startnode, int depth) {
  m_result = {};                // reset the list of results
  Nodeset<N> root{&startnode};  // create an initial nodeset containing the root node
  traverse(root, DAG::enumVisitType::UNDIRECTED, depth);
  m_visited = {};  // reset the list of visited nodes
  return m_result;
}

/**
 traverse all nodes linked to the start node using Breadth First Search
 @param N& startnode
 @param int depth - how many levels to visit (-1 = everything, 0 = start node(s), 2= start node plus 2 levels)
 @return const std::vector<N*>&  results vector of Nodes
 */
template <typename N>
const std::vector<const N*>& BFSVisitor<N>::traverseNodes(const N& startnode, DAG::enumVisitType direction, int depth) {
  m_result = {};                // reset the list of results
  Nodeset<N> root{&startnode};  // create an initial nodeset containing the root node
  traverse(root, direction, depth);
  m_visited = {};  // reset the list of visited nodes
  return m_result;
}
}

#endif /* DirectedAcyclicGraph */
