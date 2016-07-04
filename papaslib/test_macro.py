from ROOT import gSystem
#gSystem.Load("libfccphysics-papas")
#gSystem.Load("libfccphysics-myanalysis")
gSystem.Load("libpapascppstuff")
from ROOT import  papas
from ROOT import TVector3
from ROOT import randomgen
from ROOT.randomgen import RandExponential


#test random
thing = randomgen.RandExponential(49.3)
thing.setSeed(100)
print thing.next()
print thing.next()


from ROOT.papas import  Id
from ROOT.papas import Cluster as ClusterCPP
from ROOT.papas import Track as TrackCPP
from ROOT.papas import PFParticle as PFParticleCPP
from ROOT import std

#test cluster
id = Id.makeEcalId()
tv =TVector3(0,0,0)
cluster = ClusterCPP(3,tv,12,12345678)

class AliceR(long):
  @staticmethod
  def value():
    return 6

print AliceR.value()

#test vectors
m= std.vector('papas::Cluster')()
m.push_back(cluster)
m.size()
print m[0].energy()


v=std.unordered_map('uint64_t','papas::Cluster')()
v[3] = ClusterCPP(3,tv,12,12345678)
#test vectors

w=std.unordered_map('int','int')()

#try running papas simulation and reconstruction
from ROOT.papas import CMS
from ROOT.papas import PFEventDisplay, ViewPane, GDetector
from ROOT.papas import Simulator
from ROOT.papas import PFEvent, PFBlockBuilder, PFReconstructor
from math import pi

CMSdetector = CMS()
sim =Simulator(CMSdetector)


i=1
photon = sim.addParticle(22, pi / 2. + 0.025 * i, pi/ 2. + 0.3 * i, 100)
sim.simulatePhoton(photon)
hadron = sim.addParticle(211, pi / 2. + 0.5 * (i + 1), 0, 40. * (i + 1));
sim.simulateHadron(hadron)

pfEvent= PFEvent (sim)
bBuilder = PFBlockBuilder(pfEvent)
pfEvent.setBlocks(bBuilder)
pfReconstructor= PFReconstructor(pfEvent)
pfReconstructor.reconstruct()

#not quite right and much simplified but does produce the displays
from ROOT.papas import PFApp
the_app =  PFApp()
the_app.display(pfEvent, CMSdetector)

