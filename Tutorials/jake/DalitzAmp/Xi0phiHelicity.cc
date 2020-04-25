#include <cassert>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <cmath>

#include "IUAmpTools/Kinematics.h"
#include "DalitzAmp/Xi0phiHelicity.h"

#include "TLorentzVector.h"
#include "TLorentzRotation.h"
#include "TVector3.h"

Xi0phiHelicity::Xi0phiHelicity( const vector< string >& args ) :
UserAmplitude< Xi0phiHelicity >( args )
{ 
  assert( args.size() == 4 );
  
  m_PHSP = atof( args[0].c_str() );       // PHSP option complex constant amplitude
  m_s = atoi( args[1].c_str() );       // identity of spectator (1-3)
  m_alpha = AmpParameter(args[2]);     // Assymetry Parameter
  m_P = AmpParameter(args[3]);         // Polarization Parameter of Parent Baryon 

  registerParameter( m_alpha );
  registerParameter( m_P );
}

complex< GDouble >
Xi0phiHelicity::calcAmplitude( GDouble** pKin ) const {

  // if non-resonant, just return a constant
  if( m_PHSP == -1 ) return complex< GDouble >( 1, 1 );
  
  // 4-vectors are of the form: x, y, z, t
  TLorentzVector Xi      ( pKin[0][1], pKin[0][2], pKin[0][3], pKin[0][0] );
  TLorentzVector K1 ( pKin[1][1], pKin[1][2], pKin[1][3], pKin[1][0] );
  TLorentzVector K2     ( pKin[2][1], pKin[2][2], pKin[2][3], pKin[2][0] );

  std::vector<TLorentzVector> parts;
  parts.push_back(Xi); parts.push_back(K1); parts.push_back(K2);
  
  // Boost things into the CM frame
  TLorentzVector Xic = Xi + K1 + K2;
  TLorentzRotation XicRestBoost( -Xic.BoostVector() );
  TLorentzVector Xi_CM = XicRestBoost * Xi;
  TLorentzVector K1_CM = XicRestBoost * K1;
  TLorentzVector K2_CM = XicRestBoost * K2;
  //
  TLorentzRotation XiRestBoost( -Xi_CM.BoostVector() );
  TLorentzVector Lambda = XiRestBoost * Xi_CM;
  
  // the resonance is the particles not defined by m_s
  TLorentzVector resonance = XicRestBoost*parts[m_s%3] + XicRestBoost*parts[(m_s+1)%3];
  
  // the angular variables for the parent baryon
  TVector3 xic = Xic.Vect();
  //TVector3 xi = Xi_CM.Vect().Unit();
  //TVector3 lambda = Lambda.Vect().Unit();
  TVector3 bdir (0, 0, 1.0);
  //TVector3 zprime = resonance.Vect().Unit();
  //TVector3 Polarization = bdir.Cross(xic).Unit();
  TVector3 z = bdir.Cross(xic).Unit();
  TVector3 y = z.Cross(xic).Unit(); 
  
  //GDouble cosTheta_Xic = Polarization.Dot(zprime);
  TVector3 XAngles( (resonance.Vect()).Dot(xic),
		    (resonance.Vect()).Dot(y),
		    (resonance.Vect()).Dot(z) );
  GDouble cosTheta_Xic = XAngles.CosTheta();
  GDouble theta = acos( cosTheta_Xic );
  
  //GDouble cosTheta_Xi0 = xi.Dot(lambda);

  GDouble e = K1.M2() / resonance.M2();
  
  return  complex< GDouble >( 1, 0 ) + complex< GDouble >( m_alpha * m_P * cosTheta_Xic, m_alpha * m_P * sin( theta ) );// +
	   //complex< GDouble >( ( 2 * m_r1 - m_alpha ) * m_XiBr * cosTheta_Xi0, 0 ) +
	   //complex< GDouble >( ( 2 * m_r0 - 1 ) * P * m_XiBr * cosTheta_Xic * cosTheta_Xi0, 0 ) );
  
}


#ifdef GPU_ACCELERATION
void
Xi0phiHelicity::launchGPUKernel( dim3 dimGrid, dim3 dimBlock, GPU_AMP_PROTO ) const {

  GPUXi0phiHelicity_exec( dimGrid, dimBlock, GPU_AMP_ARGS,
			   m_PHSP, m_s, m_alpha, m_P );

}
#endif //GPU_ACCELERATION
