#include "papas/reconstruction/PFReconstructor.h"

#include <algorithm>
#include <math.h>
#include <vector>

#include "papas/datatypes/Event.h"
#include "papas/datatypes/HistoryHelper.h"
#include "papas/datatypes/ParticlePData.h"
#include "papas/datatypes/Path.h"
#include "papas/detectors/Calorimeter.h"
#include "papas/detectors/Detector.h"
#include "papas/graphtools/Edge.h"
#include "papas/simulation/HelixPropagator.h"
#include "papas/simulation/StraightLinePropagator.h"
#include "papas/utility/PDebug.h"

namespace papas {
  
  PFReconstructor::PFReconstructor(const Event& event, char blockSubtype, const Detector& detector, Particles& particles,
                                   Nodes& history)
  : m_event(event), m_detector(detector), m_particles(particles), m_history(history) {
    m_propHelix = std::make_shared<HelixPropagator>(HelixPropagator(detector.field()));
    m_propStraight = std::make_shared<StraightLinePropagator>(StraightLinePropagator(detector.field()));
    auto blockids = m_event.collectionIds(IdCoder::ItemType::kBlock, blockSubtype);
    for (auto bid : blockids) {
      reconstructBlock(m_event.block(bid));
    }
    if (m_unused.size() > 0) {
      PDebug::write("unused elements ");
      for (auto u : m_unused)
        PDebug::write("{},", u);
      // TODO warning message
    }
    PDebug::write ("finished reconstruction");

  }
  
  PFReconstructor::~PFReconstructor() {
    
  }
  
  void PFReconstructor::reconstructBlock(const PFBlock& block) {
    // see class description for summary of reconstruction approach
    PDebug::write("Processing {}", block);
    Ids ids = block.elementIds();
    for (auto id : ids) {
      m_locked[id] = false;
    }
    reconstructMuons(block);
    reconstructElectrons(block);
    // keeping only the elements that have not been used so far
    Ids uids;
    for (auto id : ids) {
      if (!m_locked[id]) uids.insert(id);
    }
    if (uids.size() == 1) {  //#TODO WARNING!!! LOTS OF MISSING CASES
      Identifier id = *uids.begin();
      auto parentIds = Ids{block.id(), id};
      if (IdCoder::isEcal(id)) {
        reconstructCluster(m_event.cluster(id), papas::Layer::kEcal, parentIds);
      } else if (IdCoder::isHcal(id)) {
        reconstructCluster(m_event.cluster(id), papas::Layer::kHcal, parentIds);
      } else if (IdCoder::isTrack(id)) {
        reconstructTrack(m_event.track(id), 211, parentIds);
      } else {  // ask Colin about energy balance - what happened to the associated clusters that one would expect?
                // TODO
      }
    } else {
      for (auto id : uids) {
        if (IdCoder::isHcal(id)) {
          reconstructHcal(block, id);
        }
      }
      for (auto id : ids) {
        if (IdCoder::isTrack(id) && !m_locked[id]) {
          /* unused tracks, so not linked to HCAL
           # reconstructing charged hadrons*/
          auto parentIds = Ids{block.id(), id};
          reconstructTrack(m_event.track(id), 211, parentIds);
          for (auto idlink : block.linkedIds(id, Edge::EdgeType::kEcalTrack)) {
            // TODO ask colin what happened to possible photons here:
            // TODO add in extra photons but decide where they should go?
            m_locked[idlink] = true;
          }
        }
      }
    }
    for (auto& id : ids) {
      if (!m_locked[id]) {
        m_unused.insert(id);
      }
    }
    PDebug::write ("finished block",IdCoder::pretty(block.id()));
  }
  
  void PFReconstructor::reconstructMuons(const PFBlock& block) {
    /// Reconstruct muons in block.
    Ids ids = block.elementIds();
    for (auto id : ids) {
      if (IdCoder::isTrack(id) && isFromParticle(id, "ps", 13)) {
        
        auto parentIds = Ids{block.id(), id};
        reconstructTrack(m_event.track(id), 13, parentIds);
      }
    }
  }
  
  void PFReconstructor::reconstructElectrons(const PFBlock& block) {
    /*Reconstruct electrons in block.*/
    Ids ids = block.elementIds();
    
    /* the simulator does not simulate electron energy deposits in ecal.
     # therefore, one should not lock the ecal clusters linked to the
     # electron track as these clusters are coming from other particles.*/
    for (auto id : ids) {
      if (IdCoder::isTrack(id) && isFromParticle(id, "ps", 11)) {
        
        auto parentIds = Ids{block.id(), id};
        reconstructTrack(m_event.track(id), 11, parentIds);
      }
    }
  }
  
