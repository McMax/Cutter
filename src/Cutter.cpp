#include <iostream>
#include <string>
#include <cstdlib>

#include "TMath.h"
#include "TCutG.h"

#include "ParticleTree.h"
#include "Event.h"
#include "Particle.h"
#include "PPMCut.h"

using namespace std;

void RunPPMCut(TString inputfile, TString outputfile, TString system, TString energy)
{
	cout << "Running particle population matrix mode" << endl;

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
	UInt_t particles_in, particles_out;

	PPMCut partpopmatrix("PartPopMatrix.root",system, energy);
	if(!(partpopmatrix.isPPMFileOpened()))
	{
		cout << "PPM file not opened" << endl;
		return;
	}

	float pt, p, angle;

	particles_in = particles_out = 0;

	for(ev=0; ev<treeNentries; ++ev)
	{
		if(!(ev%5000))
			cout << "Event: " << ev << endl;

			input_tree->GetEntry(ev);
			Npa = event->GetNpa();
			output_tree.BeginEvent();

			particles_in += Npa;

			for(part=0; part<Npa; part++)
			{
				particle = event->GetParticle(part);
				pt = TMath::Sqrt(TMath::Power(particle->GetPx(),2)+TMath::Power(particle->GetPy(),2));
				p = TMath::Sqrt(TMath::Power(particle->GetPx(),2)+TMath::Power(particle->GetPy(),2)+TMath::Power(particle->GetPz(),2));
				angle = TMath::ATan2(particle->GetPy(), particle->GetPx());

				//CIECIE NA AKCEPTACJE
				if(partpopmatrix.PartPopMatrixCut(particle->GetCharge(),p,pt,angle))
				{
					output_tree.AddParticle(particle->GetCharge(), particle->GetPDGpid(),
					particle->GetPx(), particle->GetPy(), particle->GetPz(),
					particle->GetMass());

					particles_out++;
				}
			}

			output_tree.EndEvent();
	}

	output_tree.Close();
	input_rootfile->Close();

	cout << "Particle population matrix cut summary\n------------" << endl
		<< "Particles before cut: " << particles_in << endl
		<< "Particles after cut: " << particles_out << endl
		<< "Cutted particles: " << (particles_in-particles_out) << endl
		<< "Ratio: " << ((Double_t)particles_out/particles_in) << endl;
}

