//
//  Created by Alice Robson on 09/11/15.
//
//

#include<cmath>
#include<iostream>
#include<utility>
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


double Cluster::s_maxEnergy = 0;


Cluster::Cluster(double energy, const TVector3& position, double size_m,
                 long id) :
   m_uniqueID(id),  m_position(position), m_subClusters(id)
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

/*

DistanceData Cluster::distanceCluster(const Cluster& clust) const{
  //fastsim::enumLayer layer1 = Identifier::layer(m_uniqueID);
  // fastsim::enumLayer layer2 = Identifier::layer(clust.ID());
      double dR = deltaR(m_position.Theta(),
                  m_position.Phi(),
                  clust.position().Theta(),
                         clust.position().Phi());
      bool linkOK = dR < (m_angularSize + clust.angularSize());
      return DistanceData{linkOK, dR};
}

DistanceData Cluster::isInsideSubClusters(const Cluster& other) {
  //see if two clusters overlap (allowing for merged clusters which contain subclusters)
  std::vector<double> subDists;
  std::vector<double> allDists;
  for (const auto & c : m_subClusters) {
    for (const auto & o : other.subClusters) {
      DistanceData distanceData =  c.isInsideCluster(o);
      alldists.append(distanceData.distance);
      if (distanceData.isLink)
        subDists.append(distanceData.distance)
    }
  }
  if (subDists.size() >  0) {
    return DistanceData{true, min(subDists)
    }
  else
    return DistanceData{false, min(allDists)};


DistanceData Cluster::distanceCluster(const Cluster& other) const {
  //now we have original unmerged clusters so we can compare directly to see if they overlap
  if (subClusters.size()==1 && other.subClusters().size()==1) {
    dR = deltaR(m_position.Theta(),
                m_position.Phi(),
                other.Theta(),
                other.Phi())
    bool linkOK = (dR < m_angularSize + other.angularSize());
    return DistanceData(linkOK,  dR);
  }
  else
    std::cout<<"Error this should not be reacheded unless is is an unmerged cluster";
  return DistanceData{false,-1};
}

  std::vector<double> subDists;
  for (auto id : m_subClusters) {
    dist=(subc.position - point).Mag()
    if (dist < subc.size())
      subdist.append(dist)
      }
   if (subDists.size()>0))
     return DistanceData(true, min(subDists));

subdists = [ (subc.position - point).Mag() for subc in self.subclusters ]
dist = min(subdists)
return False, dist
}
*/
  

Cluster& Cluster::operator+=(const Cluster& rhs){

  if (Identifier::layer(m_uniqueID) != Identifier::layer(rhs.ID())) {
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



/*DistanceData Cluster::distance(const Track& track) {
   return track.distance(this);
}*/



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





/*
bool Cluster::isInside(const TVector3& point) const
{
   return (pointDistance(point)< m_size);
   //AJR TODO
   /*subdists = [ (subc.position - point).Mag() for subc in m_subclusters ]
    dist = min(subdists)
    if dist < m_size():
    return True, dist
    else:
 
   double dist = 0.5;
   return std::pair<bool, double>(false, dist);
}

double Cluster::pointDistance(const TVector3& point) const
{
   //AJRTODO min of sub cluster distances
   return (m_position-point).Mag();
}

/*Cluster* Cluster::additem(Cluster* other)
{
   if (other->layer() != m_layer)
      std::cout <<
                "can only add cluster from same layer"; //AJRTODO raise ValueError('can only add a cluster from the same layer');
   TVector3 position = m_position * m_energy + other->position() *
                       other->energy();
   double energy = m_energy + other->energy();
   double denom  = 1. / energy;
   position *= denom;
   m_position = position;
   m_energy = energy;
   m_subclusters.splice(m_subclusters.begin(),
                        m_subclusters); //other->subClusters());
   return this;
}*/




Track::Track(const TVector3 p3, double charge,Path& path, long id) :
m_uniqueID(id),m_p3(p3),m_charge(charge),m_path(path)
{
}




SimParticle::SimParticle(long uniqueid,int pdgid, TLorentzVector tlv, double  field,TVector3 vertex) :
   Particle(uniqueid, pdgid, ParticleData::particleCharge(pdgid), tlv),
   m_vertex(vertex),
   m_path(tlv,vertex),
   m_helix(field,charge(),tlv,vertex),
   m_isHelix(fabs(charge())>0.5)
{
};

TVector3 SimParticle::pathPosition(std::string name)
{
   return path().namedPoint(name);
}

TLorentzVector makeParticleLorentzVector(int pdgid, double theta, double  phi,
      double energy)
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


/*SimParticle::SimParticle( TLorentzVector& tlv, TVector3& vertex, double charge, int pdgid):
Particle(pdgid, charge, tlv), m_vertex(vertex) {
       self.path = None
     self.clusters = dict()
     self.track = Track(self.p3(), self.q(), selfget.path)
     self.clusters_smeared = dict()
     self.track_smeared = None
}*/

bool SimParticle::isCharged() const
{ //TODO ask Colin if this is really OK
   unsigned int kind = abs(pdgid());
   if (kind == 11 || kind == 22) {
      return true;
   } else
      return false;

};
