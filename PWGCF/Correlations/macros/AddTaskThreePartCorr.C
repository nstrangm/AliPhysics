AliAnalysisTaskThreePartCorr *AddTaskThreePartCorr(const char* outputFileName = 0, const char* containerName = "ThreePartCorrOutput")
{
  // Get the pointer to the existing analysis manager via the static access method.
  //==============================================================================
  AliAnalysisManager *mgr = AliAnalysisManager::GetAnalysisManager();
  if (!mgr) {
    ::Error("AddTaskThreePartCorr", "No analysis manager to connect to.");
    return NULL;
  }  
  
  // Create the task and configure it.
  //===========================================================================
  AliAnalysisTaskThreePartCorr* ana = new  AliAnalysisTaskThreePartCorr(containerName);

  // common config,
  ana->SetDebugLevel(0); 

//   gSystem->Sleep(3000);
  
  mgr->AddTask(ana);
  
  // Create ONLY the output containers for the data produced by the task.
  // Get and connect other common input/output containers via the manager as below
  //==============================================================================
  if (!outputFileName)
    outputFileName = AliAnalysisManager::GetCommonFileName();
  
  AliAnalysisDataContainer *coutput1 = mgr->CreateContainer(containerName, TList::Class(),AliAnalysisManager::kOutputContainer,outputFileName);
  
  mgr->ConnectInput  (ana, 0, mgr->GetCommonInputContainer());
  mgr->ConnectOutput (ana, 1, coutput1 );
   
  return ana;
}
