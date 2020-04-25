#include <cassert>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <cmath>

#include "IUAmpTools/Kinematics.h"
#include "DalitzAmp/Xi0KKAngles.h"
#include "DalitzAmp/clebschGordan.h"
#include "DalitzAmp/wignerD.h"

#include "TLorentzVector.h"
#include "TLorentzRotation.h"
#include "TVector3.h"

Xi0KKAngles::Xi0KKAngles( const vector< string >& args ) :
UserAmplitude< Xi0KKAngles >( args )
{ 

  assert( args.size() == 5 );

  m_j = atof( args[0].c_str() );       // total J of resonance
  m_s = atoi( args[1].c_str() );       // identity of spectator (1-3)
  m_li = atof( args[2].c_str() );      // helicity of initial state
  m_lf = atof( args[3].c_str() );      // helicity of final state
  m_P = AmpParameter(args[4]);         //Polarization Parameters

  //assert( m_li == 0.5 || m_li == -0.5 );
  //assert( m_lf == 0.5 || m_lf == -0.5 );

  registerParameter( m_P );
  
}

complex< GDouble >
Xi0KKAngles::calcAmplitude( GDouble** pKin ) const {

  // The amplitude results
  complex< GDouble > term( 0, 0 );

  // if non-resonant, just return a constant
  if( m_j == -1 ) return complex< GDouble >( 1, 1 );
  
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

  
  // the angular variables for the X
  TVector3 x = Xic.Vect().Unit();
  TVector3 bdir (0.0, 0.0, 1.0);
  TVector3 z = bdir.Cross(x).Unit(); //Polarization Unit Vector of Xic0
  TVector3 y = z.Cross(x).Unit();

  TVector3 XAngles( (resonance.Vect()).Dot(x),
 		    (resonance.Vect()).Dot(y),
  		    (resonance.Vect()).Dot(z) );
  GDouble cosTheta_X = XAngles.CosTheta();
  GDouble phi_X = XAngles.Phi();
  GDouble thetaX = acos( cosTheta_X );
  
  // the angular variables for the L
  TVector3 zp = resonance.Vect().Unit();
  TVector3 yp = zp.Cross(z).Unit();
  TVector3 xp = yp.Cross(zp).Unit();
  // boost to the resonance rest frame
  TLorentzVector X_res = (m_s == 1) ? resRestBoost * K1_CM : resRestBoost * Xi_CM;

  TVector3 LAngles( (X_res.Vect()).Dot(xp),
  		    (X_res.Vect()).Dot(yp),
  		    (X_res.Vect()).Dot(zp) );
  GDouble cosTheta_L = LAngles.CosTheta();
  GDouble phi_L = LAngles.Phi();
  GDouble thetaL = acos( cosTheta_L );
  

  // normalization factors
  GDouble coeff = (sqrt(2.)*sqrt(2.*m_j+1)/(4*3.14159265));

  // helicity of the spectator
  float ls = (m_s == 1) ? m_lf : 0;
  // helicity of the X daughter (K or Xi)
  float ld = (m_s == 1) ? 0 : m_lf;

  // sum over helicities of the resonance
  for( float lx = -m_j; lx <= m_j; ++lx ){
    if( std::abs(lx - ls) != std::abs(m_lf) ) continue;
    //term += wignerDSmall(m_j, lx, ld, thetaL );
    //term += wignerDSmall( 0.5, m_li, (lx - ls), thetaX ) * wignerDSmall(m_j, lx, ld, thetaL );
    term += ( coeff * wignerD( 0.5, m_li, (lx-ls), cosTheta_X, phi_X, 0 ) *
    	      wignerD( m_j, lx, ld, cosTheta_L, phi_L, 0 ) );

    //std::cout << "Amp: M=" << m_li << ", Ls=" << ls << ", Lx-Ls=" << lx-ls << ", Jx=" << m_j << ", Lx=" << lx << ", Lf=" << m_lf << std::endl;
  }

  //std::cout << std::endl;
  return term * complex< GDouble >( 1 - m_P * cosTheta_X, 1 + m_P * cosTheta_X );

}


#ifdef GPU_ACCELERATION
void
Xi0KKAngles::launchGPUKernel( dim3 dimGrid, dim3 dimBlock, GPU_AMP_PROTO ) const {

  GPUXi0KKAngles_exec( dimGrid, dimBlock, GPU_AMP_ARGS,
		       m_j, m_s, m_li, m_lf, m_P );

}
#endif //GPU_ACCELERATION
