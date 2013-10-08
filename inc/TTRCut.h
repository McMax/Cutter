#include "TVector2.h"
#include "Particle.h"

class TTRCut
{
	Float_t distance_sum;
	Int_t	count;
	TVector2 trackA, trackB, mov;

public:
	TTRCut();
	Float_t	calcAvDistance(Particle*,Particle*);
};
