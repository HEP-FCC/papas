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
#include "Edge.h"
#include "GraphBuilder.h"
#include "Id.h"
#include "PDebug.h"
#include "EventRuler.h"

namespace papas {

MergedClusterBuilder::MergedClusterBuilder(const Clusters& clusters, EventRuler& ruler, Nodes& historyNodes)
    : m_merged(), m_historyNodes(historyNodes) {
  if (m_historyNodes.empty()) {
    // throw error
  }
  Ids uniqueids;
  uniqueids.reserve(clusters.size());
      //std::cout << " CSIZE "<< clusters.size()<<std::endl;
  for (auto const &cluster : clusters) {
    // if (cluster.first==8589934920)
    //  std::cout<<"STOPE HERE";
    //PDebug::write(" unique id {} {}", Id::pretty(cluster.first), cluster.second);
    uniqueids.push_back(cluster.first);
  }
  std::sort(uniqueids.begin(), uniqueids.end());

      //create unorederedmap containing all edge combinations indexed by edgeKey
  Edges edges;
  for (auto id1 : uniqueids) {
    for (auto id2 : uniqueids) {
      if (id1 < id2) {
        Distance dist = ruler.distance(id1, id2);
        Edge edge{id1, id2, dist.isLinked(), dist.distance()};
        //PDebug::write("      Add Edge {:9} - {:9}", Id::pretty(id1), Id::pretty(id2));
        Edge::EdgeKey key =edge.key();
        edges.emplace(key, std::move(edge));
      }
    }
  }
  GraphBuilder grBuilder{uniqueids, edges};
  for (auto ids : grBuilder.subGraphs()) {
    std::sort(ids.begin(), ids.end());
    auto id = ids[0];
    if (ids.size() > 1) {
      for (const auto& c : ids) {
        PDebug::write("Merged Cluster from Smeared{}", clusters.at(c));
      }
    }
    // if (id==8589934920)
    //  std::cout<<"STOPE HERE";
    auto mergedCluster =
        Cluster(clusters.at(id), Id::makeId(Id::itemType(id)));  // create a new cluster based on old one
    if (id== mergedCluster.id())
      std::cout <<"problem";
    Id::Type mid =mergedCluster.id();
    PFNode snode{mid};
    m_historyNodes.at(id).addChild(snode);
    m_historyNodes.emplace(mid, std::move(snode));
    if (ids.size() > 1) {
      for (auto elemid : ids) {
        // now add in the links between the block elements and the block into the history_nodes
        if (elemid != id) {  // we have already handled the first element
          m_historyNodes.at(elemid).addChild(snode);

          mergedCluster += clusters.at(elemid);
        }
      }
    }
    if (ids.size() > 1) PDebug::write("Made Merged{}", mergedCluster);
    m_merged.emplace(mid, std::move(mergedCluster));  // create a new cluster based on existing cluster
    
  }
}

}  // end namespace papas