#ifndef RECONSTRUCTION_BUILDPFBLOCKS_H
#define RECONSTRUCTION_BUILDPFBLOCKS_H

#include "papas/datatypes/DefinitionsCollections.h"
#include "papas/graphtools/DefinitionsNodes.h"
#include <string>

namespace papas {
class Event;

/**
Takes collections of tracks and clusters from an event and calculates the
distances and links (edges) between the elements. These are then used to construct subgraphs of
connected objects. Blocks of connected items, PFBlocks,are created from the subgraphs.
* @param[in] event Contains ecals, hcals, tracks
* @param[in] ecalTypeAndSubtype which ecals collection to use eg 'em' for merged ecals
* @param[in] hcalTypeAndSubtype which hcals collection to use
* @param[in] trackSubtype which tracks collection to use, eg 's' for smeared
* @param[inout] blocks external collection into which new blocks will be added
* @param[inout] history external collection of Nodes to which parent child relations can be added
*/
void buildPFBlocks(const Event& event, const std::string& ecalTypeAndSubtype, const std::string& hcalTypeAndSubtype,
                   char trackSubtype, Blocks& blocks, Nodes& history);

/**
 * Takes a list of Ids and an unordered map of associated edges which have distance and link info
 * It uses the distances/links between elements to construct a set of connected blocks.
 * Each element will end up in one (and only one) block.
 * Blocks retain information of the elements and the distances between elements.
 * The blocks can be used for future particle reconstruction.
 * The ids must be unique and are expected to come from the IdCoder class.
 Usage example:
 @code
 buildPFBlocks(ids, edges, history, blocks, 'r');
 @endcode
 * @param[in] ids list of identifiers eg of tracks, clusters etc
 * @param[in] edges unordered_map of edges which contains all edges between the ids (and maybe more)
 *            an edge records the distance and links between two ids
 * @param[in] subtype the subtype (eg 'r') which will be given to newly created blocks
 * @param[inout] blocks external collection into which new blocks will be added
 * @param[inout] history external collection of Nodes to which parent child relations can be added

 */
void buildPFBlocks(const Ids& ids, Edges&& edges, char subtype, Blocks& blocks, Nodes& history);

}  // end namespace papas
#endif /* BUILDPFBLOCKS_h */
