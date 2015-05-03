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
		std::cerr << "File " << filename << " not opened" << std::endl;
	temp = "";
	temp += energy;

	//std::cerr << "Opening map: " << temp << std::endl;
	
	acc_map = (TH3C*)acc_map_file->Get(temp);
}

void AccCut::closeMapFile()
{
	acc_map_file->Close();
}

bool AccCut::acceptanceCut(const int ch, const double y_pi_cms, const double angle_rad, const double pt)
{
	angle_deg = angle_rad*180/TMath::Pi();

	if(ch<0)
	{
		if(angle_deg < 0)
			angle_deg = -180 - angle_deg;
		else
			angle_deg = 180 - angle_deg;
	}

	result = acc_map->GetBinContent(acc_map->FindBin(y_pi_cms,angle_deg,pt));

	//std::cerr << "y=" << y_pi_cms << " angle=" << angle_deg << " pT=" << pt << " result: " << result << std::endl;

	if(result==1.)
		return true;
	else if(result == 0.)
	{
//		std::cout << "y=" << y << " angle=" << (angle*180/TMath::Pi()) << " pT=" << pt << std::endl;
		return false;
	}
	else
	{
		std::cout << "WTF? Result: " << result << std::endl;
		return false;
	}
}
