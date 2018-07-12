#include "TFile.h"
#include "TH3D.h"

class CentralitySelection
{
	TFile *acceptance_map;
	TH3D *acc_map_pos;
	TH3D *acc_map_neg;
	TH3D *acc_map_neu;
	int beam_momentum;

public:
	CentralitySelection() {}
	CentralitySelection(const TString, const TString, const Int_t);
	~CentralitySelection();

	int checkAcceptance(double, double, int, double);

	void openAccMapFile(const TString, const TString, const Int_t);
	void closeAccMapFile();
};

