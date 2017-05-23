#include <iostream>
#include "TTRCut.h"

TTRCut::TTRCut()
{
	count = 0;
	distance_sum = 0;
	
	trackA.Set(0.,0.);
	trackB.Set(0.,0.);
	mov.Set(0.,0.);
}

Double_t TTRCut::calcAvDistance(Particle* partA, Particle* partB)
{
	static int count;
	static float distance_sum;
	static TVector2 trackA, trackB, mov;

	count = 0;
	distance_sum = 0;


	//VTPC1 front
	trackA.Set(partA->GetVTPC1fX(),partA->GetVTPC1fY());
	trackB.Set(partB->GetVTPC1fX(),partB->GetVTPC1fY());
	mov = trackB - trackA;
	distance_sum += mov.Mod();
	//cout << "VTPC1 start. Distance: " << mov.Mod() << endl;
	++count;
	//VTPC1 end
	trackA.Set(partA->GetVTPC1bX(),partA->GetVTPC1bY());
	trackB.Set(partB->GetVTPC1bX(),partB->GetVTPC1bY());
	mov = trackB - trackA;
	distance_sum += mov.Mod();
	//cout << "VTPC1 end. Distance: " << mov.Mod() << endl;
	++count;

	//GTPC start
	trackA.Set(partA->GetGTPCfX(),partA->GetGTPCfY());
	trackB.Set(partB->GetGTPCfX(),partB->GetGTPCfY());
	mov = trackB - trackA;
	distance_sum += mov.Mod();
	//cout << "GTPC start. Distance: " << mov.Mod() << endl;
	++count;

	//GTPC end
	trackA.Set(partA->GetGTPCbX(),partA->GetGTPCbY());
	trackB.Set(partB->GetGTPCbX(),partB->GetGTPCbY());
	mov = trackB - trackA;
	distance_sum += mov.Mod();
	//cout << "GTPC end. Distance: " << mov.Mod() << endl;
	++count;

	//VTPC2 start
	trackA.Set(partA->GetVTPC2fX(),partA->GetVTPC2fY());
	trackB.Set(partB->GetVTPC2fX(),partB->GetVTPC2fY());
	mov = trackB - trackA;
	distance_sum += mov.Mod();
	//cout << "VTPC2 start. Distance: " << mov.Mod() << endl;
	++count;

	//VTPC2 end
	trackA.Set(partA->GetVTPC2bX(),partA->GetVTPC2bY());
	trackB.Set(partB->GetVTPC2bX(),partB->GetVTPC2bY());
	mov = trackB - trackA;
	distance_sum += mov.Mod();
	//cout << "VTPC2 end. Distance: " << mov.Mod() << endl;
	++count;

	//MTPC start
	trackA.Set(partA->GetMTPCfX(),partA->GetMTPCfY());
	trackB.Set(partB->GetMTPCfX(),partB->GetMTPCfY());
	mov = trackB - trackA;
	distance_sum += mov.Mod();
	//cout << "MTPC start. Distance: " << mov.Mod() << endl;
	++count;

	//MTPC end
	trackA.Set(partA->GetMTPCbX(),partA->GetMTPCbY());
	trackB.Set(partB->GetMTPCbX(),partB->GetMTPCbY());
	mov = trackB - trackA;
	distance_sum += mov.Mod();
	//cout << "MTPC end. Distance: " << mov.Mod() << endl;
	++count;

	if(count==0)
		return -1;

	distance_sum = distance_sum/count;

	return distance_sum;
}
