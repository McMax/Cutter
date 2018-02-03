#include "TVector2.h"
#include "Particle.h"

class TTRCut
{
	Double_t distance_sum;
	Int_t	count;
	TVector2 trackA, trackB, mov;
	bool IsGood(const double, const double);	//Sanity check: has the point good value (10e6 is insane)

public:
	TTRCut();
	Double_t	calcAvDistance(Particle*,Particle*);
};
