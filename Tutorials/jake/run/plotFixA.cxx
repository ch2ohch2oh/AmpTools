{
  gROOT->Reset();
  TFile f("Xic0HelicityFixA.root");
  
  TCanvas *c1 = new TCanvas("c1","M(K^{+}K^{-}}",1200,400);
  c1->Divide(3,1);
  c1->cd(1);
  hm23dat->Draw("e");
  hm23acc->SetLineColor(kBlack);
  hm23acc1->SetLineColor(kRed);
  hm23acc2->SetLineColor(kBlue);
  hm23acc->Draw("hist,same");
  hm23acc1->Draw("hist,same");
  hm23acc2->Draw("hist,same");
  //TCanvas *c2 = new TCanvas("c2","M(#Xi^{0}K^{-}}",750,600);
  c1->cd(2);
  hm13dat->Draw("e");
  hm13acc->SetLineColor(kBlack);
  hm13acc1->SetLineColor(kRed);
  hm13acc2->SetLineColor(kBlue);
  hm13acc->Draw("hist,same");
  hm13acc1->Draw("hist,same");
  hm13acc2->Draw("hist,same");
  //TCanvas *c3 = new TCanvas("c3","M(#Xi^{0}K^{+}}",750,600);
  c1->cd(3);
  hm12dat->Draw("e");
  hm12acc->SetLineColor(kBlack);
  hm12acc1->SetLineColor(kRed);
  hm12acc2->SetLineColor(kBlue);
  hm12acc->Draw("hist,same");
  hm12acc1->Draw("hist,same");
  hm12acc2->Draw("hist,same");
    
}
