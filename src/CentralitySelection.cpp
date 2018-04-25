#include <iostream>
#include <cstdlib>
#include "CentralitySelection.h"

CentralitySelection::CentralitySelection(const TString filename, const TString system, const TString energy)
{
	openAccMapFile(filename, system, energy);
	beam_momentum = energy.Atoi();
}

CentralitySelection::~CentralitySelection()
{
	closeAccMapFile();
}

int CentralitySelection::checkAcceptance(double p, double pT, int charge, double phi)
{
	if(charge == 0 && p > 2*beam_momentum) return 1;
	if(p*abs(charge) > 2*beam_momentum) return 1;

	//z-phi,y-pT*q,x-p*q
	int binX,binY,binZ;

	if (charge == 0)
	{
		binX = acc_map_neu->GetXaxis()->FindBin(p);
		binY = acc_map_neu->GetYaxis()->FindBin(pT);
		binZ = acc_map_neu->GetZaxis()->FindBin(phi);

		return (int)acc_map_neu->GetBinContent(binX,binY,binZ);
	}
	else if (charge > 0)
	{
		binX = acc_map_pos->GetXaxis()->FindBin(p*charge);
		binY = acc_map_pos->GetYaxis()->FindBin(pT*charge);
		binZ = acc_map_pos->GetZaxis()->FindBin(phi);

		return (int)acc_map_pos->GetBinContent(binX,binY,binZ);
	}
	else
	{
		binX = acc_map_neg->GetXaxis()->FindBin(-p*charge);
		binY = acc_map_neg->GetYaxis()->FindBin(-pT*charge);
		binZ = acc_map_neg->GetZaxis()->FindBin(phi);

		return (int)acc_map_neg->GetBinContent(binX,binY,binZ);
	}
}

void CentralitySelection::openAccMapFile(const TString filename, const TString system, const TString energy)
{
	acceptance_map = new TFile(filename, "READ");

	if(acceptance_map->IsZombie())
		std::cout << "File " << filename << " not opened" << std::endl;

	acc_map_pos = (TH3D*)acceptance_map->Get(TString::Format("positiveMap_%s_%s_GeVc",system.Data(),energy.Data()));
	acc_map_neg = (TH3D*)acceptance_map->Get(TString::Format("negativeMap_%s_%s_GeVc",system.Data(),energy.Data()));
	acc_map_neu = (TH3D*)acceptance_map->Get(TString::Format("neutralMap_%s_%s_GeVc",system.Data(),energy.Data()));
}

void CentralitySelection::closeAccMapFile()
{
	acceptance_map->Close();
}
