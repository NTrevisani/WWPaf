///////////////////////////////////////////////////////////////////////
//
//    FILE: StartProof.C
// AUTHORS: I. Gonzalez Caballero, A.Y. Rodriguez Marrero
//    DATE: January, 2011
//
// CONTENT: Implements the functionality needed to run an Analysis
//          using the PROOF Analysis Framework (PAF). Some more helper
//          functions may be found in PAFCompiledUtils.h/C
//
///////////////////////////////////////////////////////////////////////

// Include the singleton where the options are stored.
#include "PAFOptions.C"
// Some definitions...
#include "PAF.h"


// Uncomment the line below to get information on time spent at each step
// This setting may also be defined in the main user code from a clean 
// session.
//#define TIMERS
#ifdef TIMERS
double PAFTime0; //InitProof
double PAFTime1; //Building dataset
double PAFTime2; //Create base selector
double PAFTime3; //Include path
double PAFTime4; //Loading packages
double PAFTime5; //Compile selector
double PAFTime6; //Parameters and dynamic histograms
double PAFTime7; //Processing data
double PAFTime8; //Output file
#endif


// Creates a session dir and sets the build dir there
bool CreateSessionDir() {
  // Save previous directory
  gPAFOptions->originalBuildDir(gSystem->GetBuildDir());
  // XXX: errors are not properly catched here!
  gPAFOptions->buildDir = gSystem->GetFromPipe("mktemp -d");
  // - And let's set the build dir there so nothing interferes in the
  //   PAF directory installation
  if (gPAFOptions->buildDir == "") {
    return false;
  }
  gSystem->SetBuildDir(gPAFOptions->buildDir);
  TString ProofSandboxDir;
  ProofSandboxDir.Form("%s/.proof", gPAFOptions->buildDir.Data());
  gEnv->SetValue("Proof.Sandbox", ProofSandboxDir);
  return true;
}


// Inits Proof Lite session 
TProof *InitLite() {
  // If ProofLite: all cores
  cout << PAFINFO << "+ PROOF Lite mode selected. Using local node." << endl;
  TString nworkers("");
  if (gPAFOptions->GetNSlots() > 0) {
    SysInfo_t si;
    Int_t ncpus = 0;
    if (gSystem->GetSysInfo(&si) == 0)
      ncpus = TMath::Min(gPAFOptions->GetNSlots(), si.fCpus);

    //The syntax to set the number of slots changed in ROOT 5.30.00
    if (gROOT->GetVersionInt() < 53000)
      nworkers.Form("workers=%d",ncpus);
    else
      nworkers.Form("lite:///?workers=%d",ncpus);

    if (gPAFOptions->GetNSlots() != ncpus)
      cerr << PAFWARN << "Using only " << ncpus <<  " slots (the number of available cores)" << endl;
    cout << PAFINFO << " - Using " << ncpus << " slaves..." << endl;
  }
#if DEBUGUTILS
  cerr << PAFDEBUG << "nworkers string: " << nworkers << endl;
#endif
  return TProof::Open(nworkers.Data());
}

// Inits Proof Cluster session 
TProof *InitCluster() {
  if (gPAFOptions->GetNSlots() < 0 )
    gPAFOptions->SetNSlots(10);

  cout << PAFINFO << "+ PROOF Cluster mode selected. Using:" << endl
       << PAFINFO << " - N. Slots     = " << gPAFOptions->GetNSlots() << endl
       << PAFINFO << " - Max. N. Slav.= " << gPAFOptions->maxSlavesPerNode << endl;

  // Ask the desired number of workers:
  TString slots_petition;
  slots_petition.Form("startproof -a %d", gPAFOptions->GetNSlots());
#if DEBUGUTILS
  cerr << PAFDEBUG << "Slots petition: " << slots_petition  << endl;
#endif
  // XXX: think of a better way of doing this, 
  // startproof returns the path where the paf_url file is located
  gPAFOptions->pafSessionDir = gSystem->GetFromPipe(slots_petition);

  ifstream paf_url;
  paf_url.open(gPAFOptions->pafSessionDir + "/paf_url");
  if (!paf_url.is_open()) {
    cerr << PAFERROR << "Cannot open file with proof master address" << endl;
    return 0;
  }
  string line;
  getline(paf_url, line);
  TString proofserverchain(line); 

  cout << PAFINFO << "+ Starting PROOF session at " << proofserverchain << endl;
  // XXX: this should not be necessary for the new style PAF
  // gPAFOptions->proofSession = TProof::Reset(proofserverchain,kTRUE);
  return TProof::Open(proofserverchain);
}

