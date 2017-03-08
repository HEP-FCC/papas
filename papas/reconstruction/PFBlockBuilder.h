#ifndef PFBlockBuilder_h
#define PFBlockBuilder_h

#include "papas/datatypes/DefinitionsCollections.h"
#include "papas/graphtools/DefinitionsNodes.h"
#include "papas/reconstruction/PFBlock.h"
#include <iostream>

namespace papas {
class Event;

/// The PFBlockBuilder takes a Event and a set of ids (all of which must be in the Event)
/// It uses the Event to access the elements corresponding to each id
/// It proceeds by creating a collection of all possible edges (between ids)
/// The blockbuilder is then used to create a set of distinct blocks, and the
/// blocks look after the corresponding parts of the edges information.
class PFBlockBuilder {
public:
  /** Constructor
   * @param[in]  event contains collections of tracks, clusters and history
   * @param[in]  ecalTypeAndSubtype which ecal clusters to use from the Event
   * @param[in]  hcalTypeAndSubtype which hcal clusters to use from the Event
   * @param[in]  trackSubtype which tracks to use from the Event
   * @param[inout] blocks structure into which new blocks will be added
   * @param[inout] history structure to which history information will be added
   */
  PFBlockBuilder(const Event& m_event, const std::string& ecalTypeAndSubtype,
                 const std::string& hcalTypeAndSubtype, char trackSubtype, Blocks& blocks, Nodes& history);
private:
  void makeBlocks();               ///< does the main work of creating the blocks
  const Event& m_event;  ///< contains the collections of tracks, clusters and history
  Blocks& m_blocks;                ///< the externally owned collection that new blocks will be added to by blockbuilder
  Nodes& m_history;                ///< history will be updated to record block creation
  Ids m_uniqueIds;                 ///< list of element ids from which to construct the blocks
};

}  // end namespace papas
#endif /* PFBlockBuilder_hpp */
