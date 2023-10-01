///Please, run it by doing: <<root -l -b -q macro_control_plots_signal_vs_fake_MCrecodebugOnly.C>>
///Usually, depending on the number of events you processed & selections, will be needed to adjust y-axis range
///Run in a MC Recodebug-only tree produced by: "HITrackingStudies/HITrackingStudies/test/run_PbPb_cfg.py"
///Please, change name of input file below variable "fileName"

///*** Important parameters
//input file
auto fileName = "trk_MC_RecoDebug_general_testPR.root";
//General or pixel tracks
bool runOnPixelTrks = false; //pixel tracks do not have HP flag set
//pT cuts
float pT_min = 0.0;
float pT_max = 99999.9;
//Tracking iteration
bool selecAlgo = false; //if true will do all plots for a specific iteration (except N-1 plots), defined in the variable algoToPlot below. IMPORTANT: Please, use false for pixel tracks
int algoToPlot = 4;
//select centrality range (in 0 - 100% range) that want to see the control plots
int cent_min = 0;
int cent_max = 100;

///Auxiliar functions

//Function to select high-purity matched, unmatched and all tracks & saving standard variables
std::vector<float> func_select_trks( ROOT::VecOps::RVec<Float_t> vec_ori, ROOT::VecOps::RVec<Bool_t> vec_trk_fake, ROOT::VecOps::RVec<Bool_t> vec_trk_hp, ROOT::VecOps::RVec<Int_t> vec_trk_algo, ROOT::VecOps::RVec<Float_t> vec_trk_pt, Int_t option ){
   std::vector<float> v;
   for (int i = 0; i < vec_ori.size(); i++){
      if(vec_trk_pt[i]<pT_min || vec_trk_pt[i]>pT_max) continue;	   
      if (option == 1){	   
	 if(selecAlgo){     
            if ( !vec_trk_fake[i] && vec_trk_hp[i] && vec_trk_algo[i]==algoToPlot ) v.push_back(vec_ori[i]);
	 }else{
	    if(runOnPixelTrks){
	       if ( !vec_trk_fake[i] ) v.push_back(vec_ori[i]);	    
	    }else{  	    
               if ( !vec_trk_fake[i] && vec_trk_hp[i] ) v.push_back(vec_ori[i]);		 
	    }
         }
      }
      else if (option == 2){
	 if(selecAlgo){     
            if ( vec_trk_fake[i] && vec_trk_hp[i] && vec_trk_algo[i]==algoToPlot ) v.push_back(vec_ori[i]); 
	 }else{
            if(runOnPixelTrks){
	       if ( vec_trk_fake[i] ) v.push_back(vec_ori[i]);	    
            }else{ 		    
               if ( vec_trk_fake[i] && vec_trk_hp[i] ) v.push_back(vec_ori[i]);		 
	    }
         }
      }
      else{
	 if(selecAlgo){     
           if ( vec_trk_hp[i] && vec_trk_algo[i]==algoToPlot ) v.push_back(vec_ori[i]); 
	 }else{
	   if(runOnPixelTrks){
	      v.push_back(vec_ori[i]); 	   
	   }else{ 	   
              if ( vec_trk_hp[i] ) v.push_back(vec_ori[i]);		 
	   }
         }		 
      } 
   }
   return v;
}

