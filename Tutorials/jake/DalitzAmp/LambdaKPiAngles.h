#if !defined(LAMBDAKPIANGLES)
#define LAMBDAKPIANGLES

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

class LambdaKPiAngles : public UserAmplitude< LambdaKPiAngles >{
    
public:
	
  LambdaKPiAngles() : UserAmplitude< LambdaKPiAngles >() { }

  LambdaKPiAngles( const vector< string >& args ); 

  ~LambdaKPiAngles(){}
	
  string name() const { return "LambdaKPiAngles"; }
    
  complex< GDouble > calcAmplitude( GDouble** pKin ) const;

#ifdef GPU_ACCELERATION

  void launchGPUKernel( dim3 dimGrid, dim3 dimBlock, GPU_AMP_PROTO ) const;

        bool isGPUEnabled() const { return true; }

#endif // GPU_ACCELERATION

private:
        
  float m_j;
  int m_s;
  float m_li;
  float m_lf;

};

#endif
