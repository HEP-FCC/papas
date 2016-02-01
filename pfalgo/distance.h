//
//  distance.h
//  fastsim
//
//  Created by Alice Robson on 01/02/16.
//
//

#ifndef distance_h
#define distance_h

#include <stdio.h>
#include "enums.h"

struct DistanceData {   fastsim::enumLayer m_layer1;
   fastsim::enumLayer m_layer1;
   bool m_isOK;
   double m_distance;
};


class Distance {
   Distance(
};
'''Concrete distance calculator.
'''
def __call__(self, ele1, ele2):
'''returns a tuple:
True/False depending on the validity of the link
float      the link distance
'''
layer1, layer2 = ele1.layer, ele2.layer
if layer2 < layer1:
layer1, layer2 = layer2, layer1
ele1, ele2 = ele2, ele1
layers = layer1, layer2
func = None
if layers == ('ecal_in', 'tracker'):
func = self.ecal_track
elif layers == ('hcal_in', 'tracker'):
func = self.hcal_track
elif layers == ('ecal_in', 'hcal_in'):
func = self.ecal_hcal
elif layers == ('ecal_in', 'ecal_in'):
func = self.ecal_ecal
elif layers == ('hcal_in', 'hcal_in'):
func = self.hcal_hcal
elif layers == ('tracker', 'tracker'):
func = self.no_link
else:
raise ValueError('no such link layer:', layers)
return func(ele1, ele2)

def no_link(self, ele1, ele2):
return None, False, None

def ecal_ecal(self, ele1, ele2):
dR = deltaR(ele1.position.Theta(),
            ele1.position.Phi(),
            ele2.position.Theta(),
            ele2.position.Phi())
link_ok = dR < ele1.angular_size() + ele2.angular_size()
return ('ecal_in', 'ecal_in'), link_ok, dR

def hcal_hcal(self, ele1, ele2):
dR = deltaR(ele1.position.Theta(),
            ele1.position.Phi(),
            ele2.position.Theta(),
            ele2.position.Phi())
link_ok = dR < ele1.angular_size() + ele2.angular_size()
return ('hcal_in', 'hcal_in'), link_ok, dR

def ecal_track(self, ecal, track):
tp = track.path.points.get('ecal_in', None)
if tp is None:
# probably a looper
return ('ecal_in', 'tracker'), False, None
link_ok, dist = ecal.is_inside(tp)
return ('ecal_in', 'tracker'), link_ok, dist

def hcal_track(self, hcal, track):
tp = track.path.points.get('hcal_in', None)
if tp is None:
# probably a looper
return ('hcal_in', 'tracker'), False, None
link_ok, dist = hcal.is_inside(tp)
return ('hcal_in', 'tracker'), link_ok, dist

def ecal_hcal(self, ele1, ele2):
#TODO eta or theta?
dR = deltaR(ele1.position.Theta(),
            ele1.position.Phi(),
            ele2.position.Theta(),
            ele2.position.Phi())
link_ok = dR < ele1.angular_size() + ele2.angular_size()
return ('ecal_in', 'hcal_in'), link_ok, dR

distance = Distance()

#endif /* distance_h */
