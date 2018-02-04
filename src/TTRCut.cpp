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

bool TTRCut::IsGood(const double position_x, const double position_y)
{
	if((position_x < 100000) && (position_y < 100000))
		return true;
	else
		return false;
}

Double_t TTRCut::calcAvDistance(Particle* partA, Particle* partB)
{
	static int count;
	static float distance_sum;
	static TVector2 trackA, trackB, mov;

	double x1, y1, x2, y2;

	count = 0;
	distance_sum = 0;


	//VTPC1 front
	x1 = partA->GetVTPC1fX();
	y1 = partA->GetVTPC1fY();
	x2 = partB->GetVTPC1fX();
	y2 = partB->GetVTPC1fY();
	if(IsGood(x1,y1) && IsGood(x2,y2))
	{
		trackA.Set(x1,y1);
		trackB.Set(x2,y2);
		mov = trackB - trackA;
		distance_sum += mov.Mod();
		//cout << "VTPC1 start. Distance: " << mov.Mod() << endl;
		++count;
	}
	//VTPC1 end
	x1 = partA->GetVTPC1bX();
	y1 = partA->GetVTPC1bY();
	x2 = partB->GetVTPC1bX();
	y2 = partB->GetVTPC1bY();
	if(IsGood(x1,y1) && IsGood(x2,y2))
	{
		trackA.Set(x1,y1);
		trackB.Set(x2,y2);
		mov = trackB - trackA;
		distance_sum += mov.Mod();
		//cout << "VTPC1 end. Distance: " << mov.Mod() << endl;
		++count;
	}
	//GTPC start
	x1 = partA->GetGTPCfX();
	y1 = partA->GetGTPCfY();
	x2 = partB->GetGTPCfX();
	y2 = partB->GetGTPCfY();
	if(IsGood(x1,y1) && IsGood(x2,y2))
	{
		trackA.Set(x1,y1);
		trackB.Set(x2,y2);
		mov = trackB - trackA;
		distance_sum += mov.Mod();
		//cout << "GTPC start. Distance: " << mov.Mod() << endl;
		++count;
	}
	//GTPC end
	x1 = partA->GetGTPCbX();
	y1 = partA->GetGTPCbY();
	x2 = partB->GetGTPCbX();
	y2 = partB->GetGTPCbY();
	if(IsGood(x1,y1) && IsGood(x2,y2))
	{
		trackA.Set(x1,y1);
		trackB.Set(x2,y2);
		mov = trackB - trackA;
		distance_sum += mov.Mod();
		//cout << "GTPC end. Distance: " << mov.Mod() << endl;
		++count;
	}
	//VTPC2 start
	x1 = partA->GetVTPC2fX();
	y1 = partA->GetVTPC2fY();
	x2 = partB->GetVTPC2fX();
	y2 = partB->GetVTPC2fY();
	if(IsGood(x1,y1) && IsGood(x2,y2))
	{
		trackA.Set(x1,y1);
		trackB.Set(x2,y2);
		mov = trackB - trackA;
		distance_sum += mov.Mod();
		//cout << "VTPC2 start. Distance: " << mov.Mod() << endl;
		++count;
	}
	//VTPC2 end
	x1 = partA->GetVTPC2bX();
	y1 = partA->GetVTPC2bY();
	x2 = partB->GetVTPC2bX();
	y2 = partB->GetVTPC2bY();
	if(IsGood(x1,y1) && IsGood(x2,y2))
	{
		trackA.Set(x1,y1);
		trackB.Set(x2,y2);
		mov = trackB - trackA;
		distance_sum += mov.Mod();
		//cout << "VTPC2 end. Distance: " << mov.Mod() << endl;
		++count;
	}
	//MTPC start
	x1 = partA->GetMTPCfX();
	y1 = partA->GetMTPCfY();
	x2 = partB->GetMTPCfX();
	y2 = partB->GetMTPCfY();
	if(IsGood(x1,y1) && IsGood(x2,y2))
	{
		trackA.Set(x1,y1);
		trackB.Set(x2,y2);
		mov = trackB - trackA;
		distance_sum += mov.Mod();
		//cout << "MTPC start. Distance: " << mov.Mod() << endl;
		++count;
	}
	//MTPC end
	x1 = partA->GetMTPCbX();
	y1 = partA->GetMTPCbY();
	x2 = partB->GetMTPCbX();
	y2 = partB->GetMTPCbY();
	if(IsGood(x1,y1) && IsGood(x2,y2))
	{
		trackA.Set(x1,y1);
		trackB.Set(x2,y2);
		mov = trackB - trackA;
		distance_sum += mov.Mod();
		//cout << "MTPC end. Distance: " << mov.Mod() << endl;
		++count;
	}
	if(count==0)
		return -1;

	distance_sum = distance_sum/count;

	return distance_sum;
}
