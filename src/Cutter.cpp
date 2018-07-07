#include <iostream>
#include <string>
#include <cstdlib>
#include <vector>
#include <algorithm>

#include "TMath.h"
#include "TCutG.h"

#include "ParticleTree.h"
#include "Event.h"
#include "Particle.h"
#include "AccCut.h"
#include "dEdxCut.h"
#include "PPMCut.h"
#include "CentralitySelection.h"

using namespace std;

void RunAccCut(const TString inputfile, const TString outputfile, const int momentum)
{
	cout << "Running acceptance mode" << endl;

//Input ParticleTree file
	TFile *input_rootfile = new TFile(inputfile);
	TTree* input_tree = (TTree*)input_rootfile->Get("events");

//Output ParticleTree file
	ParticleTree output_tree(outputfile);

	Event *event = new Event();
	Particle *particle;
	input_tree->SetBranchAddress("event",&event);

	const Long64_t treeNentries = input_tree->GetEntries();
	Long64_t ev;
	UInt_t Npa;
	UInt_t part;

	AccCut acc_map("acceptance-map.root",momentum);

	float pt, E, p, y, angle;
	const float pion_mass = 0.13957018; //GeV/c^2
	const float nucleon_mass = 0.9389186795; //GeV/c^2
	const float beta = momentum/(TMath::Sqrt(momentum*momentum+nucleon_mass*nucleon_mass)+nucleon_mass);
	const float y_cms = 0.5*TMath::Log((1+beta)/(1-beta));
	
	cout << "beta = " << beta << endl << "y_cms=" << y_cms << endl;

	UInt_t particles_in = 0, particles_out = 0;

	for(ev=0; ev<treeNentries; ++ev)
	{
		if(!(ev%5000))
			cout << "Event: " << ev << endl;

		input_tree->GetEntry(ev);
		Npa = event->GetNpa();
		output_tree.BeginEvent();

		for(part=0; part<Npa; part++)
		{
			++particles_in;

			particle = event->GetParticle(part);
			pt = TMath::Sqrt(TMath::Power(particle->GetPx(),2)+TMath::Power(particle->GetPy(),2));
			p = TMath::Sqrt(TMath::Power(particle->GetPx(),2)+TMath::Power(particle->GetPy(),2)+TMath::Power(particle->GetPz(),2));
			E = TMath::Sqrt(pion_mass*pion_mass+p*p);
			y = 0.5*TMath::Log((E+particle->GetPz())/(E-particle->GetPz())) - y_cms;
			angle = TMath::ATan2(particle->GetPy(), particle->GetPx());

			if(acc_map.acceptanceCut(particle->GetCharge(),y,angle,pt))
			{
				output_tree.AddParticle(*particle);

				++particles_out;
			}
		}

		output_tree.EndEvent();
	}

	output_tree.Close();
	input_rootfile->Close();

	cout << "Acceptance map cut summary\n------------" << endl
		<< "Particles before cut: " << particles_in << endl
		<< "Particles after cut: " << particles_out << endl
		<< "Cutted particles: " << (particles_in-particles_out) << endl
		<< "Ratio: " << ((Double_t)particles_out/particles_in) << endl;
}
//Acceptance map cut

//Particle Population Matrix cut
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

