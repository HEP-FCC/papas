#ifndef PapasManager_h
#define PapasManager_h
#include "papas/datatypes/DefinitionsCollections.h"
#include "papas/datatypes/Event.h"
#include "papas/detectors/Detector.h"
#include "papas/graphtools/DefinitionsNodes.h"
#include <list>

namespace fcc {
class MCParticleCollection;
}
namespace papas {
/**
 *  @brief The PapasManager class is a high level class to own the clusters, particles etc created in an event and
       to call the algorithms such as simulation, cluster merging, block building and reconstruction.

 The PapasManager class uses the following strategy. It creates concrete collections of objects such as
 Clusters, Tracks etc. These collections are empty but are stored by the papasManager class eg clusters would be
 stored in m_ownedClusters list. The empty collections are passed by reference to the Algorithms such as simulate and
 reconstruct and the algorithms fill these collections with concrete objects such as clusters.
 After the algorithm has been run the address of the collection is also stored in the Event. This is a
 lightweight class containing unordered_maps of pointers to const collections that can be used along side an object id
 to retreive a specific object.
 The choice to make the PapasManager own the cluster collections, and to make the Event store pointers to const
 collections means that the Papas Algorithms and the Event object can be called as Gaudi Algorithms within the
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
// forward declarations

class PapasManager {
public:
  /** Constructor
   * @param[in] detector : the detector to be used in the simulation and reconstruction
   */
  PapasManager(const Detector& detector);
  /**
   *   @brief  Runs Papas simulation on a collection of papas simulated particles creating
   *          their clusters and tracks and paths and also history information of the connections
   *           between objects. Note that normally the particles arhument will pass a particles
   *           collection already owned by PapasManager iusing createParticles.
   *   @param[in]  particles: unordered map of generated Particles to be simulated.
   *
   *   This method will create the following objects and add them to Event and to
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
  void simulate(Particles& particles);
  /**
   *   @brief  Merges a set of clusters according to detector sensitivities.
   *   @param[in]  typeAndSubtype: identifies which clusters to merge, eg "hs" for hcals-smeared clusters
   *
   *   This method will create the following objects and add them to Event and to
   *   the internal class lists of owned objects:
   *       merged clusters of typeAndSubtype "em" or "hm"
   *       history
   */
  void mergeClusters(const std::string& typeAndSubtype);
  void buildBlocks(const std::string& ecalTypeAndSubtype = "em",
                   const std::string& hcalTypeAndSubtype = "hm",
                   char trackSubtype = 's');
  void simplifyBlocks(char blockSubtype);
  // void mergeHistories();
  void reconstruct(char blockSubtype);
  /**
   *   @brief  return Event object
   *   @return  Event     */
  const Event& event() const { return m_event; }                          //< Access the event
  const Detector& detector() const { return m_detector; }                 ///< Access the detector
  void setEventNo(unsigned int eventNo) { m_event.setEventNo(eventNo); }  ///< Set the event No
  void clear();                                                           ///<clears all owned objects and the Event
  Particles& createParticles();  // Create an empty concrete collection of particles for filling by an algorithm

protected:
  Clusters& createClusters();  // Create an empty concrete collection of clusters ready for filling by an algorithm
  Tracks& createTracks();      // Create an empty concrete collection of tracks ready for filling by an algorithm
  Blocks& createBlocks();      // Create an empty concrete collection of blocks ready for filling by an algorithm
  Nodes& createHistory();      // Create an empty concrete collection of history nodes ready for filling by an algorithm
  const Detector& m_detector;

  std::list<Clusters> m_ownedClusters;    //<Holds all the clusters collections created during an event
  std::list<Tracks> m_ownedTracks;        //<Holds all the tracks collections created during an event
  std::list<Blocks> m_ownedBlocks;        //<Holds all the blocks collections created during an event
  std::list<Particles> m_ownedParticles;  //<Holds all the particles collections created during an event
  std::list<Nodes> m_ownedHistory;        //<Holds all the histories created during an event
  Event m_event;  // object that can be passed to algorithms to allow access to objects such as a track

  // bool operator()(Identifier i, Identifier j);//todo reinstate was used for sorting ids
};
}

#endif /* PapasManager_h */