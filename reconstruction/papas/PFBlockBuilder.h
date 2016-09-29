#ifndef PFBlockBuilder_h
#define PFBlockBuilder_h

#include "DefinitionsCollections.h"
#include "DefinitionsNodes.h"
#include "PFBlock.h"
#include <iostream>

namespace papas {
class PFEvent;

/// The PFBlockBuilder takes a PFEvent and a set of ids (all of which must be in the PFEvent)
/// It uses the PFEvent to access the elements corresponding to each id
/// It proceeds by creating a collection of all possible edges (between ids)
/// The blockbuilder is then used to create a set of distinct blocks, and the
/// blocks look after the corresponding parts of the edges information.
/// Class to create blocks of connected elements from a PF event eg connected tracs and clusters.
/// It creates what is needed in order to call BlockBuilder
class PFBlockBuilder {
public:
  /** Constructor
   * @param[in]  pfevent contains collections of tracks, clusters and historyNodes
   * @param[in] ids  list of element ids from which to construct a block
   */
  PFBlockBuilder(const Ids& ids, PFEvent& pfevent); //PFevent history will be updated
  Blocks blocks() { return std::move(m_blocks); };  ///<return the unordered map of the resulting blocks;

private:
  void makeBlocks();      ///< does the main work of creating the blocks
  Blocks m_blocks;        ///< the blocks made by blockbuilder
  const PFEvent& m_pfEvent;     ///< contains the collections of tracks, clusters and historyNodes
  Nodes& m_historyNodes;  ///< history will be updated to store block creation
  Ids m_uniqueIds;        ///< list of element ids from which to construct the blocks
  // void sortIds(Ids& ids); //TODO ? sorts elements by type
};
}  // end namespace papas
#endif /* PFBlockBuilder_hpp */
