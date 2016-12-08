//
//  TesMergedClusterBuilder.cpp
//  papas
//
//  Created by Alice Robson on 07/06/16.
//
//
#include "papas/reconstruction/TestMergedClusterBuilder.h"
#include "papas/datatypes/Cluster.h"
#include "papas/datatypes/Definitions.h"
#include "papas/datatypes/DefinitionsCollections.h"
#include "papas/graphtools/Distance.h"
#include "papas/graphtools/Edge.h"
#include "papas/graphtools/Ruler.h"
#include "papas/graphtools/GraphBuilder.h"
#include "papas/datatypes/Identifier.h"
#include "papas/utility/PDebug.h"


namespace papas {

    
    TestMergedClusterBuilder::TestMergedClusterBuilder(const Clusters& clusters, const Ruler& ruler,Clusters& merged, Nodes& historyNodes)
    : m_merged(merged), m_historyNodes(historyNodes) {
      bool addHistory = true;
      if (m_historyNodes.empty()) {
        addHistory = false;
      }
      Ids uniqueids;
      uniqueids.reserve(clusters.size());
      for (auto const& cluster : clusters) {
        uniqueids.push_back(cluster.first);
      }
#if WITHSORT
      std::sort(uniqueids.begin(), uniqueids.end());
#endif
      // create unordered map containing all edge combinations indexed by edgeKey
      Edges edges;
      for (auto id1 : uniqueids) {
        for (auto id2 : uniqueids) {
          if (id1 < id2) {
            Distance dist = ruler.clusterClusterDistance(clusters.at(id1), clusters.at(id2));
            Edge edge{id1, id2, dist.isLinked(), dist.distance()};
            Edge::EdgeKey key = edge.key();
            edges.emplace(key, std::move(edge));
          }
        }
      }
      GraphBuilder grBuilder{uniqueids, std::move(edges)};
      for (auto ids : grBuilder.subGraphs()) {
#if WITHSORT
        std::sort(ids.begin(), ids.end());
#endif
        auto id = ids[0];
        double totalenergy = 0.;
        if (ids.size() > 1) {
          for (const auto& c : ids) {
            totalenergy += clusters.at(c).energy();
            PDebug::write("Merged Cluster from Smeared{}",
                          clusters.at(c));  // hmmm not quite right we don't really know it is smeared
          }
        }
        auto mergedCluster =
        Cluster(clusters.at(id), Identifier::itemType(id), 'm', totalenergy);  // create a new cluster based on old one
        if (id == mergedCluster.id()) {
          throw "MergedCluster has same id as existing cluster";
        }
        IdType mid = mergedCluster.id();
        
        if (addHistory) {
          m_historyNodes.emplace(mid, std::move(PFNode(mid)));
          PFNode snode = m_historyNodes[mid];
          m_historyNodes.at(id).addChild(snode);
        }
        if (ids.size() > 1) {
          for (auto elemid : ids) {
            // now add in the links between the block elements and the block into the history_nodes
            if (elemid != id) {  // we have already handled the first element
              if (addHistory) {
                PFNode snode = m_historyNodes[mid];
                m_historyNodes.at(elemid).addChild(snode);
              }
              mergedCluster += clusters.at(elemid);
            }
          }
        }
        if (ids.size() > 1) {
          PDebug::write("Made Merged{}", mergedCluster);
        }
        m_merged.emplace(mid, std::move(mergedCluster));  // create a new cluster based on existing cluster
      }
    }
}  // end namespace papas
