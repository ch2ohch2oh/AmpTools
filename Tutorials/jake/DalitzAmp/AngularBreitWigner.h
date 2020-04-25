#if !defined(ANGULARBREITWIGNER)
#define ANGULARBREITWIGNER

#include "IUAmpTools/Amplitude.h"
#include "IUAmpTools/UserAmplitude.h"
#include "IUAmpTools/AmpParameter.h"
#include "GPUManager/GPUCustomTypes.h"

#include <utility>
#include <string>
#include <complex>
#include <vector>

#ifdef GPU_ACCELERATION
void GPUAngularBreitWigner_exec( dim3 dimGrid, dim3 dimBlock, GPU_AMP_PROTO,
				 float PHSP, float mass, float width, int spec, float assym, float polariz );

#endif //GPU_ACCELERATION

using std::complex;
using namespace std;

// j is the total spin of X
// s is the identity of the spectator

class Kinematics;

class AngularBreitWigner : public UserAmplitude< AngularBreitWigner >{
    
public:
	
  AngularBreitWigner() : UserAmplitude< AngularBreitWigner >() { }

  AngularBreitWigner( const vector< string >& args ); 

  ~AngularBreitWigner(){}
	
  string name() const { return "AngularBreitWigner"; }
    
  complex< GDouble > calcAmplitude( GDouble** pKin ) const;

#ifdef GPU_ACCELERATION

  void launchGPUKernel( dim3 dimGrid, dim3 dimBlock, GPU_AMP_PROTO ) const;

        bool isGPUEnabled() const { return true; }

#endif // GPU_ACCELERATION

private:

	float m_PHSP;  
	AmpParameter m_mass;
	AmpParameter m_width;
	int m_s;
	int m_compP;
	AmpParameter m_alpha;
	AmpParameter m_P;

};

#endif