  void PFReconstructor::insertParticle(const Ids& parentIds, Particle& newparticle) {
    /* The new particle will be inserted into the history_nodes (if present).
     A new node for the particle will be created if needed.
     It will have as its parents the block and all the elements of the block.
     '''
     #Note that although it may be possible to specify more closely that the particle comes from
     #some parts of the block, there are frequently ambiguities and so for now the particle is
     #linked to everything in the block*/
    // if (newparticle) :
    Identifier newid = newparticle.id();
    m_particles.emplace(newid, std::move(newparticle));
    makeHistoryLinks(parentIds, {newid}, m_history);
  }
  
  bool PFReconstructor::isFromParticle(Identifier id, const std::string& typeAndSubtype, int pdgid) const {
    /*returns: True if object identifier comes, directly or indirectly,
     from a particle of type type_and_subtype, with this absolute pdgid.
     */
    auto historyHelper = HistoryHelper(m_event);
    auto parentIds = historyHelper.linkedIds(id, typeAndSubtype, DAG::enumVisitType::PARENTS);
    bool isFromPdgId = false;
    for (auto pid : parentIds) {
      if (abs(m_event.particle(pid).pdgId()) == abs(pdgid)) isFromPdgId = true;
    }
    return isFromPdgId;
  }
  
  double PFReconstructor::neutralHadronEnergyResolution(double energy, double eta) const {
    /*Currently returns the hcal resolution of the detector in use.
     That's a generic solution, but CMS is doing the following
     (implementation in commented code)
     http://cmslxr.fnal.gov/source/RecoParticleFlow/PFProducer/src/PFAlgo.cc#3350
     */
    auto resolution = m_detector.hcal()->energyResolution(energy, eta);
    return resolution;
  }
  
  double PFReconstructor::nsigmaHcal(const Cluster& cluster) const {
    /*Currently returns 2.
     CMS is doing the following (implementation in commented code)
     http://cmslxr.fnal.gov/source/RecoParticleFlow/PFProducer/src/PFAlgo.cc#3365
     */
    return 2;
  }
  
  void PFReconstructor::reconstructHcal(const PFBlock& block, Identifier hcalId) {
    /*
     block: element ids and edges
     hcalid: id of the hcal being processed her
     
     has hcal and has a track
     -> add up all connected tracks, turn each track into a charged hadron
     -> add up all ecal energies
     -> if track energies is greater than hcal energy then turn the missing energies into an ecal (photon)
     NB this links the photon to the hcal rather than the ecals
     -> if track energies are less than hcal then make a neutral hadron with rest of hcal energy and turn all ecals into
     photons
     has hcal but no track (nb by design there will be no attached ecals because hcal ecal links have been removed)
     -> make a neutral hadron
     has hcals
     -> each hcal is treated using rules above
     */
    
    // hcal used to make ecal_in has a couple of possible issues
    // TODO assert(len(block.linked_ids(hcalid, "hcal_hcal"))==0  )
    
    Ids ecalIds;
    Ids trackIds(block.linkedIds(hcalId, Edge::EdgeType::kHcalTrack));
    for (auto trackId : trackIds) {
      for (auto ecalId : block.linkedIds(trackId, Edge::EdgeType::kEcalTrack)) {
        /*the ecals get all grouped together for all tracks in the block
         # Maybe we want to link ecals to their closest track etc?
         # this might help with history work
         # ask colin.*/
        if (!m_locked[ecalId]) {
          ecalIds.insert(ecalId);
          m_locked[ecalId] = true;
        }
      }
    }
    // hcal should be the only remaining linked hcal cluster (closest one)
    const Cluster& hcal = m_event.cluster(hcalId);
    double hcalEnergy = hcal.energy();
    double ecalEnergy = 0.;
    double trackEnergy = 0.;
    
    if (!trackIds.empty()) {
      for (auto id : trackIds) {
        const Track& track = m_event.track(id);
        auto parentIds = Ids{block.id(), id};
        auto ecalLinks = block.linkedIds(id, Edge::kEcalTrack);
        parentIds.insert(ecalLinks.begin(), ecalLinks.end());
        auto hcalLinks = block.linkedIds(id, Edge::kHcalTrack);
        parentIds.insert(hcalLinks.begin(), hcalLinks.end());
        reconstructTrack(track, 211, parentIds);
        trackEnergy += track.energy();
      }
      for (auto id : ecalIds) {
        ecalEnergy += m_event.cluster(id).energy();
      }
      double deltaERel = (hcalEnergy + ecalEnergy) / trackEnergy - 1.;
      double caloERes = neutralHadronEnergyResolution(trackEnergy, hcal.eta());
      if (deltaERel > nsigmaHcal(hcal) * caloERes) {  //# approx means hcal energy + ecal energies > track energies
        
        double excess = deltaERel * trackEnergy;  // energy in excess of track energies
                                                  // print( 'excess = {excess:5.2f}, ecal_E = {ecal_e:5.2f}, diff = {diff:5.2f}'.format(
                                                  //   excess=excess, ecal_e = ecal_energy, diff=excess-ecal_energy))
        if (excess <= ecalEnergy) { /* # approx means hcal energy > track energies
                                     # Make a photon from the ecal energy
                                     # We make only one photon using only the combined ecal energies*/
          auto parentIds = ecalIds;
          parentIds.insert(block.id());
          reconstructCluster(hcal, papas::Layer::kEcal, parentIds, excess);
        }
        
        else {  // approx means that hcal energy>track energies so we must have a neutral hadron
                // excess-ecal_energy is approximately hcal energy  - track energies
          auto parentIds = Ids{block.id(), hcalId};
          reconstructCluster(hcal, papas::Layer::kHcal, parentIds, excess - ecalEnergy);
          if (ecalEnergy) {
            // make a photon from the remaining ecal energies
            // again history is confusingbecause hcal is used to provide direction
            // be better to make several smaller photons one per ecal?
            auto parentIds = ecalIds;
            parentIds.insert(block.id());
            reconstructCluster(hcal, papas::Layer::kEcal, parentIds, ecalEnergy);
          }
        }
      }
    } else {  //  case whether there are no tracks make a neutral hadron for each hcal
              // note that hcal-ecal links have been removed so hcal should only be linked to
              // other hcals
      auto parentIds = Ids{block.id(), hcalId};
      reconstructCluster(hcal, papas::Layer::kHcal, parentIds);
    }
    m_locked[hcalId] = true;
  }
  
