#include "TCutG.h"

#include <iostream>

#include "dEdxCut.h"

TCutG* initialise_dedx_cutg(Int_t energy)
{
	TCutG *cutg = new TCutG();
	if((energy == 160) || (energy == 158))
	{

		cutg = new TCutG("dedx_cut",18);
		//cutg->SetTitle("e^{-} dE/dx cut for 12E002 p+p @ 158 GeV/c");
		cutg->SetTitle("e^{-} dE/dx cut for 00B Pb+Pb @ 160 GeV/c");
		/*
		   cutg->SetFillColor(1);
		   cutg->SetPoint(0,0.344283,1.90);
		   cutg->SetPoint(1,0.344283,1.20);
		   cutg->SetPoint(2,1.58,1.35);
		   cutg->SetPoint(3,3.4829,1.40727);
		   cutg->SetPoint(4,10.50171,1.45138);
		   cutg->SetPoint(5,20,1.49148);
		   cutg->SetPoint(6,20,1.65589);
		   cutg->SetPoint(7,2.57344,1.90);
		   cutg->SetPoint(8,0.396975,1.90);
		   cutg->SetPoint(9,0.3442825,1.90);
		 */
		cutg->SetPoint(0,0.425317,1.89959);
		cutg->SetPoint(1,2.7223,1.89959);
		cutg->SetPoint(2,4.71651,1.86649);
		cutg->SetPoint(3,7.05759,1.83339);
		cutg->SetPoint(4,10.8218,1.7861);
		cutg->SetPoint(5,19.449,1.66315);
		cutg->SetPoint(6,19.2129,1.5733);
		cutg->SetPoint(7,12.0792,1.53074);
		cutg->SetPoint(8,6.02148,1.48818);
		cutg->SetPoint(9,3.78571,1.43143);
		cutg->SetPoint(10,2.59251,1.38887);
		cutg->SetPoint(11,1.5522,1.35577);
		cutg->SetPoint(12,1.02473,1.28484);
		cutg->SetPoint(13,0.701748,1.25646);
		cutg->SetPoint(14,0.529891,1.21863);
		cutg->SetPoint(15,0.376419,1.2139);
		cutg->SetPoint(16,0.376419,1.89959);
		cutg->SetPoint(17,0.425317,1.89959);

	}
	else if(energy == 20)
	{

		cutg = new TCutG("dedx_cut",15);
		cutg->SetTitle("e^{-} dE/dx cut for 03A Pb+Pb @ 20 GeV/c");
		cutg->SetFillColor(1);
		cutg->SetPoint(0,0.0606792,1.7197);
		cutg->SetPoint(1,1.02568,1.83407);
		cutg->SetPoint(2,2.90105,1.80999);
		cutg->SetPoint(3,6.87075,1.70766);
		cutg->SetPoint(4,6.87075,1.60534);
		cutg->SetPoint(5,2.90105,1.52709);
		cutg->SetPoint(6,1.59862,1.44282);
		cutg->SetPoint(7,0.837351,1.37059);
		cutg->SetPoint(8,0.523797,1.34651);
		cutg->SetPoint(9,0.260795,1.31641);
		cutg->SetPoint(10,0.192372,1.2803);
		cutg->SetPoint(11,0.157049,1.21409);
		cutg->SetPoint(12,0.0630318,1.25622);
		cutg->SetPoint(13,0.0599146,1.5933);
		cutg->SetPoint(14,0.0606792,1.7197);

	}
	cutg->SetVarX("TMath::Log10(p)");
	cutg->SetVarY("dedx");

	return cutg;
}