// Inits Proof PoD session 
TProof *InitPoD() {
  if (!gSystem->Getenv("POD_LOCATION")) {
    cerr << PAFERROR << "PoD path not set. Try to source something like /opt/PoD/PoD_env.sh" << endl
         << PAFERROR << "Exiting!" << endl;
    return 0;
  }
  int wait = 1; //Interval between checks in seconds
  int n_waits = gPAFOptions->GetPoDTimeOut() / wait;
  if (gPAFOptions->GetNSlots() < 0 )
    gPAFOptions->SetNSlots(10);

  // If ProofLite: all cores
  cout << PAFINFO << "+ PROOF on Demand (PoD)  mode selected. Using local node." << endl;
  // Start pod-server if it isn't runnign already
  TString podserverstatus=gSystem->GetFromPipe("pod-server status 2>&1");
  if (podserverstatus.Contains("NOT")) {
    cout << PAFINFO << "+ PoD server not running, starting it: " << endl;
    gSystem->Exec("pod-server start");
  }
  else
    cout << PAFINFO << "+ PoD server already running, reusing it" << endl;

  //Find if there are already slots being used
  int activeSlots = gSystem->GetFromPipe("pod-info -n").Atoi();

  if (activeSlots) {
    cout << PAFINFO << "+ You already have " << activeSlots 
         << " slots allocated." << endl;
  }

  //Initially assume no slots has been allocated
  int missingSlots = gPAFOptions->GetNSlots() - activeSlots;

  if (missingSlots < 0) {
    gPAFOptions->SetNSlots(activeSlots);
  }

  if (missingSlots > 0) {
    cout << PAFINFO << "+ Asking for " << missingSlots << " slots" << endl;

#if DEBUGUTILS
    cerr << PAFDEBUG << Form("pod-submit -r pbs -n %d", missingSlots) << endl;
#endif
    gSystem->Exec(Form ("pod-submit -r pbs -n %d", missingSlots));
    //We have to wait until we have all our WNs
    cout << PAFINFO << "+ Waiting for batch system. Found 0 slots (" 
         << gPAFOptions->GetPoDTimeOut() << " s)  " << flush; 

    int slotsReady = 0;
    int srmsize = 1;
    int trmsize = (int) TMath::Log10(gPAFOptions->GetPoDTimeOut()) + 1;
    int rmsize  = srmsize + trmsize + 13;
    do{
      gSystem->Sleep(wait*1000);
      slotsReady = gSystem->GetFromPipe("pod-info -n").Atoi();
      n_waits--;
      for (int i = 0; i < rmsize; i++)
        cout << '\b';
      cout << slotsReady << " slots (" << n_waits * wait << " s)  "<< flush;
      srmsize = (slotsReady == 0?1:(int) TMath::Log10(slotsReady) + 1);
      trmsize = (int) TMath::Log10(n_waits*wait) + 1;
      rmsize = srmsize + trmsize + 13;
    } while((slotsReady < gPAFOptions->GetNSlots()) && (n_waits > 0) );
    cout << endl;
  }

  cout << PAFINFO << "+ We will use " << gSystem->GetFromPipe("pod-info -n").Atoi()
       << " slots" << endl;

  // Open proof session using the server we launched with PoD
  cout << PAFINFO << "+ Opening Proof session using " 
       << gSystem->GetFromPipe("pod-info -c") << endl;
  return TProof::Open(gSystem->GetFromPipe("pod-info -c"));
}

