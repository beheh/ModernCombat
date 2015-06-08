/*-- Athletik - I --*/

#strict 2
#include SSY2

static sect1told;

/* 1 - Athletik I */

public func SetupSection()
{
  for(var clonk in FindObjects(Find_ID(PCMK)))
  {
    SetPosition(RandomX(30,120),-50,clonk);
    CreateObject(PARA,0,0,GetOwner(clonk))->Set(clonk);
  }
  
  TutorialMessage("$Txt01$");
  SectionGoto(4);
  sect1told = false;
  

  //Zeug
  CreateObject(FDMG,10,10,-1);
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
    Sound("Cheer.ogg");
    TutorialMessage("$WellDone1$");
    unlocked[1] = 2;
    if(unlocked[2] == 0)
      unlocked[2] = 1;
    LoadSection(0);
    SectionStop();
  }
  if(!FindObject(BBTP) && !sect1told)
  {
    TutorialMessage("$Txt02$");
    sect1told = true;
    wait(36*6);
  }
  if(FindObject2(Find_ID(PCMK),Find_InRect(290,270,140,80)))
    TutorialMessage("$Txt03$");
  if(FindObject2(Find_ID(PCMK),Find_InRect(460,40,80,30)))
    TutorialMessage("$Txt04$");
    
  SectionGoto(4);
}
