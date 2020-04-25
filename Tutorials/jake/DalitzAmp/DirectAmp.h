#if !defined(DIRECTAMP)
#define DIRECTAMP

#include "IUAmpTools/Amplitude.h"
#include "IUAmpTools/UserAmplitude.h"
#include "IUAmpTools/AmpParameter.h"
#include "GPUManager/GPUCustomTypes.h"

#include <utility>
#include <string>
#include <complex>
#include <vector>

#ifdef GPU_ACCELERATION
void GPULambdaKPiAngles_exec(dim3 dimGrid, dim3 dimBlock, GPU_AMP_PROTO,
                             float j, int s, float li, float lf);

#endif //GPU_ACCELERATION

using std::complex;
using namespace std;

class Kinematics;

class DirectAmp : public UserAmplitude<DirectAmp>
{

public:
    DirectAmp() : UserAmplitude<Sigmac2455Amp>() {}

    Sigmac2DirectAmp455Amp(const vector<string> &args);

    ~DirectAmp() {}

    string name() const { return "DirectAmp"; }

    complex<GDouble> calcAmplitude(GDouble **pKin) const;

#ifdef GPU_ACCELERATION

    void launchGPUKernel(dim3 dimGrid, dim3 dimBlock, GPU_AMP_PROTO) const;

    bool isGPUEnabled() const { return true; }

#endif // GPU_ACCELERATION

private:
    float m_initial; // initial z component of spin
    float m_final;   // final z component of spin
};

#endif
