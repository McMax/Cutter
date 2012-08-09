#include <vector>
#include "TFile.h"
#include "TRandom3.h"
#include "TH3C.h"
#include "TH2D.h"

class AccCut
{
	TFile *acc_map_file;
	TH3C *acc_map_pos, *acc_map_neg;

	Double_t result;

public:
	AccCut() {}
	AccCut(const TString, const int);
	~AccCut();	//zamknac plik

	bool acceptanceCut(const double, const double, const int, const double, double);

	void openMapFile(const TString, const int);
	void closeMapFile();
};
