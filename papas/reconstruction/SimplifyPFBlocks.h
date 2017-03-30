#ifndef SIMPLIFYPFBLOCKS_h
#define SIMPLIFYPFBLOCKS_h

#include "papas/datatypes/DefinitionsCollections.h"
#include "papas/graphtools/DefinitionsNodes.h"
#include "papas/reconstruction/PFBlock.h"
#include <iostream>

namespace papas {
class Event;


  /** simplifyPFBlocks
   /// simplifyPFBlocks takes a collection of PFBlocks and tries to simplify them.
   /// The PFBlocks are selected from the Event using the block identifier subtype.
   /// The function takes each block in turn and looks to see if any links can be dropped.
   /// It then uses the new links to recreate the blocks.
   /// The new blocks are placed in separate blocks collection with subtype 's'.
   /// Note: For any blocks that are not simplified, a copy of the original PFBlock is taken and placed in the simplified
   /// blocks collection. The simplified block collection is therefore complete and free standing.
   
   Usage example:
   @code
   auto& simplifiedblocks = createBlocks();
   auto& history = createHistory();
   void simplifyPFBlocks(m_event, 'r',simplifiedblocks, history);
   //store a pointer to the ouputs into the event
   m_event.addCollection(simplifiedblocks);
   m_event.addHistory(history);  }
   @endcode
   * @param[in]  event contains collections of tracks, clusters and history
   * @param[in] blockSubtype which blocks to use from the Event
   * @param[inout] externally owned simplifiedBlocks structure into which simplified blocks will be added
   * @param[inout] externally owned history structure to which history information will be added
   */
  void simplifyPFBlocks(const Event& event, char blockSubtype, Blocks& simplifiedblocks,
                        Nodes& history);
  /** simplifyPFBlock
   * Does the main work to simplify a block and add to the simplifiedBlocks collection
   * @param[in] edges contains collections of tracks, clusters and history
   * @param[in] block Block which is to be simplified
   * @param[inout] externally owned simplifiedBlocks structure into which simplified blocks will be added
   * @param[inout] externally owned history structure to which history information will be added
  */
  void simplifyPFBlock(const Edges& edges, const PFBlock& block,  Blocks& simplifiedblocks,
                       Nodes& history);

  /** Checks PFBlock and removes unneeded edge links
   * @param[in] block Block which is to be simplified
   */
  Edges findEdgesToUnlink(const PFBlock& block) ;
}  // end namespace papas
#endif /* SimplifyPFBlocks_h */
