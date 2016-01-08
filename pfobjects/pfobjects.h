//
//  Created by Alice Robson on 29/11/15.
//
//
#ifndef  PFOBJECTS_H
#define PFOBJECTS_H

#include <string>
#include <list>
#include <utility>
#include "particle.h"
#include "TVector3.h"


/// Function to create a new TLorentzVector
/**
 \file pfobjects.h
 note it is not possible to use move or Rvalue references because TLorentzVector does not support move
 */
TLorentzVector MakeParticleLorentzVector(int pdgid, double theta, double  phi,
      double energy);

class PFObject {
public:
   PFObject(double energy, const TVector3& position, const std::string& layer,
            const Particle& particle);
   PFObject(double energy, const TVector3& position, const std::string& layer);

   TVector3 getPosition()  const  {return m_position;};
   double getEnergy()      const  {return m_energy;};
   std::string getLayer()  const  {return m_layer;};
   double getPt()    const       {return m_pt;};
   double getEta()   const       {return m_position.Eta();};
   int getPdgid()    const       {return m_particle.getPdgid();}
   long getPFObjectID() const    {return m_objectid;}
   void setEnergy(double energy) {m_energy = energy;};
protected:

   TVector3 m_position;
   double m_energy;
   std::string m_layer;
   Particle m_particle; //probably remove this
   double m_pt;
   long m_objectid;
};

/*def __init__(self):
 m_linked = []
 m_locked = False
 m_block_label = None

 def accept(self, visitor):
 '''Called by visitors, such as FloodFill.'''
 notseen = visitor.visit(self)
 if notseen:
 for elem in m_linked:
 elem.accept(visitor)

 def __repr__(self):
 return str(self)
 */

class Cluster : public PFObject {
public:
   Cluster(double energy, const TVector3& position, double size_m,
           const std::string& layer, Particle& particle);
   Cluster(double energy, const TVector3& position, double size_m,
           const std::string& layer);

   double getAngularSize() const {return m_angularsize;};
   double getSize() const {return m_size;};
   std::list<Cluster*> getSubClusters() const {return m_subclusters;};
   std::pair<bool, double> isInside(const TVector3& point) const;
   Cluster* additem(Cluster* other);
   std::string StringDescription();

   void setSize(double value) ;
   void setEnergy(double value) ;

   static double s_maxenergy; //AJR is this in the right place

private:
   double m_size;
   double m_angularsize;
   std::list<Cluster*> m_subclusters;
};


class SmearedCluster: public Cluster {
   SmearedCluster(const Cluster& mother, double energy, const TVector3& position,
                  double size_m, const std::string& layer,
                  Particle&);
   SmearedCluster(const Cluster& mother, double energy, const TVector3& position,
                  double size_m, const std::string& layer);
private:
   Cluster  m_mother;
};


class SimParticle: public Particle {
public:
   bool IsCharged() const;

   //SimParticle( TLorentzVector& tlv, TVector3& vertex, double charge, int pdgid=0);
   //SimParticle(int pdgid, double theta, double  phi, double energy, TVector3&& vertex=TVector3(0., 0., 0.));

   SimParticle(int pdgid, TLorentzVector& tlv, TVector3&& vertex = TVector3(0., 0.,
               0.));
private:
   TVector3 m_vertex;
};
/*
 super(Particle, self).__init__(pdgid, charge, tlv)
 self.vertex = vertex
 self.path = None
 self.clusters = dict()
 self.track = Track(self.p3(), self.q(), self.path)
 self.clusters_smeared = dict()
 self.track_smeared = None

 def __getattr__(self, name):
 if name=='points':
 # if self.path is None:
 #     import pdb; pdb.set_trace()
 return self.path.points
 else:
 raise AttributeError*/

/*def set_path(self, path, option=None):
 if option == 'w' or self.path is None:
 self.path = path
 self.track = Track(self.p3(), self.q(), self.path)
 */


#endif




/*
 class Track{
 Track(TVector3& p3, double charge, Path& path, Particle& p):

 TVector3 m_p3 ;= p3
 m_pt = p3.Perp()
 m_energy = p3.Mag()  #TODO clarify energy and momentum
 m_charge = charge
 m_path = path
 m_particle = particle
 m_layer = 'tracker'

 super(Track, self).__init__()
 self.p3 = p3
 self.pt = p3.Perp()
 self.energy = p3.Mag()  #TODO clarify energy and momentum
 self.charge = charge
 self.path = path
 self.particle = particle
 self.layer = 'tracker'

 def __str__(self):
 return '{classname:15}: {e:7.2f} {pt:7.2f} {theta:5.2f} {phi:5.2f}'.format(
 classname = self.__class__.__name__,
 pt = self.pt,
 e = self.energy,
 theta = math.pi/2. - self.p3.Theta(),
 phi = self.p3.Phi()
 )
 }



 class Track(PFObject):

 def __init__(self, p3, charge, path, particle=None):
 super(Track, self).__init__()
 m_p3 = p3
 m_pt = p3.Perp()
 m_energy = p3.Mag()  #TODO clarify energy and momentum
 m_charge = charge
 m_path = path
 m_particle = particle
 m_layer = 'tracker'

 def __str__(self):
 return '{classname:15}: {e:7.2f} {pt:7.2f} {theta:5.2f} {phi:5.2f}'.format(
 classname = m___class__.__name__,
 pt = m_pt,
 e = m_energy,
 theta = math.pi/2. - m_p3.Theta(),
 phi = m_p3.Phi()
 )



 class SmearedTrack(Track):

 def __init__(self, mother, *args, **kwargs):
 m_mother = mother
 m_path = mother.path
 super(SmearedTrack, self).__init__(*args, **kwargs)


 class Particle(BaseParticle):
 def __init__(self, tlv, vertex, charge, pdgid=None):
 super(Particle, self).__init__(pdgid, charge, tlv)
 m_vertex = vertex
 m_path = None
 m_clusters = dict()
 m_track = Track(m_p3(), m_q(), m_path)
 m_clusters_smeared = dict()
 m_track_smeared = None

 def __getattr__(self, name):
 if name=='points':
 # if m_path is None:
 #     import pdb; pdb.set_trace()
 return m_path.points
 else:
 raise AttributeError


 def is_em(self):
 kind = abs(m_pdgid())
 if kind==11 or kind==22:
 return True
 else:
 return False

 def set_path(self, path, option=None):
 if option == 'w' or m_path is None:
 m_path = path
 m_track = Track(m_p3(), m_q(), m_path)


 if __name__ == '__main__':
 from ROOT import TVector3
 cluster = Cluster(10., TVector3(1,0,0), 1, 1)
 print cluster.pt
 cluster.set_energy(5.)
 print cluster.pt

 */