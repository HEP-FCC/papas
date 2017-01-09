#ifndef HistoryHelper_h
#define HistoryHelper_h

#include "papas/datatypes/DefinitionsCollections.h"
#include "papas/datatypes/Identifier.h"
#include "papas/datatypes/PapasEvent.h"
#include "papas/graphtools/DirectedAcyclicGraph.h"


namespace papas {
/**
 *  @brief The HistoryHelper is used to access history information stored in a PapasEvent.
 *  The PapasEvent contains history information eg which clusters and tracks were used
 *  to reconstruct a particle. The HistoryHelper allows questions such as
 *  what simulation particles(s) gave rise to this reconstructed particle, or what was
 *  reconstructed from this simulation particle.
 *
    Usage example:
 @code
 auto hhelper = HistoryHelper(papasEvent);
 //find what is connected to (say) a reconstructed particle
 auto ids =hhelper.linkedIds(id);
 //filter the connected ids selecting only the ecals of subtype 'm'
 auto fids = hhelper.filteredIds(ids, Identifier::kEcalCluster, 'm');
 ...
 @endcode
  */
class HistoryHelper {
public:
  //enum class enumVisitType { CHILDREN, PARENTS, UNDIRECTED };
  /** @brief  Constructor
      @param[in]  papasEvent: papasEvent whose history is to be investigated
  */
  HistoryHelper(const PapasEvent& papasEvent);
  /**
   *   @brief  Finds all ids which have a history link with the input id
   *   @param[in]  id: Identifier for which we want to find connected items
   */
  Ids linkedIds(IdType id, DAG::enumVisitType direction = DAG::enumVisitType::UNDIRECTED) const;
  /**
   *   @brief  Filters a vector of ids to find a subset which have the required type and subtype
   *         for example could be used to identify all ids which are merged Ecal clusters.
   *   @param[in]  ids: vector of identifiers that is to be filtered
   *   @param[in]  type: Itemtype for which we are filtering eg Identifier::kEcalCluster
   *   @param[in]  subtype: Subtype for the filtered items eg 'm' for merged
   */
  Ids linkedIds(IdType id, std::string typeAndSubType, DAG::enumVisitType direction = DAG::enumVisitType::UNDIRECTED) const;
  /**
   *   @brief  Filters a vector of ids to find a subset which have the required type and subtype
   *         for example could be used to identify all ids which are merged Ecal clusters.
   *   @param[in]  ids: vector of identifiers that is to be filtered
   *   @param[in]  type: Itemtype for which we are filtering eg Identifier::kEcalCluster
   *   @param[in]  subtype: Subtype for the filtered items eg 'm' for merged
   */
  Ids filteredIds(Ids ids, const Identifier::ItemType type, const Identifier::SubType subtype) const;
  Ids filteredIds(Ids ids, std::string typeAndSubtype) const;
private:
  const PapasEvent& m_papasEvent;  /// Contains the history
};
}

#endif /* HistoryHelper_h */