//Function to select high-purity matched, unmatched and all tracks & saving DCA significance or pT resolution
std::vector<float> func_ratio_select_trks( ROOT::VecOps::RVec<Float_t> vec_num, ROOT::VecOps::RVec<Float_t> vec_den, ROOT::VecOps::RVec<Bool_t> vec_trk_fake, ROOT::VecOps::RVec<Bool_t> vec_trk_hp, ROOT::VecOps::RVec<Int_t> vec_trk_algo, ROOT::VecOps::RVec<Float_t> vec_trk_pt, Int_t option ){
   std::vector<float> v;
   for (int i = 0; i < vec_num.size(); i++){
      if(vec_trk_pt[i]<pT_min || vec_trk_pt[i]>pT_max) continue;	   
      if (option == 1){
	 if(selecAlgo){     
            if ( !vec_trk_fake[i] && vec_trk_hp[i] && vec_trk_algo[i]==algoToPlot ) v.push_back(vec_num[i]/vec_den[i]);
	 }else{
	    if(runOnPixelTrks){
               if ( !vec_trk_fake[i] ) v.push_back(vec_num[i]/vec_den[i]);
            }else{
               if ( !vec_trk_fake[i] && vec_trk_hp[i] ) v.push_back(vec_num[i]/vec_den[i]);
            }
         }
      }
      else if (option == 2){
	 if(selecAlgo){     
            if ( vec_trk_fake[i] && vec_trk_hp[i] && vec_trk_algo[i]==algoToPlot ) v.push_back(vec_num[i]/vec_den[i]);
	 }else{
	    if(runOnPixelTrks){
               if ( vec_trk_fake[i] ) v.push_back(vec_num[i]/vec_den[i]);
            }else{
               if ( vec_trk_fake[i] && vec_trk_hp[i] ) v.push_back(vec_num[i]/vec_den[i]);
            }
         }
      }
      else{
	 if(selecAlgo){     
            if ( vec_trk_hp[i] && vec_trk_algo[i]==algoToPlot ) v.push_back(vec_num[i]/vec_den[i]);
	 }else{
	    if(runOnPixelTrks){
               v.push_back(vec_num[i]/vec_den[i]);
            }else{
               if ( vec_trk_hp[i] ) v.push_back(vec_num[i]/vec_den[i]);
            }
         }
      }
   }
   return v;
}

//Function to select high-purity matched, unmatched and all tracks & saving chi2/ndf/nlayers
std::vector<float> func_chi2_select_trks( ROOT::VecOps::RVec<Float_t> vec_chi2, ROOT::VecOps::RVec<Float_t> vec_ndf,ROOT::VecOps::RVec<Float_t> vec_nlayers, ROOT::VecOps::RVec<Bool_t> vec_trk_fake, ROOT::VecOps::RVec<Bool_t> vec_trk_hp, ROOT::VecOps::RVec<Int_t> vec_trk_algo, ROOT::VecOps::RVec<Float_t> vec_trk_pt, Int_t option ){
   std::vector<float> v;
   for (int i = 0; i < vec_chi2.size(); i++){
      if(vec_trk_pt[i]<pT_min || vec_trk_pt[i]>pT_max) continue;	   
      if (option == 1){
	 if(selecAlgo){     
            if ( !vec_trk_fake[i] && vec_trk_hp[i] && vec_trk_algo[i]==algoToPlot ) v.push_back(vec_chi2[i]/vec_ndf[i]/vec_nlayers[i]);
	 }else{
	    if(runOnPixelTrks){
               if ( !vec_trk_fake[i] ) v.push_back(vec_chi2[i]/vec_ndf[i]/vec_nlayers[i]);
            }else{
               if ( !vec_trk_fake[i] && vec_trk_hp[i] ) v.push_back(vec_chi2[i]/vec_ndf[i]/vec_nlayers[i]);
            }
	 }
      }
      else if (option == 2){
	 if(selecAlgo){     
            if ( vec_trk_fake[i] && vec_trk_hp[i] && vec_trk_algo[i]==algoToPlot ) v.push_back(vec_chi2[i]/vec_ndf[i]/vec_nlayers[i]);
	 }else{
	    if(runOnPixelTrks){
               if ( vec_trk_fake[i] ) v.push_back(vec_chi2[i]/vec_ndf[i]/vec_nlayers[i]);
            }else{
               if ( vec_trk_fake[i] && vec_trk_hp[i] ) v.push_back(vec_chi2[i]/vec_ndf[i]/vec_nlayers[i]);
            }
         }
      }
      else{
	 if(selecAlgo){     
            if ( vec_trk_hp[i] && vec_trk_algo[i]==algoToPlot ) v.push_back(vec_chi2[i]/vec_ndf[i]/vec_nlayers[i]);
	 }else{
	    if(runOnPixelTrks){
               v.push_back(vec_chi2[i]/vec_ndf[i]/vec_nlayers[i]);
            }else{
               if ( vec_trk_hp[i] ) v.push_back(vec_chi2[i]/vec_ndf[i]/vec_nlayers[i]);
            }
	 }
      }
   }
   return v;
}

