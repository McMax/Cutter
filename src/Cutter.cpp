#include <iostream>
#include <string>
#include <cstdlib>

#include "TMath.h"
#include "TCutG.h"

#include "ParticleTree.h"
#include "Event.h"
#include "Particle.h"
#include "AccCut.h"
#include "dEdxCut.h"
#include "PPMCut.h"

using namespace std;

void RunAccCut(const int ener)
{
	cout << "Running acceptance mode" << endl;

	TFile *input_rootfile = new TFile("ParticleTree.root");
	TTree* input_tree = (TTree*)input_rootfile->Get("events");

	ParticleTree output_tree("ParticleTree_acc.root");

	Event *event = new Event();
	Particle *particle;
	input_tree->SetBranchAddress("event",&event);

	const Long64_t treeNentries = input_tree->GetEntries();
	Long64_t ev;
	UInt_t Npa;
	UInt_t part;

	AccCut acc_map("acceptance-map-medium.root",ener);

	float pt, E, p, y, angle;
	const float pion_mass = 0.13957018; //GeV/c^2
	const float nucleon_mass = 0.9389186795; //GeV/c^2
	const float beta = (ener/(ener+nucleon_mass));
	const float y_cms = 0.5*TMath::Log((1+beta)/(1-beta));

	for(ev=0; ev<treeNentries; ++ev)
	{
		if(!(ev%5000))
			cout << "Event: " << ev << endl;

			input_tree->GetEntry(ev);
			Npa = event->GetNpa();
			output_tree.BeginEvent();

			for(part=0; part<Npa; part++)
			{
				particle = event->GetParticle(part);
				pt = TMath::Sqrt(TMath::Power(particle->GetPx(),2)+TMath::Power(particle->GetPy(),2));
				p = TMath::Sqrt(TMath::Power(particle->GetPx(),2)+TMath::Power(particle->GetPy(),2)+TMath::Power(particle->GetPz(),2));
				E = TMath::Sqrt(pion_mass*pion_mass+p*p);
				y = 0.5*TMath::Log((E+particle->GetPz())/(E-particle->GetPz())) - y_cms;
				angle = TMath::ATan2(particle->GetPy(), particle->GetPx());

				//CIECIE NA AKCEPTACJE
				if(acc_map.acceptanceCut(particle->GetPx(),pt,particle->GetCharge(),y,angle))
					output_tree.AddParticle(particle->GetCharge(),
					particle->GetBx(), particle->GetBy(),
					particle->GetPx(), particle->GetPy(), particle->GetPz(),
					particle->GetdEdx(), particle->GetdEdxVtpc1(), particle->GetdEdxVtpc2(), particle->GetdEdxMtpc(),
					particle->GetNdEdx(), particle->GetNdEdxVtpc1(), particle->GetNdEdxVtpc2(), particle->GetNdEdxMtpc());
			}

			output_tree.EndEvent();
	}

	output_tree.Close();
	input_rootfile->Close();
}

void RunPPMCut()
{
	cout << "Running particle population map mode" << endl;

	TFile *input_rootfile = new TFile("ParticleTree.root");
	TTree* input_tree = (TTree*)input_rootfile->Get("events");

	ParticleTree output_tree("ParticleTree_ppm.root");

	Event *event = new Event();
	Particle *particle;
	input_tree->SetBranchAddress("event",&event);

	const Long64_t treeNentries = input_tree->GetEntries();
	Long64_t ev;
	UInt_t Npa;
	UInt_t part;

	PPMCut partpopmatrix("PartPopMatrix.root");

	float pt, p, angle;

	for(ev=0; ev<treeNentries; ++ev)
	{
		if(!(ev%5000))
			cout << "Event: " << ev << endl;

			input_tree->GetEntry(ev);
			Npa = event->GetNpa();
			output_tree.BeginEvent();

			for(part=0; part<Npa; part++)
			{
				particle = event->GetParticle(part);
				pt = TMath::Sqrt(TMath::Power(particle->GetPx(),2)+TMath::Power(particle->GetPy(),2));
				p = TMath::Sqrt(TMath::Power(particle->GetPx(),2)+TMath::Power(particle->GetPy(),2)+TMath::Power(particle->GetPz(),2));
				angle = TMath::ATan2(particle->GetPy(), particle->GetPx());

				//CIECIE NA AKCEPTACJE
				if(partpopmatrix.PartPopMatrixCut(particle->GetCharge(),p,pt,angle))
					output_tree.AddParticle(particle->GetCharge(),
					particle->GetBx(), particle->GetBy(),
					particle->GetPx(), particle->GetPy(), particle->GetPz(),
					particle->GetdEdx(), particle->GetdEdxVtpc1(), particle->GetdEdxVtpc2(), particle->GetdEdxMtpc(),
					particle->GetNdEdx(), particle->GetNdEdxVtpc1(), particle->GetNdEdxVtpc2(), particle->GetNdEdxMtpc());
			}

			output_tree.EndEvent();
	}

	output_tree.Close();
	input_rootfile->Close();
}

