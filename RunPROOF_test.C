///////////////////////////////////////////////////////////////////////
//
//    FILE: RunProof.C
// AUTHORS: I. Gonzalez Caballero, A.Y. Rodriguez Marrero
//    DATE: 2010
//
// CONTENT: Main macro to run over MiniTrees or TESCO Trees using PROOF
//          in PROOF-Lite, PROOF-Cluster or Sequential mode
///////////////////////////////////////////////////////////////////////

#include<iostream>
#include<string>

TProof* proof = 0;

/////////////////////////////////////////////////////////////
/////////////           TOP ANALYSIS            /////////////
/////////////////////////////////////////////////////////////

void RunPROOF_test(double luminosity,
		   const char* data, 
		   TString  theSample,  
		   Int_t JetChannel,
		   TString FlavorChannel) 
{
 
  // This loads all the PROOF Analysis Framework utilities
  gROOT->LoadMacro("$PAFPATH/PAF.C");
  
  //  double luminosity = 19468;
  TString dataInfo;
  TString mcInfo;

  Bool_t runAtOviedo = false;
  Bool_t runAtIfca   = !runAtOviedo;

  /////////////////////////////////////////////////////////////////////////
  //
  // PROOF SETTINGS
  // ==============
  //
  // (You may inspect scripts/PAFOptions.h to see all the posible settings)
  //
  // Edit the lines below to select tree type, input data sample, output
  // file name ...
  //
  
  ///////////////////////////////
  // PROOF MODE
  // Defines the mode in which you want to run PROOF. Read the documentation
  // for details:
  // * kSequential: No PROOF. Plain sequential code
  // * kLite: PROOF Lite mode
  // * kCluster: PROOF Cluter mode
  // * kPoD: PROOF on Demand mode
  gPAFOptions->proofMode = kLite;
  //
  // Optional parameters for PROOF Cluster (kCluster):
  //   + The number of slots you would like to use (default is 10)
  //gPAFOptions->NSlots = 10; 
  //   + Proof server and port (default are proof.ifca.es and 1093) 
   gPAFOptions->proofServer = "proof.ifca.es";
   gPAFOptions->proofServerPort = 1093;
  //   + Maximum number of slaves per node (default is 9999, i.e. all)
  // gPAFOptions->maxSlavesPerNode = 9999;
  //
  // Start PROOF
  //
  cout << ">> Starting PROOF..." << endl;
  proof = InitProof(); 
  if (!proof && gPAFOptions->proofMode != kSequential) {
    cerr << "ERROR: I could not initialise a PROOF session!" << endl;
    return;
  }
 

  ///////////////////////////////
  // TREE TYPE
  // Defines the data formats that may be used: MiniTrees (default) or TESCO
  gPAFOptions->SetTreeType(kMiniTrees);
  //gPAFOptions->treeType = kTESCO;


  //  gPAFOptions->SetTreeDir("demo");
  gPAFOptions->SetTreeName("latino");   

  ///////////////////////////////
  // INPUT DATA SAMPLE
  //
   //TString dataPath="/gpfs/csic_projects/cms/arodrig/"; //IFCA   (gridui)
  // 1) Set the base path to files
  // TString dataPath="/gpfs/csic_projects/cms/calderon/TreesCSA14"; //IFCA   (gridui)
  //  TString dataPath="/gpfs/csic_projects/tier3data/LatinosSkims/ReducedTrees/DiferentialXSection";
  // TString dataPath="/gpfs/csic_projects/tier3data"; //IFCA   (gridui)
   //        TString dataPath="/hadoop";                      //UniOvi (fanaeui)
  //   TString dataPath="/pool/data1/MiniTrees";        //CERN   (cmsovd02)

  TString filesPath = "/gpfs/csic_projects/tier3data/LatinosSkims/ReducedTrees/Systematics2013_nominals_fromMaiko/";
  
  // 2) Asign the files to gPAFOptions->dataFiles (a vector<TString>)
  // Ex. MiniTree...
   //  gPAFOptions->dataFiles.push_back(dataPath + "/Data/Data7TeVRun2010A/Tree_Mu_Run2010A_Sep27ReReco_Skim2LPt1010.root");
  //
  // Note: You may consider using DatasetManager for this. See the
  //       documentation in the wiki

  TString Signal = data;

   bool isdata = true;
    int nEventsInTheSample = 99555; 
    double xSection =  1.0 ;
    int whichRun = 2; 


  // *********** DATA

  if (Signal=="test") {

    if (theSample == "DataRun2012_Total"){    

   gPAFOptions->dataFiles.push_back("/gpfs/csic_projects/tier3data/LatinosSkims/ReducedTrees/R53X_S1_V08_S2_V09_S3_V13/Data_TightTight/4L/latino_RunA_892pbinv.root");
   gPAFOptions->dataFiles.push_back("/gpfs/csic_projects/tier3data/LatinosSkims/ReducedTrees/R53X_S1_V08_S2_V09_S3_V13/Data_TightTight/4L/latino_RunB_4404pbinv.root");
   gPAFOptions->dataFiles.push_back("/gpfs/csic_projects/tier3data/LatinosSkims/ReducedTrees/R53X_S1_V08_S2_V09_S3_V13/Data_TightTight/4L/latino_RunC_7032pbinv.root");
   gPAFOptions->dataFiles.push_back("/gpfs/csic_projects/tier3data/LatinosSkims/ReducedTrees/R53X_S1_V08_S2_V09_S3_V13/Data_TightTight/4L/latino_RunD_7274pbinv.root");
   
    }
    /*
   bool isdata = true;
    int nEventsInTheSample = 99555; 
    double xSection =  1.0 ;
    int whichRun = 2; 
  */
 
  else if (theSample == "WJetsFakes_Total_old") {

    gPAFOptions->dataFiles.push_back("/gpfs/csic_projects/tier3data/LatinosSkims/ReducedTrees/R53X_S1_V08_S2_V09_S3_V13/Moriond13/latino_LooseLoose_19.5fb.root");

  }

  else if (theSample == "WJetsFakes_Total") {

    // Using latest jetEt thresholds. 
    gPAFOptions->dataFiles.push_back("/gpfs/csic_projects/tier3data/LatinosSkims/ReducedTrees/R53X_S1_V08_S2_V09_S3_V13/WJet_LooseLoose/latino_RunA_892pbinv_LooseLoose.root");
    gPAFOptions->dataFiles.push_back("/gpfs/csic_projects/tier3data/LatinosSkims/ReducedTrees/R53X_S1_V08_S2_V09_S3_V13/WJet_LooseLoose/latino_RunB_4404pbinv_LooseLoose.root");
    gPAFOptions->dataFiles.push_back("/gpfs/csic_projects/tier3data/LatinosSkims/ReducedTrees/R53X_S1_V08_S2_V09_S3_V13/WJet_LooseLoose/latino_RunC_7032pbinv_LooseLoose.root");
    gPAFOptions->dataFiles.push_back("/gpfs/csic_projects/tier3data/LatinosSkims/ReducedTrees/R53X_S1_V08_S2_V09_S3_V13/WJet_LooseLoose/latino_RunD_7274pbinv_LooseLoose.root");

    gPAFOptions->dataFiles.push_back("/gpfs/csic_projects/tier3data/LatinosSkims/ReducedTrees/R53X_S1_V08_S2_V09_S3_V13/WJet_LooseLoose/latino_085_WgammaToLNuG.root");
    gPAFOptions->dataFiles.push_back("/gpfs/csic_projects/tier3data/LatinosSkims/ReducedTrees/R53X_S1_V08_S2_V09_S3_V13/WJet_LooseLoose/latino_082_WGstarToElNuMad.root");
    gPAFOptions->dataFiles.push_back("/gpfs/csic_projects/tier3data/LatinosSkims/ReducedTrees/R53X_S1_V08_S2_V09_S3_V13/WJet_LooseLoose/latino_083_WGstarToMuNuMad.root");
    gPAFOptions->dataFiles.push_back("/gpfs/csic_projects/tier3data/LatinosSkims/ReducedTrees/R53X_S1_V08_S2_V09_S3_V13/WJet_LooseLoose/latino_084_WGstarToTauNuMad.root");
    gPAFOptions->dataFiles.push_back("/gpfs/csic_projects/tier3data/LatinosSkims/ReducedTrees/R53X_S1_V08_S2_V09_S3_V13/WJet_LooseLoose/latino_086_ZgammaToLLuG.root");

  }

  else if (theSample == "ggWWto2L") {

    gPAFOptions->dataFiles.push_back("/gpfs/csic_projects/tier3data/LatinosSkims/ReducedTrees/Systematics2013_nominals_fromMaiko/latino_001_GluGluToWWTo4L.root");

  }

  else if (theSample == "WWTo2L2Nu") {

    gPAFOptions->dataFiles.push_back("/gpfs/csic_projects/tier3data/LatinosSkims/ReducedTrees/Systematics2013_nominals_fromMaiko/latino_000_WWJets2LMad.root");

  }

  else if (theSample == "WWTo2L2Nu_pow") {

    gPAFOptions->dataFiles.push_back("/gpfs/csic_projects/tier3data/LatinosSkims/ReducedTrees/Systematics2013_nominals_fromMaiko/latino_006_WWJets2LPowheg.root");

  }

  else if (theSample == "WWTo2L2Nu_nonSkim_pow") {

    gPAFOptions->dataFiles.push_back("/gpfs/csic_projects/tier3data/LatinosSkims/ReducedTrees/DiferentialXSection/NoSkim_puW_effW_triggW/latino006.root");
    
  }

 else if (theSample == "WWTo2L2Nu_mcnlo") {

   gPAFOptions->dataFiles.push_back("/gpfs/csic_projects/tier3data/LatinosSkims/ReducedTrees/Systematics2013_nominals_fromMaiko/latino_002_WWto2L2NuMCatNLO.root");
  }

 else if (theSample == "WWTo2L2Nu_mcnlo_nnll") {

   gPAFOptions->dataFiles.push_back("/gpfs/csic_projects/cms/calderon/WWGEN/nnllResummation/latino002_nll_ewk.root");

 }

 else if (theSample == "WWTo2L2Nu_pow_nnll") {

   gPAFOptions->dataFiles.push_back("/gpfs/csic_projects/cms/calderon/WWGEN/nnllResummation/latino006_nll_ewk.root");
   
 }

 else if (theSample == "WWTo2L2Nu_mad_nnll") {

   gPAFOptions->dataFiles.push_back("/gpfs/csic_projects/cms/calderon/WWGEN/nnllResummation/latino000_nll_ewk.root");
 }

 else if (theSample == "WZ") {

    gPAFOptions->dataFiles.push_back("/gpfs/csic_projects/tier3data/LatinosSkims/ReducedTrees/Systematics2013_nominals_fromMaiko/latino_074_WZJetsMad.root");
    gPAFOptions->dataFiles.push_back("/gpfs/csic_projects/tier3data/LatinosSkims/ReducedTrees/Systematics2013_nominals_fromMaiko/latino_078_WZTo2L2QMad.root");

  }

  else if (theSample == "ZZ") {

    gPAFOptions->dataFiles.push_back("/gpfs/csic_projects/tier3data/LatinosSkims/ReducedTrees/Systematics2013_nominals_fromMaiko/latino_075_ZZJetsMad.root");
    gPAFOptions->dataFiles.push_back("/gpfs/csic_projects/tier3data/LatinosSkims/ReducedTrees/Systematics2013_nominals_fromMaiko/latino_079_ZZTo2L2QMad.root"); 

  }

  else if (theSample == "TTbar") {
    gPAFOptions->dataFiles.push_back("/gpfs/csic_projects/tier3data/LatinosSkims/ReducedTrees/Systematics2013_nominals_fromMaiko/latino_019_TTTo2L2Nu2B.root"); 

  }

  else if (theSample == "TW") {

    gPAFOptions->dataFiles.push_back("/gpfs/csic_projects/tier3data/LatinosSkims/ReducedTrees/Systematics2013_nominals_fromMaiko/latino_011_TtWFullDR.root");
    gPAFOptions->dataFiles.push_back("/gpfs/csic_projects/tier3data/LatinosSkims/ReducedTrees/Systematics2013_nominals_fromMaiko/latino_012_TbartWFullDR.root");

  }

  else if (theSample == "Top") {

    gPAFOptions->dataFiles.push_back("/gpfs/csic_projects/tier3data/LatinosSkims/ReducedTrees/Systematics2013_nominals_fromMaiko/latino_019run_TTTo2L2Nu2B.root");
    gPAFOptions->dataFiles.push_back("/gpfs/csic_projects/tier3data/LatinosSkims/ReducedTrees/Systematics2013_nominals_fromMaiko/latino_011_TtWFullDR.root");
    gPAFOptions->dataFiles.push_back("/gpfs/csic_projects/tier3data/LatinosSkims/ReducedTrees/Systematics2013_nominals_fromMaiko/latino_012_TbartWFullDR.root");

  }

  else if (theSample == "DY") {

    gPAFOptions->dataFiles.push_back("/gpfs/csic_projects/tier3data/LatinosSkims/ReducedTrees/Systematics2013_nominals_fromMaiko/latino_036_DY10toLLMad.root");
    gPAFOptions->dataFiles.push_back("/gpfs/csic_projects/tier3data/LatinosSkims/ReducedTrees/Systematics2013_nominals_fromMaiko/latino_037_DY50toLLMad.root");

  }

  else if (theSample == "DYtautau") { // Only for OF channels!!! 

    gPAFOptions->dataFiles.push_back("/gpfs/csic_projects/tier3data/LatinosSkims/ReducedTrees/R53X_S1_V08_S2_V09_S3_V13/MoriondeffW/TauTau/latino_DYtt_19.5fb.root");
    
  }

  else if (theSample == "Zgamma") { 

    gPAFOptions->dataFiles.push_back("/gpfs/csic_projects/tier3data/LatinosSkims/ReducedTrees/Systematics2013_nominals_fromMaiko/latino_086_ZgammaToLLuG.root");

  }

  else if (theSample == "WgammaNoStar") {

        gPAFOptions->dataFiles.push_back("/gpfs/csic_projects/tier3data/LatinosSkims/ReducedTrees/Systematics2013_nominals_fromMaiko/latino_085_WgammaToLNuG.root");

  }

  else if (theSample == "WgammaStar") {

    gPAFOptions->dataFiles.push_back("/gpfs/csic_projects/tier3data/LatinosSkims/ReducedTrees/Systematics2013_nominals_fromMaiko/latino_082_WGstarToElNuMad.root");
    gPAFOptions->dataFiles.push_back("/gpfs/csic_projects/tier3data/LatinosSkims/ReducedTrees/Systematics2013_nominals_fromMaiko/latino_083_WGstarToMuNuMad.root");
    gPAFOptions->dataFiles.push_back("/gpfs/csic_projects/tier3data/LatinosSkims/ReducedTrees/Systematics2013_nominals_fromMaiko/latino_084_WGstarToTauNuMad.root");

  }

  else if (theSample == "HWW125") { 

    gPAFOptions->dataFiles.push_back("/gpfs/csic_projects/tier3data/LatinosSkims/ReducedTrees/Systematics2013_nominals_fromMaiko/latino_1125_ggToH125toWWTo2LAndTau2Nu.root");
    gPAFOptions->dataFiles.push_back("/gpfs/csic_projects/tier3data/LatinosSkims/ReducedTrees/Systematics2013_nominals_fromMaiko/latino_2125_vbfToH125toWWTo2LAndTau2Nu.root");
    gPAFOptions->dataFiles.push_back("/gpfs/csic_projects/tier3data/LatinosSkims/ReducedTrees/Systematics2013_nominals_fromMaiko/latino_3125_wzttH125ToWW.root");

  }

  else if (theSample =="GamGamWW"){

    gPAFOptions->dataFiles.push_back("/gpfs/csic_projects/tier3data/LatinosSkims/ReducedTrees/Systematics2013_nominals_fromMaiko/latino_008_GamGamWW.root");    

  }

  else if (theSample == "WJets") {

    gPAFOptions->dataFiles.push_back("/gpfs/csic_projects/tier3data/LatinosSkims/ReducedTrees/R53X_S1_V08_S2_V09_S3_V13/MC_LooseLoose/4L/latino_080_WJetsToLNuMad.root");

  }

  else if (theSample == "VVV") {

    gPAFOptions->dataFiles.push_back("/gpfs/csic_projects/tier3data/LatinosSkims/ReducedTrees/Systematics2013_nominals_fromMaiko/latino_088_WWGJets.root");
    gPAFOptions->dataFiles.push_back("/gpfs/csic_projects/tier3data/LatinosSkims/ReducedTrees/Systematics2013_nominals_fromMaiko/latino_089_WZZJets.root");
    gPAFOptions->dataFiles.push_back("/gpfs/csic_projects/tier3data/LatinosSkims/ReducedTrees/Systematics2013_nominals_fromMaiko/latino_090_ZZZJets.root");
    gPAFOptions->dataFiles.push_back("/gpfs/csic_projects/tier3data/LatinosSkims/ReducedTrees/Systematics2013_nominals_fromMaiko/latino_091_WWZJets.root");
    gPAFOptions->dataFiles.push_back("/gpfs/csic_projects/tier3data/LatinosSkims/ReducedTrees/Systematics2013_nominals_fromMaiko/latino_092_WWWJets.root");
    gPAFOptions->dataFiles.push_back("/gpfs/csic_projects/tier3data/LatinosSkims/ReducedTrees/Systematics2013_nominals_fromMaiko/latino_093_TTWJets.root");
    gPAFOptions->dataFiles.push_back("/gpfs/csic_projects/tier3data/LatinosSkims/ReducedTrees/Systematics2013_nominals_fromMaiko/latino_094_TTZJets.root");
    gPAFOptions->dataFiles.push_back("/gpfs/csic_projects/tier3data/LatinosSkims/ReducedTrees/Systematics2013_nominals_fromMaiko/latino_095_TTWWJets.root");
    gPAFOptions->dataFiles.push_back("/gpfs/csic_projects/tier3data/LatinosSkims/ReducedTrees/Systematics2013_nominals_fromMaiko/latino_096_TTGJets.root");

  }
  else {
 
   return;

  }



  }
  

  ///////////////////////////////
  // OUTPUT FILE NAME
  // Specify the name of the file where you want your histograms to be saved
  

  std::ostringstream out;
 
  TString outTest = out.str();

  //TString output = TString("csa14_GluGluToHToWWTo2LAndTau2Nu_M-125_13TeV_PUS14_Vertex.root"); 
  //TString output = TString("csa14_W1234JetsToLNu_Tune4C_13TeV_PUS14.root");
  //TString output = TString("csa14_WToMuNu_Tune4C_13TeV_PUS14.root");
  //TString output = TString("csa14_GluGluToHToWWTo2LAndTau2Nu_M-125_13TeV_PU20bx25.root");
  //TString output = TString("csa14_WJetsToLNu_13TeV-madgraph_PU20bx25.root"); 
  //TString output = TString("csa14_WJets_Madgraph_8Tev.root");
  //TString output = TString("csa14_HWW125_8Tev.root");

  TString jets = Form("%d",JetChannel);

  //  TString path = Form("rootfiles/%djet/%s/", JetChannel, FlavorChannel.Data());

  TString output = TString(theSample+".root");



  //TString output = TString(Signal+".root"); 

  gPAFOptions->outputFile=output;
  


  ///////////////////////////////
  // PARAMETERS FOR THE ANALYSIS
  // This parameters are passed to the analysis class and can be use there.
  // They are stored in a InputParameters object. They are saved to the 
  // output file.
  // See packages/InputParameters/InputParameters.h for information on how
  // to use this class.

  //  std::string sample = theSample;
 
  gPAFOptions->inputParameters = new InputParameters();

  gPAFOptions->inputParameters->SetNamedBool("IsDATA", isdata);
  gPAFOptions->inputParameters->SetNamedString("Signal", data);
  gPAFOptions->inputParameters->SetNamedDouble("XSection", xSection);
  gPAFOptions->inputParameters->SetNamedDouble("Luminosity", luminosity);
  gPAFOptions->inputParameters->SetNamedInt("NEvents", nEventsInTheSample); // all
  gPAFOptions->inputParameters->SetNamedFloat("luminosityPU", 19468.3);  
  gPAFOptions->inputParameters->SetNamedInt("WhichRun", whichRun);
  gPAFOptions->inputParameters->SetNamedString("theSample", theSample.Data());
  gPAFOptions->inputParameters->SetNamedString("FlavorChannel", FlavorChannel.Data());
  gPAFOptions->inputParameters->SetNamedInt("jetChannel", JetChannel);

  ////// I.G.
  //Find the total number of entries in the dataset and send it to the input parameters
  /*  TChain* chain = new TChain("Tree", "Tree");
      for (unsigned int i = 0; i < dataFiles.size(); i++) 
      chain->Add(dataFiles[i]);
      gPAFOptions->inputParameters->SetNamedInt("NEventsTotal", chain->GetEntries()); //after skimming
      TString eventsfile(gSystem->pwd());
      eventsfile+="/";
      eventsfile+=Signal;
      eventsfile+="_events.log";
      gPAFOptions->inputParameters->SetNamedString("fFileList", (const char*) eventsfile);
  */

  ///////////////////////////////
  // DYNAMIC HISTOGRAMS
  // Specify the name of the histograms you would like to monitor as they are
  // filled by PROOF
  //
  //  gPAFOptions->dynamicHistograms.push_back("myHistogram");
  //...
  
  /////////////////////////////
  // NUMBER OF EVENTS 
  // Specify the number (Long64_t) of events to process.
  // Set it to -1 to use the full sample.
 
  gPAFOptions->SetNEvents(-1);
  
  //
  /////////////////////////////////////////////////////////////////////////







  /////////////////////////////////////////////////////////////////////////
  //
  // EXTRA proof settings:
  // ====================
  //
  // It is unlikely that you need to edit anything below. At least at the
  // beginning of your PAF experience. However we provide a couple of hooks
  // for extensions.
  //

  ///////////////////////////////
  // NAME OF ANALYSIS CLASS. 
  // If 0 the default name schema will be used, i.e. depending on the value
  // of gPAFOptions->treeType: MyAnalysisTESCO or MyAnalsyisMiniTrees
  //
  
  gPAFOptions->SetAnalysis("test");


  ///////////////////////////////
  // ADDITIONAL PACKAGES TO BE UPLOADED TO PROOF.
  // The mandatory ones are added automatically in PAFOptions
  //


  gPAFOptions->AddPackage("PUWeight");
  //  gPAFOptions->AddPackage("MuonIsoMVA");


  ///////////////////////////////
  // CONTROL OUTPUT AND CHECKS
  // + If true (default) PAF checks for new version in CVS every time
  // gPAFOptions->checkVersion = true;
  // + If true (default) the output file is reopened so the objects in the
  //   file can be interactively accessed. The object in the output are also
  //   listed
  // gPAFOptions->reopenOutputFile = false;

  //
  /////////////////////////////////////////////////////////////////////////



 
  /////////////////////////////////////////////////////////////////////////
  //
  // RUN THE ANALYSIS
  // ================
  //
  // If something needs to be edited below (or inside), contact the 
  // developers.
  //
  // Run the analysis
  //
  //gPAFOptions->reopenOutputFile = false;
  gPAFOptions->reopenOutputFileRemoved= false;

  if (!RunAnalysis())
    cerr << "ERROR: There was a problem running the analysis!" << endl;
  //
  /////////////////////////////////////////////////////////////////////////


}
