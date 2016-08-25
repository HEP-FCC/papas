# Writing a new Detector

An example of the implementation of the CMS detetctor can be found in papas/detectors

CMS.h/ CMS.cpp
CMSEcal.h/ CMSEcal.cpp
CMSHcal.h/ CMSHcal.cpp
CMSTracker.h/ CMSTracker.cpp
CMSField.h/ CMSField.cpp

## Calorimeters

The detector calorimeter implementations of HCAL and ECAL should inherit from the PAPAS Calorimeter class.
They should each implement detector specific versions of

    clusterSize - minimum detectable cluster size
    acceptance - whether or not a cluster will be recorded by the calorimeter
    energyResolution - granularity of the energy detected
    energyResponse -??

For example:

    class CMSECAL : public Calorimeter {
    CMSECAL(const VolumeCylinder&& volume, const Material&& material, double eta_crack, std::vector<double> emin,
    std::vector<std::vector<double>> eres);

    double clusterSize(const Particle& ptc) const override;
    bool acceptance(const Cluster& cluster) const override;
    double energyResolution(double energy, double eta = 0) const override;
    double energyResponse(double energy = 0, double eta = 0) const override;
    ...
    };


    double CMSECAL::clusterSize(const Particle& ptc) const {

    int pdgid = abs(ptc.pdgId());
    if ((pdgid == 22) | (pdgid == 11))
      return 0.04;
    else
      return 0.07;
    }

    bool CMSECAL::acceptance(const Cluster& cluster) const {
    double energy = cluster.energy();
    double eta = fabs(cluster.eta());
    if (eta < m_etaCrack)
      return energy > m_emin[kBarrel];
    else if (eta < 2.93)
      return ((energy > m_emin[kEndCap]) & (cluster.pt() > 0.2));
    else
      return false;
    }

 etc


## Tracker

This should be defined in a similar way to the two Calorimeters.

The following functions must be provided by the user:

    ptResolution - momentum detection resolution of the tracker
    acceptance - whether a track will be detected

eg

    bool CMSTracker::acceptance(const Track& track) const {
    double pt = track.pt();
    double eta = fabs(track.eta());
    randomgen::RandUniform rUniform{0, 1};
    bool accept = false;
    if (eta < 1.35 && pt > 0.5) {
      accept = rUniform.next() < 0.95;
    } else if (eta < 2.5 && pt > 0.5) {
      accept = rUniform.next() < 0.9;
    }
    return accept;
    }


## Field (eg CMSField.h/CMSField.cpp)

Allows the magnitude of the field to be defined. At present no other functions need to be defined.


    field = std::shared_ptr<const Field>{new CMSField(VolumeCylinder(Layer::kField, 2.9, 3.6), 3.8)};
   


## Main detector definition (eg CMS.h/CMS.cpp)

Once the detector elements are written, the detector class can created. The new detector class inherits from the provided PAPAS Detector class
eg

    class CMS : public Detector {
    public:
    CMS();
    private:
    };

Then define in the Detector constructor each of the detector elements (ECAL, HCAL, tracker, field)

    CMS::CMS() : Detector() {
    // ECAL detector Element
    m_ecal = std::shared_ptr<const class Calorimeter>{
    new CMSECAL(VolumeCylinder(Layer::kEcal, 1.55, 2.1, 1.30, 2),
    Material(8.9e-3, 0.275),
    1.5,                                                                       // eta_crack
    std::vector<double>{0.3, 1},                                               // emin barrel and endcap
    std::vector<std::vector<double>>{{.073, .1, .005}, {.213, .224, .005}})};  // barrel and endcap

    // HCAL detector element
    m_hcal = std::shared_ptr<const class Calorimeter>{
    new CMSHCAL(VolumeCylinder(Layer::kHcal, 2.9, 3.6, 1.9, 2.6),
    Material(0.0, 0.17),
    1.3,  // eta crack
    std::vector<std::vector<double>>{{1.25829, 0., 0.175950}, {1.32242e-06, 6.99123, 2.70281e-01}},
    std::vector<std::vector<double>>{{1.03430, 5.23646, -2.03400}, {1.06742, 9.41242, -2.75191}})};

    // Tracker detector element
    m_tracker = std::shared_ptr<const Tracker>{new CMSTracker(VolumeCylinder(Layer::kTracker, 1.29, 1.99))};

    // Field detector element
    m_field = std::shared_ptr<const Field>{new CMSField(VolumeCylinder(Layer::kField, 2.9, 3.6), 3.8)};
    }









  
        
