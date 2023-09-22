///Please, run it by doing: <<root -l -b -q macro_control_plots_ThreeSamples.C>>
///Usually, depending on the number of events you processed & selections, will be needed to adjust y-axis range
///Run in Recodebug, Reco, AOD, MiniAOD MC and Data samples produced by: "HITrackingStudies/HITrackingStudies/test/run_PbPb_cfg.py"
///Please, change name of input files below variables "fileName1" and "fileName2" and "fileName3"

///*** Important parameters
bool runOnMiniAOD = true;
bool selecAlgo = false; //if true will do all plots (except N-1 plots) for a specific iteration, defined in the variable algoToPlot below. Please, use false for MiniAOD
int algoToPlot = 22;
//select centrality range (in 0 - 100% range) that want to see the control plots 
int cent_min = 0;
int cent_max = 100;
//input file
auto fileName1 = "trk.root"; //ratio plots are fileName1/fileName2, fileName1/fileName3
auto fileName2 = "trk.root";
auto fileName3 = "trk.root";

///Auxiliar functions

//Function to select high-purity tracks & saving standard variables
std::vector<float> func_select_trks( ROOT::VecOps::RVec<Float_t> vec_ori, ROOT::VecOps::RVec<Bool_t> vec_trk_fake, ROOT::VecOps::RVec<Bool_t> vec_trk_hp, ROOT::VecOps::RVec<Int_t> vec_trk_algo){
   std::vector<float> v;
   for (int i = 0; i < vec_ori.size(); i++){
      if(selecAlgo){     
         if ( vec_trk_hp[i] && vec_trk_algo[i]==algoToPlot ) v.push_back(vec_ori[i]);
      }else{
	 if(runOnMiniAOD){     
            v.push_back(vec_ori[i]);		 
	 }else{
	    if ( vec_trk_hp[i] ) v.push_back(vec_ori[i]);	 
	 }
      }
   }
   return v;
}

//Function to select high-purity tracks & saving DCA significance or pT resolution
std::vector<float> func_ratio_select_trks( ROOT::VecOps::RVec<Float_t> vec_num, ROOT::VecOps::RVec<Float_t> vec_den, ROOT::VecOps::RVec<Bool_t> vec_trk_fake, ROOT::VecOps::RVec<Bool_t> vec_trk_hp, ROOT::VecOps::RVec<Int_t> vec_trk_algo){
   std::vector<float> v;
   for (int i = 0; i < vec_num.size(); i++){
      if(selecAlgo){     
         if ( vec_trk_hp[i] && vec_trk_algo[i]==algoToPlot ) v.push_back(vec_num[i]/vec_den[i]);
      }else{
	 if(runOnMiniAOD){
            v.push_back(vec_num[i]/vec_den[i]);		 
	 }else{ 
	    if ( vec_trk_hp[i] ) v.push_back(vec_num[i]/vec_den[i]); 	 
         }
      }
   }
   return v;
}

//Function to select high-purity tracks & saving chi2/ndf/nlayers
std::vector<float> func_chi2_select_trks( ROOT::VecOps::RVec<Float_t> vec_chi2, ROOT::VecOps::RVec<Float_t> vec_ndf,ROOT::VecOps::RVec<Float_t> vec_nlayers, ROOT::VecOps::RVec<Bool_t> vec_trk_fake, ROOT::VecOps::RVec<Bool_t> vec_trk_hp, ROOT::VecOps::RVec<Int_t> vec_trk_algo){
   std::vector<float> v;
   for (int i = 0; i < vec_chi2.size(); i++){
      if(selecAlgo){     
         if ( vec_trk_hp[i] && vec_trk_algo[i]==algoToPlot ) v.push_back(vec_chi2[i]/vec_ndf[i]/vec_nlayers[i]);
      }else{
	 if(runOnMiniAOD){
            v.push_back(vec_chi2[i]/vec_nlayers[i]);		 
	 }else{   	 
            if ( vec_trk_hp[i] ) v.push_back(vec_chi2[i]/vec_ndf[i]/vec_nlayers[i]);		 
	 }
      }
   }
   return v;
}

