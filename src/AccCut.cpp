#include <iostream>
#include <time.h>
#include "AccCut.h"
#include "TRandom3.h"
#include "TMath.h"

AccCut::AccCut(const TString filename, const int energy)
{
	openMapFile(filename, energy);
}

AccCut::~AccCut()
{
	closeMapFile();	
}

void AccCut::openMapFile(const TString filename, const int energy)
{
	static TString temp;

	acc_map_file = new TFile(filename, "READ");	

	if(acc_map_file->IsZombie())
		std::cout << "File " << filename << " not opened" << std::endl;
	temp = "";
	temp += energy;
	temp += "/pos";

	//std::cout << "pos: " << temp << std::endl;
	
	acc_map_pos = (TH3C*)acc_map_file->Get(temp);

	temp = "";
	temp += energy;
	temp += "/neg";
	//std::cout << "neg: " << temp << std::endl;
	acc_map_neg = (TH3C*)acc_map_file->Get(temp);
}

void AccCut::closeMapFile()
{
	acc_map_file->Close();
}

bool AccCut::acceptanceCut(const double p_x, const double pt, const int ch, const double y, double angle)
{
	if(ch<0)
		result = acc_map_neg->GetBinContent(acc_map_neg->FindBin(y,(angle*180/TMath::Pi()),pt));
	
	else if(ch>0)
		result = acc_map_pos->GetBinContent(acc_map_pos->FindBin(y,(angle*180/TMath::Pi()),pt));
		
	else
		std::cout << "WTF? Charge:" << ch << std::endl;

	if(result==1.)
		return true;
	else if(result == 0.)
	{
//		std::cout << "y=" << y << " angle=" << (angle*180/TMath::Pi()) << " pT=" << pt << std::endl;
		return false;
	}
	else
		std::cout << "WTF? Result: " << result << std::endl;
}
