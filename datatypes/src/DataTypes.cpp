//
//  Created by Alice Robson on 09/11/15.
//
//

#include <cmath>
#include <iostream>
#include <utility>
#include "datatypes.h"
#include "Identifier.h"
#include "deltar.h"
#include "distance.h"


const double ParticleData::m_e = 0.000511;
const double ParticleData::m_mu = 0.105;
const double ParticleData::m_pi = 0.139;
const double ParticleData::m_K0 = 0.498;
const double ParticleData::m_n = 1.;
const double ParticleData::m_p = 1.;

std::unordered_map<int, std::pair<double, int>> ParticleData::m_datamap =  {
  {11,  {ParticleData::m_e,   1}},
  {-11, {ParticleData::m_e,  -1}},
  {13,  {ParticleData::m_mu,  1}},
  {-13, {ParticleData::m_mu, -1}},
  {22,  {0,                   0}},
  {130, {ParticleData::m_K0,  0}},
  {211, {ParticleData::m_pi,  1}},
  {-211,{ParticleData::m_pi, -1}}
}  ;


/*Cluster::Cluster(const Cluster&) {
 
 std::cout<< "copy" ;
 } ;*/


double Cluster::s_maxEnergy = 0;


Cluster::Cluster(double energy, const TVector3& position, double size_m, long id) :
m_uniqueID(id),  m_position(position), m_subClusters()
{
  setSize(size_m);
  setEnergy(energy);
}


void Cluster::setSize(double value)
{
  m_size = value;
  //AJRTODO put in exception here
  //try:
  m_angularSize = atan(m_size / m_position.Mag()) ;
  //except:
  //   import pdb; pdb.set_trace()
}

void Cluster::setEnergy(double energy)
{
  m_energy = energy;
  if (energy > s_maxEnergy)
    s_maxEnergy = energy;
  m_pt = energy * m_position.Unit().Perp() ;
}


//TODO need to change this so that the merged cluster has its ownID and the cluster from which it is made is
//put into subClusters its not rght just yet
Cluster& Cluster::operator+=(const Cluster& rhs){
  
  if (Identifier::layer(m_uniqueID) != Identifier::layer(rhs.id())) {
    std::cout << "can only add a cluster from the same layer";
  }
  m_position = m_position * m_energy + rhs.position() * rhs.energy();
  m_energy = m_energy + rhs.energy();
  double denom  = 1./m_energy;
  m_position *= denom;
  if (rhs.subClusters().size() != 1)
    std::cout << "can only add in a cluster which is not already merged";
  if (m_subClusters.size()==0)
    m_subClusters.push_back(m_uniqueID);
  m_subClusters.insert(m_subClusters.end(), rhs.subClusters().begin(), rhs.subClusters().end());
  
  return *this;
}

std::ostream& operator<<(std::ostream& os, const Cluster& cluster) { //TODO move to helper class
  os << "Cluster:" << Identifier::layer(cluster.m_uniqueID)  << ": "<< std::setw(9) << std::fixed  << cluster.m_energy ;
  os << cluster.m_position.Phi() << ", " << M_PI/2. - cluster.m_position.Theta()<< std::endl;
  return os;
}





/*
 Cluster::Cluster( Cluster && c) :
 m_size(c.m_size),
 m_angularsize(c.m_angularsize),
 m_pt(c.m_pt),
 m_uniqueid(c.m_uniqueid),
 m_energy(c.m_energy)
 {
 m_position=c.m_position;
 }
 
 
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





/*Track::Track(const TVector3 p3, double charge,const Path& path, long id) :
    m_uniqueID(id),
    m_p3(p3),
    m_charge(charge),
    m_path(path)
{
}*/


Track::Track(const TVector3 p3, double charge, sptrPath path, long id) :
m_uniqueId(id),
m_p3(p3),
m_charge(charge),
m_path(path)
{
}

PFParticle::PFParticle(long uniqueid,int pdgid, TLorentzVector tlv, TVector3 vertex, double field) :
Particle(uniqueid, pdgid, ParticleData::particleCharge(pdgid), tlv),
m_vertex(vertex),
m_isHelix(fabs(charge())>0.5)
{
  if (m_isHelix)
    m_path = std::make_shared<Helix>(tlv, vertex, field, charge());
  else
    m_path = std::make_shared<Path>(tlv, vertex, field);

  
}


PFParticle::PFParticle(long uniqueid,const Track& track) :
PFParticle(uniqueid,
            211 * track.charge(),
            TLorentzVector(track.p3(), track.energy()),
            track.path()->namedPoint("vertex"),
            track.path()->field()) {
}
/*
void PFParticle::setHelix(const Path& path) {
  m_helix = path; //copy??
  }

void PFParticle::setPath(const Path& path) {
  m_path = path;
}*/


TVector3 PFParticle::pathPosition(std::string name) const
{
  return m_path->namedPoint(name);
}

TLorentzVector makeTLorentzVector(int pdgid, double theta, double phi, double energy)
{
  double mass = ParticleData::particleMass(pdgid);
  double momentum = sqrt(pow(energy, 2) - pow(mass, 2));
  double costheta = cos(theta);
  double sintheta = sin(theta);
  double cosphi = cos(phi);
  double sinphi = sin(phi);
  TLorentzVector p4(momentum * sintheta * cosphi,
                    momentum * sintheta * sinphi,
                    momentum * costheta,
                    energy);
  /*std::cout << "TLV " << p4.X() << " " << p4.Y() << " " << p4.Z() << " " <<
  p4.Et() << " ";
  std::cout << "energy " << energy << " mom " << momentum << " " << costheta <<
  " " << cosphi <<
  " " << sintheta << " ";*/
  return p4;
}


/*PFParticle::PFParticle( TLorentzVector& tlv, TVector3& vertex, double charge, int pdgid):
 Particle(pdgid, charge, tlv), m_vertex(vertex) {
 self.path = None
 self.clusters = dict()
 self.track = Track(self.p3(), self.q(), selfget.path)
 self.clusters_smeared = dict()
 self.track_smeared = None
 }*/

bool PFParticle::isElectroMagnetic() const
{
  unsigned int kind = abs(pdgId());
  if (kind == 11 || kind == 22) {
    return true;
  } else
    return false;
  
};