//Function to select with ``Full Selection Minus 1Variable'' for HP tracks
std::vector<float> func_NminusOneVar_select_trks( std::vector<float> vec_nHits, std::vector<float> vec_chi2, std::vector<float> vec_dzSig, std::vector<float> vec_dxySig, std::vector<float> vec_ptRes, bool isNhit, bool isChi2, bool isDzSig, bool isDxySig, bool isPtRes ){

   ///IMPORTANT: in case need to test other track selections, should change here	
   float cut_dzSig = 3.0;
   float cut_dxySig = 3.0;
   float cut_ptRes = 0.10;
   float cut_chi2 = 0.18;
   float cut_nhits = 11;

   std::vector<float> v;
   for (int i = 0; i < vec_nHits.size(); i++){
	 if(isNhit){  
            if ( std::abs(vec_dzSig[i])<cut_dzSig && std::abs(vec_dxySig[i])<cut_dxySig && vec_ptRes[i]<cut_ptRes && vec_chi2[i]<cut_chi2 ){
               v.push_back(vec_nHits[i]);
	    }   
	 }
         else if (isChi2){
	    if ( std::abs(vec_dzSig[i])<cut_dzSig && std::abs(vec_dxySig[i])<cut_dxySig && vec_ptRes[i]<cut_ptRes && vec_nHits[i]>=cut_nhits ){
               v.push_back(vec_chi2[i]);
            } 
	 }
         else if (isDzSig){
            if ( vec_chi2[i]<cut_chi2 && std::abs(vec_dxySig[i])<cut_dxySig && vec_ptRes[i]<cut_ptRes && vec_nHits[i]>=cut_nhits ){
               v.push_back(vec_dzSig[i]);
	    } 
         }
         else if (isDxySig){
            if ( vec_chi2[i]<cut_chi2 && std::abs(vec_dzSig[i])<cut_dzSig && vec_ptRes[i]<cut_ptRes && vec_nHits[i]>=cut_nhits ){
               v.push_back(vec_dxySig[i]);
            }
         }
         else if (isPtRes){
	    if ( std::abs(vec_dzSig[i])<cut_dzSig && std::abs(vec_dxySig[i])<cut_dxySig && vec_chi2[i]<cut_chi2 && vec_nHits[i]>=cut_nhits ){
               v.push_back(vec_ptRes[i]);
            }
         }	 
	 else {}
   }
   return v;
}


//Function to customize Legend of the histograms
void legendStyle(TLegend* tl, TH1D* h_all1, TH1D* h_all2, TH1D* h_all3){
   tl->AddEntry(h_all1,"Sample1","l");
   tl->AddEntry(h_all2,"Sample2","l");
   tl->AddEntry(h_all3,"Sample3","l");
   tl->SetFillStyle(0);
   tl->SetBorderSize(0);
   tl->SetTextSize(0.045);
   tl->SetTextFont(42);
}

