#ifndef TestPapasManager_h
#define TestPapasManager_h
#include "papas/datatypes/DefinitionsCollections.h"
#include "papas/datatypes/PapasEvent.h"
#include "papas/detectors/Detector.h"
#include "papas/graphtools/DefinitionsNodes.h"
#include <list>

namespace papas {
/**
 *  @brief The PapasManager class is a high level class to own the clusters, particles etc created in an event and
       to call the algorithms such as simulation, cluster merging, block building and reconstruction.

 The PapasManager class uses the following strategy. It creates concrete collections of objects such as
 Clusters, Tracks etc. These collections are empty but are stored by the papasManager class eg clusters would be
 stored in m_ownedClusters list. The empty collections are passed by reference to the Algorithms such as simulate and
 reconstruct and the algorithms fill these collections with concrete objects such as clusters.
 After the algorithm has been run the address of the collection is also stored in the PapasEvent. This is a
 lightweight class containing unordered_maps of pointers to const collections that can be used along side an object id
 to retreive a specific object.
 The choice to make the PapasManager own the cluster collections, and to make the PapasEvent store pointers to const
 collections means that the Papas Algorithms and the PapasEvent object can be called as Gaudi Algorithms within the
   FCCSW framework.
 The PapasManager object must be cleared between events.

 Usage example:
 @code
      papasManager.simulate(papasparticles);
      papasManager.mergeClusters("es");
      papasManager.mergeClusters("hs");
      ...
      papasManager.clear;
 @endcode
 *
 *  @author  Alice Robson
 *  @date    2016-12-06
 */

// TODO: support a choice of subtype for the collections created by the various algorithms called
// from this class
// TODO: extend to support blockbuilder/reconstruct
class TestPapasManager {
public:
  /** Constructor
   * @param[in] detector : the detector to be used in the simulation and reconstruction
   */
  TestPapasManager(Detector& detector);
  /**
   *   @brief  Runs Papas simulation on a collection of generated particles producing papas simulated particles
   *          and their clusters and tracks and also history information of the connections between objects
   *   @param[in]  particles: unordered map of generated Particles to be simulated.
   *
   *   This method will create the following objects and add them to PapasEvent and to
   *   the internale class lists of owned objects:
   *       ecals-true "et"
   *       hcals-true "ht"
   *       ecals-smeared "es"
   *       hcals-smeared "hs"
   *       tracks-true "tt"
   *       tracks-smeared "ts"
   *       particles-simulated "ps"
   *       history
   */
  void simulate(const Particles& particles);
  /**
   *   @brief  Merges a set of clusters according to detector sensitivities.
   *   @param[in]  typeAndSubtype: identifies which clusters to merge, eg "hs" for hcals-smeared clusters
   *
   *   This method will create the following objects and add them to PapasEvent and to
   *   the internal class lists of owned objects:
   *       merged clusters of typeAndSubtype "em" or "hm"
   *       history
   */
  void mergeClusters(const std::string& typeAndSubtype);

  /*TODO
  void buildBlocks(const std::string& ecalTypeAndSubtype = "em",
                   const std::string& hcalTypeAndSubtype = "hm",
                   const std::string& trackTypeAndSubtype = "ts");
  void reconstruct(const std::string& blockTypeAndSubtype = "br");*/
  /**
   *   @brief  return PapasEvent object
   *   @return  PapasEvent     */
  const PapasEvent& papasEvent() { return m_papasEvent; }  //< Access the papasEvent

  /**
   *   @brief  clears all owned objects and the PapasEvent, resets the Identifier counter.
   */
  void clear();
private:
  Clusters& createClusters();       // Create an empty concrete collection of clusters ready for filling by an algorithm
  Tracks& createTracks();           // Create an empty concrete collection of tracks ready for filling by an algorithm
  Blocks& createBlocks();           // Create an empty concrete collection of blocks ready for filling by an algorithm
  SimParticles& createParticles();  // Create an empty concrete collection of particles ready for filling by an
                                    // algorithm
  Nodes& createHistory();  // Create an empty concrete collection of history nodes ready for filling by an algorithm
  const Detector& m_detector;
  PapasEvent m_papasEvent;  // object that can be passed to algorithms to allow access to objects such as a track
  std::list<Clusters> m_ownedClusters;       //<Holds all the clusters collections created during an event
  std::list<Tracks> m_ownedTracks;           //<Holds all the tracks collections created during an event
  std::list<Blocks> m_ownedBlocks;           //<Holds all the blocks collections created during an event
  std::list<SimParticles> m_ownedParticles;  //<Holds all the particles collections created during an event
  std::list<Nodes> m_ownedHistory;           //<Holds all the histories created during an event

  // bool operator()(IdType i, IdType j);//todo reinstate was used for sorting ids
};
}

#endif /* TestPapasManager_h */
