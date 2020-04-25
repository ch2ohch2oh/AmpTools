
#include "DalitzPlot/DalitzPlotGenerator.h"
#include "IUAmpTools/Histogram1D.h"
#include "IUAmpTools/Kinematics.h"
#include "TLorentzVector.h"

DalitzPlotGenerator::DalitzPlotGenerator( const FitResults& results ) :
PlotGenerator( results )
{
  bookHistogram( khm12, new Histogram1D( 40, 1.8, 2.0, "hm12", "M( #Xi^{0} K^{+} )" ) );
  bookHistogram( khm13, new Histogram1D( 40, 1.8, 2.0, "hm13", "M( #Xi^{0} K^{-} )" ) );
  bookHistogram( khm23, new Histogram1D( 38, 0.98, 1.17, "hm23", "M( K^{+} K^{-} )" ) );
  bookHistogram( kdltz, new Histogram2D( 76, 3.24, 4.0, 59, 0.96, 1.37, "dltz", "Dalitz Plot" ) );
}

void
DalitzPlotGenerator::projectEvent( Kinematics* kin ){
          
  TLorentzVector P1 = kin->particle(0);
  TLorentzVector P2 = kin->particle(1);
  TLorentzVector P3 = kin->particle(2);
      
  fillHistogram( khm12, (P1+P2).M() );
  fillHistogram( khm13, (P1+P3).M() );
  fillHistogram( khm23, (P2+P3).M() );
  fillHistogram( kdltz, (P1+P2).M2(), (P2+P3).M2() );
}
