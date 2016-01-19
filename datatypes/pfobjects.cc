//
//  Created by Alice Robson on 09/11/15.
//
//
#include "pfobjects.h"
#include<cmath>
#include<iostream>
#include<utility>
#include "identifier.h"


const double ParticleData::m_e = 0.000511;
const double ParticleData::m_mu = 0.105;
const double ParticleData::m_pi = 0.139;
const double ParticleData::m_K0 = 0.498;
const double ParticleData::m_n = 1.;
const double ParticleData::m_p = 1.;

std::unordered_map<int, std::pair<double, int>> ParticleData::m_datamap =  {
   {11,  {ParticleData::m_e,   1}},
   { -11, {ParticleData::m_e,  -1}},
   {13,  {ParticleData::m_mu,  1}},
   { -13, {ParticleData::m_mu, -1}},
   {22,  {0,                   0}},
   {130, {ParticleData::m_K0,  0}},
   {211, {ParticleData::m_pi,  1}},
   { -211, {ParticleData::m_pi, -1}}
}  ;


double Cluster::s_maxenergy = 0;


Cluster::Cluster(double energy, const TVector3& position, double size_m,
                 long id) :
   m_uniqueid(id),  m_position(position)
{
   setSize(size_m);
   setEnergy(energy);
}

Cluster::Cluster() :
   m_uniqueid(0), m_position( {0., 0., 0.})
{
   setEnergy(0);
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

/*Cluster* Cluster::additem(Cluster* other)
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
}*/


void Cluster::setEnergy(double energy)
{
   m_energy = energy;
   if (energy > s_maxenergy)
      s_maxenergy = energy;
   m_pt = energy * m_position.Unit().Perp() ;
}


/*
 Cluster& Cluster::operator=(Cluster&& c) {
 m_energy=c.m_energy;
 m_position=c.m_position;
 m_size=c.m_size;
 m_pt=c.m_pt;
 m_uniqueid=c.m_uniqueid;
 std::cout<< "move assign cluster" <<std::endl;
 return *this;
 };

 Cluster& Cluster::operator=(const Cluster& c) {
 m_energy=c.m_energy;
 m_position=c.m_position;
 m_size=c.m_size;
 m_pt=c.m_pt;
 m_uniqueid=c.m_uniqueid;
 std::cout<< "copy cluster" <<std::endl;
 return *this;
 };*/

/*
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
*/


//TODO make work with helix path
SimParticle::SimParticle(int pdgid, TLorentzVector& tlv, TVector3&& vertex) :
   Particle(Identifier::makeParticleID(fastsim::enumSource::SIMULATION), pdgid,
            0.0, tlv),
   m_vertex(vertex),
   m_path(tlv, vertex)
{

};


const TVector3& SimParticle::getPathPosition(std::string name)
{

   return m_path.getNamedPoint(name);

}

TLorentzVector MakeParticleLorentzVector(int pdgid, double theta, double  phi,
      double energy)
{
   double mass = ParticleData::getParticleMass(pdgid);
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
   std::cout << "energy " << energy << " mom " << momentum << " " << costheta <<
             " " << cosphi <<
             " " << sintheta << " ";
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
