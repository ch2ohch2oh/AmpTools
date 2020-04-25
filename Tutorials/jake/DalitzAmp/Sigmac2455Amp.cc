#include <cassert>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <cmath>

#include "IUAmpTools/Kinematics.h"
#include "DalitzAmp/Sigmac2455Amp.h"
#include "DalitzAmp/clebschGordan.h"
#include "DalitzAmp/wignerD.h"

#include "TLorentzVector.h"
#include "TLorentzRotation.h"
#include "TVector3.h"

Sigmac2455::Sigmac2455(const vector<string> &args) : UserAmplitude<Sigmac2455>(args)
{

    assert(args.size() == 3);

    m_pion = atof(args[0].c_str());    //
    m_initial = atoi(args[1].c_str()); //
    m_final = atof(args[2].c_str());   //

    // Some sanity checks
    // Pion should not be the 0-th particle
    assert(m_pion == 1 || m_pion == 2);

    // S_z for Lambda_c(2625) should be 
    // one of the following +-1/2, +-3/2
    assert(fabs(m_initial) == 0.5 || fabs(m_intial) == 0.15);

    // S_z of the ground state Lambda_c should be +-1/2
    assert(fabs(m_final) == 0.5);
}

complex<GDouble>
Sigmac2455Amp::calcAmplitude(GDouble **pKin) const
{
    // The amplitude results
    complex<GDouble> term(0, 0);

    // 4-vectors are of the form: x, y, z, t
    TLorentzVector Lambdac(pKin[0][1], pKin[0][2], pKin[0][3], pKin[0][0]);
    TLorentzVector pion1(pKin[1][1], pKin[1][2], pKin[1][3], pKin[1][0]);
    TLorentzVector pion2(pKin[2][1], pKin[2][2], pKin[2][3], pKin[2][0]);

    // Boost things into the CM frame
    TLorentzVector Lambdac2625 = Lambdac + pion1 + pion2;

    // The boost vector is (beta_x, beta_y, beta_z)
    TLorentzRotation Lambdac2625RestBoost(-Lambdac2625RestBoost.BoostVector()); 
    TLorentzVector Lambdac_CM = Lambdac2625RestBoost * Lambdac;
    TLorentzVector pion1_CM = Lambdac2625RestBoost * pion1;
    TLorentzVector pion2_CM = Lambdac2625RestBoost * pion2;

    // sigma \dot p_2
    TMatrix factor1(2, 2);
    factor1 = sigma_x * pion2[1] + sigma_y * pion2[2] + sigma_z * pion2[3];
    
    // \simga \dot p_1
    TMatrix factor2(2, 2);
    factor2 = simga_x * pion1[1] + sigma_y * pion1[2] + sigma_z * pion1[3];
    
    // S \dot p_1
    TMatrix factor3(2, 3);
    factor3 = S_x * pion1[1] + S_y * pion1[2] + S_z * pion1[3];



    return term;
}

#ifdef GPU_ACCELERATION
void LambdaKPiAngles::launchGPUKernel(dim3 dimGrid, dim3 dimBlock, GPU_AMP_PROTO) const
{

    GPULambdaKPiAngles_exec(dimGrid, dimBlock, GPU_AMP_ARGS,
                            m_j, m_s, m_li, m_lf);
}
#endif //GPU_ACCELERATION