void RunMultSplit(TString inputfile, TString outputfile, const TString mult_string)
{
	cout << "Running multiplicity splitter (" << mult_string << ") mode" << endl;
	const UInt_t multiplicity = atoi(mult_string);

	TFile *input_rootfile = new TFile(inputfile);
	TTree* input_tree = (TTree*)input_rootfile->Get("events");

	ParticleTree output_tree_all(outputfile + "_all.root");
	ParticleTree output_tree_pos(outputfile + "_pos.root");
	ParticleTree output_tree_neg(outputfile + "_neg.root");

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
		if(!(ev%10000))
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
				output_tree_all.AddParticle(particle->GetCharge(), particle->GetPDGpid(),
						particle->GetPx(), particle->GetPy(), particle->GetPz(),
						particle->GetMass());
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
				//if((particle->GetCharge())>0)		//oops
					//output_tree_pos.AddParticle(particle->GetPid(), particle->GetCharge(), particle->GetBx(), particle->GetBy(), particle->GetPx(), particle->GetPy(), particle->GetPz(), particle->GetDedx());
				output_tree_pos.AddParticle(particle->GetCharge(), particle->GetPDGpid(),
						particle->GetPx(), particle->GetPy(), particle->GetPz(),
						particle->GetMass());
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
				//if((particle->GetCharge())<0)		//oops
					//output_tree_neg.AddParticle(particle->GetPid(), particle->GetCharge(), particle->GetBx(), particle->GetBy(), particle->GetPx(), particle->GetPy(), particle->GetPz(), particle->GetDedx());
				output_tree_neg.AddParticle(particle->GetCharge(), particle->GetPDGpid(),
						particle->GetPx(), particle->GetPy(), particle->GetPz(),
						particle->GetMass());
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

void RunElasticCut(TString inputfile, TString outputfile, Int_t energy)
{
	TFile *input_rootfile = new TFile(inputfile);
	TTree* input_tree = (TTree*)input_rootfile->Get("events");

	ParticleTree output_tree(outputfile);

	Event *event = new Event();
	Particle *particle;
	input_tree->SetBranchAddress("event",&event);

	const Long64_t treeNentries = input_tree->GetEntries();
	Long64_t ev;
	Long_t particles_in = 0, particles_out = 0, events_out = 0;
	UInt_t part, Npa;

	Float_t p;
	Bool_t event_cancelled;

	for(ev=0; ev<treeNentries; ++ev)
	{
		if(!(ev%500))
			cout << "Event: " << ev << endl;

		input_tree->GetEntry(ev);

		Npa = event->GetNpa();

		event_cancelled = false;
		for(part=0; part<Npa; ++part)
		{
			particle = event->GetParticle(part);
			p = TMath::Sqrt(TMath::Power(particle->GetPx(),2)+TMath::Power(particle->GetPy(),2)+TMath::Power(particle->GetPz(),2));
			if((particle->isPositive()) && (p > energy - 3))
			{
				event_cancelled = true;
				break;
			}
		}

		particles_in+=Npa;

		if(event_cancelled)
			continue;

		events_out++;
		
		output_tree.BeginEvent();

		for(part=0; part<Npa; part++)
		{
			particle = event->GetParticle(part);
			
			output_tree.AddParticle(particle->GetCharge(), particle->GetPDGpid(),
					particle->GetPx(), particle->GetPy(), particle->GetPz(),
					particle->GetMass());
		}
		output_tree.EndEvent();
		particles_out+=Npa;
	}

	output_tree.Close();
	input_rootfile->Close();

	cout << "Elastic cut summary\n------------" << endl
		<< "Events before cut: " << treeNentries  << endl
		<< "Events after cut: " << events_out << endl
		<< "Cutted events: " << (treeNentries-events_out) << endl
		<< "Ratio: " << ((Double_t)treeNentries/events_out) << "\n------------" << endl
		<< "Particles before cut: " << particles_in << endl
		<< "Particles after cut: " << particles_out << endl
		<< "Cutted particles: " << (particles_in-particles_out) << endl
		<< "Ratio: " << ((Double_t)particles_out/particles_in) << endl;
}

void RunPtCut(TString inputfile, TString outputfile, const Float_t ptcut=1.5)
{
	TFile *input_rootfile = new TFile(inputfile);
	TTree* input_tree = (TTree*)input_rootfile->Get("events");

	ParticleTree output_tree(outputfile);

	Event *event = new Event();
	Particle *particle;
	input_tree->SetBranchAddress("event",&event);

	const Long64_t treeNentries = input_tree->GetEntries();
	Long_t particles_in = 0, particles_out = 0;
	Long64_t ev;
	UInt_t Npa;
	UInt_t part;

	Double_t pt;

	for(ev=0; ev<treeNentries; ++ev)
	{
		if(!(ev%1000))
			cout << "Event: " << ev << endl;

		input_tree->GetEntry(ev);
		Npa = event->GetNpa();
		output_tree.BeginEvent();

		particles_in += Npa;
		for(part=0; part<Npa; part++)
		{
			particle = event->GetParticle(part);
			pt = TMath::Sqrt(TMath::Power(particle->GetPx(),2)+TMath::Power(particle->GetPy(),2));

			if(ptcut >= 0)
			{
				if(pt > ptcut)
					continue;
			}
			else if(ptcut < 0)
			{
				if(pt < TMath::Abs(ptcut))
					continue;
			}
			

			++particles_out;
			
			output_tree.AddParticle(particle->GetCharge(), particle->GetPDGpid(),
					particle->GetPx(), particle->GetPy(), particle->GetPz(),
					particle->GetMass());
		}
		output_tree.EndEvent();
	}

	output_tree.Close();
	input_rootfile->Close();

	cout << "pT cut summary\n------------" << endl
		<< "Particles before cut: " << particles_in << endl
		<< "Particles after cut: " << particles_out << endl
		<< "Cutted particles: " << (particles_in-particles_out) << endl
		<< "Ratio: " << ((Double_t)particles_out/particles_in) << endl;
}

int main(int argc, char** argv)
{
	if(argc <= 1)
	{
		cout << "USAGE: cutter <inputfile> <outputfile> <cut_mode> [<energy>/<multsplit> [<system>]]" << endl;
		return 0;
	}

	TString inputfile = argv[1];
	TString outputfile = argv[2];
	TString cut_mode = argv[3];
	TString energy = argv[4];
	TString system = argv[5];
	TString mult_string;
	TString pt_cut_string;

	cout << "cut mode:" << cut_mode << endl;

	if(!(cut_mode.CompareTo("PPM")))
	{
		if(argc != 6)
		{
			cout << "PPM cut requires additional arguments: 1.energy, 2.system" << endl;
			return 0;
		}
		RunPPMCut(inputfile, outputfile, system, energy);
	}
	else if(!(cut_mode.CompareTo("MULTSPLIT")))
	{
		mult_string = argv[4];
		RunMultSplit(inputfile, outputfile, mult_string);
	}
	else if(!(cut_mode.CompareTo("ELASTIC")))
	{
		if(argc != 5)
		{
			cout << "ELASTIC cut requires additional argument: energy" << endl;
			return 0;
		}
		cout << "Elastic cut mode" << endl;
		RunElasticCut(inputfile,outputfile, energy.Atoi());
	}
	else if(!(cut_mode.CompareTo("PT")))
	{
		if(argc == 5)
		{
			pt_cut_string = argv[4];
			cout << "Transverse momentum cut mode: pt < " << pt_cut_string << " GeV/c" << endl;
			RunPtCut(inputfile, outputfile, pt_cut_string.Atof());
		}
		else 
		{
			cout << "Transverse momentum cut mode: pt < 1.5 GeV/c" << endl;
			RunPtCut(inputfile, outputfile);	//default 1.5 GeV
		}
	}
}		
