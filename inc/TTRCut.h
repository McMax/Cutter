#include "TVector2.h"
#include "Particle.h"

class TTRCut
{
	Double_t distance_sum;
	Int_t	count;
	TVector2 trackA, trackB, mov;

public:
	TTRCut();
	Double_t	calcAvDistance(Particle*,Particle*);
};
