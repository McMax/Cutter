#include "TFile.h"
#include "TRandom3.h"
#include "TH3I.h"

class PPMCut
{
	TFile *ppm_file;
	TH3I *partpopmatrix_pos;
	TH3I *partpopmatrix_neg;

	Double_t result;

public:
	PPMCut() {}
	PPMCut(const TString, const TString, const TString);
	~PPMCut();

	bool PartPopMatrixCut(const int, const double, const double, const double);

	void openPPMFile(const TString, const TString, const TString);
	void closePPMFile();
};