// Inits Proof PoD session 
TProof *InitCloud() {
  cout << PAFINFO << "+ PROOF Cloud mode selected." << endl;
  TString proofserverchain = "";
  if ( gPAFOptions->proofRequest ) {  // User may disable the request
#if DEBUGUTILS
    cerr << PAFDEBUG << "Calling startproof cloud!" << endl;
#endif
    if (gSystem->Exec("proofcloud start") != 0) {
      cerr << PAFERROR << "ERROR Trying to get machines" << endl;
      return 0;
    }
    proofserverchain = gSystem->GetFromPipe("proofcloud getserver");
  } else {
    // Build the full user@proofserver:port string
    proofserverchain = "proof@";
    proofserverchain +=gPAFOptions->proofServer;
    proofserverchain +=":";
    proofserverchain +=gPAFOptions->proofServerPort;
  }
  cout << PAFINFO << "+ Starting PROOF session at " << proofserverchain << "..."
       << endl;
  //gPAFOptions->proofSession = TProof::Open(proofserverchain);
  //gPAFOptions->proofSession = TProof::Reset(proofserverchain, kTRUE);
  return TProof::Open(proofserverchain);
}

///////////////////////////////////////////////////////////////////////
//
// Function to initialize the PROOF session depending on the mode 
// selected
//
TProof* InitProof() {
#if DEBUGUTILS
  cerr << PAFDEBUG << ">> InitProof(" << gPAFOptions->proofMode << ")" << endl;
#endif
#ifdef TIMERS
  cout << PAFINFO << "Setting up InitProof timer..." << endl;
  TStopwatch PAFTimer;
  PAFTimer.Start();
#endif

  ///////////////////
  // Initial checks
  //

  // ++ LITE
  if (gPAFOptions->proofMode == kLite) {
    // Nothing to check
  }

  // ++ Cluster
  else if (gPAFOptions->proofMode == kCluster) {
    // XXX - I.G. Nothing to check?
    // XXX - I.G. Perhaps the existance of PROOF Cluster utilities in the path?
  }

  // ++ PoD
  else if (gPAFOptions->proofMode == kPoD) {
    // Check for env variable POD_LOCATION that should be set if the
    // environent was properly set
    if (!gSystem->Getenv("POD_LOCATION")) {
      cerr << PAFERROR 
	   << "PoD path not set. Try to source something like /opt/PoD/PoD_env.sh" 
	   << endl
	   << PAFERROR << "Exiting!" << endl;
      return 0;
    }
  } 

  // ++ Cloud
  else if ( gPAFOptions->proofMode == kCloud ) {
    // XXX - I.G. Nothing to check?
    // XXX - I.G. Perhaps the existance of Cloud utilities in the path?
  }

  // ++ Sequential
  else if (gPAFOptions->proofMode == kSequential) {
    // Nothing to check
  } 

  // ++ Something else
  else {
    cerr << PAFERROR << "You have chosen a PROOF mode (" 
	 << gPAFOptions->proofMode << ") not yet implemented" << endl;
    cerr << PAFERROR << "Exiting!" << endl;
    return 0;
  }

  ///////////////////
  // Set build dir
  //
  if (!CreateSessionDir()) {
    cerr << PAFERROR << "Unable to continue without a valid build dir!" << endl;
    cerr << PAFERROR << "Exiting!" << endl;
    return 0;
  }

  // Check PAF Version (no more needed)
  //if (gPAFOptions->checkVersion)
  //  gSystem->Exec("$PAFPATH/bin/checkversion.sh&");

  ///////////////////
  // Start PROOF
  //
  gPAFOptions->proofSession = 0;

  // ++ LITE
  if (gPAFOptions->GetPAFMode() == kLite) {
    gPAFOptions->proofSession = InitLite();
  } 
  // ++ CLUSTER
  else if (gPAFOptions->GetPAFMode() == kCluster) {
    gPAFOptions->proofSession = InitCluster();
  } 
  // ++ POD
  else  if ( gPAFOptions->GetPAFMode() == kPoD ) {
    gPAFOptions->proofSession = InitPoD();
  } 
  // ++ CLOUD
  else if ( gPAFOptions->GetPAFMode() == kCloud ) {
    gPAFOptions->proofSession = InitCloud();
  } 
  // ++ SEQUENTIAL
  else if (gPAFOptions->GetPAFMode() == kSequential) {
    cout << PAFINFO << "+ Sequential mode selected. No PROOF will be used." << endl;
  }


  // Weird! This will avoid that a lot of output is printed whenever something
  // is run on PROOF
  if (gPAFOptions->GetPAFMode() != kSequential) {
    if (gPAFOptions->GetPROOFSession()) {
      gPAFOptions->GetPROOFSession()->Exec("int kkkkkkkkkkkkkkkkkkkkk;");
      if (gPAFOptions->GetPAFMode() != kLite) {
        gPAFOptions->GetPROOFSession()->SetParameter("PROOF_MaxSlavesPerNode", 
                                                gPAFOptions->maxSlavesPerNode);
      }
    }
  }

  ///////////////////
  // Load extra utils
  //

  // ++ PAFCompiledUtils
  gROOT->LoadMacro("$PAFPATH/scripts/PAFCompiledUtils.C+");

  // ++ Load or Enable InputParameters
  vector<TString> ip;
  ip.push_back("InputParameters");
  if (gPAFOptions->GetPAFMode() == kSequential) {
    LoadPackages(ip, false);
  } else {
    UploadAndEnablePackages(gPAFOptions->GetPROOFSession(), ip, false);
  }

#ifdef TIMERS
  PAFTime0 = PAFTimer.RealTime();
  cout << PAFINFO << "  + TIME (InitProof): " << PAFTime0 << endl;
#endif

#if DEBUGUTILS
  cerr << PAFDEBUG << "<< InitProof(" << gPAFOptions->GetPROOFSession() << ")" << endl;
#endif
  return gPAFOptions->GetPROOFSession();
}
//
///////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////
//
// Functions needed to run the analysis
//

