#ifndef PFBlockBuilder_h
#define PFBlockBuilder_h

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
class PFBlockBuilder {
public:
  /** Constructor
   * @param[in]  pfevent contains collections of tracks, clusters and history
   * @param[in] ids  list of element ids from which to construct a block
   */
  PFBlockBuilder(const PapasEvent& m_papasEvent, const std::string& ecalTypeAndSubtype, const std::string& hcalTypeAndSubtype, char trackSubtype,
                Blocks& blocks, Nodes& history);
  Blocks& blocks() { return m_blocks; };   ///<return the unordered map of the resulting blocks;

private:
  void makeBlocks();         ///< does the main work of creating the blocks
  const PapasEvent& m_papasEvent;  ///< contains the collections of tracks, clusters and history
  Blocks& m_blocks;           ///< the blocks made by blockbuilder
  Nodes& m_history;     ///< history will be updated to store block creation
  Ids m_uniqueIds;           ///< list of element ids from which to construct the blocks
};
}  // end namespace papas
#endif /* PFBlockBuilder_hpp */
