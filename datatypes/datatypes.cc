//
//  Created by Alice Robson on 09/11/15.
//
//

#include<cmath>
#include<iostream>
#include<utility>
#include "datatypes.h"
#include "identifier.h"
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


double Cluster::s_maxenergy = 0;


Cluster::Cluster(double energy, const TVector3& position, double size_m,
                 long id) :
   m_uniqueid(id),  m_position(position)
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

void Cluster::setEnergy(double energy)
{
   m_energy = energy;
   if (energy > s_maxenergy)
      s_maxenergy = energy;
   m_pt = energy * m_position.Unit().Perp() ;
}

DistanceData Cluster::getDistance(const Cluster& clust) const{
   fastsim::enumLayer layer1 = Identifier::getLayer(m_uniqueid);
   fastsim::enumLayer layer2 = Identifier::getLayer(clust.getID());
      double dR = deltaR(m_position.Theta(),
                  m_position.Phi(),
                  clust.getPosition().Theta(),
                         clust.getPosition().Phi());
      bool link_ok = dR < m_angularsize + clust.getAngularSize();
      return DistanceData{layer1,layer2,link_ok,dR};
}
   
/*DistanceData Cluster::getDistance(const Track& track) {
   return track.getDistance(this);
}*/




/*Cluster::Cluster( Cluster && c) :
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






bool Cluster::isInside(const TVector3& point) const
{
   return (getPointDistance(point)< m_size);
   //AJR TODO
   /*subdists = [ (subc.position - point).Mag() for subc in m_subclusters ]
    dist = min(subdists)
    if dist < m_size():
    return True, dist
    else:
 
   double dist = 0.5;
   return std::pair<bool, double>(false, dist);*/
}

double Cluster::getPointDistance(const TVector3& point) const
{
   //AJRTODO min of sub cluster distances
   return (m_position-point).Mag();
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




Track::Track(const TVector3 p3, double charge,Path& path, long id) :
m_uniqueid(id),m_p3(p3),m_charge(charge),m_path(&path)
{
}

DistanceData Track::getDistance(const Track& track) const{
   fastsim::enumLayer layer1 = Identifier::getLayer(m_uniqueid);
   fastsim::enumLayer layer2 = Identifier::getLayer(track.getID());
   return DistanceData{layer1,layer2,false,0};
}

DistanceData Track::getDistance(const Cluster& clust) {
   fastsim::enumLayer layer1 = Identifier::getLayer(m_uniqueid);
   fastsim::enumLayer layer2 = Identifier::getLayer(clust.getID());
   
   std::string lname="_ECALin";
   if(layer2==fastsim::enumLayer::HCAL) //TODO sort this and make an ENUM instead of lname
      lname="_HCALin";
   
   if (m_path->hasNamedPoint(lname)) { //check exists {
      TVector3 pos =m_path->getNamedPoint(lname);
      if (clust.isInside(pos)) {
         return DistanceData{layer1,layer2,true,clust.getPointDistance(pos)};
      }
   }
   return DistanceData{layer1,layer2,false,0};
}


SimParticle::SimParticle(long uniqueid,int pdgid, TLorentzVector tlv, double  field,TVector3 vertex) :
   Particle(uniqueid, pdgid, ParticleData::getParticleCharge(pdgid), tlv),
   m_vertex(vertex),
   m_path(tlv,vertex),
   m_helix(field,getCharge(),tlv,vertex),
   m_isHelix(fabs(getCharge())>0.5)
{
};

TVector3 SimParticle::getPathPosition(std::string name)
{
   return getPath().getNamedPoint(name);
}

TLorentzVector makeParticleLorentzVector(int pdgid, double theta, double  phi,
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

bool SimParticle::IsCharged() const
{ //TODO ask Colin if this is really OK
   unsigned int kind = abs(getPdgid());
   if (kind == 11 || kind == 22) {
      return true;
   } else
      return false;

};
