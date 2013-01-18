//========= Macro generated from object: CUTG/Graph
//========= by ROOT version5.30/05

TCutG *cutg = new TCutG("dedx_cut",10);
void initialise_dedx_cutg(){
  cutg->SetVarX("TMath::Log10(p)");
  cutg->SetVarY("dedx");
  cutg->SetTitle("e^{-} dE/dx cut for 12E002 p+p @ 158 GeV/c");
  cutg->SetFillColor(1);
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
  /*
  cutg->SetPoint(0,0.828572,1.24279);
  cutg->SetPoint(1,1.18813,1.54846);
  cutg->SetPoint(2,2.18922,1.65283);
  cutg->SetPoint(3,4.5015,1.71248);
  cutg->SetPoint(4,7.31708,1.70502);
  cutg->SetPoint(5,11.5267,1.7423);
  cutg->SetPoint(6,11.3475,1.98832);
  cutg->SetPoint(7,5.7843,2.2418);
  cutg->SetPoint(8,3.09042,2.43564);
  cutg->SetPoint(9,0.969144,2.42818);
  cutg->SetPoint(10,0.665348,2.14488);
  cutg->SetPoint(11,0.634792,1.84667);
  cutg->SetPoint(12,0.665348,1.29498);
  cutg->SetPoint(13,0.828572,1.24279);
  */
  cutg->Draw();
}
