#ifndef PFBlockSplitter_h
#define PFBlockSplitter_h

#include "papas/datatypes/DefinitionsCollections.h"
#include "papas/graphtools/DefinitionsNodes.h"
#include "papas/reconstruction/PFBlock.h"
#include <iostream>

namespace papas {
class PapasEvent;

/// The PFBlockSplitter takes a collection of PFBlocks and tries to simplify them
/// The PFBlocks are seleced from the PapasEvent using the selected block identifier subtype
/// The PFBlockSplitter takes each block in turn and looks to see if any links can be dropped
/// It then uses the new links to recreate the blocks
/// The new blocks are placed in separate blocks collection with subtype 's'.
/// Note: For any blocks that are not simplified, a copy of the original PFBlock is taken and placed in the simplified
/// blocks collection. The simplified block collection is therefore complete and free standing.
/**
Usage example:
@code
 auto& simplifiedblocks = createBlocks();
 auto& history = createHistory();
 auto blockBuilder = PFBlockSplitter(m_papasEvent, blockSubtype, simplifiedblocks, history);

 //store a pointer to the ouputs into the papasEvent
 m_papasEvent.addCollection(simplifiedblocks);
 m_papasEvent.addHistory(history);  }
@endcode
*/
class PFBlockSplitter {
public:
  /** Constructor
   * @param[in]  papasEvent contains collections of tracks, clusters and history
   * @param[in] blockSubtype which blocks to use from the PapasEvent
   * @param[inout] externally owned simplifiedBlocks structure into which split blocks will be added
   * @param[inout] externally owned history structure to which history information will be added
   */
  PFBlockSplitter(const PapasEvent& papasEvent, char blockSubType, Blocks& simplifiedBlocks, Nodes& history);
  /** Does the main work to simplify a block and add to the simplifiedBlocks collection
   * @param[in] edges contains collections of tracks, clusters and history
   * @param[in] block Block which is to be simplified
 
  */
  void simplifyBlock(const Edges& edges, const PFBlock& block);
private:
  Edges findEdgesToUnlink(const PFBlock& block) const; ///< returns list of edges in block that can be unlinked
  const PapasEvent& m_papasEvent;  ///< contains the collections of tracks, clusters and history
  Blocks& m_simplifiedBlocks;      ///< the blocks produced by the blocksplitter are added into this collection
  Nodes& m_history;                ///< history will be updated to store block creation
};
}  // end namespace papas
#endif /* PFBlockSplitter_h */