///////////////////////////////
//
// Set include path
//
void SetIncludePath() {
  // XXX this is a noop, should be removed!
  TString pwdpath = "-I";
  pwdpath+=gSystem->pwd(); //path to local dir
  TString curpath = gSystem->GetIncludePath(); //current path
#if DEBUGUTILS
  cerr << PAFDEBUG << "+ Current PWD path in the master is \"" 
       << pwdpath << "\"" << endl;
  cerr << PAFDEBUG << "+ Current include path in the master is \"" 
       << curpath << "\"" << endl;
#endif
}

/////////////////////////////////
// Load Packages (used in sequential mode)
//
void LoadPackages(const vector<TString>& packages, bool isSelector) {
  TString packages_dir;
  packages_dir.Form("%s/packages/", gSystem->GetBuildDir());
  gSystem->MakeDirectory(packages_dir);

  TString pwddir = gSystem->pwd();
  for (unsigned int i = 0; i < packages.size(); i++) {
    cout << PAFINFO << "Creating package " << packages[i] << "... " << endl; 
    
#if DEBUGUTILS
    cerr << PAFDEBUG << "+ par file... " << endl;
#endif
    BuildParFile(packages[i], isSelector);
    
    //This is equivalent to enable package
    gSystem->ChangeDirectory(packages_dir + packages[i]);

#if DEBUGUTILS    
    cerr << PAFDEBUG << "In directory " << gSystem->pwd() << endl;
#endif

    // + First execute BUILD.sh
#if DEBUGUTILS    
    cerr << PAFDEBUG << "building... " << endl;
#endif
    gSystem->Exec("./PROOF-INF/BUILD.sh");
    // + Then load and execute SETUP.C
#if DEBUGUTILS    
    cerr << PAFDEBUG << "loading... " << endl;
#endif
    gROOT->LoadMacro("PROOF-INF/SETUP.C");
    SETUP();
    // ENOL: SETUP seems not to work for me :( XXX
    // Manually loading the library
    gSystem->Load("lib"+packages[i]);

    // Changes for root 5.34: add the package to the include path
    gSystem->AddIncludePath("-I" + packages_dir + packages[i]);
    
    //Back to the pwd directory
    gSystem->ChangeDirectory(pwddir);
  }
}