void RunMultSplit(const TString mult_string)
{
	cout << "Running multiplicity splitter mode" << endl;
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
		if(!(ev%5000))
			cout << "Event: " << ev << endl;
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
				//output_tree_all.AddParticle(particle->GetPid(), particle->GetCharge(), particle->GetBx(), particle->GetBy(), particle->GetPx(), particle->GetPy(), particle->GetPz(), particle->GetDedx());
				output_tree_all.AddParticle(particle->GetCharge(),
						particle->GetBx(), particle->GetBy(),
						particle->GetPx(), particle->GetPy(), particle->GetPz(),
						particle->GetdEdx(), particle->GetdEdxVtpc1(), particle->GetdEdxVtpc2(), particle->GetdEdxMtpc(),
						particle->GetNdEdx(), particle->GetNdEdxVtpc1(), particle->GetNdEdxVtpc2(), particle->GetNdEdxMtpc());
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
					//output_tree_pos.AddParticle(particle->GetPid(), particle->GetCharge(), particle->GetBx(), particle->GetBy(), particle->GetPx(), particle->GetPy(), particle->GetPz(), particle->GetDedx());
				output_tree_pos.AddParticle(particle->GetCharge(),
						particle->GetBx(), particle->GetBy(),
						particle->GetPx(), particle->GetPy(), particle->GetPz(),
						particle->GetdEdx(), particle->GetdEdxVtpc1(), particle->GetdEdxVtpc2(), particle->GetdEdxMtpc(),
						particle->GetNdEdx(), particle->GetNdEdxVtpc1(), particle->GetNdEdxVtpc2(), particle->GetNdEdxMtpc());
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
					//output_tree_neg.AddParticle(particle->GetPid(), particle->GetCharge(), particle->GetBx(), particle->GetBy(), particle->GetPx(), particle->GetPy(), particle->GetPz(), particle->GetDedx());
				output_tree_neg.AddParticle(particle->GetCharge(),
						particle->GetBx(), particle->GetBy(),
						particle->GetPx(), particle->GetPy(), particle->GetPz(),
						particle->GetdEdx(), particle->GetdEdxVtpc1(), particle->GetdEdxVtpc2(), particle->GetdEdxMtpc(),
						particle->GetNdEdx(), particle->GetNdEdxVtpc1(), particle->GetNdEdxVtpc2(), particle->GetNdEdxMtpc());
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

Float_t choose_dedx(Particle *particle)
{
	static Int_t vtpc1_part;
	static Int_t vtpc2_part;
	static Int_t mtpc_part;

	vtpc1_part = particle->GetNdEdxVtpc1();
	vtpc2_part = particle->GetNdEdxVtpc2();
	mtpc_part = particle->GetNdEdxMtpc();

	//std::cout << "dE/dx: VTPC1 part: " << vtpc1_part << "\tVTPC2 part: " << vtpc2_part << "\tMTPC part: " << mtpc_part << std::endl;
	if((vtpc1_part == 0) && (vtpc2_part == 0) && (mtpc_part == 0))
	{
		std::cout << "WTF? Particle with no dE/dx information!" << std::endl;
		return 0;
	}
	else
	{
		if(mtpc_part > 0)
			return (particle->GetdEdxMtpc());
		else if(vtpc2_part >= vtpc1_part)
			return (particle->GetdEdxVtpc2());
		else
			return (particle->GetdEdxVtpc1());
	}
}

void RunDedxCut(TString inputfile, TString outputfile)
{
	cout << "Running dE/dx mode" << endl;
	initialise_dedx_cutg();
	
	TFile *input_rootfile = new TFile(inputfile);
	TTree* input_tree = (TTree*)input_rootfile->Get("events");

	ParticleTree output_tree(outputfile);

	Event *event = new Event();
	Particle *particle;
	input_tree->SetBranchAddress("event",&event);

	const Long64_t treeNentries = input_tree->GetEntries();
	Long64_t ev;
	UInt_t Npa;
	UInt_t part;

	float p;

	for(ev=0; ev<treeNentries; ++ev)
	{
		if(!(ev%500))
			cout << "Event: " << ev << endl;

		input_tree->GetEntry(ev);
		Npa = event->GetNpa();
		output_tree.BeginEvent();

		for(part=0; part<Npa; part++)
		{
			particle = event->GetParticle(part);
			p = TMath::Sqrt(TMath::Power(particle->GetPx(),2)+TMath::Power(particle->GetPy(),2)+TMath::Power(particle->GetPz(),2));
			if(cutg->IsInside(p,choose_dedx(particle)))
				continue;

			output_tree.AddParticle(particle->GetCharge(),
					particle->GetBx(), particle->GetBy(),
					particle->GetPx(), particle->GetPy(), particle->GetPz(),
					particle->GetdEdx(), particle->GetdEdxVtpc1(), particle->GetdEdxVtpc2(), particle->GetdEdxMtpc(),
					particle->GetNdEdx(), particle->GetNdEdxVtpc1(), particle->GetNdEdxVtpc2(), particle->GetNdEdxMtpc());
		}
		output_tree.EndEvent();
	}

	output_tree.Close();
	input_rootfile->Close();
}

int main(int argc, char** argv)
{
	TString cut_mode = argv[1];
	TString inputfile = argv[2];
	TString outputfile = argv[3];
	TString mult_string;

	cout << "cut mode:" << cut_mode << endl;
	if(!(cut_mode.CompareTo("NA61ACC")))
	{
		cout << "WARNING: Energy fixed to 158 GeV!" << endl;
		RunAccCut(158);
	}
	else if(!(cut_mode.CompareTo("PPM")))
		RunPPMCut();
	else if(!(cut_mode.CompareTo("MULTSPLIT")))
	{
		mult_string = argv[2];
		RunMultSplit(mult_string);
	}
	else if(!(cut_mode.CompareTo("DEDX")))
	{
		cout << "WARNING: Using only pp@158 graphical cut!" << endl;
		//cout << "WARNING: Using only PbPb@160 graphical cut!" << endl;
		RunDedxCut(inputfile, outputfile);
	}
}
