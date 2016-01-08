//
//  Created by Alice Robson on 09/11/15.
//
//
#include "pfobjects.h"
#include<cmath>
#include<iostream>
#include<utility>

PFObject::PFObject(double energy, const TVector3& position,
                   const std::string& layer, const Particle& p) :
   m_position(position),
   m_layer(layer),
   m_particle(p)
{
   setEnergy(energy);
}

PFObject::PFObject(double energy, const TVector3& position,
                   const std::string& layer) :
   m_position(position),
   m_layer(layer),
   m_particle(0., 0.)
{
   setEnergy(energy);
}

double Cluster::s_maxenergy = 0;


Cluster::Cluster(double energy, const TVector3& position, double size_m,
                 const std::string& layer, Particle& p) :
   PFObject(energy, position, layer, p), m_subclusters{this}
{
   setSize(size_m);
   setEnergy(energy);

}

Cluster::Cluster(double energy, const TVector3& position, double size_m,
                 const std::string& layer) :
   PFObject(energy, position, layer), m_subclusters{this}
{
   setSize(size_m);
   setEnergy(energy);

}

void Cluster::setSize(double value)
{
   m_size = value;
   //AJRTODO put in exception here
   //try:
   m_angularsize = atan(m_size / m_position.Mag()) ;
   //except:
   //   import pdb; pdb.set_trace()
}



std::pair<bool, double> Cluster::isInside(const TVector3& point) const
{
   /*AJRTODOsubdists = [ (subc.position - point).Mag() for subc in m_subclusters ]
    dist = min(subdists)
    if dist < m_size():
    return True, dist
    else:
    */
   double dist = 0.5;
   return std::pair<bool, double>(false, dist);

}

Cluster* Cluster::additem(Cluster* other)
{
   if (other->getLayer() != m_layer)
      std::cout <<
                "can only add cluster from same layer"; //AJRTODO raise ValueError('can only add a cluster from the same layer');
   TVector3 position = m_position * m_energy + other->getPosition() *
                       other->getEnergy();
   double energy = m_energy + other->getEnergy();
   double denom  = 1. / energy;
   position *= denom;
   m_position = position;
   m_energy = energy;
   m_subclusters.splice(m_subclusters.begin(),
                        m_subclusters); //other->getSubClusters());
   return this;
}

void Cluster::setEnergy(double energy)
{
   m_energy = energy;
   if (energy > s_maxenergy)
      s_maxenergy = energy;
   m_pt = energy * m_position.Unit().Perp() ;
}



std::string Cluster::StringDescription()
{
   return "A string o decribe a cluser TODO";
   /*eturn '{classname:15}: {layer:10} {energy:7.2f} {theta:5.2f} {phi:5.2f}'.format(
    classname = m___class__.__name__,
    layer = m_layer,
    energy = m_energy,
    theta = math.pi/2. - m_position.Theta(),
    phi = m_position.Phi()*/
}


SmearedCluster::SmearedCluster(const Cluster& mother, double energy,
                               const TVector3& position, double size_m,
                               const std::string& layer, Particle& p):
   Cluster(energy,  position,  size_m, layer, p),
   m_mother(mother)
{}

SmearedCluster::SmearedCluster(const Cluster& mother, double energy,
                               const TVector3& position, double size_m,
                               const std::string& layer):
   Cluster(energy,  position,  size_m, layer),
   m_mother(mother) {}


SimParticle::SimParticle(int pdgid, TLorentzVector& tlv, TVector3&& vertex) :
   Particle(pdgid, 0.0, tlv), m_vertex(vertex)
{
};

TLorentzVector MakeParticleLorentzVector(int pdgid, double theta, double  phi,
      double energy)
{

   //mass, charge = particle_data[pdgid] //AJRTODO
   double mass = 0. ;
   double momentum = sqrt(pow(energy, 2) - pow(mass, 2));
   double costheta = cos(theta);
   double sintheta = sin(theta);
   double cosphi = cos(phi);
   double sinphi = sin(phi);
   TLorentzVector p4(momentum * sintheta * cosphi,
                     momentum * sintheta * sinphi,
                     momentum * costheta,
                     energy);
   std::cout << "TLV " << p4.X() << " " << p4.Y() << " " << p4.Z() << " " <<
             p4.Et() << " ";
   return p4;
}


/*SimParticle::SimParticle( TLorentzVector& tlv, TVector3& vertex, double charge, int pdgid):
Particle(pdgid, charge, tlv), m_vertex(vertex) {
       self.path = None
     self.clusters = dict()
     self.track = Track(self.p3(), self.q(), selfget.path)
     self.clusters_smeared = dict()
     self.track_smeared = None
}*/

bool SimParticle::IsCharged() const
{
   unsigned int kind = abs(getPdgid());
   if (kind == 11 || kind == 22) {
      return true;
   } else
      return false;

};
