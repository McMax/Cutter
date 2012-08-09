//========= Macro generated from object: CUTG/Graph
//========= by ROOT version5.30/05

TCutG *cutg = new TCutG("dedx_cut",10);
void initialise_dedx_cutg(){
  cutg->SetVarX("TMath::Log10(p)");
  cutg->SetVarY("dedx");
  cutg->SetTitle("e^{-} dE/dx cut for 12E002 p+p @ 158 GeV/c");
  cutg->SetFillColor(1);
//   cutg->SetPoint(0,-0.463085,1.90);
//   cutg->SetPoint(1,-0.463085,1.20);
//   cutg->SetPoint(2,0.2,1.35);
//   cutg->SetPoint(3,0.54194,1.40727);
//   cutg->SetPoint(4,1.02126,1.45138);
//   cutg->SetPoint(5,TMath::Log10(20),1.49148);
//   cutg->SetPoint(6,TMath::Log10(20),1.65589);
//   cutg->SetPoint(7,0.410514,1.90);
//   cutg->SetPoint(8,-0.401237,1.90);
//   cutg->SetPoint(9,-0.463085,1.90);
   cutg->SetPoint(0,0.344283,1.90);
   cutg->SetPoint(1,0.344283,1.20);
   cutg->SetPoint(2,1.58,1.35);
   cutg->SetPoint(3,3.4829,1.40727);
   cutg->SetPoint(4,10.50171,1.45138);
   cutg->SetPoint(5,20,1.49148);
   cutg->SetPoint(6,20,1.65589);
   cutg->SetPoint(7,2.57344,1.90);
   cutg->SetPoint(8,0.396975,1.90);
   cutg->SetPoint(9,0.3442825,1.90);
   cutg->Draw();
  
}
