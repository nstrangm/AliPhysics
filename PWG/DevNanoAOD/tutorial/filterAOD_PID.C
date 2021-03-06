void filterAOD_PID()
{
  AliAnalysisManager* mgr = new AliAnalysisManager("NanoAOD Filter", "NanoAOD filter for nanoAOD production");
    
  AliAODInputHandler* iH = new AliAODInputHandler();
  mgr->SetInputEventHandler(iH);

  // Define aod output handler
  AliAODHandler* aodOutputHandler = new AliAODHandler();
  aodOutputHandler->SetOutputFileName("AliAOD.NanoAOD.root");
  mgr->SetOutputEventHandler(aodOutputHandler);
  
  // PID response
  gInterpreter->ExecuteMacro("$ALICE_ROOT/ANALYSIS/macros/AddTaskPIDResponse.C");
  
  AliAnalysisTaskNanoAODFilter* task = (AliAnalysisTaskNanoAODFilter*) gInterpreter->ExecuteMacro("$ALICE_PHYSICS/PWG/DevNanoAOD/macros/AddTaskNanoAODFilter.C(0, kFALSE)");
  task->AddSetter(new AliNanoAODSimpleSetter);
  
  // Event selection
  AliAnalysisNanoAODEventCuts* evtCuts = new AliAnalysisNanoAODEventCuts;
  // NOTE filter bit set in AliEventCuts automatically

  // Track selection
  AliAnalysisNanoAODTrackCuts* trkCuts = new AliAnalysisNanoAODTrackCuts;
  trkCuts->SetBitMask((1 << 8) | (1 << 9)); // hybrid 2011
  trkCuts->SetMaxEta(0.9);
  trkCuts->SetMinPt(1.0);
  
  // Fields to store
  // event level
  // vertices kept by default
  task->SetVarListHeader("OfflineTrigger,MagField,T0Spread,NumberOfESDTracks");
  // track level
  task->SetVarListTrack("pt,theta,phi");
  task->AddPIDField(AliNanoAODTrack::kSigmaTPC, AliPID::kPion);
  task->AddPIDField(AliNanoAODTrack::kSigmaTPC, AliPID::kKaon);
  task->AddPIDField(AliNanoAODTrack::kSigmaTPC, AliPID::kProton);
  task->AddPIDField(AliNanoAODTrack::kSigmaTOF, AliPID::kPion);
  task->AddPIDField(AliNanoAODTrack::kSigmaTOF, AliPID::kKaon);
  task->AddPIDField(AliNanoAODTrack::kSigmaTOF, AliPID::kProton);

  task->SetTrkCuts(trkCuts);
  task->AddEvtCuts(evtCuts);

  mgr->SetDebugLevel(1); // enable debug printouts
  if (!mgr->InitAnalysis()) 
    return;
  mgr->PrintStatus();
  
  // Input files
  TChain * chain = new TChain("aodTree");
  chain->Add("AliAOD.root");

  Printf("Starting Analysis....");
  mgr->StartAnalysis("local", chain, 1000);
}
