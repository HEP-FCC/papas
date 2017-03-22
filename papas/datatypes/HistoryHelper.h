#ifndef HistoryHelper_h
#define HistoryHelper_h

#include "papas/datatypes/DefinitionsCollections.h"
#include "papas/datatypes/IdCoder.h"
#include "papas/datatypes/Event.h"
#include "papas/graphtools/DirectedAcyclicGraph.h"

namespace papas {
/**
 *  @brief The HistoryHelper is used to access history information stored in a Event.
 *  The Event contains history information eg which clusters and tracks were used
 *  to reconstruct a particle. The HistoryHelper allows questions such as -
 *  what simulation particles(s) gave rise to this reconstructed particle, or what was
 *  reconstructed from this simulation particle.
 *
 
 @code
 Usage example:
 HistoryHelper hhelper(event);
 //find what is connected to (say) a reconstructed particle
 auto ids =hhelper.linkedIds(id);
 //filter the connected ids selecting only the ecals of subtype 'm'
 auto fids = hhelper.filteredIds(ids, IdCoder::kEcalCluster, 'm');
 ...
 @endcode
  */
class HistoryHelper {
public:
  /** @brief  Constructor
      @param[in]  event event whose history is to be investigated
  */
  HistoryHelper(const Event& event);
  /**
   *   @brief Finds all ids which have a history link with the input id
   *   @param[in] id identifier for which we want to find connected items
   *   @param[in] direction whether to search parents, children or both
   */
  Ids linkedIds(Identifier id, DAG::enumVisitType direction = DAG::enumVisitType::UNDIRECTED) const;
  /**
   *   @brief Finds all ids which have a history link with the input id and have specified typeAndSubtype
   *   @param[in] id Identifier for which we want to find connected items
   *   @param[in] typeAndSubType The Identifier type and subtype for which we are searching eg "pr"
   for a reconstructed particle, should be a string of length 2
   *   @param[in] direction whether to search parents, children or both
   */
  Ids linkedIds(Identifier id, const std::string& typeAndSubType,
                DAG::enumVisitType direction = DAG::enumVisitType::UNDIRECTED) const;
  /**
   *   @brief  Filters a vector of ids to find a subset which have the required type and subtype
   *         for example could be used to identify all ids which are merged Ecal clusters.
   *   @param[in]  ids vector of identifiers that is to be filtered
   *   @param[in]  type Itemtype for which we are filtering eg IdCoder::kEcalCluster
   *   @param[in]  subtype Subtype for the filtered items eg 'm' for merged
   */
  Ids filteredIds(Ids ids, const IdCoder::ItemType type, const IdCoder::SubType subtype) const;
  /**
   *   @brief  Filters a vector of ids to find a subset which have the required typeAndSubtype
   *         for example could be used to identify all ids which are merged Ecal clusters.
   *   @param[in]  ids vector of identifiers that is to be filtered
   *   @param[in] typeAndSubType The identifier type and subtype which we are filtering on eg "pr"
   */
  Ids filteredIds(Ids ids, const std::string& typeAndSubtype) const;

private:
  const Event& m_event;  ///< Contains pointers to data collections and to history
};
}

#endif /* HistoryHelper_h */