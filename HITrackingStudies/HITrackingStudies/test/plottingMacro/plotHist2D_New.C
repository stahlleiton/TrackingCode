#include "RiceStyle.h"

void plotHist2D_New() {

  RiceStyle();

  gStyle->SetOptStat(0);

  TFile *f = new TFile("trk.root");

  char ndir[256] = "HITrackCorrections";
  double ptmax = 100.;

  // sim-to-reco hists
  TH2F *hSim = (TH2F*) f->Get(Form("%s/hsim",ndir)); hSim->GetYaxis()->SetRangeUser(0.2,ptmax);
  TH2F *hEff = (TH2F*) f->Get(Form("%s/heff",ndir)); hEff->GetYaxis()->SetRangeUser(0.2,ptmax);
  TH2F *hMul = (TH2F*) f->Get(Form("%s/hmul",ndir)); hMul->GetYaxis()->SetRangeUser(0.2,ptmax);

  // reco-to-sim hists
  TH2F *hRec = (TH2F*) f->Get(Form("%s/hrec",ndir)); hRec->GetYaxis()->SetRangeUser(0.2,ptmax);
  TH2F *hFak = (TH2F*) f->Get(Form("%s/hfak",ndir)); hFak->GetYaxis()->SetRangeUser(0.2,ptmax);
  TH2F *hSec = (TH2F*) f->Get(Form("%s/hsec",ndir)); hSec->GetYaxis()->SetRangeUser(0.2,ptmax);

  // ratio histograms
  TH2F *rEff = (TH2F*) hEff->Clone("rEff");
  TH2F *rMul = (TH2F*) hMul->Clone("rMul");
  TH2F *rFak = (TH2F*) hFak->Clone("rFak");
  TH2F *rSec = (TH2F*) hSec->Clone("rSec");

  //---------------------------------------------

  // reco efficiency fraction
  TCanvas *c2 = new TCanvas("c2","Reco Efficiency Fraction",600,500);
  //c2->SetLogy();
  gPad->SetRightMargin(0.15);
  rEff->Divide(hEff,hSim,1,1,"B");
  rEff->SetStats(0);
  rEff->SetMaximum(1.0); rEff->SetMinimum(0.0);
  rEff->SetTitle("Absolute Efficiency");
  rEff->Draw("colz");

  // multiple reco fraction
  TCanvas *c3 = new TCanvas("c3","Multiple Reco Fraction",600,500);
  //c3->SetLogy();
  gPad->SetRightMargin(0.15);
  rMul->Divide(hMul,hSim,1,1,"B");
  rMul->SetStats(0);
  rMul->SetMaximum(0.05); rMul->SetMinimum(0.0);
  rMul->SetTitle("Multiple Reconstruction Fraction");
  rMul->Draw("colz");

  // fake reco fraction
  TCanvas *c4 = new TCanvas("c4","Fake Reco Fraction",600,500);
  //c4->SetLogy();
  gPad->SetRightMargin(0.15);
  rFak->Divide(hFak,hRec,1,1,"B");
  rFak->SetStats(0);
  rFak->SetMaximum(0.25); rFak->SetMinimum(0.0);
  rFak->SetTitle("Fake Reconstruction Fraction");
  rFak->Draw("colz");

  // secondary reco fraction
  TCanvas *c5 = new TCanvas("c5","Secondary Fraction",600,500);  
  //c5->SetLogy();
  gPad->SetRightMargin(0.15);
  rSec->Divide(hSec,hRec,1,1,"B");
  rSec->SetStats(0);
  rSec->SetMaximum(0.05); rSec->SetMinimum(0.0);
  rSec->SetTitle("Non-Primary Reconstruction Fraction");
  rSec->Draw("colz");

  //---------------------------------------------

  // find bins corresponding to projections for below
  Int_t ptbin04=hSim->GetYaxis()->FindBin(0.41);
  Int_t ptbin20=hSim->GetYaxis()->FindBin(2.01);
  Int_t ptbins=hSim->GetYaxis()->GetNbins();
  Int_t etabin24m=hSim->GetXaxis()->FindBin(-2.39);
  Int_t etabin24p=hSim->GetXaxis()->FindBin(2.39);
  Int_t etabin10m=hSim->GetXaxis()->FindBin(-0.99);
  Int_t etabin10p=hSim->GetXaxis()->FindBin(0.99);

  cout << "etabin10m: " << etabin10m << " etabin10p: " << etabin10p << endl;
  cout << "etabin10m: " << etabin24m << " etabin10p: " << etabin24p << endl;

  // projected hists: pt > 0.4 GeV/c
  TH1D* hSimEta = (TH1D*) hSim->ProjectionX("hSimEta",ptbin04,ptbins,"e");
  TH1D* hEffEta = (TH1D*) hEff->ProjectionX("hEffEta",ptbin04,ptbins,"e");
  TH1D* hMulEta = (TH1D*) hMul->ProjectionX("hMulEta",ptbin04,ptbins,"e");
  TH1D* hRecEta = (TH1D*) hRec->ProjectionX("hRecEta",ptbin04,ptbins,"e");
  TH1D* hFakEta = (TH1D*) hFak->ProjectionX("hFakEta",ptbin04,ptbins,"e");
  TH1D* hSecEta = (TH1D*) hSec->ProjectionX("hSecEta",ptbin04,ptbins,"e");

  // projected hists: pt > 2.0 GeV/c
  TH1D* hSimEta2 = (TH1D*) hSim->ProjectionX("hSimEta2",ptbin20,ptbins,"e");
  TH1D* hEffEta2 = (TH1D*) hEff->ProjectionX("hEffEta2",ptbin20,ptbins,"e");
  TH1D* hMulEta2 = (TH1D*) hMul->ProjectionX("hMulEta2",ptbin20,ptbins,"e");
  TH1D* hRecEta2 = (TH1D*) hRec->ProjectionX("hRecEta2",ptbin20,ptbins,"e");
  TH1D* hFakEta2 = (TH1D*) hFak->ProjectionX("hFakEta2",ptbin20,ptbins,"e");
  TH1D* hSecEta2 = (TH1D*) hSec->ProjectionX("hSecEta2",ptbin20,ptbins,"e");

  TH1D* hDumEta = new TH1D("hDumEta",";#eta",60,-2.4,2.4); hDumEta->SetMaximum(1.0);
  hDumEta->GetXaxis()->CenterTitle(); hDumEta->GetYaxis()->SetTitleOffset(1.8);
  TH1D* hDumEta2 = (TH1D*) hDumEta->Clone("hDumEta2"); hDumEta2->SetMaximum(0.1); 
  TH1D* hDumEta3 = (TH1D*) hDumEta->Clone("hDumEta3"); hDumEta3->SetMaximum(0.00049); 


  // projected hists: abs(eta) < 1.0
  TH1D* hSimPt  = (TH1D*) hSim->ProjectionY("hSimPt",etabin10m,etabin10p,"e");
  TH1D* hEffPt  = (TH1D*) hEff->ProjectionY("hEffPt",etabin10m,etabin10p,"e");
  TH1D* hMulPt  = (TH1D*) hMul->ProjectionY("hMulPt",etabin10m,etabin10p,"e");
  TH1D* hRecPt  = (TH1D*) hRec->ProjectionY("hRecPt",etabin10m,etabin10p,"e");
  TH1D* hFakPt  = (TH1D*) hFak->ProjectionY("hFakPt",etabin10m,etabin10p,"e");
  TH1D* hSecPt  = (TH1D*) hSec->ProjectionY("hSecPt",etabin10m,etabin10p,"e");

  // projected hists: abs(eta) < 2.4
  TH1D* hSimPt2  = (TH1D*) hSim->ProjectionY("hSimPt2",etabin24m,etabin24p,"e");
  TH1D* hEffPt2  = (TH1D*) hEff->ProjectionY("hEffPt2",etabin24m,etabin24p,"e");
  TH1D* hMulPt2  = (TH1D*) hMul->ProjectionY("hMulPt2",etabin24m,etabin24p,"e");
  TH1D* hRecPt2  = (TH1D*) hRec->ProjectionY("hRecPt2",etabin24m,etabin24p,"e");
  TH1D* hFakPt2  = (TH1D*) hFak->ProjectionY("hFakPt2",etabin24m,etabin24p,"e");
  TH1D* hSecPt2  = (TH1D*) hSec->ProjectionY("hSecPt2",etabin24m,etabin24p,"e");
  
  TH1D* hDumPt = new TH1D("hDumPt",";p_{T} [GeV/c]",80,0.1,10.0); hDumPt->SetMaximum(1.0);
  hDumPt->GetXaxis()->CenterTitle(); hDumPt->GetYaxis()->SetTitleOffset(1.8);
  TH1D* hDumPt2 = (TH1D*) hDumPt->Clone("hDumPt2"); hDumPt2->SetMaximum(0.1); 
  TH1D* hDumPt3 = (TH1D*) hDumPt->Clone("hDumPt3"); hDumPt3->SetMaximum(0.00049); 

  // Efficiency
  TGraphAsymmErrors *gEffEta = new TGraphAsymmErrors(); gEffEta->SetName("gEffEta");
  gEffEta->BayesDivide(hEffEta,hSimEta);
  gEffEta->SetMarkerStyle(25);
  gEffEta->SetLineStyle(2);
  gEffEta->SetLineColor(2);
  gEffEta->SetMarkerColor(2);

  TGraphAsymmErrors *gEffPt = new TGraphAsymmErrors(); gEffPt->SetName("gEffPt");
  gEffPt->BayesDivide(hEffPt,hSimPt);
  gEffPt->SetMarkerStyle(24);
  gEffPt->SetLineColor(4);
  gEffPt->SetMarkerColor(4);

  TGraphAsymmErrors *gEffEta2 = new TGraphAsymmErrors(); gEffEta2->SetName("gEffEta2");
  gEffEta2->BayesDivide(hEffEta2,hSimEta2);
  gEffEta2->SetMarkerStyle(24);
  gEffEta2->SetLineColor(4);
  gEffEta2->SetMarkerColor(4);

  TGraphAsymmErrors *gEffPt2 = new TGraphAsymmErrors(); gEffPt2->SetName("gEffPt2");
  gEffPt2->BayesDivide(hEffPt2,hSimPt2);
  gEffPt2->SetMarkerStyle(25);
  gEffPt2->SetLineStyle(2);
  gEffPt2->SetLineColor(2);
  gEffPt2->SetMarkerColor(2);

  TLegend *legEta = new TLegend(0.35,0.15,0.65,0.30);
  legEta->SetFillColor(0); legEta->SetBorderSize(0);
  legEta->AddEntry(gEffEta,"p_{T} > 0.4 GeV/c","lp");
  legEta->AddEntry(gEffEta2,"p_{T} > 2.0 GeV/c","lp");

  TLegend *legPt = new TLegend(0.4,0.2,0.6,0.35);
  legPt->SetFillColor(0); legPt->SetBorderSize(0);
  legPt->AddEntry(gEffPt2,"|#eta| < 2.4","lp");
  legPt->AddEntry(gEffPt,"|#eta| < 1.0","lp");

  //Dener--------------------------------
  TH1F* hDumEtaEff;
  TH1F* hDumPtEff;
  //-------------------------------------
  TCanvas* c7 = makeMultiCanvas("c7", "Efficiency Fraction", 2,1 );
  hDumEtaEff=(TH1F*) hDumEta->Clone("hDumEtaEff"); fixedFontHist1D(hDumEtaEff,1.05,1.2);
  hDumEtaEff->GetYaxis()->SetTitle("Absolute efficiency");
  hDumPtEff=(TH1F*) hDumPt->Clone("hDumPtEff"); fixedFontHist1D(hDumPtEff,1.05,1.2);
  hDumPtEff->GetYaxis()->SetTitle("Absolute efficiency");
  c7->cd(1); gPad->SetTicks(); c7->GetPad(1)->SetLeftMargin(0.12); c7->GetPad(1)->SetBottomMargin(0.13); c7->GetPad(1)->SetLogx(0); hDumEtaEff->Draw(); gEffEta->Draw("pc"); gEffEta2->Draw("pc"); legEta->Draw();
  c7->cd(2); gPad->SetTicks(); c7->GetPad(2)->SetLeftMargin(0.12); c7->GetPad(2)->SetBottomMargin(0.13); c7->GetPad(2)->SetLogx(); hDumPtEff->Draw(); gEffPt->Draw("pc"); gEffPt2->Draw("pc"); legPt->Draw();
  saveCanvas(c7, "files", "AbsoluteEfficiency");

  // Multiple Reco
  TGraphAsymmErrors *gMulEta = new TGraphAsymmErrors(); gMulEta->SetName("gMulEta");
  gMulEta->BayesDivide(hMulEta,hSimEta);
  gMulEta->SetMarkerStyle(25);
  gMulEta->SetLineStyle(2);
  gMulEta->SetLineColor(2);
  gMulEta->SetMarkerColor(2);

  TGraphAsymmErrors *gMulPt = new TGraphAsymmErrors(); gMulPt->SetName("gMulPt");
  gMulPt->BayesDivide(hMulPt,hSimPt);
  gMulPt->SetMarkerStyle(24);
  gMulPt->SetLineColor(4);
  gMulPt->SetMarkerColor(4);

  TGraphAsymmErrors *gMulEta2 = new TGraphAsymmErrors(); gMulEta2->SetName("gMulEta2");
  gMulEta2->BayesDivide(hMulEta2,hSimEta2);
  gMulEta2->SetMarkerStyle(24);
  gMulEta2->SetLineColor(4);
  gMulEta2->SetMarkerColor(4);

  TGraphAsymmErrors *gMulPt2 = new TGraphAsymmErrors(); gMulPt2->SetName("gMulPt2");
  gMulPt2->BayesDivide(hMulPt2,hSimPt2);
  gMulPt2->SetMarkerStyle(25);
  gMulPt2->SetLineStyle(2);
  gMulPt2->SetLineColor(2);
  gMulPt2->SetMarkerColor(2);

  //Dener--------------------------------
  TH1F* hDumEtaMul;
  TH1F* hDumPtMul;
  TLegend* legEta2;
  TLegend* legPt2;
  //      //-------------------------------------
  TCanvas *c8 = makeMultiCanvas("c8","Multiple Fraction", 2,1);
  hDumEtaMul=(TH1F*) hDumEta3->Clone("hDumEtaMul"); fixedFontHist1D(hDumEtaMul, 1.05,1.2); hDumEtaMul->GetYaxis()->SetRangeUser(0,0.0009);
  hDumEtaMul->GetYaxis()->SetTitle("Multiple Reconstruction Fraction");
  hDumPtMul=(TH1F*) hDumPt3->Clone("hDumPtMul"); fixedFontHist1D(hDumPtMul, 1.05, 1.2); hDumPtMul->GetYaxis()->SetRangeUser(0,0.0009);
  hDumPtMul->GetYaxis()->SetTitle("Multiple Reconstruction Fraction");
  legEta2 = (TLegend*) legEta->Clone(); legEta2->SetY1(0.65); legEta2->SetY2(0.85);
  legPt2 = (TLegend*) legPt->Clone(); legPt2->SetY1(0.65); legPt2->SetY2(0.85);
  c8->cd(1); gPad->SetLogx(0); gPad->SetTicks(); c8->GetPad(1)->SetLeftMargin(0.12); c8->GetPad(1)->SetBottomMargin(0.13); hDumEtaMul->Draw(); gMulEta->Draw("pc"); gMulEta2->Draw("pc"); legEta2->Draw();
  c8->cd(2); gPad->SetLogx(1); gPad->SetTicks(); c8->GetPad(2)->SetLeftMargin(0.12); c8->GetPad(2)->SetBottomMargin(0.13); hDumPtMul->Draw(); gMulPt->Draw("pc"); gMulPt2->Draw("pc"); legPt2->Draw();
  saveCanvas(c8, "files", "MultipleReconstruction");
  
  // Fakes
  TGraphAsymmErrors *gFakEta = new TGraphAsymmErrors();  gFakEta->SetName("gFakEta");
  gFakEta->BayesDivide(hFakEta,hRecEta);
  gFakEta->SetMarkerStyle(25);
  gFakEta->SetLineStyle(2);
  gFakEta->SetLineColor(2);
  gFakEta->SetMarkerColor(2);

  TGraphAsymmErrors *gFakPt = new TGraphAsymmErrors(); gFakPt->SetName("gFakPt");
  gFakPt->BayesDivide(hFakPt,hRecPt);
  gFakPt->SetMarkerStyle(24);
  gFakPt->SetLineColor(4);
  gFakPt->SetMarkerColor(4);

  TGraphAsymmErrors *gFakEta2 = new TGraphAsymmErrors(); gFakEta2->SetName("gFakEta2");
  gFakEta2->BayesDivide(hFakEta2,hRecEta2);
  gFakEta2->SetMarkerStyle(24);
  gFakEta2->SetLineColor(4);
  gFakEta2->SetMarkerColor(4);

  TGraphAsymmErrors *gFakPt2 = new TGraphAsymmErrors();  gFakPt2->SetName("gFakPt2");
  gFakPt2->BayesDivide(hFakPt2,hRecPt2);
  gFakPt2->SetMarkerStyle(25);
  gFakPt2->SetLineStyle(2);
  gFakPt2->SetLineColor(2);
  gFakPt2->SetMarkerColor(2);

  //TCanvas *c9 = new TCanvas("c9","Fake Fraction",900,500);

  //Dener--------------------------------
  TH1F* hDumEtaFak;
  TH1F* hDumPtFak;
  //-------------------------------------        
  TCanvas* c9 = makeMultiCanvas("c9", "Fake Fraction", 2,1);
  hDumEtaFak=(TH1F*) hDumEta2->Clone("hDumEtaMul"); fixedFontHist1D(hDumEtaFak, 1.05,1.2); hDumEtaFak->GetYaxis()->SetRangeUser(0.,0.7);
  hDumEtaFak->GetYaxis()->SetTitle("Fake Reconstruction Fraction");
  hDumPtFak=(TH1F*) hDumPt2->Clone("hDumPtMul"); fixedFontHist1D(hDumPtFak, 1.05,1.2); hDumPtFak->GetYaxis()->SetRangeUser(0,1);
  hDumPtFak->GetYaxis()->SetTitle("Fake Reconstruction Fraction");
  c9->cd(1); hDumEtaFak->Draw(); gFakEta->Draw("pc"); gFakEta2->Draw("pc"); legEta2->Draw();
  gPad->SetTicks(); gPad->SetLeftMargin(0.12); gPad->SetBottomMargin(0.13);
  c9->cd(2); hDumPtFak->Draw(); gFakPt->Draw("pc"); gFakPt2->Draw("pc"); legPt2->Draw();
  gPad->SetTicks(); gPad->SetLeftMargin(0.12); gPad->SetBottomMargin(0.13); gPad->SetLogx(1);
  saveCanvas(c9, "files", "FakeRate");
//  c9->GetPad(2)->SetLogx();

  // Secondaries
  TGraphAsymmErrors *gSecEta = new TGraphAsymmErrors(); gSecEta->SetName("gSecEta");
  gSecEta->BayesDivide(hSecEta,hRecEta);
  gSecEta->SetMarkerStyle(25);
  gSecEta->SetLineStyle(2);
  gSecEta->SetLineColor(2);
  gSecEta->SetMarkerColor(2);

  TGraphAsymmErrors *gSecPt = new TGraphAsymmErrors(); gSecPt->SetName("gSecPt");
  gSecPt->BayesDivide(hSecPt,hRecPt);
  gSecPt->SetMarkerStyle(24);
  gSecPt->SetLineColor(4);
  gSecPt->SetMarkerColor(4);

  TGraphAsymmErrors *gSecEta2 = new TGraphAsymmErrors(); gSecEta2->SetName("gSecEta2");
  gSecEta2->BayesDivide(hSecEta2,hRecEta2);
  gSecEta2->SetMarkerStyle(24);
  gSecEta2->SetLineColor(4);
  gSecEta2->SetMarkerColor(4);

  TGraphAsymmErrors *gSecPt2 = new TGraphAsymmErrors();  gSecPt2->SetName("gSecPt2");
  gSecPt2->BayesDivide(hSecPt2,hRecPt2);
  gSecPt2->SetMarkerStyle(25);
  gSecPt2->SetLineStyle(2);
  gSecPt2->SetLineColor(2);
  gSecPt2->SetMarkerColor(2);

    //Dener--------------------------------
      TH1F* hDumEtaSec;
      TH1F* hDumPtSec;
    //-------------------------------------
  TCanvas* c10 = makeMultiCanvas("c10", "Secondary Fraction", 2, 1);
  hDumEtaSec=(TH1F*) hDumEta2->Clone("hDumEtaMul"); fixedFontHist1D(hDumEtaSec, 1.05,1.3); hDumEtaSec->GetYaxis()->SetRangeUser(0.,0.012);
  hDumEtaSec->GetYaxis()->SetTitle("Non-Primary Reconstruction Fraction");
  hDumPtSec=(TH1F*) hDumPt2->Clone("hDumPtMul"); fixedFontHist1D(hDumPtSec, 1.05, 1.3);hDumPtSec->GetYaxis()->SetRangeUser(0.,0.1);
  hDumPtSec->GetYaxis()->SetTitle("Non-Primary Reconstruction Fraction");
  c10->cd(1); hDumEtaSec->Draw(); gSecEta->Draw("pc"); gSecEta2->Draw("pc"); legEta2->Draw();
  gPad->SetTicks(); gPad->SetLeftMargin(0.15); gPad->SetBottomMargin(0.13); 
  c10->cd(2); hDumPtSec->Draw(); gSecPt->Draw("pc"); gSecPt2->Draw("pc"); legPt2->Draw();
  gPad->SetTicks(); gPad->SetLeftMargin(0.15); gPad->SetBottomMargin(0.13); gPad->SetLogx(1);
  saveCanvas(c10, "files", "SecondaryReconstruction");

  TFile *fout = new TFile("test.root","RECREATE");
  gEffPt->Write(); gEffPt2->Write(); gEffEta->Write(); gEffEta2->Write();
  gMulPt->Write(); gMulPt2->Write(); gMulEta->Write(); gMulEta2->Write();
  gFakPt->Write(); gFakPt2->Write(); gFakEta->Write(); gFakEta2->Write();
  gSecPt->Write(); gSecPt2->Write(); gSecEta->Write(); gSecEta2->Write();
  fout->Close();
}


void set_plot_style() {

  // nicer colz plots

  const Int_t NRGBs = 5;
  const Int_t NCont = 255;

  Double_t stops[NRGBs] = { 0.00, 0.34, 0.61, 0.84, 1.00 };
  Double_t red[NRGBs]   = { 0.00, 0.00, 0.87, 1.00, 0.51 };
  Double_t green[NRGBs] = { 0.00, 0.81, 1.00, 0.20, 0.00 };
  Double_t blue[NRGBs]  = { 0.51, 1.00, 0.12, 0.00, 0.00 };
  TColor::CreateGradientColorTable(NRGBs, stops, red, green, blue, NCont);
  gStyle->SetNumberContours(NCont);

}
