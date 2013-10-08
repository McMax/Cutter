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

Float_t TTRCut::calcAvDistance(Particle* particleA, Particle* particleB)
{
				count = 0;
				distance_sum = 0;
				//VTPC1 start
				if((particleA->GetVTPC1_Sx()!=9999) && (particleB->GetVTPC1_Sx()!=9999))
				{
					trackA.Set(particleA->GetVTPC1_Sx(),particleA->GetVTPC1_Sy());
					trackB.Set(particleB->GetVTPC1_Sx(),particleB->GetVTPC1_Sy());
					mov = trackB - trackA;
					distance_sum += mov.Mod();
					//std::cout << "VTPC1 start. Distance: " << mov.Mod() << std::endl;
					++count;
				}

				//VTPC1 end
				if((particleA->GetVTPC1_Ex()!=9999) && (particleB->GetVTPC1_Ex()!=9999))
				{
					trackA.Set(particleA->GetVTPC1_Ex(),particleA->GetVTPC1_Ey());
					trackB.Set(particleB->GetVTPC1_Ex(),particleB->GetVTPC1_Ey());
					mov = trackB - trackA;
					distance_sum += mov.Mod();
					//std::cout << "VTPC1 end. Distance: " << mov.Mod() << std::endl;
					++count;
				}

				//VTPC2 start
				if((particleA->GetVTPC2_Sx()!=9999) && (particleB->GetVTPC2_Sx()!=9999))
				{
					trackA.Set(particleA->GetVTPC2_Sx(),particleA->GetVTPC2_Sy());
					trackB.Set(particleB->GetVTPC2_Sx(),particleB->GetVTPC2_Sy());
					mov = trackB - trackA;
					distance_sum += mov.Mod();
					//std::cout << "VTPC2 start. Distance: " << mov.Mod() << std::endl;
					++count;
				}

				//VTPC2 end
				if((particleA->GetVTPC2_Ex()!=9999) && (particleB->GetVTPC2_Ex()!=9999))
				{
					trackA.Set(particleA->GetVTPC2_Ex(),particleA->GetVTPC2_Ey());
					trackB.Set(particleB->GetVTPC2_Ex(),particleB->GetVTPC2_Ey());
					mov = trackB - trackA;
					distance_sum += mov.Mod();
					//std::cout << "VTPC2 end. Distance: " << mov.Mod() << std::endl;
					++count;
				}

				//MTPC start
				if((particleA->GetMTPC_Sx()!=9999) && (particleB->GetMTPC_Sx()!=9999))
				{
					trackA.Set(particleA->GetMTPC_Sx(),particleA->GetMTPC_Sy());
					trackB.Set(particleB->GetMTPC_Sx(),particleB->GetMTPC_Sy());
					mov = trackB - trackA;
					distance_sum += mov.Mod();
					//std::cout << "MTPC start. Distance: " << mov.Mod() << std::endl;
					++count;
				}

				//MTPC end
				if((particleA->GetMTPC_Ex()!=9999) && (particleB->GetMTPC_Ex()!=9999))
				{
					trackA.Set(particleA->GetMTPC_Ex(),particleA->GetMTPC_Ey());
					trackB.Set(particleB->GetMTPC_Ex(),particleB->GetMTPC_Ey());
					mov = trackB - trackA;
					distance_sum += mov.Mod();
					//std::cout << "MTPC end. Distance: " << mov.Mod() << std::endl;
					++count;
				}

				if(count==0)
					return -1;

				distance_sum = distance_sum/count;
				//std::cout << "Average: " << distance_sum << std::endl;
				
				return distance_sum;
}
