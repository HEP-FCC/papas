from ROOT import gSystem
gSystem.Load("libfastsim")
from ROOT import RandExponential
ma = Alice()
thing = RandExponential(49.3)
thing.setSeed(100)
print thing.next()
print thing.next()
#ma.loop("example.root")
