#include <cassert>
#include <iostream>
#include <string>
#include <complex>
#include <cstdlib>

#include "TLorentzVector.h"
#include "IUAmpTools/Kinematics.h"
#include "DalitzAmp/Cheby.h"

Cheby::Cheby( const vector< string >& args ) :
UserAmplitude< Cheby >(args)
{

  assert( args.size() == 4 );

  m_real = AmpParameter(args[0]);
  m_imaginary = AmpParameter(args[1]);
  m_daughter1 = atoi(args[2].c_str());
  m_daughter2 = atoi(args[3].c_str());
  
  // need to register any free parameters so the framework knows about them
  registerParameter( m_real );
  registerParameter( m_imaginary );

}


complex< GDouble >
Cheby::calcAmplitude( GDouble** pKin, GDouble* userVars ) const {

  /****
   *  Equivalently one could switch needsUserVarsOnly to false
   *  and uncomment the following lines to get the same 
   *  result without the calcUserVars function defined. 
   *
   *  The role of user data is to optimize this function
   *  call in the instance it is repeated multiple times throughout
   *  a fit, which only happens if there are free parameters
   *  in this amplitude calculation (as there are in this one).
   
   TLorentzVector P1(pKin[m_daughter1-1][1], pKin[m_daughter1-1][2],
                      pKin[m_daughter1-1][3], pKin[m_daughter1-1][0]);

  TLorentzVector P2(pKin[m_daughter2-1][1], pKin[m_daughter2-1][2],
                      pKin[m_daughter2-1][3], pKin[m_daughter2-1][0]);

  GDouble mass2 = (P1+P2).M2();
  */
  
  GDouble mass2 = userVars[kMass2];
    
  return  complex<GDouble>(1.0 + m_real*mass2,m_imaginary*mass2);

}

void
Cheby::calcUserVars( GDouble** pKin, GDouble* userVars ) const {
  
  // This method can be used to calculate more CPU-intensive quantities
  // that the amplitudes themselves depend on.  IMPORTANT:  it is called
  // ONCE per fit so do NOT calculate quantities that depend on
  // free parameters in the fit.
  //
  // These quantities are always calculated on the CPU and, in the
  // case of GPU accelerated fits, they wil be propagated to the GPU
  // for use in the amplitude kernel.  It is recommended to index
  // them with an enum.  Unfortunately there is no straightforward
  // way to propagate this indexing to the GPU, so the GPU kernel
  // will need to fetch the quantities using integers.
  
  
  TLorentzVector P1(pKin[m_daughter1-1][1], pKin[m_daughter1-1][2],
                    pKin[m_daughter1-1][3], pKin[m_daughter1-1][0]);
  
  TLorentzVector P2(pKin[m_daughter2-1][1], pKin[m_daughter2-1][2],
                    pKin[m_daughter2-1][3], pKin[m_daughter2-1][0]);

  userVars[kMass2] = (P1+P2).M2();
}


#ifdef GPU_ACCELERATION
void
Cheby::launchGPUKernel( dim3 dimGrid, dim3 dimBlock, GPU_AMP_PROTO ) const {
  
  Cheby_exec( dimGrid,  dimBlock, GPU_AMP_ARGS, 
                       m_real, m_imaginary, m_daughter1, m_daughter2 );

}
#endif //GPU_ACCELERATION
