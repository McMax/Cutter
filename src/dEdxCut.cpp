#include "TCutG.h"

#include <iostream>

#include "dEdxCut.h"

//Definitions of graphical cuts for all pp energies and 20/158 energies of PbPb
TCutG* initialise_dedx_cutg(TString system, Int_t energy)
{
	TCutG *cutg = new TCutG();

	if(!(system.CompareTo("pp")))
	{
		switch(energy)
		{
			case 158:
			{
				std::cout << "Using p+p@158 cut (12E002)" << std::endl;
				cutg = new TCutG("dedx_cut",10);
				cutg->SetTitle("e^{-} dE/dx cut for 12E002 p+p @ 158 GeV/c");
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
				break;
			}

			case 80:
			{
				std::cout << "Using p+p@80 cut (12E002)" << std::endl;
				cutg = new TCutG("dedx_cut",13);
				cutg->SetTitle("e^{-} dE/dx cut for 12E002 p+p @ 80 GeV/c");
				cutg->SetFillColor(1);
				cutg->SetPoint(0,0.284871,1.84884);
				cutg->SetPoint(1,0.369055,1.36919);
				cutg->SetPoint(2,0.705019,1.33804);
				cutg->SetPoint(3,2.03808,1.38164);
				cutg->SetPoint(4,5.38127,1.43148);
				cutg->SetPoint(5,12.3227,1.46885);
				cutg->SetPoint(6,24.1579,1.52492);
				cutg->SetPoint(7,23.5404,1.62458);
				cutg->SetPoint(8,8.14319,1.76786);
				cutg->SetPoint(9,2.67477,1.89867);
				cutg->SetPoint(10,0.974441,1.95473);
				cutg->SetPoint(11,0.404061,1.96096);
				cutg->SetPoint(12,0.284871,1.84884);
				break;
			}

			case 40:
			{
				std::cout << "Using p+p@40 cut (12E002)" << std::endl;
				cutg = new TCutG("dedx_cut",12);
				cutg->SetTitle("e^{-} dE/dx cut for 12E002 p+p @ 40 GeV/c");
				cutg->SetFillColor(1);
				cutg->SetPoint(0,0.171942,1.26952);
				cutg->SetPoint(1,0.761963,1.33804);
				cutg->SetPoint(2,2.11879,1.38164);
				cutg->SetPoint(3,9.6357,1.48754);
				cutg->SetPoint(4,13.4915,1.51246);
				cutg->SetPoint(5,10.8264,1.72425);
				cutg->SetPoint(6,1.47457,1.87998);
				cutg->SetPoint(7,0.580588,1.91736);
				cutg->SetPoint(8,0.153032,1.86752);
				cutg->SetPoint(9,0.0843649,1.72425);
				cutg->SetPoint(10,0.0960249,1.61836);
				cutg->SetPoint(11,0.171942,1.26952);
				break;
			}

			case 31:
			{
				std::cout << "Using p+p@31 cut (12E002)" << std::endl;
				cutg = new TCutG("dedx_cut",13);
				cutg->SetTitle("e^{-} dE/dx cut for 12E002 p+p @ 31 GeV/c");
				cutg->SetFillColor(1);
				cutg->SetPoint(0,0.127665,1.26329);
				cutg->SetPoint(1,0.420062,1.30689);
				cutg->SetPoint(2,1.06687,1.3505);
				cutg->SetPoint(3,2.78069,1.40656);
				cutg->SetPoint(4,10.1478,1.46262);
				cutg->SetPoint(5,12.6459,1.51869);
				cutg->SetPoint(6,12.1642,1.6495);
				cutg->SetPoint(7,2.74493,1.74917);
				cutg->SetPoint(8,0.611444,1.81146);
				cutg->SetPoint(9,0.267016,1.83638);
				cutg->SetPoint(10,0.101128,1.73671);
				cutg->SetPoint(11,0.0877058,1.56229);
				cutg->SetPoint(12,0.127665,1.26329);
				break;
			}

			case 20:
			{
				std::cout << "Using p+p@20 cut (12E002)" << std::endl;
				cutg = new TCutG("dedx_cut",11);
				cutg->SetTitle("e^{-} dE/dx cut for 12E002 p+p @ 20 GeV/c");
				cutg->SetFillColor(1);
				cutg->SetPoint(0,0.0459112,1.62458);
				cutg->SetPoint(1,0.627482,1.78654);
				cutg->SetPoint(2,3.20625,1.7554);
				cutg->SetPoint(3,9.38942,1.61836);
				cutg->SetPoint(4,9.14944,1.48131);
				cutg->SetPoint(5,3.46521,1.42525);
				cutg->SetPoint(6,1.0396,1.38787);
				cutg->SetPoint(7,0.465898,1.31312);
				cutg->SetPoint(8,0.193189,1.27575);
				cutg->SetPoint(9,0.0843649,1.14493);
				cutg->SetPoint(10,0.0459112,1.62458);
				break;
			}
		}
	}
	else if(!(system.CompareTo("BeBe")))
	{
		switch(energy)
		{
			case 150:
			{
				cutg = new TCutG("dedx_cut",12);
				cutg->SetTitle("e^{-} dE/dx cut for Be+Be @ 150 GeV/c");
				cutg->SetFillColor(1);
				cutg->SetPoint(0,0.5747005,1.803988);
				cutg->SetPoint(1,0.7542676,1.366245);
				cutg->SetPoint(2,0.8557563,1.341926);
				cutg->SetPoint(3,2.03808,1.38164);
				cutg->SetPoint(4,5.38127,1.43148);
				cutg->SetPoint(5,12.3227,1.55);
				cutg->SetPoint(6,23.5404,1.62458);
				cutg->SetPoint(7,8.14319,1.76786);
				cutg->SetPoint(8,2.67477,1.89867);
				cutg->SetPoint(9,0.974441,1.95473);
				cutg->SetPoint(10,0.7917934,1.954767);
				cutg->SetPoint(11,0.5747005,1.803988);
				break;
			}
			case 75:
			{
				cutg = new TCutG("dedx_cut",12);
				cutg->SetTitle("e^{-} dE/dx cut for Be+Be @ 75 GeV/c");
				cutg->SetFillColor(1);
				cutg->SetPoint(0,0.284871,1.84884);
				cutg->SetPoint(1,0.369055,1.36919);
				cutg->SetPoint(2,0.705019,1.33804);
				cutg->SetPoint(3,2.03808,1.38164);
				cutg->SetPoint(4,5.38127,1.43148);
				cutg->SetPoint(5,12.3227,1.55);
				cutg->SetPoint(6,23.5404,1.62458);
				cutg->SetPoint(7,8.14319,1.76786);
				cutg->SetPoint(8,2.67477,1.89867);
				cutg->SetPoint(9,0.974441,1.95473);
				cutg->SetPoint(10,0.404061,1.96096);
				cutg->SetPoint(11,0.284871,1.84884);
				break;
			}
		}
	}
	else if(!(system.CompareTo("PbPb")))
	{
		switch(energy)
		{
			case 160:
			case 158:
				{
					std::cout << "Using Pb+Pb@158 cut (00B)" << std::endl;
					cutg = new TCutG("dedx_cut",18);
					cutg->SetTitle("e^{-} dE/dx cut for 00B Pb+Pb @ 160 GeV/c");
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
					break;
				}

			case 20:
				{
					std::cout << "Using Pb+Pb@20 cut (03A)" << std::endl;
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
					break;
				}
		}
	}
	else
		std::cout << "Unknown system" << std::endl << "Type \"pp\" or \"PbPb\"" << std::endl;

	cutg->SetVarX("TMath::Log10(p)");
	cutg->SetVarY("dedx");

	return cutg;
}
