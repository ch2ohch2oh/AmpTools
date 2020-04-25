#if !defined(XI0PHIHELICITY)
#define XI0PHIHELICITY

#include "IUAmpTools/Amplitude.h"
#include "IUAmpTools/UserAmplitude.h"
#include "IUAmpTools/AmpParameter.h"
#include "GPUManager/GPUCustomTypes.h"

#include <utility>
#include <string>
#include <complex>
#include <vector>

#ifdef GPU_ACCELERATION
void GPUXi0phiHelicity_exec( dim3 dimGrid, dim3 dimBlock, GPU_AMP_PROTO,
			      float phsp, int spec, float assym, float polariz );

#endif //GPU_ACCELERATION

using std::complex;
using namespace std;

// j is the total spin of X
// s is the identity of the spectator

class Kinematics;

class Xi0phiHelicity : public UserAmplitude< Xi0phiHelicity >{
    
public:
	
  Xi0phiHelicity() : UserAmplitude< Xi0phiHelicity >() { }

  Xi0phiHelicity( const vector< string >& args ); 

  ~Xi0phiHelicity(){}
	
  string name() const { return "Xi0phiHelicity"; }
    
  complex< GDouble > calcAmplitude( GDouble** pKin ) const;

#ifdef GPU_ACCELERATION

  void launchGPUKernel( dim3 dimGrid, dim3 dimBlock, GPU_AMP_PROTO ) const;

        bool isGPUEnabled() const { return true; }

#endif // GPU_ACCELERATION

private:
        
  float m_PHSP;
  int m_s;
  AmpParameter m_alpha;
  AmpParameter m_P;

};

#endif
