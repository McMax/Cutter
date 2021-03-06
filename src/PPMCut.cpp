#include <iostream>

#include "TMath.h"

#include "PPMCut.h"

PPMCut::PPMCut(const TString filename, const TString system, const TString energy)
{
	openPPMFile(filename, system, energy);
}

PPMCut::~PPMCut()
{
	closePPMFile();
}

void PPMCut::openPPMFile(const TString filename, const TString system, const TString energy)
{
	static TString temp;

	ppm_file = new TFile(filename, "READ");

	if(ppm_file->IsZombie())
		std::cout << "File " << filename << " not opened" << std::endl;

	temp = system + "/" + energy + "/histPartPopMatrixPos";
	std::cout << "Opening histogram " << temp << std::endl;
	partpopmatrix_pos = (TH3I*)ppm_file->Get(system+"/"+energy+"/histPartPopMatrixPos");
	partpopmatrix_neg = (TH3I*)ppm_file->Get(system+"/"+energy+"/histPartPopMatrixNeg");
}

void PPMCut::closePPMFile()
{
	ppm_file->Close();
}

bool PPMCut::PartPopMatrixCut(const int ch, const double p, const double pt, const double angle)
{
	//std::cout << "angle=" << (angle*180/TMath::Pi()) << " pT=" << pt << " p=" << p << std::endl;
	if(ch>0)
		result = partpopmatrix_pos->GetBinContent(partpopmatrix_pos->FindBin(p,pt,angle));
	else if(ch<0)
		result = partpopmatrix_neg->GetBinContent(partpopmatrix_neg->FindBin(p,pt,angle));
	else
		//std::cout << "WTF? charge == 0" << std::endl;
		result = 0;

	if(result == 0)
	{
		//std::cout << "REJECTED" << std::endl;
		return false;
	}
	else if(result < 0)
	{
		//std::cout << "Bin should not has bin content < 0" << std::endl;
		return false;
	}
	else
	{
		//std::cout << "ACCEPTED" << std::endl;
		return true;
	}
}
