//
//  Created by Alice Robson on 09/11/15.
//
//
/**
 * @brief Base Particle from which more complex particles are derived
 */

#include "baseparticle.h"
#include <iostream>
#include <sstream> //AJRTODO temp
#include "enums.h"
#include "Identifier.h"



//BaseParticle::BaseParticle()
//{}

std::string BaseParticle::StringDescription() const
{
   std::ostringstream oss;

   oss << (std::string)typeid(this).name();
   oss << " : pdgid = " << m_particleId; //AJRTODO  make length 5
   oss << " status = " << m_status; //AJRTODO  make length 3
   oss << " q = " << m_charge; //AJRTODO  make length 3
   oss << " status = " << m_status; //AJRTODO  make length 3
   //todo continue oss<<" status = " << status; //AJRTODO  make length 3
   return oss.str();
}

/*BaseParticle::BaseParticle(int pdgid, double charge, TLorentzVector&& tlv, double status):
   m_tlv(tlv), m_particleId(pdgid), m_charge(charge), m_status(status)
{
    std::cout<<tlv.X() <<" " <<tlv.Y() <<" "<<tlv.Z() <<" "<<tlv.Et() <<" ";
std::cout<<m_tlv.X() <<" " <<m_tlv.Y() <<" "<<m_tlv.Z() <<" "<<m_tlv.Et() <<" ";
}*/


BaseParticle::BaseParticle(int pdgid, double charge, TLorentzVector tlv,
                           double status) :
   m_uniqueid(Identifier::makeParticleID(fastsim::enumSource::NONE)),
   m_tlv(tlv), m_particleId(pdgid), m_charge(charge), m_status(status)

{

   /*std::cout << "BASE " << tlv.X() << " " << tlv.Y() << " " << tlv.Z() << " " <<
             tlv.Et() << " ";
   std::cout << m_tlv.X() << " " << m_tlv.Y() << " " << m_tlv.Z() << " " <<
             m_tlv.Et() << " ";*/
}


BaseParticle::BaseParticle(int pdgid, double charge) :
   m_uniqueid(Identifier::makeParticleID(fastsim::enumSource::NONE)),
   m_particleId(pdgid), m_charge(charge), m_status(0)
{
   m_tlv = TLorentzVector{0., 0., 0., 0.};
   ;
}
//TODO tidy tlv
BaseParticle::BaseParticle() :
   m_uniqueid(Identifier::makeParticleID(fastsim::enumSource::NONE)),
   m_particleId(0), m_charge(0), m_status(0)
{

}

BaseParticle::BaseParticle(long id, int pdgid, double charge) :
   m_uniqueid(id), m_particleId(pdgid), m_charge(charge), m_status(0)
{
   m_tlv = TLorentzVector{0., 0., 0., 0.};

}


BaseParticle::BaseParticle(long id, int pdgid, double charge,
                           TLorentzVector tlv,
                           double status) :
   m_uniqueid(id), m_tlv(tlv), m_particleId(pdgid), m_charge(charge),
   m_status(status)
{

   /*std::cout << "BASE " << tlv.X() << " " << tlv.Y() << " " << tlv.Z() << " " <<
             tlv.Et() << " ";
   std::cout << m_tlv.X() << " " << m_tlv.Y() << " " << m_tlv.Z() << " " <<
             m_tlv.Et() << " ";*/
}



/*string tmp = "{className} : pdgid = {pdgid:5}, status = {status:3}, q = {q:2} e = {e:5.1f}, theta = {theta:5.2f}, phi = {phi:5.2f}, mass = {m:5.2f}"
return tmp.format(
                  className = self.__class__.__name__,
                  pdgid = self.pdgid(),
                  status = self.status(),
                  q = self.q(),
                  e = self.e(),
                  theta = self.theta(),
                  phi = self.phi(),
                  m = self.m()
                  )*/

