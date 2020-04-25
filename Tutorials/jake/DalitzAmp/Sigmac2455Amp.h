#if !defined(SIGMAC2455AMP)
#define SIGMAC2455AMP

#include "IUAmpTools/Amplitude.h"
#include "IUAmpTools/UserAmplitude.h"
#include "IUAmpTools/AmpParameter.h"
#include "GPUManager/GPUCustomTypes.h"

#include <utility>
#include <string>
#include <complex>
#include <vector>

#ifdef GPU_ACCELERATION
void GPULambdaKPiAngles_exec( dim3 dimGrid, dim3 dimBlock, GPU_AMP_PROTO,
			      float j, int s, float li, float lf );

#endif //GPU_ACCELERATION

using std::complex;
using namespace std;

// j is the total spin of X
// s is the identity of the spectator
// li is the helicity of the initial state
// lf is the helicity of the final state

class Kinematics;

class Sigmac2455Amp : public UserAmplitude< Sigmac2455Amp >{
    
public:
	
  Sigmac2455Amp() : UserAmplitude< Sigmac2455Amp >() { }

  Sigmac2455Amp( const vector< string >& args ); 

  ~Sigmac2455Amp(){}
	
  string name() const { return "Sigmac2455Amp"; }
    
  complex< GDouble > calcAmplitude( GDouble** pKin ) const;

#ifdef GPU_ACCELERATION

  void launchGPUKernel( dim3 dimGrid, dim3 dimBlock, GPU_AMP_PROTO ) const;

        bool isGPUEnabled() const { return true; }

#endif // GPU_ACCELERATION

private:
        
  int m_pion;           // which one is the spectator pion (1 or 2)
  float m_initial;      // initial z component of spin
  float m_final;        // final z component of spin

};

#endif
