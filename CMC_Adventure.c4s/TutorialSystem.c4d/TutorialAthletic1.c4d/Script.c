/*-- Athletic 1 --*/

#strict 2

#include TSYS

public func UsedRules() { return [NOFD, FDMG, NOFF]; }
public func SectionName() { return "Athletic1"; }
public func RelaunchPosition(int &iX, int &iY, object pCrew) { iX = RandomX(30, 120); iY = -50; } // Relaunchpositionen.

public func Initialize()
{
	SetPlayList("CMC_Base Groove.ogg;CMC_Moving Squad.ogg");
  Music("CMC_Base Groove.ogg");
  
  // Ausgang
  CreateObject(EXIT, 605, 290, NO_OWNER);
  
  TutorialMessage("$Txt01$");
  
  return _inherited(...);
}

public func OnClonkRelaunch(object pCrew)
{
	CreateObject(PARA, 0, 0, GetOwner(pCrew))->Set(pCrew);
	return true; 
}

public func CreateDecoration()
{
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

public func CreateTriggers()
{
	// TRIGGER_Find
	BindTrigger(CreateObject(TIM1, 0, 0, NO_OWNER), TRIGGER_Find, [[Find_ID(PCMK), Find_InRect(640, 240, 160, 100)]], "WellDone1", this);
	BindTrigger(CreateObject(TIM1, 0, 0, NO_OWNER), TRIGGER_Find, [[Find_ID(PCMK), Find_InRect(290, 270, 140, 80)]], "InfoMessage03", this);
	BindTrigger(CreateObject(TIM1, 0, 0, NO_OWNER), TRIGGER_Find, [[Find_ID(PCMK), Find_InRect(460, 40, 80, 30)]], "InfoMessage04", this);
	
	// TRIGGER_Removed
	var bbtp = CreateObject(BBTP, 426, 228, NO_OWNER);
	bbtp->Set(-70,-1);
	BindTrigger(bbtp, TRIGGER_Removed, [], "InfoMessage02", this);
}

public func ExitArrived(object pByObj) 
{
	GainTutorialAccess(1);  
	return _inherited(pByObj); 
}

public func WellDone1(array pObjects)
{
	for(var obj in pObjects)
	{
		Sound("Cheer.ogg", true, 0, 100, GetOwner(obj));
		TutorialMessage("$WellDone1$", GetOwner(obj)+1);
	}
}

public func InfoMessage02(array pObjects)
{
	TutorialMessage("$Txt02$");
  wait(36*6, true);
}

public func InfoMessage03(array pObjects)
{
	TutorialMessage("$Txt03$");
}

public func InfoMessage04(array pObjects)
{
	TutorialMessage("$Txt04$");
}

/*public func Script5()
{
	if(FindObject2(Find_ID(PCMK),Find_InRect(640,240,160,100)))
  {
    Sound("Cheer.ogg");
    TutorialMessage("$WellDone1$");
    // Hier gehört btw ein Ausgang hin.
  }
  if(!FindObject(BBTP) && !sect1told)
  {
    TutorialMessage("$Txt02$");
    wait(36*6, true);
  }
  if(FindObject2(Find_ID(PCMK),Find_InRect(290,270,140,80)))
    TutorialMessage("$Txt03$");
  if(FindObject2(Find_ID(PCMK),Find_InRect(460,40,80,30)))
    TutorialMessage("$Txt04$");
  
	FakeGoto(4);
}*/

