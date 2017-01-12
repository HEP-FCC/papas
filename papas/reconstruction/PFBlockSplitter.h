//
//  PFBlcokSplitter.h
//  papas
//
//  Created by Alice Robson on 10/01/17.
//
//

#ifndef PFBlockSplitter_h
#define PFBlockSplitter_h

#include "papas/datatypes/DefinitionsCollections.h"
#include "papas/graphtools/DefinitionsNodes.h"
#include "papas/reconstruction/PFBlock.h"
#include <iostream>

namespace papas {
  class PapasEvent;
  
  /// The PFBlockBuilder takes a PapasEvent and a set of ids (all of which must be in the PapasEvent)
  /// It uses the PapasEvent to access the elements corresponding to each id
  /// It proceeds by creating a collection of all possible edges (between ids)
  /// The blockbuilder is then used to create a set of distinct blocks, and the
  /// blocks look after the corresponding parts of the edges information.
  /// Class to create blocks of connected elements from a PF event eg connected tracs and clusters.
  /// It creates what is needed in order to call BlockBuilder
  class PFBlockSplitter {
  public:
    /** Constructor
     * @param[in]  papasEvent contains collections of tracks, clusters and history
     * @param[in] blockSubtype which blocks to use from the PapasEvent
     * @param[inout] simplifiedblocks structure into which split blocks will be added
     * @param[inout] history structure to which history information will be added
     */
    PFBlockSplitter(const PapasEvent& papasEvent,  Blocks& simplifiedblocks, Nodes& history);
    void splitBlocks(char blockSubtype);
    Blocks& blocks() const{ return m_simplifiedBlocks; };   ///<return the unordered map of the resulting blocks;
    void simplifyBlock(const Edges&, const PFBlock& block);
  private:
    Edges findEdgesToUnlink(const PFBlock& block) const;
     ///take a block and simplify it by removing edges and splitting if appropriate
    const PapasEvent& m_papasEvent;  ///< contains the collections of tracks, clusters and history
    Blocks& m_simplifiedBlocks;     ///< the blocks produced by the blocksplitter, unsplit blocks will be copied across
    Nodes& m_history;     ///< history will be updated to store block creation
  };
}  // end namespace papas
#endif /* PFBlcokSplitter_h */