///////////////////////////////
//
// Run the full analysis
// This function assumes the PROOF session is already initialised (if not
// InitProof() should be called before. It returns true if everything went
// fine, or false otherwise.
//

bool RunAnalysis() {
#ifdef TIMERS
  cout << PAFINFO << "+ Setting up timers..." << endl;
  TStopwatch PAFTimer;
  PAFTimer.Start();
#endif

  if (gPAFOptions->analysisDone) {
    cerr << PAFERROR << "A previous RunAnalysis was run, unable to continue!" << endl
         << PAFERROR << "Exiting! " << endl;
    return false;
  }

  //
  // If we are in a PROOF like session, check it was initialized
  //
  if (gPAFOptions->GetPAFMode() != kSequential && (!gPAFOptions->GetPROOFSession())) {
    cerr << PAFERROR "The PROOF Session was not initialized in RunAnalysis()"
	 << endl
         << PAFERROR << "Exiting! " << endl;
    return false;
  }

  //Get Selector
  TString selector = gPAFOptions->GetAnalysis();
  
  //
  //Data
  //
  cout << PAFINFO << ">> Building dataset..." << endl;

  if (gPAFOptions->dataFiles.size() < 1) {
    cerr << PAFERROR << "Cannot continue with an empty dataset" << endl
         << PAFERROR << "Exiting!" << endl;
    return false;
  }
  
  // In sequetial mode create TChain
  TString chainname;
  if (gPAFOptions->GetTreeDir() != "" )
    chainname = gPAFOptions->GetTreeDir() + "/" + gPAFOptions->GetTreeName();
  else
    chainname = gPAFOptions->GetTreeName();
  TChain* tchaindataset = new TChain(chainname); // For all modes

  for (unsigned int i = 0; i < gPAFOptions->dataFiles.size(); i++)
    tchaindataset->Add(gPAFOptions->dataFiles[i]);

#if DEBUGUTILS
  cerr << PAFDEBUG << "Files in dataset:" << endl;
  tchaindataset->GetListOfFiles()->Print();
#endif

#ifdef TIMERS
  //PAFTIME1
  PAFTime1 = PAFTimer.RealTime();
  cout << PAFINFO << "  + TIME (Building dataset): " << PAFTime1 << endl;
  PAFTimer.Start();
#endif

  //
  //Make selector
  if (gPAFOptions->createSelector) {
    cout << PAFINFO << ">> Creating Selector..." << endl;
    TString pkgName;
    if (!CreateSelector(gPAFOptions->dataFiles[0], pkgName,
			gPAFOptions->GetTreeDir(), 
			gPAFOptions->GetTreeName(),
			gPAFOptions->GetAnalysis(),
			gPAFOptions->GetSelectorPackages())) {
        cerr << PAFERROR "Error creating selector..." << endl;
        return false;
    } else {
        gPAFOptions->AddSelectorPackage(pkgName); 
    }
  } else {
    // XXX try to use an already existing PAFAnalysis package !?
    cerr << PAFWARN << "No Selector created, your code will probably fail!" << endl;
  }

#ifdef TIMERS
  //PAFTIME2
  PAFTime2 = PAFTimer.RealTime();
  cout << PAFINFO << "  + TIME (Create base selector): " << PAFTime2 << endl;
  PAFTimer.Start();
#endif

  //
  // Set include path
  cout << PAFINFO << ">> Setting include path..." << endl;
  SetIncludePath();

#ifdef TIMERS
  //PAFTIME3
  PAFTime3 = PAFTimer.RealTime();
  cout << PAFINFO << "  + TIME (Include path): " << PAFTime3 << endl;
  PAFTimer.Start();
#endif

  //
  // Upload and enable packages / Load packages
  cout << PAFINFO << ">> Loading packages ..." << endl;
  if (gPAFOptions->GetPAFMode() == kSequential) {
    LoadPackages(gPAFOptions->GetPackages(), false);
    LoadPackages(gPAFOptions->GetSelectorPackages(), true);
  }
  else {
    UploadAndEnablePackages(gPAFOptions->GetPROOFSession(), 
			    gPAFOptions->GetPackages(), false);
    UploadAndEnablePackages(gPAFOptions->GetPROOFSession(), 
			    gPAFOptions->GetSelectorPackages(), true);
  }


#ifdef TIMERS
  //PAFTIME4
  PAFTime4 = PAFTimer.RealTime();
  cout << PAFINFO << "  + TIME (Loading packages): " << PAFTime4 << endl;
  PAFTimer.Start();
#endif

  //
  // Compile Selector
  //
  TString selectorcfile = selector + ".C";
  TString selectorhfile = selector + ".h";

  TString dest = gSystem->GetBuildDir();
  dest += "/";
  dest += selectorcfile;

  if (gSystem->CopyFile(selectorcfile, dest)) {
      cerr << PAFERROR << "Could not copy " << selectorcfile << " to temp directory" << endl;
      return false;
  }

  if (gSystem->FindFile(".", selectorhfile)) {
    dest.Replace(dest.Length() - 1, dest.Length(), "h");
    if (gSystem->CopyFile(selectorhfile, dest)) {
      cerr << PAFERROR << "ERROR: Could not copy " << selectorhfile << " to temp directory" << endl;
      return false;
    }
  }

  // Selector
  TString selectorcplus = selectorcfile; 
  selectorcplus+= "++";
  PAFBaseSelector *pafbaseselector = 0;

  if (gPAFOptions->GetPAFMode() == kSequential) {
    cout << PAFINFO << ">> Creating selector..." << endl;
    pafbaseselector = (PAFBaseSelector*)TSelector::GetSelector(selectorcplus);
  }

#ifdef TIMERS
  //PAFTIME5
  PAFTime5 = PAFTimer.RealTime();
  cout << PAFINFO << "  + TIME (Compile selector): " << PAFTime5 << endl;
  PAFTimer.Start();
#endif
  

  //  
  // Add the set of parameters to the input lists
  //
  if (gPAFOptions->inputParameters) {
    cout << PAFINFO << ">> Adding input parameters..." << endl;
    
    if (gPAFOptions->GetPAFMode() == kSequential) {
      TList* inputlist = new TList;
      if (gPAFOptions->inputParameters) {
	inputlist->Add(gPAFOptions->inputParameters);
      }
      pafbaseselector->SetInputList(inputlist);
    }
    else {
      if (gPAFOptions->inputParameters) {
	gPAFOptions->GetPROOFSession()->AddInput(gPAFOptions->inputParameters);
      }
    }
  }

  //
  // Add dynamic histograms (feedback)
  //
  if (gPAFOptions->GetPAFMode() != kSequential && 
      gPAFOptions->dynamicHistograms.size() > 0) {
    cout << PAFINFO << ">> Adding dynamic histograms (Feedback)" << endl;
    for (unsigned int i = 0; 
	 i < gPAFOptions->dynamicHistograms.size(); 
	 i++) {
      cout << PAFINFO << "+ " << gPAFOptions->dynamicHistograms[i] << endl;
      gPAFOptions->GetPROOFSession()->AddFeedback(gPAFOptions->dynamicHistograms[i]);
    }
    new TDrawFeedback(proof);
  }
  else {
#if DEBUGUTILS
    cerr << PAFDEBUG << ">> No dynamic histograms (Feedback)" << endl;
#endif
  }
#ifdef TIMERS
  PAFTime6 = PAFTimer.RealTime();
  cout << PAFINFO << "  + TIME (Parametes and dynamic histograms): " 
       << PAFTime6 << endl;
  PAFTimer.Start();
#endif

  //
  // Processing...
  //
  cout << PAFINFO << ">> Processing " << selector << " in ";
  if (gPAFOptions->GetPAFMode() == kSequential) {
    cout << "sequential mode..." << endl;
    cerr << PAFWARN << "Note that no dialog or progressing bar will be shown!" << endl;
  }
  else {
    cout << gPAFOptions->GetPROOFSession()->GetParallel() << " nodes..." << endl;
  }

  Long64_t nevents    = gPAFOptions->GetNEvents();
  Long64_t firstevent = gPAFOptions->GetFirstEvent();
  cout << PAFINFO << "+ ";  
  if (nevents == TChain::kBigNumber) cout << "All"; else cout << nevents;
  cout << " events will be scanned, starting";
  if (firstevent)
    cout << " on event " << firstevent << endl;
  else
    cout << " at first event" << endl;
  if (gPAFOptions->GetPAFMode() == kSequential) {
    tchaindataset->Process(pafbaseselector, 0, nevents, firstevent);
  }
  else {
    tchaindataset->SetProof();
    tchaindataset->Process(selectorcplus, 0, nevents, firstevent);
  }

#ifdef TIMERS
  PAFTime7 = PAFTimer.RealTime();
  cout << PAFINFO << "  + TIME (Processing data): " << PAFTime7 << endl;
  PAFTimer.Start();
#endif

  //
  // Create the ouptut file and fill it
  //
  cout << PAFINFO << ">> Saving results to " << gPAFOptions->outputFile << " ..." << endl;
  if (gSystem->FindFile(".", TString(gPAFOptions->outputFile))) {
    TString outputFileBak = gPAFOptions->outputFile + ".bak";
    cerr << PAFWARN << "File " << gPAFOptions->outputFile << " already exists! "
         << "Moving it to " << outputFileBak << endl;
    gSystem->CopyFile(gPAFOptions->outputFile, outputFileBak, kTRUE);
    gSystem->Unlink(gPAFOptions->outputFile);
  }
  TFile histoAnalysis(gPAFOptions->outputFile, "NEW");
  if (histoAnalysis.IsOpen()) {
    TList* li = 0;
    TList* lo = 0;
    if (gPAFOptions->GetPAFMode() == kSequential) {
      li = inputlist;
      lo = pafbaseselector->GetOutputList();
    }
    else {
      li = gPAFOptions->GetPROOFSession()->GetInputList();
      lo = gPAFOptions->GetPROOFSession()->GetOutputList();
    }
    li->Write();
    lo->Write();
    histoAnalysis.Close();

    //
    // This is used to have the objects available in the shell
    // XXX should only be done if interactive!?
    //
    if (gPAFOptions->reopenOutputFile) {
      cout << PAFINFO << ">> You may directly access the following objects that you saved ..."
           << endl;
      TFile *f = TFile::Open(gPAFOptions->outputFile);
      f->ls();
    }
  }
  else {
    cerr << PAFERROR << "Could not open output file " << gPAFOptions->outputFile
	 << endl;
  }
  
  
  if ( gPAFOptions->GetPAFMode() == kCluster ) {
    // XXX: enol, remove the active file, so others can reuse!
    gSystem->Unlink(gPAFOptions->pafSessionDir + "/active");
    cerr << PAFWARN << "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl
         << PAFWARN << " REMINDER: If you do not need anymore the slots you asked for" << endl 
         << PAFWARN << " please free them after leaving the ROOT session," << endl
         << PAFWARN << " you can do that just by typing 'endproof' " << endl  
         << PAFWARN << "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
  }


#ifdef TIMERS
  PAFTime8 = PAFTimer.RealTime();
  cout << PAFINFO << "  + TIME (Output file): " << PAFTime8 << endl;

  ofstream ftimes(Form("tiempos_ejecucion%d", gPAFOptions->GetNSlots()),
		   ios::app);
  ftimes << Form("%f;%f;%f;%f;%f;%f;%f;%f;%f;\n",
		 PAFTime0,
		 PAFTime1, PAFTime2, PAFTime3, PAFTime4,
		 PAFTime5, PAFTime6, PAFTime7, PAFTime8);
  ftimes.flush();
  ftimes.close();
#endif

  gPAFOptions->analysisDone = true;
  TString rmdircmd;
  rmdircmd.Form("rm -rf %s", gPAFOptions->buildDir.Data());
  // change the build directory to the original one
  gSystem->SetBuildDir(gPAFOptions->originalBuildDir);
  // and remove the mess in /tmp with a rm -rf (!!)
  cout << PAFINFO << "Removing temporary directory: " << rmdircmd << endl;
  gSystem->GetFromPipe(rmdircmd);

  return true;
}
//
///////////////////////////////////////////////////////////////////////