//Function to select with ``Full Selection Minus 1Variable'' matched, unmatched and all tracks
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
void legendStyle(TLegend* tl, TH1D* h_all, TH1D* h_unmatch, TH1D* h_match){
   tl->AddEntry(h_all,"All","l");	
   tl->AddEntry(h_unmatch,"Fake","l");
   tl->AddEntry(h_match,"Signal","l");
   tl->SetFillStyle(0);
   tl->SetBorderSize(0);
   tl->SetTextSize(0.045);
   tl->SetTextFont(42);
}


void macro_control_plots_signal_vs_fake_MCrecodebugOnly(){

std::string cent_cut = "centrality>="+to_string(2*cent_min)+" && centrality<="+to_string(2*cent_max);

auto treeName = "anaTrack/trackTree"; //tree name

// We read the tree from the file and create a RDataFrame object called "d"
ROOT::RDataFrame d(treeName, fileName);

auto d_select = d.Filter(cent_cut,"Centrality cut") //filter events only in this centrality range --- see definition above
	         .Define("trkPt_sig","func_select_trks(trkPt.trkPt,trkFake.trkFake,highPurity.highPurity,trkAlgo.trkAlgo,trkPt.trkPt,1)") //pt
	         .Define("trkPt_fak","func_select_trks(trkPt.trkPt,trkFake.trkFake,highPurity.highPurity,trkAlgo.trkAlgo,trkPt.trkPt,2)") //pt
		 .Define("trkPt_all","func_select_trks(trkPt.trkPt,trkFake.trkFake,highPurity.highPurity,trkAlgo.trkAlgo,trkPt.trkPt,3)") //pt 
                 .Define("trkEta_sig","func_select_trks(trkEta.trkEta,trkFake.trkFake,highPurity.highPurity,trkAlgo.trkAlgo,trkPt.trkPt,1)") //eta
                 .Define("trkEta_fak","func_select_trks(trkEta.trkEta,trkFake.trkFake,highPurity.highPurity,trkAlgo.trkAlgo,trkPt.trkPt,2)") //eta
                 .Define("trkEta_all","func_select_trks(trkEta.trkEta,trkFake.trkFake,highPurity.highPurity,trkAlgo.trkAlgo,trkPt.trkPt,3)") //eta 
                 .Define("trkPhi_sig","func_select_trks(trkPhi.trkPhi,trkFake.trkFake,highPurity.highPurity,trkAlgo.trkAlgo,trkPt.trkPt,1)") //phi
                 .Define("trkPhi_fak","func_select_trks(trkPhi.trkPhi,trkFake.trkFake,highPurity.highPurity,trkAlgo.trkAlgo,trkPt.trkPt,2)") //phi
                 .Define("trkPhi_all","func_select_trks(trkPhi.trkPhi,trkFake.trkFake,highPurity.highPurity,trkAlgo.trkAlgo,trkPt.trkPt,3)") //phi 
		 .Define("trkNHit_sig","func_select_trks(trkNHit.trkNHit,trkFake.trkFake,highPurity.highPurity,trkAlgo.trkAlgo,trkPt.trkPt,1)") //nhits
                 .Define("trkNHit_fak","func_select_trks(trkNHit.trkNHit,trkFake.trkFake,highPurity.highPurity,trkAlgo.trkAlgo,trkPt.trkPt,2)") //nhits
                 .Define("trkNHit_all","func_select_trks(trkNHit.trkNHit,trkFake.trkFake,highPurity.highPurity,trkAlgo.trkAlgo,trkPt.trkPt,3)") //nhits
		 .Define("trkDzSig_sig","func_ratio_select_trks(trkDz1.trkDz1,trkDzError1.trkDzError1,trkFake.trkFake,highPurity.highPurity,trkAlgo.trkAlgo,trkPt.trkPt,1)") //dzSig
                 .Define("trkDzSig_fak","func_ratio_select_trks(trkDz1.trkDz1,trkDzError1.trkDzError1,trkFake.trkFake,highPurity.highPurity,trkAlgo.trkAlgo,trkPt.trkPt,2)") //dzSig
                 .Define("trkDzSig_all","func_ratio_select_trks(trkDz1.trkDz1,trkDzError1.trkDzError1,trkFake.trkFake,highPurity.highPurity,trkAlgo.trkAlgo,trkPt.trkPt,3)") //dzSig
	         .Define("trkDxySig_sig","func_ratio_select_trks(trkDxy1.trkDxy1,trkDxyError1.trkDxyError1,trkFake.trkFake,highPurity.highPurity,trkAlgo.trkAlgo,trkPt.trkPt,1)") //dxySig
                 .Define("trkDxySig_fak","func_ratio_select_trks(trkDxy1.trkDxy1,trkDxyError1.trkDxyError1,trkFake.trkFake,highPurity.highPurity,trkAlgo.trkAlgo,trkPt.trkPt,2)") //dxySig
                 .Define("trkDxySig_all","func_ratio_select_trks(trkDxy1.trkDxy1,trkDxyError1.trkDxyError1,trkFake.trkFake,highPurity.highPurity,trkAlgo.trkAlgo,trkPt.trkPt,3)") //dxySig
		 .Define("trkPtRes_sig","func_ratio_select_trks(trkPtError.trkPtError,trkPt.trkPt,trkFake.trkFake,highPurity.highPurity,trkAlgo.trkAlgo,trkPt.trkPt,1)") //ptRes
                 .Define("trkPtRes_fak","func_ratio_select_trks(trkPtError.trkPtError,trkPt.trkPt,trkFake.trkFake,highPurity.highPurity,trkAlgo.trkAlgo,trkPt.trkPt,2)") //ptRes
                 .Define("trkPtRes_all","func_ratio_select_trks(trkPtError.trkPtError,trkPt.trkPt,trkFake.trkFake,highPurity.highPurity,trkAlgo.trkAlgo,trkPt.trkPt,3)") //ptRes
		 .Define("trkChi2_sig","func_chi2_select_trks(trkChi2.trkChi2,trkNdof.trkNdof,trkNlayer.trkNlayer,trkFake.trkFake,highPurity.highPurity,trkAlgo.trkAlgo,trkPt.trkPt,1)") //ptRes
                 .Define("trkChi2_fak","func_chi2_select_trks(trkChi2.trkChi2,trkNdof.trkNdof,trkNlayer.trkNlayer,trkFake.trkFake,highPurity.highPurity,trkAlgo.trkAlgo,trkPt.trkPt,2)") //ptRes
                 .Define("trkChi2_all","func_chi2_select_trks(trkChi2.trkChi2,trkNdof.trkNdof,trkNlayer.trkNlayer,trkFake.trkFake,highPurity.highPurity,trkAlgo.trkAlgo,trkPt.trkPt,3)")//ptRes
		 .Define("trkAlgo_sig","func_select_trks(trkAlgo.trkAlgo,trkFake.trkFake,highPurity.highPurity,trkAlgo.trkAlgo,trkPt.trkPt,1)") //algo
                 .Define("trkAlgo_fak","func_select_trks(trkAlgo.trkAlgo,trkFake.trkFake,highPurity.highPurity,trkAlgo.trkAlgo,trkPt.trkPt,2)") //algo
                 .Define("trkAlgo_all","func_select_trks(trkAlgo.trkAlgo,trkFake.trkFake,highPurity.highPurity,trkAlgo.trkAlgo,trkPt.trkPt,3)")//algo
		 .Define("trkNHit_NminusOne_sig","func_NminusOneVar_select_trks(trkNHit_sig,trkChi2_sig,trkDzSig_sig,trkDxySig_sig,trkPtRes_sig,true,false,false,false,false)") //nhits -- NminusOne
                 .Define("trkNHit_NminusOne_fak","func_NminusOneVar_select_trks(trkNHit_fak,trkChi2_fak,trkDzSig_fak,trkDxySig_fak,trkPtRes_fak,true,false,false,false,false)") //nhits
                 .Define("trkNHit_NminusOne_all","func_NminusOneVar_select_trks(trkNHit_all,trkChi2_all,trkDzSig_all,trkDxySig_all,trkPtRes_all,true,false,false,false,false)") //nhits
		 .Define("trkChi2_NminusOne_sig","func_NminusOneVar_select_trks(trkNHit_sig,trkChi2_sig,trkDzSig_sig,trkDxySig_sig,trkPtRes_sig,false,true,false,false,false)") //chi2 -- NminusOne
                 .Define("trkChi2_NminusOne_fak","func_NminusOneVar_select_trks(trkNHit_fak,trkChi2_fak,trkDzSig_fak,trkDxySig_fak,trkPtRes_fak,false,true,false,false,false)") //chi2
                 .Define("trkChi2_NminusOne_all","func_NminusOneVar_select_trks(trkNHit_all,trkChi2_all,trkDzSig_all,trkDxySig_all,trkPtRes_all,false,true,false,false,false)") //chi2
                  .Define("trkDzSig_NminusOne_sig","func_NminusOneVar_select_trks(trkNHit_sig,trkChi2_sig,trkDzSig_sig,trkDxySig_sig,trkPtRes_sig,false,false,true,false,false)") //dzSig -- NminusOne
                 .Define("trkDzSig_NminusOne_fak","func_NminusOneVar_select_trks(trkNHit_fak,trkChi2_fak,trkDzSig_fak,trkDxySig_fak,trkPtRes_fak,false,false,true,false,false)") //dzSig
                 .Define("trkDzSig_NminusOne_all","func_NminusOneVar_select_trks(trkNHit_all,trkChi2_all,trkDzSig_all,trkDxySig_all,trkPtRes_all,false,false,true,false,false)") //dzSig
                  .Define("trkDxySig_NminusOne_sig","func_NminusOneVar_select_trks(trkNHit_sig,trkChi2_sig,trkDzSig_sig,trkDxySig_sig,trkPtRes_sig,false,false,false,true,false)") //dxySig -- NminusOne
                 .Define("trkDxySig_NminusOne_fak","func_NminusOneVar_select_trks(trkNHit_fak,trkChi2_fak,trkDzSig_fak,trkDxySig_fak,trkPtRes_fak,false,false,false,true,false)") //dxySig
                 .Define("trkDxySig_NminusOne_all","func_NminusOneVar_select_trks(trkNHit_all,trkChi2_all,trkDzSig_all,trkDxySig_all,trkPtRes_all,false,false,false,true,false)") //dxySig
                 .Define("trkPtRes_NminusOne_sig","func_NminusOneVar_select_trks(trkNHit_sig,trkChi2_sig,trkDzSig_sig,trkDxySig_sig,trkPtRes_sig,false,false,false,false,true)") //ptRes -- NminusOne
                 .Define("trkPtRes_NminusOne_fak","func_NminusOneVar_select_trks(trkNHit_fak,trkChi2_fak,trkDzSig_fak,trkDxySig_fak,trkPtRes_fak,false,false,false,false,true)") //ptRes
                 .Define("trkPtRes_NminusOne_all","func_NminusOneVar_select_trks(trkNHit_all,trkChi2_all,trkDzSig_all,trkDxySig_all,trkPtRes_all,false,false,false,false,true)"); //ptRes

///inputs for the loop to plot histigrams
const unsigned int N_variables = 14; //Sequence: pt, eta, phi, nhits, dzSig, dxySig, ptRes, chi2, algo, nhits_nMinusOne, dzSig_nMinusOne, dxySig_nMinusOne, chi2_nMinusOne, ptRes_nMinusOne...
std::string variable_name_all[N_variables]={"trkPt_all","trkEta_all","trkPhi_all","trkNHit_all","trkDzSig_all","trkDxySig_all","trkPtRes_all","trkChi2_all","trkAlgo_all","trkNHit_NminusOne_all","trkDzSig_NminusOne_all","trkDxySig_NminusOne_all","trkPtRes_NminusOne_all","trkChi2_NminusOne_all"}; //variables' name
std::string variable_name_sig[N_variables]={"trkPt_sig","trkEta_sig","trkPhi_sig","trkNHit_sig","trkDzSig_sig","trkDxySig_sig","trkPtRes_sig","trkChi2_sig","trkAlgo_sig","trkNHit_NminusOne_sig","trkDzSig_NminusOne_sig","trkDxySig_NminusOne_sig","trkPtRes_NminusOne_sig","trkChi2_NminusOne_sig"};
std::string variable_name_fak[N_variables]={"trkPt_fak","trkEta_fak","trkPhi_fak","trkNHit_fak","trkDzSig_fak","trkDxySig_fak","trkPtRes_fak","trkChi2_fak","trkAlgo_fak","trkNHit_NminusOne_fak","trkDzSig_NminusOne_fak","trkDxySig_NminusOne_fak","trkPtRes_NminusOne_fak","trkChi2_NminusOne_fak"};
TString hist_name_all[N_variables]={"trkPt","trkEta","trkPhi","trkNHit","trkDzSig","trkDxySig","trkPtRes","trkChi2","trkAlgo","trkNHit_NminusOne","trkDzSig_NminusOne","trkDxySig_NminusOne","trkPtRes_NminusOne","trkChi2_NminusOne"}; //histograms' name
TString hist_name_sig[N_variables]={"trkPt_sig","trkEta_sig","trkPhi_sig","trkNHit_sig","trkDzSig_sig","trkDxySig_sig","trkPtRes_sig","trkChi2_sig","trkAlgo_sig","trkNHit_NminusOne_sig","trkDzSig_NminusOne_sig","trkDxySig_NminusOne_sig","trkPtRes_NminusOne_sig","trkChi2_NminusOne_sig"};
TString hist_name_fak[N_variables]={"trkPt_fak","trkEta_fak","trkPhi_fak","trkNHit_fak","trkDzSig_fak","trkDxySig_fak","trkPtRes_fak","trkChi2_fak","trkAlgo_fak","trkNHit_NminusOne_fak","trkDzSig_NminusOne_fak","trkDxySig_NminusOne_fak","trkPtRes_NminusOne_fak","trkChi2_NminusOne_fak"};
TString hist_title_all[N_variables]={"p_{T} of HP tracks","#eta of HP tracks","#phi of HP tracks", "NHits of HP tracks", "DzSig of HP tracks", "DxySig of HP tracks", "Pt Resolution of HP tracks","#chi^{2}/ndf/nlayers of HP tracks","Algo of HP tracks","NHits with All Other Cuts","DzSig with All Other Cuts", "DxySig with All Other Cuts","Pt Resolution with All Other Cuts","#chi^{2}/ndf/nlayers with All Other Cuts"}; //histograms' title
TString hist_title_sig[N_variables]={"p_{T} of HP matched tracks", "#eta of HP matched tracks","#phi of HP matched tracks", "NHits of HP matched tracks","DzSig of HP matched tracks", "DxySig of HP matched tracks", "Pt Resolution of HP matched tracks","#chi^{2}/ndf/nlayers of HP matched tracks","Algo of HP matched tracks", "NHits with All Other Cuts matched","DzSig with All Other Cuts matched", "DxySig with All Other Cuts matched","Pt Resolution with All Other Cuts matched","#chi^{2}/ndf/nlayers with All Other Cuts matched"};
TString hist_title_fak[N_variables]={"p_{T} of HP unmatched tracks", "#eta of HP unmatched tracks","#phi of HP unmatched tracks", "NHits of HP unmatched tracks","DzSig of HP unmatched tracks", "DxySig of HP unmatched tracks", "Pt Resolution of HP unmatched tracks", "#chi^{2}/ndf/nlayers of HP unmatched tracks","Algo of HP unmatched tracks","NHits with All Other Cuts unmatched","DzSig with All Other Cuts unmatched", "DxySig with All Other Cuts unmatched","Pt Resolution with All Other Cuts unmatched","#chi^{2}/ndf/nlayers with All Other Cuts unmatched"};
const float hist_Nbins[N_variables]= {100,100,100,50,100,100,100,100,30,50,100,100,100,100}; //Number of bins 
const float hist_Xrange_min[N_variables]= {0,-3.5,-4.5,0,-30,-30,0,0,0,0,-30,-30,0,0}; //Minimum value of the x-axis range
const float hist_Xrange_max[N_variables]= {10,3.5,4.5,50,30,30,0.3,1,30,50,30,30,0.3,1}; //Maximum value of the x-axis range
TString hist_XaxisTitle[N_variables]={"p_{T} (GeV/c)","#eta","#phi","Number of Hits", "dZ/#sigma_{dZ}", "dXY/#sigma_{dXY}","p_{T}Err/p_{T}","#chi^{2}/ndf/nlayers","Algo","Number of Hits","dZ/#sigma_{dZ}", "dXY/#sigma_{dXY}","p_{T}Err/p_{T}","#chi^{2}/ndf/nlayers"}; //Title of x-axis
const float hist_Yrange_min[N_variables]={0.9,0.9,0.9,0.9,0.9,0.9,0.9,0.9,0.9,0.9,0.9,0.9,0.9,0.9}; //Minimum of the y-axis range
const float hist_Yrange_max[N_variables]={10000000,10000000,10000000,10000000,10000000,10000000,10000000,10000000,10000000,10000000,10000000,10000000,10000000,10000000}; //Maximum of the y-axis
const float histNorm_Yrange_min[N_variables]={0.00001,0.00001,0.00001,0.00001,0.00001,0.00001,0.00001,0.00001,0.00001,0.00001,0.00001,0.00001,0.00001,0.00001}; //Minimum of the y-axis range
const float histNorm_Yrange_max[N_variables]={0.15,0.03,0.03,0.30,0.30,0.30,0.30,0.12,0.30,0.30,0.30,0.30,0.30,0.15}; //Maximum of the y-axis range 
TString fig_name[N_variables]={"hist_pt_sig_vs_fak_NoSel.pdf","hist_eta_sig_vs_fak_NoSel.pdf","hist_phi_sig_vs_fak_NoSel.pdf","hist_nhits_sig_vs_fak_NoSel.pdf","hist_dZsig_sig_vs_fak_NoSel.pdf","hist_dXYsig_sig_vs_fak_NoSel.pdf","hist_pTres_sig_vs_fak_NoSel.pdf","hist_chi2_sig_vs_fak_NoSel.pdf","hist_algo_sig_vs_fak_NoSel.pdf","hist_nhits_sig_vs_fak_NimusOneCuts.pdf","hist_dZsig_sig_vs_fak_NimusOneCuts.pdf","hist_dXYsig_sig_vs_fak_NimusOneCuts.pdf","hist_pTres_sig_vs_fak_NimusOneCuts.pdf","hist_chi2_sig_vs_fak_NimusOneCuts.pdf"}; //Name of the figure file with the histograms saved
auto yaxis_label1 = "Number of Tracks / Bin"; //y-axis label integral histos
auto yaxis_label2 = "Normalized Distributions"; //y-axis label normalized histos

//Book histograms using RDataFrame, plot and save the figure in a PDF format

TCanvas tc("tc","tc",1000,500); //ROOT TCanvas to plot the variables - it will be updated on-the-fly after saving the PDF with the plot
tc.Divide(2,1,0.02,0.02);

gSystem->Exec("mkdir -p ControlPlotsMCrecodebug"); //Directory to save the histograms

for (unsigned int i=0; i<N_variables; i++){

   //Book histograms		
   auto h_all = d_select.Histo1D(TH1D(hist_name_all[i],hist_title_all[i]+Form(", %d-%d",cent_min,cent_max)+"%",hist_Nbins[i],hist_Xrange_min[i],hist_Xrange_max[i]),variable_name_all[i]);
   h_all->GetYaxis()->SetTitle(yaxis_label1);
   h_all->GetYaxis()->SetTitleOffset(1.4);
   h_all->GetXaxis()->SetTitle(hist_XaxisTitle[i]);
   h_all->SetLineColor(1);
   h_all->SetMarkerColor(1);
   h_all->SetLineWidth(2);
   h_all->GetYaxis()->SetRangeUser(hist_Yrange_min[i], hist_Yrange_max[i]);

   auto h_sig = d_select.Histo1D(TH1D(hist_name_sig[i],hist_title_sig[i],hist_Nbins[i],hist_Xrange_min[i],hist_Xrange_max[i]),variable_name_sig[i]);
   h_sig->GetYaxis()->SetTitle(yaxis_label1);
   h_sig->GetXaxis()->SetTitle(hist_XaxisTitle[i]);
   h_sig->SetLineColor(4);
   h_sig->SetMarkerColor(4);
   h_sig->SetLineWidth(2);

   auto h_fak = d_select.Histo1D(TH1D(hist_name_fak[i],hist_title_fak[i],hist_Nbins[i],hist_Xrange_min[i],hist_Xrange_max[i]),variable_name_fak[i]);
   h_fak->GetYaxis()->SetTitle(yaxis_label1);
   h_fak->GetXaxis()->SetTitle(hist_XaxisTitle[i]);
   h_fak->SetLineColor(2);
   h_fak->SetMarkerColor(2);
   h_fak->SetLineStyle(2);
   h_fak->SetLineWidth(2);

   //Draw and save histograms - use two situations: 
   // 1) With all entries in y-axis (better to see total number of tracks in each case); 
   // 2) Normalized in y-axis by its integral (better to visualize the shape of the distributions) 

   //First, case "1)"...
   tc.cd(1);
   gPad->SetLogy(); //use log scale in y-axis
   gPad->SetTickx(1); 
   gPad->SetTicky(1);
   h_all->Draw();
   gPad->Update();     
   TPaveStats *st_all = (TPaveStats*)h_all->FindObject("stats"); //To adjust the position of the Statistics box
   st_all->SetY1NDC(0.75);
   st_all->SetY2NDC(0.90);
   st_all->SetX1NDC(0.80);
   st_all->SetX2NDC(1.00);
   h_fak->Draw("sameS"); //draw in the same canvas and with statistcs box
   gPad->Update();
   TPaveStats *st_fak = (TPaveStats*)h_fak->FindObject("stats"); //To adjust the position of the Statistics box
   st_fak->SetY1NDC(0.60);
   st_fak->SetY2NDC(0.75);
   st_fak->SetX1NDC(0.80);
   st_fak->SetX2NDC(1.00);
   h_sig->Draw("sameS");  //draw in the same canvas and with statistcs box
   gPad->Update();
   TPaveStats *st_sig = (TPaveStats*)h_sig->FindObject("stats"); //To adjust the position of the Statistics box
   st_sig->SetY1NDC(0.45);
   st_sig->SetY2NDC(0.60);
   st_sig->SetX1NDC(0.80);
   st_sig->SetX2NDC(1.00);
   TLegend *leg = new TLegend(0.6,0.68,0.85,0.9); //Create a legend for the plots
   legendStyle(leg, h_all.GetPtr(), h_fak.GetPtr(), h_sig.GetPtr()); //call function to customize style of TCanvas
   leg->Draw();
   //...and then, case "2)"
   tc.cd(2);
   gPad->SetLogy(); //use log scale in y-axis
   gPad->SetTickx(1);
   gPad->SetTicky(1);
   TH1D *h_clone_all = (TH1D *)h_all->Clone("h_clone_all"); //clone the histogram "h_all" in order to not messup the original one
   h_clone_all->GetYaxis()->SetTitle(yaxis_label2);
   h_clone_all->SetStats(0);
   h_clone_all->GetYaxis()->SetTitleOffset(1.5);
   h_clone_all->Scale(1./(h_clone_all->Integral())); //Normalize it by multiplying the distribution by 1./integral
   h_clone_all->Draw("h");
   h_clone_all->GetYaxis()->SetRangeUser(histNorm_Yrange_min[i], histNorm_Yrange_max[i]);
   TH1D *h_clone_fak = (TH1D *)h_fak->Clone("h_clone_fak"); //clone the histogram "h_fak" in order to not messup the original one
   h_clone_fak->Scale(1./(h_clone_fak->Integral())); //Normalize it by multiplying the distribution by 1./integral
   h_clone_fak->SetStats(0);
   h_clone_fak->Draw("hsame");
   TH1D *h_clone_sig = (TH1D *)h_sig->Clone("h_clone_sig"); //clone the histogram "h_sig" in order to not messup the original one
   h_clone_sig->Scale(1./(h_clone_sig->Integral()));
   h_clone_sig->SetStats(0);
   h_clone_sig->Draw("hsame");
   TLegend *leg_norm = new TLegend(0.7,0.68,0.95,0.9); //Create a legend for the plots
   legendStyle(leg_norm, h_clone_all, h_clone_fak, h_clone_sig);  //call function to customize style of TCanvas
   leg_norm->Draw();
   tc.Print("ControlPlotsMCrecodebug/"+fig_name[i]); //save Canvas in a PDF file...we can change format if needed PNG, JPG, ..etc...

}

}
