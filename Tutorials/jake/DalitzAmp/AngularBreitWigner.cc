#include <cassert>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <cmath>

#include "IUAmpTools/Kinematics.h"
#include "DalitzAmp/AngularBreitWigner.h"

#include "TLorentzVector.h"
#include "TLorentzRotation.h"
#include "TVector3.h"

AngularBreitWigner::AngularBreitWigner( const vector< string >& args ) :
UserAmplitude< AngularBreitWigner >( args )
{ 
  assert( args.size() == 7 );

  m_PHSP = atof( args[0].c_str() );       // PHSP or resonance
  //
  m_mass = AmpParameter( args[1] );
  m_width = AmpParameter( args[2] );
  //
  m_s = atoi( args[3].c_str() );       // identity of spectator (1-3)
  m_compP = atoi( args[4].c_str() );   // (m_compP == 1) Use the polarization from bdir.cross(x) Else: use m_P variable to fit
  //
  m_alpha = AmpParameter(args[5]);     // Assymetry Parameter
  m_P = AmpParameter(args[6]);         // Polarization of Parent Baryon (Unused if m_compP == 0)

  registerParameter( m_mass );
  registerParameter( m_width );
  registerParameter( m_alpha );
  registerParameter( m_P );
}

complex< GDouble >
AngularBreitWigner::calcAmplitude( GDouble** pKin ) const {

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

  // the resonance is the particles not defined by m_s
  TLorentzVector resonance = XicRestBoost*parts[m_s%3] + XicRestBoost*parts[(m_s+1)%3];
  // construct the boost needed to get to cm frame of the resonance
  TLorentzRotation resRestBoost( -resonance.BoostVector() );

  GDouble mass2 = resonance.M2();

  // the angular variables for the parent baryon
  TVector3 x = Xic.Vect().Unit();
  TVector3 bdir (0.0, 0.0, 1.0);
  TVector3 z = bdir.Cross(x).Unit();
  TVector3 y = z.Cross(x).Unit();

  TVector3 XAngles( (resonance.Vect()).Dot(x),
		    (resonance.Vect()).Dot(y),
		    (resonance.Vect()).Dot(z) );

  GDouble cosTheta_X = XAngles.CosTheta();
  GDouble phi_X = XAngles.Phi();

  // the angular variables for the resonance
  //TVector3 zp = resonance.Vect().Unit();
  //TVector3 yp = zp.Cross(z).Unit();
  //TVector3 xp = yp.Cross(zp).Unit();

  // boost to the resonance rest frame
  //TLorentzVector X_res = (m_s == 1) ? resRestBoost * Xi_CM : resRestBoost * K1_CM;

  //TVector3 LAngles( (X_res.Vect()).Dot(xp),
  //		    (X_res.Vect()).Dot(yp),
  //		    (X_res.Vect()).Dot(zp) );

  //GDouble cosTheta_L = LAngles.CosTheta();
  //GDouble phi_L = LAngles.Phi();

  double P = (m_compP == 1) ? z.Mag() : m_P;

  GDouble e = K1.M2() / resonance.M2();
  
  return complex< GDouble >( 1 + m_alpha * P * cosTheta_X, 0) * complex< GDouble >( sqrt( 1 - 4*e ), 0) * complex< GDouble >( ( 1 + 2*e ), 0) / complex<GDouble>( mass2 - m_mass*m_mass, m_mass*m_width);
  
}


#ifdef GPU_ACCELERATION
void
AngularBreitWigner::launchGPUKernel( dim3 dimGrid, dim3 dimBlock, GPU_AMP_PROTO ) const {

  GPUAngularBreitWigner_exec( dimGrid, dimBlock, GPU_AMP_ARGS,
			      m_PHSP, m_mass, m_width, m_s, m_alpha, m_P );

}
#endif //GPU_ACCELERATION