  void PFReconstructor::reconstructCluster(const Cluster& cluster, papas::Layer layer, const Ids& parentIds,
                                           double energy, const TVector3& vertex) {
    // construct a photon if it is an ecal
    // construct a neutral hadron if it is an hcal
    int pdgId = 0;
    if (energy < 0) {
      energy = cluster.energy();
    }
    if (layer == papas::Layer::kEcal) {
      pdgId = 22;  // photon
    } else if (layer == papas::Layer::kHcal) {
      pdgId = 130;  // K0
    } else {
      // TODO raise ValueError('layer must be equal to ecal_in or hcal_in')
    }
    double mass = ParticlePData::particleMass(pdgId);
    
    if (energy < mass)  // no particle
      return;
    
    double momentum;
    if (mass == 0) {
      momentum = energy;
    }  //#avoid sqrt for zero mass
    else {
      momentum = sqrt(pow(energy, 2) - pow(mass, 2));
    }
    TVector3 p3(cluster.position().Unit() * momentum);
    TLorentzVector p4(p3.Px(), p3.Py(), p3.Pz(), energy);  // mass is not accurate here
    Particle particle(pdgId, 0., p4, m_particles.size(), 'r', vertex);
    propagator(particle.charge())->propagateOne(particle, m_detector.ecal()->volumeCylinder().inner());
    if (layer == papas::Layer::kHcal) {  // alice not sure
      particle.path()->addPoint(papas::Position::kHcalIn, cluster.position());
    }
    
    // alice: Colin this may be a bit strange
    // because we can make a photon with a
    // path where the point is actually that
    // of the hcal?
    // nb this only is problem if the cluster and the assigned layer are different
    m_locked[cluster.id()] = true;  // alice : just OK but not nice if hcal used to make ecal.
                                    // TODO make more flexible and able to detect what type of cluster
    PDebug::write("Made {} from Merged{}", particle, cluster);
    insertParticle(parentIds, particle);
  }
  
  void PFReconstructor::reconstructTrack(const Track& track, int pdgId, const Ids& parentIds) {
    /*construct a charged hadron/electron/muon from the track
     */
    if (m_locked[track.id()]) return;
    pdgId = pdgId * track.charge();
    TLorentzVector p4 = TLorentzVector();
    p4.SetVectM(track.p3(), ParticlePData::particleMass(pdgId));
    Particle particle(pdgId, track.charge(), p4, m_particles.size(), 'r',
                      track.path()->namedPoint(papas::Position::kVertex));
    
    //#todo fix this so it picks up smeared track points (need to propagate smeared track)
    // particle.set_path(track.path)
    m_locked[track.id()] = true;
    PDebug::write("Made {} from Smeared{}", particle, track);
    insertParticle(parentIds, particle);
  }
  
  std::shared_ptr<const Propagator> PFReconstructor::propagator(double charge) const {
    if (fabs(charge) < 0.5)
      return m_propStraight;
    else
      return m_propHelix;
  }
}  // end namespace papas