//PPMCut class defined in PPMCut.h
	PPMCut partpopmatrix("PartPopMatrix.root",system, energy);

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

			//The cut itself
			if(partpopmatrix.PartPopMatrixCut(particle->GetCharge(),p,pt,angle))
			{
				output_tree.AddParticle(*particle);

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
//Particle Population Matrix cut

//Multiplicity splitter to multiplicity bins analysis
void RunMultSplit(TString inputfile, TString outputfile, const TString mult_string, const TString multmax_string)
{
	const UInt_t multiplicity = atoi(mult_string);
	const UInt_t multiplicity_max = atoi(multmax_string);

	if(multiplicity==multiplicity_max)
		cout << "Running multiplicity splitter (" << mult_string << ") mode" << endl;
	else
		cout << "Running multiplicity splitter for range [" << mult_string << ", " << multmax_string << "]" << endl;
	

	TFile *input_rootfile = new TFile(inputfile);
	TTree* input_tree = (TTree*)input_rootfile->Get("events");

//Three output files of all, positively and negatively charged particles
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

	for(ev=0; ev<treeNentries; ++ev)
	{
		if(!(ev%10000))
			cout << "Event: " << ev << endl;
		input_tree->GetEntry(ev);
		Npa = event->GetNpa();
		Npos = event->GetNpos();
		Nneg = event->GetNneg();

		if((Npa >= multiplicity) && (Npa <= multiplicity_max))
		{
			output_tree_all.BeginEvent();
			for(part=0; part<Npa; part++)
			{
				particle = event->GetParticle(part);
				output_tree_all.AddParticle(*particle);
			}
			++all_count;
			output_tree_all.EndEvent();
		}

		if((Npos >= multiplicity) && (Npos <= multiplicity_max))
		{
			output_tree_pos.BeginEvent();
			for(part=0; part<Npa; part++)
			{
				particle = event->GetParticle(part);

				if(!(particle->isPositive()))
					continue;

				output_tree_pos.AddParticle(*particle);
			}
			++pos_count;
			output_tree_pos.EndEvent();
		}

		if((Nneg >= multiplicity) && (Nneg <= multiplicity_max))
		{
			output_tree_neg.BeginEvent();
			for(part=0; part<Npa; part++)
			{
				particle = event->GetParticle(part);

				if(particle->isPositive())
					continue;

				output_tree_neg.AddParticle(*particle);
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
//Multiplicity splitter to multiplicity bins analysis

#ifdef data_analysis
//Function needed to dEdx cut described below
Float_t choose_dedx(Particle *particle, TString system)
{
	static Int_t vtpc1_part;
	static Int_t vtpc2_part;
	static Int_t mtpc_part;

	if(!(system.CompareTo("pp")))
		return particle->GetdEdx();
	else if(!(system.CompareTo("BeBe")))
		return particle->GetdEdx();
	else if(!(system.CompareTo("PbPb")))
	{
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
	else
		return -1;
}
//Function needed to dEdx cut described below

//dEdx cut based on graphical cut defined in dEdxCut.cpp
void RunDedxCut(TString inputfile, TString outputfile, TString system, Int_t energy)
{
	cout << "Running dE/dx mode with energy " << energy << endl;
	TCutG* cutg = initialise_dedx_cutg(system, energy);
	cout << "Graphcut intialized" << endl;
	
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
	UInt_t particles_in = 0, particles_out = 0;

	Float_t local_dedx;
	Float_t dedx_uppercut = 3.;

	if(!(system.CompareTo("PbPb")))
	{	
		if((energy == 158 ) || (energy == 160))
			dedx_uppercut = 1.65;
		else if(energy == 20)
			dedx_uppercut = 1.6;
	}

	float p;

	cout << "Cut dE/dx > " << dedx_uppercut << " applied" << endl;

	for(ev=0; ev<treeNentries; ++ev)
	{
		if(!(ev%500))
			cout << "Event: " << ev << endl;

		input_tree->GetEntry(ev);
		Npa = event->GetNpa();
		output_tree.BeginEvent();

		for(part=0; part<Npa; part++)
		{
			++particles_in;
			particle = event->GetParticle(part);
			p = TMath::Sqrt(TMath::Power(particle->GetPx(),2)+TMath::Power(particle->GetPy(),2)+TMath::Power(particle->GetPz(),2));
			local_dedx = choose_dedx(particle, system);
			if(cutg->IsInside(p,local_dedx))
				continue;

			if(local_dedx > dedx_uppercut)
				continue;

			output_tree.AddParticle(*particle);

			++particles_out;
		}
		output_tree.EndEvent();
	}

	cout << "dEdx cut summary\n------------" << endl
		<< "Particles before cut: " << particles_in << endl
		<< "Particles after cut: " << particles_out << endl
		<< "Cutted particles: " << (particles_in-particles_out) << endl
		<< "Ratio: " << ((Double_t)particles_out/particles_in) << endl;

	output_tree.Close();
	input_rootfile->Close();
}
//dEdx cut

Bool_t is_electron(double logP, double dEdx) {
	//static const double a1 = 0.108696;
	//static const double b1 = 1.358696;
	static const double a1 = 0.15;
	static const double b1 = 1.2;
	//static const double a2 = -0.152174;
	static const double a2 = -0.1;
	static const double b2 = 2;

	//if((logP >= 0.2) && (dEdx > 1.78))
	if(dEdx > 1.5)
		return kTRUE;
	else if((logP > -2) && (logP < 2.4))
	{
		if((dEdx > a1*logP + b1) && (dEdx < a2*logP + b2))
			return kTRUE;
		else
			return kFALSE;
	}
	else
		return kFALSE;
} 

//dEdx cut based on parametrized curves defined in "is_electron" function
void RunDedxCut2(TString inputfile, TString outputfile)
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

	//Float_t dedx_uppercut = 3.;

	float p;

	//cout << "Cut dE/dx > " << dedx_uppercut << " applied" << endl;

	for(ev=0; ev<treeNentries; ++ev)
	{
		if(!(ev%500))
			cout << "Event: " << ev << endl;

		input_tree->GetEntry(ev);
		Npa = event->GetNpa();
		output_tree.BeginEvent();

		particles_in += Npa;
		for(part=0; part<Npa; part++)
		{
			particle = event->GetParticle(part);
			p = TMath::Sqrt(TMath::Power(particle->GetPx(),2)+TMath::Power(particle->GetPy(),2)+TMath::Power(particle->GetPz(),2));

			if(is_electron(TMath::Log10(p),particle->GetdEdx()))
				continue;

			++particles_out;
			
			output_tree.AddParticle(*particle);
		}
		output_tree.EndEvent();
	}

	output_tree.Close();
	input_rootfile->Close();

	cout << "dEdx cut summary\n------------" << endl
		<< "Particles before cut: " << particles_in << endl
		<< "Particles after cut: " << particles_out << endl
		<< "Cutted particles: " << (particles_in-particles_out) << endl
		<< "Ratio: " << ((Double_t)particles_out/particles_in) << endl;
}
#endif

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

	for(ev=0; ev<treeNentries; ++ev)
	{
		if(!(ev%500))
			cout << "Event: " << ev << endl;

		input_tree->GetEntry(ev);
		Npa = event->GetNpa();

		//If, after cuts before, we left only with one positively charged particle with momentum close to beam momentum, I assume that it is a proton from beam which interacted elastically with target. I reject this proton and therefore whole event.
		if(Npa == 1)
		{
			particle = event->GetParticle(0);
			p = TMath::Sqrt(TMath::Power(particle->GetPx(),2)+TMath::Power(particle->GetPy(),2)+TMath::Power(particle->GetPz(),2));
			if((particle->isPositive()) && (p > energy - 3))
			{
				particles_in++;
				continue;
			}
		}

		particles_in+=Npa;
		events_out++;
		
		output_tree.BeginEvent();

		for(part=0; part<Npa; part++)
		{
			particle = event->GetParticle(part);
			
			output_tree.AddParticle(*particle);
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
		<< "Ratio: " << ((Double_t)events_out/treeNentries) << "\n------------" << endl
		<< "Particles before cut: " << particles_in << endl
		<< "Particles after cut: " << particles_out << endl
		<< "Cutted particles: " << (particles_in-particles_out) << endl
		<< "Ratio: " << ((Double_t)particles_out/particles_in) << endl;
}
//Elastic event cut

//pT cut. Default value is 1.5 GeV/c. Which means it takes everything between 0 and 1.5.
//For ptcut values lower than 0, the cut will take everything between abs(ptcut) and infinity.
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
			
			output_tree.AddParticle(*particle);
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
//pT cut

void RunYCut(TString inputfile, TString outputfile, const Double_t beam_momentum)
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

	const Double_t proton_mass = 0.939;
	const Double_t E_beam = TMath::Sqrt(proton_mass*proton_mass + beam_momentum*beam_momentum);
	const Double_t y_beam = 0.5*TMath::Log((E_beam+beam_momentum)/(E_beam-beam_momentum));

	cout << "y_target=0" << endl << "y_beam=" << y_beam << endl << "Everything outside 0.5 < y_prot < " << (y_beam - 0.5) << " will be rejected" << endl;

	Double_t E, p2, y_prot;

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
			p2 = TMath::Power(particle->GetPx(),2)+TMath::Power(particle->GetPy(),2)+TMath::Power(particle->GetPz(),2);
			E = TMath::Sqrt(proton_mass*proton_mass + p2);
			y_prot = 0.5*TMath::Log((E+particle->GetPz())/(E-particle->GetPz()));

			if((y_prot < 0.5) || (y_prot > y_beam-0.5))
				continue;

			++particles_out;
			
			output_tree.AddParticle(*particle);
		}
		output_tree.EndEvent();
	}

	output_tree.Close();
	input_rootfile->Close();

	cout << "y cut summary\n------------" << endl
		<< "Particles before cut: " << particles_in << endl
		<< "Particles after cut: " << particles_out << endl
		<< "Cutted particles: " << (particles_in-particles_out) << endl
		<< "Ratio: " << ((Double_t)particles_out/particles_in) << endl;

}

void RunPSDAccSelection(TString inputfile, TString outputfile, TString system, Int_t energy, Double_t centrality_percentile=5.)
{
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
	UInt_t ev_out = 0;

	CentralitySelection psdacc(TString::Format("PSD_acceptance_maps_BeBe_%d_GeVc.root",energy),system,energy);

	double p, pT, phi, mass;
	short charge;
	vector<double> Ef_vector;
	double EF;
	double EF_of_centrality_edge;	// Forward energy of the X% of the most central events
	int index_of_centrality_edge;	// Ef_vector index of the X% of the most central events

	//First iteration - calculation of mininum bias forward energy
	cout << "First iteration - calculation of minimum bias forward energy" << endl;
	for(ev=0; ev<treeNentries; ++ev)
	{
		if(!(ev%500))
			cout << "Event: " << ev << endl;

		input_tree->GetEntry(ev);
		Npa = event->GetNpa();

		EF = 0;

		for(part=0; part<Npa; part++)
		{
			particle = event->GetParticle(part);
			p = TMath::Sqrt(TMath::Power(particle->GetPx(),2)+TMath::Power(particle->GetPy(),2)+TMath::Power(particle->GetPz(),2));
			pT = TMath::Sqrt(TMath::Power(particle->GetPx(),2)+TMath::Power(particle->GetPy(),2));
			phi = TMath::ATan2(particle->GetPy(), particle->GetPx());
			charge = particle->GetCharge();
			mass = particle->GetMass();

			if(psdacc.checkAcceptance(p, pT, charge, phi) == 1)
				EF += TMath::Sqrt(p*p + mass*mass);
		}
		
		//if forward energy sum is greater than 0, add it 
		if(EF > 0)
			Ef_vector.push_back(EF);
	}

	//Sorting to choose X% of the lowest values
	sort(Ef_vector.begin(), Ef_vector.end());
	//Taking the index of value corresponding to X% of the sorted forward energy vector
	index_of_centrality_edge = (int)(Ef_vector.size()*centrality_percentile/100);
	//Taking the value of EF of that index
	EF_of_centrality_edge = Ef_vector[index_of_centrality_edge];


	//Second iteration - choosing the centrality
	for(ev=0; ev<treeNentries; ++ev)
	{
		if(!(ev%500))
			cout << "Event: " << ev << endl;

		input_tree->GetEntry(ev);
		Npa = event->GetNpa();
		output_tree.BeginEvent();
		EF = 0;

		for(part=0; part<Npa; part++)
		{
			particle = event->GetParticle(part);
			p = TMath::Sqrt(TMath::Power(particle->GetPx(),2)+TMath::Power(particle->GetPy(),2)+TMath::Power(particle->GetPz(),2));
			pT = TMath::Sqrt(TMath::Power(particle->GetPx(),2)+TMath::Power(particle->GetPy(),2));
			phi = TMath::ATan2(particle->GetPy(), particle->GetPx());
			charge = particle->GetCharge();
			mass = particle->GetMass();

			if(psdacc.checkAcceptance(p, pT, charge, phi) == 1)
				EF += TMath::Sqrt(p*p + mass*mass);
		}
		
		//if forward energy sum is smaller than EF edge for X% of the most central, accept the event
		if(EF >= EF_of_centrality_edge)
			continue;

		for(part=0; part<Npa; part++)
		{
			particle = event->GetParticle(part);
			output_tree.AddParticle(*particle);
		}

		output_tree.EndEvent();
		ev_out++;
	}

	output_tree.Close();
	input_rootfile->Close();

	cout << "PSD ACC selection summary\n------------" << endl
		<< "Events before cut: " << treeNentries << endl
		<< "Events after cut: " << ev_out << endl
		<< "Cutted events: " << (treeNentries-ev_out) << endl
		<< "Ratio: " << ((Double_t)ev_out/treeNentries) << endl;

}

int main(int argc, char** argv)
{
	if(argc <= 3)
	{
		cout << "USAGE: cutter <inputfile> <outputfile> <cut_mode> [<beam_momentum>/<multsplit> [<system>]]" << endl
			<< "<cut_mode> = ACC, MULTSPLIT, DEDX, ELASTIC, PT, Y" << endl
			<< "ACC <beam_momentum>" << endl
			<< "MULTSPLIT <n>" << endl
#ifdef data_analysis
			<< "DEDX <beam_momentum> <system>" << endl
#endif
			<< "ELASTIC <beam_momentum>" << endl
			<< "PT <pt=1.5> (will cut from 0 to 1.5 GeV/c, negative <pt> values will result with cutting from <pt> to infinity)" << endl
			<< "Y <beam_momentum>" << endl
			<< "PSDACC <beam_momentum> <system> [<centrality>]" << endl;
		return 0;
	}

	TString inputfile = argv[1];
	TString outputfile = argv[2];
	TString cut_mode = argv[3];
	TString energy = argv[4];
	TString system = argv[5];
	TString mult_string, multmax_string;
	TString pt_cut_string;
	TString y_cut_string;
	TString centrality_string;

	cout << "cut mode:" << cut_mode << endl;
	if(!(cut_mode.CompareTo("ACC")))
	{
		if(argc != 5)
		{
			cout << "ACC cut requires additional argument: beam momentum" << endl;
			return 0;
		}

		RunAccCut(inputfile, outputfile, energy.Atoi());
	}
	else if(!(cut_mode.CompareTo("PPM")))
	{
		if(argc != 6)
		{
			cout << "PPM cut requires additional arguments: 1.beam momentum, 2.system" << endl;
			return 0;
		}
		RunPPMCut(inputfile, outputfile, system, energy);
	}
	else if(!(cut_mode.CompareTo("MULTSPLIT")))
	{
		if(argc == 5)
		{
			mult_string = argv[4];
			RunMultSplit(inputfile, outputfile, mult_string, mult_string);
		}
		else if(argc == 6)
		{
			mult_string = argv[4];
			multmax_string = argv[5];
			RunMultSplit(inputfile, outputfile, mult_string, multmax_string);
		}
		else
		{
			cout << "MULTSPLIT cut requires additional one or two arguments:" << endl
				<< "Single multiplicity bin to be cropped out of the dataset or" << endl
				<< "Multiplicity range. Bins from MIN do MAX (including) will be cropped." << endl
				<< "Example: MULTSPLIT <MIN> [<MAX>]" << endl;
			return 0;
		}
	}
#ifdef data_analysis
	else if(!(cut_mode.CompareTo("DEDX")))
	{
		if(argc != 6)
		{
			cout << "DEDX cut requires additional arguments: 1.beam momentum, 2. system" << endl;
			return 0;
		}
		RunDedxCut(inputfile, outputfile, system, energy.Atoi());
		//RunDedxCut2(inputfile, outputfile);
	}
#endif
	else if(!(cut_mode.CompareTo("ELASTIC")))
	{
		if(argc != 5)
		{
			cout << "ELASTIC cut requires additional argument: beam momentum" << endl;
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
	else if(!(cut_mode.CompareTo("Y")))
	{
		if(argc == 5)
		{
			y_cut_string = argv[4];
			cout << "Rapidity cut on spectators: y_target+0.5 < y < y_beam-0.5" << endl;
			RunYCut(inputfile, outputfile, y_cut_string.Atof());
		}
		else
			cout << "Y cut requires additional argument: beam momentum" << endl;

		return 0;
	}
	else if(!(cut_mode.CompareTo("PSDACC")))
	{
		if(argc == 6)
		{
			cout << "5\% centrality selection for " << system << "@" << energy << "GeV" << endl;
			RunPSDAccSelection(inputfile, outputfile, system, energy.Atoi());
		}
		else if(argc == 7)
		{
			centrality_string = argv[6];
			cout << centrality_string << "\% centrality selection for " << system << "@" << energy << "GeV" << endl;
			RunPSDAccSelection(inputfile, outputfile, system, energy.Atoi(), centrality_string.Atof());
		}
		else
			cout << "PSDACC cut requires: 1. system, 2. energy, 3. centrality (optional)" << endl;
		return 0;
	}
}		