void macro_control_plots_ThreeSamples(){

std::string cent_cut = "centrality>="+to_string(2*cent_min)+" && centrality<="+to_string(2*cent_max);

auto treeName = "anaTrack/trackTree"; //tree name

// We read the tree from the file and create a RDataFrame object called "d"
ROOT::RDataFrame d1(treeName, fileName1);
ROOT::RDataFrame d2(treeName, fileName2);
ROOT::RDataFrame d3(treeName, fileName3);

auto d1_select = d1.Filter(cent_cut,"Centrality cut") //filter events only in this centrality range --- see definition above
		 .Define("trkPt_all","func_select_trks(trkPt.trkPt,trkFake.trkFake,highPurity.highPurity,trkAlgo.trkAlgo)") //pt 
                 .Define("trkEta_all","func_select_trks(trkEta.trkEta,trkFake.trkFake,highPurity.highPurity,trkAlgo.trkAlgo)") //eta 
                 .Define("trkPhi_all","func_select_trks(trkPhi.trkPhi,trkFake.trkFake,highPurity.highPurity,trkAlgo.trkAlgo)") //phi 
                 .Define("trkNHit_all","func_select_trks(trkNHit.trkNHit,trkFake.trkFake,highPurity.highPurity,trkAlgo.trkAlgo)") //nhits
                 .Define("trkDzSig_all","func_ratio_select_trks(trkDz1.trkDz1,trkDzError1.trkDzError1,trkFake.trkFake,highPurity.highPurity,trkAlgo.trkAlgo)") //dzSig
                 .Define("trkDxySig_all","func_ratio_select_trks(trkDxy1.trkDxy1,trkDxyError1.trkDxyError1,trkFake.trkFake,highPurity.highPurity,trkAlgo.trkAlgo)") //dxySig
                 .Define("trkPtRes_all","func_ratio_select_trks(trkPtError.trkPtError,trkPt.trkPt,trkFake.trkFake,highPurity.highPurity,trkAlgo.trkAlgo)") //ptRes
                 .Define("trkChi2_all","func_chi2_select_trks(trkChi2.trkChi2,trkNdof.trkNdof,trkNlayer.trkNlayer,trkFake.trkFake,highPurity.highPurity,trkAlgo.trkAlgo)")//ptRes
                 .Define("trkAlgo_all","func_select_trks(trkAlgo.trkAlgo,trkFake.trkFake,highPurity.highPurity,trkAlgo.trkAlgo)")//algo
                 .Define("trkNHit_NminusOne_all","func_NminusOneVar_select_trks(trkNHit_all,trkChi2_all,trkDzSig_all,trkDxySig_all,trkPtRes_all,true,false,false,false,false)") //nhits
                 .Define("trkChi2_NminusOne_all","func_NminusOneVar_select_trks(trkNHit_all,trkChi2_all,trkDzSig_all,trkDxySig_all,trkPtRes_all,false,true,false,false,false)") //chi2
                 .Define("trkDzSig_NminusOne_all","func_NminusOneVar_select_trks(trkNHit_all,trkChi2_all,trkDzSig_all,trkDxySig_all,trkPtRes_all,false,false,true,false,false)") //dzSig
                 .Define("trkDxySig_NminusOne_all","func_NminusOneVar_select_trks(trkNHit_all,trkChi2_all,trkDzSig_all,trkDxySig_all,trkPtRes_all,false,false,false,true,false)") //dxySig
                 .Define("trkPtRes_NminusOne_all","func_NminusOneVar_select_trks(trkNHit_all,trkChi2_all,trkDzSig_all,trkDxySig_all,trkPtRes_all,false,false,false,false,true)"); //ptRes

auto d2_select = d2.Filter(cent_cut,"Centrality cut") //filter events only in this centrality range --- see definition above
		 .Define("trkPt_all","func_select_trks(trkPt.trkPt,trkFake.trkFake,highPurity.highPurity,trkAlgo.trkAlgo)") //pt 
                 .Define("trkEta_all","func_select_trks(trkEta.trkEta,trkFake.trkFake,highPurity.highPurity,trkAlgo.trkAlgo)") //eta 
                 .Define("trkPhi_all","func_select_trks(trkPhi.trkPhi,trkFake.trkFake,highPurity.highPurity,trkAlgo.trkAlgo)") //phi 
                 .Define("trkNHit_all","func_select_trks(trkNHit.trkNHit,trkFake.trkFake,highPurity.highPurity,trkAlgo.trkAlgo)") //nhits
                 .Define("trkDzSig_all","func_ratio_select_trks(trkDz1.trkDz1,trkDzError1.trkDzError1,trkFake.trkFake,highPurity.highPurity,trkAlgo.trkAlgo)") //dzSig
                 .Define("trkDxySig_all","func_ratio_select_trks(trkDxy1.trkDxy1,trkDxyError1.trkDxyError1,trkFake.trkFake,highPurity.highPurity,trkAlgo.trkAlgo)") //dxySig
                 .Define("trkPtRes_all","func_ratio_select_trks(trkPtError.trkPtError,trkPt.trkPt,trkFake.trkFake,highPurity.highPurity,trkAlgo.trkAlgo)") //ptRes
                 .Define("trkChi2_all","func_chi2_select_trks(trkChi2.trkChi2,trkNdof.trkNdof,trkNlayer.trkNlayer,trkFake.trkFake,highPurity.highPurity,trkAlgo.trkAlgo)")//ptRes
                 .Define("trkAlgo_all","func_select_trks(trkAlgo.trkAlgo,trkFake.trkFake,highPurity.highPurity,trkAlgo.trkAlgo)")//algo
                 .Define("trkNHit_NminusOne_all","func_NminusOneVar_select_trks(trkNHit_all,trkChi2_all,trkDzSig_all,trkDxySig_all,trkPtRes_all,true,false,false,false,false)") //nhits
                 .Define("trkChi2_NminusOne_all","func_NminusOneVar_select_trks(trkNHit_all,trkChi2_all,trkDzSig_all,trkDxySig_all,trkPtRes_all,false,true,false,false,false)") //chi2
                 .Define("trkDzSig_NminusOne_all","func_NminusOneVar_select_trks(trkNHit_all,trkChi2_all,trkDzSig_all,trkDxySig_all,trkPtRes_all,false,false,true,false,false)") //dzSig
                 .Define("trkDxySig_NminusOne_all","func_NminusOneVar_select_trks(trkNHit_all,trkChi2_all,trkDzSig_all,trkDxySig_all,trkPtRes_all,false,false,false,true,false)") //dxySig
                 .Define("trkPtRes_NminusOne_all","func_NminusOneVar_select_trks(trkNHit_all,trkChi2_all,trkDzSig_all,trkDxySig_all,trkPtRes_all,false,false,false,false,true)"); //ptRes

auto d3_select = d3.Filter(cent_cut,"Centrality cut") //filter events only in this centrality range --- see definition above
		 .Define("trkPt_all","func_select_trks(trkPt.trkPt,trkFake.trkFake,highPurity.highPurity,trkAlgo.trkAlgo)") //pt 
                 .Define("trkEta_all","func_select_trks(trkEta.trkEta,trkFake.trkFake,highPurity.highPurity,trkAlgo.trkAlgo)") //eta 
                 .Define("trkPhi_all","func_select_trks(trkPhi.trkPhi,trkFake.trkFake,highPurity.highPurity,trkAlgo.trkAlgo)") //phi 
                 .Define("trkNHit_all","func_select_trks(trkNHit.trkNHit,trkFake.trkFake,highPurity.highPurity,trkAlgo.trkAlgo)") //nhits
                 .Define("trkDzSig_all","func_ratio_select_trks(trkDz1.trkDz1,trkDzError1.trkDzError1,trkFake.trkFake,highPurity.highPurity,trkAlgo.trkAlgo)") //dzSig
                 .Define("trkDxySig_all","func_ratio_select_trks(trkDxy1.trkDxy1,trkDxyError1.trkDxyError1,trkFake.trkFake,highPurity.highPurity,trkAlgo.trkAlgo)") //dxySig
                 .Define("trkPtRes_all","func_ratio_select_trks(trkPtError.trkPtError,trkPt.trkPt,trkFake.trkFake,highPurity.highPurity,trkAlgo.trkAlgo)") //ptRes
                 .Define("trkChi2_all","func_chi2_select_trks(trkChi2.trkChi2,trkNdof.trkNdof,trkNlayer.trkNlayer,trkFake.trkFake,highPurity.highPurity,trkAlgo.trkAlgo)")//ptRes
                 .Define("trkAlgo_all","func_select_trks(trkAlgo.trkAlgo,trkFake.trkFake,highPurity.highPurity,trkAlgo.trkAlgo)")//algo
                 .Define("trkNHit_NminusOne_all","func_NminusOneVar_select_trks(trkNHit_all,trkChi2_all,trkDzSig_all,trkDxySig_all,trkPtRes_all,true,false,false,false,false)") //nhits
                 .Define("trkChi2_NminusOne_all","func_NminusOneVar_select_trks(trkNHit_all,trkChi2_all,trkDzSig_all,trkDxySig_all,trkPtRes_all,false,true,false,false,false)") //chi2
                 .Define("trkDzSig_NminusOne_all","func_NminusOneVar_select_trks(trkNHit_all,trkChi2_all,trkDzSig_all,trkDxySig_all,trkPtRes_all,false,false,true,false,false)") //dzSig
                 .Define("trkDxySig_NminusOne_all","func_NminusOneVar_select_trks(trkNHit_all,trkChi2_all,trkDzSig_all,trkDxySig_all,trkPtRes_all,false,false,false,true,false)") //dxySig
                 .Define("trkPtRes_NminusOne_all","func_NminusOneVar_select_trks(trkNHit_all,trkChi2_all,trkDzSig_all,trkDxySig_all,trkPtRes_all,false,false,false,false,true)"); //ptRes


///inputs for the loop to plot histigrams
const unsigned int N_variables = 14; //Sequence: pt, eta, phi, nhits, dzSig, dxySig, ptRes, chi2, algo, nhits_nMinusOne, dzSig_nMinusOne, dxySig_nMinusOne, chi2_nMinusOne, ptRes_nMinusOne...
std::string variable_name_all[N_variables]={"trkPt_all","trkEta_all","trkPhi_all","trkNHit_all","trkDzSig_all","trkDxySig_all","trkPtRes_all","trkChi2_all","trkAlgo_all","trkNHit_NminusOne_all","trkDzSig_NminusOne_all","trkDxySig_NminusOne_all","trkPtRes_NminusOne_all","trkChi2_NminusOne_all"}; //variables' name
TString hist_name_all[N_variables]={"trkPt","trkEta","trkPhi","trkNHit","trkDzSig","trkDxySig","trkPtRes","trkChi2","trkAlgo","trkNHit_NminusOne","trkDzSig_NminusOne","trkDxySig_NminusOne","trkPtRes_NminusOne","trkChi2_NminusOne"}; //histograms' name
TString hist_title_all[N_variables]={"p_{T} of HP tracks","#eta of HP tracks","#phi of HP tracks", "NHits of HP tracks", "DzSig of HP tracks", "DxySig of HP tracks", "Pt Resolution of HP tracks","#chi^{2}/ndf/nlayers of HP tracks","Algo of HP tracks","NHits with All Other Cuts","DzSig with All Other Cuts", "DxySig with All Other Cuts","Pt Resolution with All Other Cuts","#chi^{2}/ndf/nlayers with All Other Cuts"}; //histograms' title
const float hist_Nbins[N_variables]= {100,100,100,50,100,100,100,100,30,50,100,100,100,100}; //Number of bins 
const float hist_Xrange_min[N_variables]= {0,-3.5,-4.5,0,-30,-30,0,0,0,0,-30,-30,0,0}; //Minimum value of the x-axis range
const float hist_Xrange_max[N_variables]= {10,3.5,4.5,50,30,30,0.3,1,30,50,30,30,0.3,1}; //Maximum value of the x-axis range
TString hist_XaxisTitle[N_variables]={"p_{T} (GeV/c)","#eta","#phi","Number of Hits", "dZ/#sigma_{dZ}", "dXY/#sigma_{dXY}","p_{T}Err/p_{T}","#chi^{2}/ndf/nlayers","Algo","Number of Hits","dZ/#sigma_{dZ}", "dXY/#sigma_{dXY}","p_{T}Err/p_{T}","#chi^{2}/ndf/nlayers"}; //Title of x-axis
const float hist_Yrange_min[N_variables]={0.9,0.9,0.9,0.9,0.9,0.9,0.9,0.9,0.9,0.9,0.9,0.9,0.9,0.9}; //Minimum of the y-axis range
const float hist_Yrange_max[N_variables]={10000000,10000000,10000000,10000000,10000000,10000000,10000000,10000000,10000000,10000000,10000000,10000000,10000000,10000000}; //Maximum of the y-axis
const float histNorm_Yrange_min[N_variables]={0.00001,0.00001,0.00001,0.00001,0.00001,0.00001,0.00001,0.00001,0.00001,0.00001,0.00001,0.00001,0.00001,0.00001}; //Minimum of the y-axis range
const float histNorm_Yrange_max[N_variables]={0.15,0.03,0.03,0.30,0.30,0.30,0.30,0.12,0.30,0.30,0.30,0.30,0.30,0.15}; //Maximum of the y-axis range 
TString fig_name[N_variables]={"hist_pt_allTrks_ThreeSamples_NoSel.pdf","hist_eta_allTrks_ThreeSamples_NoSel.pdf","hist_phi_allTrks_ThreeSamples_NoSel.pdf","hist_nhits_allTrks_ThreeSamples_NoSel.pdf","hist_dZsig_allTrks_ThreeSamples_NoSel.pdf","hist_dXYsig_allTrks_ThreeSamples_NoSel.pdf","hist_pTres_allTrks_ThreeSamples_NoSel.pdf","hist_chi2_allTrks_ThreeSamples_NoSel.pdf","hist_algo_allTrks_ThreeSamples_NoSel.pdf","hist_nhits_allTrks_ThreeSamples_NimusOneCuts.pdf","hist_dZsig_allTrks_ThreeSamples_NimusOneCuts.pdf","hist_dXYsig_allTrks_ThreeSamples_NimusOneCuts.pdf","hist_pTres_allTrks_ThreeSamples_NimusOneCuts.pdf","hist_chi2_allTrks_ThreeSamples_NimusOneCuts.pdf"}; //Name of the figure file with the histograms saved
auto yaxis_label1 = "Normalized Distributions"; //y-axis label integral histos

//Book histograms using RDataFrame, plot and save the figure in a PDF format

TCanvas tc("tc","tc",1000,500); //ROOT TCanvas to plot the variables - it will be updated on-the-fly after saving the PDF with the plot
tc.Divide(2,1,0.02,0.02);

gSystem->Exec("mkdir -p ControlPlotsThreeSamples"); //Directory to save the histograms
gStyle->SetOptStat(0);

for (unsigned int i=0; i<N_variables; i++){

   //Book histograms		
   auto h_all1 = d1_select.Histo1D(TH1D(hist_name_all[i],hist_title_all[i]+Form(", %d-%d",cent_min,cent_max)+"%",hist_Nbins[i],hist_Xrange_min[i],hist_Xrange_max[i]),variable_name_all[i]);
   h_all1->GetYaxis()->SetTitle(yaxis_label1);
   h_all1->GetYaxis()->SetTitleOffset(1.4);
   h_all1->GetXaxis()->SetTitle(hist_XaxisTitle[i]);
   h_all1->SetLineColor(1);
   h_all1->SetMarkerColor(1);
   h_all1->SetLineWidth(2);
   h_all1->GetYaxis()->SetRangeUser(hist_Yrange_min[i], hist_Yrange_max[i]);
   
   auto h_all2 = d2_select.Histo1D(TH1D(hist_name_all[i]+Form("_2"),hist_title_all[i]+Form("_2, %d-%d",cent_min,cent_max)+"%",hist_Nbins[i],hist_Xrange_min[i],hist_Xrange_max[i]),variable_name_all[i]);
   h_all2->SetLineColor(2);
   h_all2->SetMarkerColor(2);
   h_all2->SetLineWidth(2);
   h_all2->GetYaxis()->SetRangeUser(hist_Yrange_min[i], hist_Yrange_max[i]);

   auto h_all3 = d3_select.Histo1D(TH1D(hist_name_all[i]+Form("_3"),hist_title_all[i]+Form("_3, %d-%d",cent_min,cent_max)+"%",hist_Nbins[i],hist_Xrange_min[i],hist_Xrange_max[i]),variable_name_all[i]);
   h_all3->SetLineColor(4);
   h_all3->SetMarkerColor(4);
   h_all3->SetLineWidth(2);
   h_all3->GetYaxis()->SetRangeUser(hist_Yrange_min[i], hist_Yrange_max[i]);

   //normalized distributions... 
   tc.cd(1);
   gPad->SetLogy(); //use log scale in y-axis
   gPad->SetTickx(1); 
   gPad->SetTicky(1);
   h_all1->Scale(1.0/h_all1->Integral());
   h_all1->Draw();
   h_all2->Scale(1.0/h_all2->Integral());
   h_all2->Draw("same");
   h_all3->Scale(1.0/h_all3->Integral());
   h_all3->Draw("same");
   TLegend *leg_norm = new TLegend(0.6,0.68,0.85,0.9); //Create a legend for the plots
   legendStyle(leg_norm, h_all1.GetPtr(), h_all2.GetPtr(), h_all3.GetPtr());  //call function to customize style of TCanvas
   leg_norm->Draw();
   gPad->Update();     

   //ratio plots
   tc.cd(2);
   gPad->SetTickx(1);
   gPad->SetTicky(1);
   TH1D *h_ratio12 = (TH1D*)h_all1->Clone(hist_name_all[i]+"_ratio12");
   h_ratio12->Divide(h_all2.GetPtr());
   h_ratio12->Draw();
   h_ratio12->SetLineColor(2);
   h_ratio12->SetMarkerColor(2);
   h_ratio12->SetLineWidth(2);
   h_ratio12->GetYaxis()->SetTitle("Ratio (Sample1/Samples2and3)");
   h_ratio12->GetYaxis()->SetTitleOffset(1.4);
   h_ratio12->GetXaxis()->SetTitle(hist_XaxisTitle[i]);
   h_ratio12->GetYaxis()->SetRangeUser(.0,10.);
   TH1D *h_ratio13 = (TH1D*)h_all1->Clone(hist_name_all[i]+"_ratio13");
   h_ratio13->Divide(h_all3.GetPtr());
   h_ratio13->Draw("same");
   h_ratio13->SetLineColor(4);
   h_ratio13->SetMarkerColor(4);
   h_ratio13->SetLineWidth(2);
   gPad->Update();

   tc.Print("ControlPlotsThreeSamples/"+fig_name[i]); //save Canvas in a PDF file...we can change format if needed PNG, JPG, ..etc...

}

}
