//
//  MergedClusterBuilder.cpp
//  papas
//
//  Created by Alice Robson on 07/06/16.
//
//
#include "MergedClusterBuilder.h"
#include "Cluster.h"
#include "Definitions.h"
#include "DefinitionsCollections.h"
#include "Distance.h"
#include "GraphBuilder.h"
#include "Ruler.h"
#include "Edge.h"
#include "Log.h"
#include "Id.h"


namespace papas {

MergedClusterBuilder::MergedClusterBuilder(const Clusters& clusters, Ruler& ruler, Nodes& historyNodes)
    : m_merged(), m_historyNodes(historyNodes) {
  if (m_historyNodes.empty()) {
    // throw error
  }
  Ids uniqueids;
  for (auto cluster : clusters)
    uniqueids.push_back(cluster.first);

  Edges edges;
  for (auto id1 : uniqueids) {
    for (auto id2 : uniqueids) {
      if (id1 < id2) {
        auto dist = ruler.distance(id1, id2);
        Edge edge{id1, id2,  dist.isLinked(), dist.distance()};
        edges.emplace(edge.key(), std::move(edge));
      }
    }
  }
  GraphBuilder grBuilder{uniqueids, edges};
  for (auto ids : grBuilder.subGraphs()) {
    auto id = ids[0];
    Cluster mergedCluster{clusters.at(id), Id::makeId(Id::itemType(id))};  // create a new cluster based on old one
    PDebug::write("Made Merged{}", mergedCluster);
    m_merged.emplace(mergedCluster.id(), std::move(mergedCluster));  // create a new cluster based on existing cluster
    PFNode snode {mergedCluster.id()};
    m_historyNodes.emplace(mergedCluster.id(), snode);
    m_historyNodes.at(id).addChild(snode);
      if (ids.size() > 1) {
        for (auto elemid : ids) {
          //now add in the links between the block elements and the block into the history_nodes
          if (elemid != id) { // we have already handled the first element
                      m_historyNodes.at(elemid).addChild(snode);

            mergedCluster += clusters.at(id);
          }
        }
      }
    }
  }
  
}  // end namespace papas