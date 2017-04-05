#ifndef Event_h
#define Event_h

#include "papas/datatypes/DefinitionsCollections.h"
#include "papas/datatypes/IdCoder.h"
#include "papas/graphtools/DefinitionsNodes.h"
#include "papas/reconstruction/PFBlock.h"

namespace papas {
/**
 *  @brief The Event stores pointers to collections of Clusters, Tracks, Blocks, Particles in its
 *  internal ClusterCollections, Track Collections etc. It also contains a shared pointer to a  history object
 *  which records the historiccal connections between the objects in the Event.
 *
 *  The Event is a lightweight obejct that can be used from Papas Standalone or from
 *  Gaudi modules.
 *    The collections stored in the Event are unordered_maps of unordered_maps eg an unordered map of Clusters.
 *    The collections are indexed by the typeAndSubtype of the identifiers of each item
 *    (eg of each Cluster in Clusters).
 *       Therefore each collection to be stored must contain only one typeAndSubtype
*       @code
 *       Examples of typeAndSubtype are:
 *       "es" ecal-smeared
 *       "ps" particle-simulated
 *       etc see also IdCoder class
 *      The types are
 *        e = ecal
 *        h = hcal
 *        p = particle
 *        t = track
 *        b = block (particle flow block)
 *      The subtypes may include
 *        t = true
 *        s = smeared or simulated
 *        g = generated
 *        r = reconstructed
 *        m = merged
 Usage example:
  m_event.addCollection(ecalClusters);
  m_event.addCollection(hcalClusters);
  ...
  const Cluster& cluster1 = m_event.cluster(id1);
 @endcode
 *
 * @author  Alice Robson
 * @date    2016-12-01
 */
class Event {
public:
  /// @brief  Constructor
  Event(std::shared_ptr<Nodes> hist = std::make_shared<Nodes>(Nodes()));
  
  /**
   *   @brief  adds a pointer to a Clusters collection (unordered map) into the Event
   *   @param[in]  clusters unordered map of Clusters, all of which have the same identifier typeAndSubtype.
   *               The typeAndSubtype will be used as the map index, eg "em" for ecals-merged.
   */
  void addCollection(const Clusters& clusters);
  
  /**
   *   @brief  adds a pointer to a Tracks collection (unordered map) into the Event
   *   @param[in]  tracks unordered map of Tracks, all of which have the same identifier typeAndSubtype
   *               The typeAndSubtype will be used as the map index, eg "tt" for track-true.
   */
  void addCollection(const Tracks& tracks);
  
  /**
   *   @brief  adds a pointer to a Blocks collection (unordered map) into the Event
   *   @param[in]  blocks unordered map of Blocks, all of which have the same identifier typeAndSubtype
   *               The typeAndSubtype will be used as the map index, eg "br" for blocks-raw.
   */
  void addCollection(const Blocks& blocks);
  
  /**
   *   @brief  adds a pointer to a  Particles collection (unordered map) into the Event
   *   @param[in]  blocks unordered map of Particles, all of which have the same identifier typeAndSubtype
   *               The typeAndSubtype will be used as the map index, eg "pr" for particles-reconstructed.
   */
  void addCollection(const Particles& particles);
  
  /**
   *   @brief  makes history in Event point to an external history object
   *   @param[in]  history unordered map of Nodes,    *
   */
  void setHistory(Nodes& history) { m_history = std::make_shared<Nodes>(history); }
  
  /**
   *   @brief adds new history into existing papasevent history
   *
   */
  void extendHistory(const Nodes& history);
  
  /**
   *   @brief  returns true if a collection with  type and subtype  of id  is found
   *   @param[in]  id the identifier of an object
   */
  bool hasCollection(Identifier id) const;
  
  /**
   *   @brief  returns true if a collection with matching type and subtype  is found
   *   @param[in]  type The type of an object eg IdCoder::kEcalCluster
   *   @param[in]  subtype The subtype of an object eg 'm' for merged
   */
  bool hasCollection(const IdCoder::ItemType type, const IdCoder::SubType subtype) const;
  
  /**
   *   @brief  returns true if an object matching the identifier is found in the Event collections
   *   @param[in]  id the identifier of an object
   */
  bool hasObject(Identifier id) const;
  
  /**
   *   @brief  returns a Clusters collection with typeAndSubtype that match the identifier
   *   @param[in]  id the identifier of an object
   */
  const Clusters& clusters(Identifier id) const;
  
  /**
   *   @brief  returns a Clusters collection matching type and subtype
   *   @param[in]  type The type of an object eg IdCoder::kEcalCluster
   *   @param[in]  subtype The subtype of an object eg 'm' for merged
   */
  const Clusters& clusters(const IdCoder::ItemType type, const IdCoder::SubType subtype) const;
  
  /**
   *   @brief  returns a Clusters collection matching type and subtype as a string
   *   @param[in]  typeAndSubtype The type and subtype of an object as a string eg "em" for ecals-merged.
   */
  const Clusters& clusters(const std::string& typeAndSubtype) const;
  
  /**
   *   @brief  returns a Cluster with the required id
   *   @param[in]  id the identifier of an object
   */
  const Cluster& cluster(Identifier id) const { return clusters(id).at(id); };

  /**
   *   @brief  returns a Tracks collection matching type and subtype
   *   @param[in]  subtype The subtype of the track eg 's' for smeared
   */
  const Tracks& tracks(const IdCoder::SubType subtype) const;
  
