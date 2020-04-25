{
  gROOT->Reset();
  TFile f("resXic0Helicity.root");
  
  TCanvas *c1 = new TCanvas("c1","M(K^{+}K^{-}}",1200,400);
  c1->Divide(3,1);
  c1->cd(1);
  hm23dat->Draw("e");
  hm23acc->SetLineColor(kBlack);
  hm23acc->Draw("hist,same");
  //TCanvas *c2 = new TCanvas("c2","M(#Xi^{0}K^{-}}",750,600);
  c1->cd(2);
  hm13dat->Draw("e");
  hm13acc->SetLineColor(kBlack);
  hm13acc->Draw("hist,same");
  //TCanvas *c3 = new TCanvas("c3","M(#Xi^{0}K^{+}}",750,600);
  c1->cd(3);
  hm12dat->Draw("e");
  hm12acc->SetLineColor(kBlack);
  hm12acc->Draw("hist,same");
}
