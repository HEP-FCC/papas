#ifndef SIMPLIFYPFBLOCKS_h
#define SIMPLIFYPFBLOCKS_h

#include "papas/datatypes/DefinitionsCollections.h"
#include "papas/graphtools/DefinitionsNodes.h"

namespace papas {
class Event;

/**
 simplifyPFBlocks takes a collection of PFBlocks and tries to simplify them.
 The PFBlocks to be simplified are selected from the Event using the block identifier subtype.
 The goal is to remove, if needed, some links from the block so that each track links to
 at most one hcal within a block. In some cases this may separate a block into smaller
 blocks. The new smaller blocks are added into the externally owned simplifiedBlocks colelctions.
 If a block is unchanged its content will be copied into a new Block with a new Block Id and stored in simplifiedBlocks.
 The history is updated so that the simplified blocks will have the tracks and cluster elements as parents.

 Usage example:
 @code
 auto& simplifiedblocks = createBlocks();
 auto& history = createHistory();
 void simplifyPFBlocks(m_event, 'r',simplifiedblocks, history);
 //store a pointer to the ouputs into the event
 m_event.addCollectionToFolder(simplifiedblocks);
 m_event.addHistory(history);  }
 @endcode
 * @param[in]  event contains collections of tracks, clusters and history
 * @param[in] blockSubtype which blocks to use from the Event
 * @param[inout] simplifiedBlocks externally owned  structure into which simplified blocks will be added
 * @param[inout] history externally owned structure to which history information will be added
 */
void simplifyPFBlocks(const Event& event, char blockSubtype, Blocks& simplifiedblocks, Nodes& history);

/**
 * Does the main work to simplify a block and add to the simplifiedBlocks collection
   The function takes each block in turn and looks to see if any links can be dropped.
   It then uses the new links to create the simplified blocks.
   The new blocks are placed in separate blocks collection with subtype 's'.
   Note: For any blocks that are not simplified, a copy of the original PFBlock is taken and placed in the simplified
    blocks collection. The simplified block collection is therefore complete and free standing.
 * @param[in] edges contains collections of tracks, clusters and history
 * @param[in] block Block which is to be simplified
 * @param[inout] simplifiedBlocks externally owned structure into which simplified blocks will be added
 * @param[inout] history externally owned structure to which history information will be added
*/
void simplifyPFBlock(const Edges& edges, const PFBlock& block, Blocks& simplifiedblocks, Nodes& history);

/** Checks PFBlock and removes unneeded edge links
 * @param[in] block Block which is to be simplified
 */
Edges edgesToUnlink(const PFBlock& block);
}  // end namespace papas
#endif /* SimplifyPFBlocks_h */
