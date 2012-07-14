#include <iostream>
#include <cstdlib>

#include "ParticleTree.h"
#include "Event.h"
#include "Particle.h"

using namespace std;

int main(int argc, char** argv)
{
	const TString mult_string = argv[1];
	const UInt_t multiplicity = atoi(mult_string);

	TFile *input_rootfile = new TFile("ParticleTree.root");
	TTree* input_tree = (TTree*)input_rootfile->Get("events");

	ParticleTree output_tree_all("ParticleTree_mult" + mult_string + "_all.root");
	ParticleTree output_tree_pos("ParticleTree_mult" + mult_string + "_pos.root");
	ParticleTree output_tree_neg("ParticleTree_mult" + mult_string + "_neg.root");

	Event *event = new Event();
	Particle *particle;
	input_tree->SetBranchAddress("event",&event);

	const Long64_t treeNentries = input_tree->GetEntries();
	Long64_t ev;
	UInt_t Npa;
	UInt_t Npos;
	UInt_t Nneg;
	UInt_t part;
	UInt_t all_count = 0;
	UInt_t pos_count = 0;
	UInt_t neg_count = 0;
	
	//cout << "Event\t\tAll\tPos\tNeg" << endl;

	//DLA WSZYSTKICH
	for(ev=0; ev<treeNentries; ++ev)
	{
		cout << "\rEvent: " << ev;
		input_tree->GetEntry(ev);
		Npa = event->GetNpa();
		Npos = event->GetNpos();
		Nneg = event->GetNneg();

		if(Npa==multiplicity)
		{
			output_tree_all.BeginEvent();
			for(part=0; part<Npa; part++)
			{
				particle = event->GetParticle(part);
				//output_tree_all.AddParticle(particle->GetPid(), particle->GetCharge(), particle->GetBx(), particle->GetBy(), particle->GetPx(), particle->GetPy(), particle->GetPz());
				output_tree_all.AddParticle(particle->GetCharge(), particle->GetBx(), particle->GetBy(), particle->GetPx(), particle->GetPy(), particle->GetPz());
			}
			++all_count;
			output_tree_all.EndEvent();
		}

		if(Npos==multiplicity)
		{
			output_tree_pos.BeginEvent();
			for(part=0; part<Npa; part++)
			{
				particle = event->GetParticle(part);
				if((particle->GetCharge())>0)
					//output_tree_pos.AddParticle(particle->GetPid(), particle->GetCharge(), particle->GetBx(), particle->GetBy(), particle->GetPx(), particle->GetPy(), particle->GetPz());
					output_tree_pos.AddParticle(particle->GetCharge(), particle->GetBx(), particle->GetBy(), particle->GetPx(), particle->GetPy(), particle->GetPz());
			}
			++pos_count;
			output_tree_pos.EndEvent();
		}

		if(Nneg==multiplicity)
		{
			output_tree_neg.BeginEvent();
			for(part=0; part<Npa; part++)
			{
				particle = event->GetParticle(part);
				if((particle->GetCharge())<0)
					//output_tree_neg.AddParticle(particle->GetPid(), particle->GetCharge(), particle->GetBx(), particle->GetBy(), particle->GetPx(), particle->GetPy(), particle->GetPz());
					output_tree_neg.AddParticle(particle->GetCharge(), particle->GetBx(), particle->GetBy(), particle->GetPx(), particle->GetPy(), particle->GetPz());
			}
			++neg_count;
			output_tree_neg.EndEvent();
		}
		//cout << "\r" << ev << "\t\t" << all_count << "\t" << pos_count << "\t" << neg_count;
	}
	
	cout << "\nEvents\t\tAll\tPos\tNeg" << endl << ev << "\t\t" << all_count << "\t" << pos_count << "\t" << neg_count << endl;

	output_tree_all.Close();
	output_tree_pos.Close();
	output_tree_neg.Close();
	input_rootfile->Close();
}