  /**
   *   @brief  returns a Tracks collection with typeAndSubtype that match the identifier
   *   @param[in]  id the identifier of an object
   */
  const Tracks& tracks(Identifier id) const { return tracks(IdCoder::subtype(id)); };
  
  /**
   *   @brief  returns a Track with the required id
   *   @param[in]  id the identifier of an object
   */
  const Track& track(Identifier id) const { return tracks(id).at(id); };
  
  /**
   *   @brief  returns a Particles collection matching type and subtype
   *   @param[in]  subtype The subtype of the particle eg 'r' for reconstructed
   */
  const Particles& particles(const IdCoder::SubType subtype) const;
  
  /**
   *   @brief  returns a Particles collection with typeAndSubtype that match the identifier
   *   @param[in]  id the identifier of an object
   */
  const Particles& particles(Identifier id) const { return particles(IdCoder::subtype(id)); };
  
  /**
   *   @brief  returns a Particle with the required id
   *   @param[in]  id the identifier of an object
   */
  const Particle& particle(Identifier id) const { return particles(id).at(id); };
  
  /**
   *   @brief  returns a Blocks collection matching type and subtype
   *   @param[in]  subtype The subtype of the block eg 'r' for reconstructed
   */
  const Blocks& blocks(const IdCoder::SubType subtype) const;
  
  /**
   *   @brief  returns a Blocks collection with typeAndSubtype that match the identifier
   *   @param[in]  id the identifier of an object
   */
  const Blocks& blocks(Identifier id) const { return blocks(IdCoder::subtype(id)); };
  
  /**
   *   @brief  returns a Block with the required id
   *   @param[in]  id the identifier of an object
   */
  const PFBlock& block(Identifier id) const { return blocks(id).at(id); };

  /**
   *   @brief  returns a list of all the Ids inside a collection
   *   @param[in]  collection the collection
   */
  template <class T>
  Ids collectionIds(const T& collection) const;
  
  /**
   *   @brief  takes all the history collection and merged them into one single history
   */
  // void mergeHistories();
  /**
   *   @brief  returns the merged history
   */
  std::shared_ptr<const Nodes> history() const { return m_history; }

  /**
   *   @brief  resets everything, deletes all the clusters, tracks etc etc
   */
  void clear();
  void setEventNo(unsigned int eventNo) { m_eventNo = eventNo; }
  unsigned int eventNo() const { return m_eventNo; }

  /**
   *   @brief  returns a set of Ids  corresponding to a  collection
   *   @param[in]  type The type of a collection eg IdCoder::kEcalCluster
   *   @param[in]  subtype The subtype of a collection eg 'm' for merged
   */
  Ids collectionIds(IdCoder::ItemType type, IdCoder::SubType subtype) const;

  /**
   *   @brief  returns a set of Ids  corresponding to a collection
   *   @param[in]  typeAndSubtype The type and subtype of a collection eg "em" for ecal merged
   */
  Ids collectionIds(const std::string& typeAndSubtype) const;

private:
  /**
   *   @brief  templated class method used by the AddCollection methods to check that typeAndSubype match and that
   *           this collection type does not already exist. It then adds the collection into the Event.
   */
  template <class T>
  void
  addCollectionInternal(const std::unordered_map<Identifier, T>& collection,
                        std::unordered_map<IdCoder::SubType, const std::unordered_map<Identifier, T>*>& collections);
  /// Unordered map of pointers to unordered map of (concrete) Ecal Clusters
  CollectionClusters m_ecalClustersCollection;
  /// Unordered map of pointers to unordered map of (concrete) Hcal Clusters
  CollectionClusters m_hcalClustersCollection;
  /// Unordered map of pointers to unordered map of (concrete) Tracks
  CollectionTracks m_tracksCollection;
  /// Unordered map of pointers to unordered map of (concrete) Particles
  CollectionParticles m_particlesCollection;
  /// Unordered map of pointers to Blocks
  CollectionBlocks m_blocksCollection;
  std::shared_ptr<Nodes> m_history;  ///< points to the merged history (built from the sucessive histories)
  Clusters m_emptyClusters;          ///<Used to return an empty collection when no collection is found
  Tracks m_emptyTracks;              ///<Used to return an empty collection when no collection is found
  Particles m_emptyParticles;        ///<Used to return an empty collection when no collection is found
  Blocks m_emptyBlocks;              ///<Used to return an empty collection when no collection is found
  unsigned int m_eventNo;            ///<event number
};

template <class T>
void Event::addCollectionInternal(
    const std::unordered_map<Identifier, T>& collection,
    std::unordered_map<IdCoder::SubType, const std::unordered_map<Identifier, T>*>& collections) {
  Identifier firstId = 0;
  if (collection.size() == 0) return;
  for (const auto& it : collection) {
    if (!firstId) {
      firstId = it.first;
      if (hasCollection(firstId)) throw "Collection already exists";
    }
    if (IdCoder::typeAndSubtype(it.first) != IdCoder::typeAndSubtype(firstId)) {
      std::cout << IdCoder::pretty(it.first) << " : " << IdCoder::pretty(firstId) << std::endl;
      throw "more than one typeandSubtype found in collection";
    }
  }
  collections.emplace(IdCoder::subtype(firstId), &collection);
}

template <class T>
Ids Event::collectionIds(const T& collection) const {
  Ids ids;
  for (const auto& item : collection) {
    ids.insert(item.first);
  }
  return ids;
}
}

#endif /* Event_h */
