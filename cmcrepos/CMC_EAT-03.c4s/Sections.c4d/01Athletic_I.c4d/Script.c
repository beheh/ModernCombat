/*-- Athletik - I --*/

#strict 2

static sect1told;

/* 1 - Athletik I */

global func SetupSection1()
{
  for(var clonk in FindObjects(Find_ID(PCMK)))
  {
    SetPosition(RandomX(30,120),-50,clonk);
    CreateObject(PARA,0,0,GetOwner(clonk))->Set(clonk);
  }
  
  TutorialMessage("$Mobility_I_1$");
  wait(3*5);
  goto(4);
  
  sect1told = false;
  

  //Zeug
  CreateObject(BSH2,434,43,-1);
  CreateObject(LADR,543,206,-1)->Set(18);
  CreateObject(STNE,37,223,-1);
  CreateObject(STNE,14,231,-1);
  CreateObject(RAI3,240,230,-1);
  CreateObject(XTNK,278,229,-1);
  CreateObject(RAI3,270,230,-1);
  CreateObject(RAI3,300,230,-1);
  CreateObject(BBTP,426,228,-1)->Set(-70,-1);
  CreateObject(BSH2,429,243,-1);
  CreateObject(DGNN,450,220,-1);
  CreateObject(WCR2,479,229,-1);
  CreateObject(LFTP,476,241,-1);
  CreateObject(WCR2,533,214,-1);
  CreateObject(WCR2,549,232,-1);
  CreateObject(WCR2,533,232,-1);
  CreateObject(STNE,577,284,-1);
  CreateObject(STNE,730,313,-1);
  CreateObject(STNE,757,305,-1);
  CreateObject(STNE,83,322,-1);
  CreateObject(BSH2,321,321,-1);
  CreateObject(STNE,425,349,-1);
  CreateObject(STNE,508,332,-1);
}

func Script5()
{
  if(FindObject2(Find_ID(PCMK),Find_InRect(640,240,160,100)))
  {
    LoadSection(0);
    Sound("Cheer.ogg");
    TutorialMessage("$Ace_WellDone2$");
    unlocked[1] = 2;
    unlocked[2] = 1;
    ScriptGo(0);
  }
  if(!FindObject(BBTP) && !sect1told)
  {
    TutorialMessage("$Mobility_I_2$");
    sect1told = true;
  }
  goto(4);
}
