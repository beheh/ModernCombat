/*-- Riot Shield --*/

#strict 2

#include TSYS

public func UsedRules() { return [NOFD, NOFF]; }
public func SectionName() { return "RiotShield"; }

public func Initialize()
{
	SetPlayList("CMC_Showtime.ogg");
  Music("CMC_Showtime.ogg",1);
	
  // Ausgang
  CreateObject(EXIT, 220, 380, NO_OWNER);
  
  return _inherited(...);
}

public func InitializePlayer(int iPlr)
{
	if(GetType(arRespawn[iPlr]) != C4V_Array)
		arRespawn[iPlr] = [];
	
	SetRespawnPosition(1810, 920, iPlr);
	
	return _inherited(iPlr, ...);
}

public func OnClonkRelaunch(object pCrew)
{
	CreateObject(PARA, 0, 0, GetOwner(pCrew))->Set(pCrew);
}

public func CreateInterior()
{
	//Selbstschussanlage
  var tmp = CreateObject (SEGU, 1022, 1035, -1);
  tmp ->SetR(90);
  tmp ->Arm(MISA);
  tmp ->TurnOn();

  //Sprungschanzen
  CreateObject(JMPD, 1550, 990, -1)->Set(150, -30);
  CreateObject(JMPD, 920, 780, -1)->Set(110, 10);
  CreateObject(JMPD, 790, 540, -1)->Set(90, 40);

  //Kisten
  CreateObject(WCR2, 920, 970, -1);
  CreateObject(WCR2, 850, 910, -1);
  CreateObject(WCR2, 860, 840, -1);

  CreateObject(WCR2, 1080, 510, -1);

  CreateObject(WCR2, 1050, 360, -1);
  CreateObject(WCR2, 1010, 360, -1);
  CreateObject(WCR2, 970, 360, -1);
  CreateObject(WCR2, 930, 360, -1);
  CreateObject(WCR2, 890, 360, -1);
  CreateObject(WCR2, 850, 360, -1);

  CreateObject(WCR2, 220, 380, -1);
  CreateObject(WCR2, 220, 360, -1);

  //Explosivfass
  CreateObject(XBRL, 610, 400, -1)->AutoRespawn(10);

  //Benzinfass
  CreateObject(PBRL, 510, 400, -1)->AutoRespawn(10);

  //Giftfass
  CreateObject(TBRL, 410, 400, -1)->AutoRespawn(10);

  //Leitern
  CreateObject(LADR, 1170, 445, -1)->Set(9);

  //Orientierungslichter
  CreateObject(OLGH, 1810, 1010, -1)->Set(3, 15, 1, 1, 40);
  CreateObject(OLGH, 1665, 990, -1)->Set(2, 15, 1, 1, 40);
  CreateObject(OLGH, 1565, 990, -1)->Set(2, 15, 1, 1, 40);

  CreateObject(OLGH, 625, 1050, -1)->Set(2, 15, 1, 1, 40);
  CreateObject(OLGH, 725, 1050, -1)->Set(2, 15, 1, 1, 40);

  CreateObject(OLGH, 890, 1030, -1)->Set(6, 15, 1, 1, 40);

  CreateObject(OLGH, 900, 780, -1)->Set(3, 15, 1, 1, 40);

  CreateObject(OLGH, 745, 280, -1)->Set(1, 15, 1, 1, 40);

  CreateObject(OLGH, 575, 400, -1)->Set(1, 30, 1, 1, 60);
  CreateObject(OLGH, 475, 400, -1)->Set(1, 30, 1, 1, 60);
  CreateObject(OLGH, 375, 400, -1)->Set(1, 30, 1, 1, 60);

  CreateObject(OLGH, 220, 380, -1)->Set(2, 15, 1, 1, 40);

  //Metallkiste
  CreateObject(MWCR, 300, 400, -1);
  
  return true;
}

public func CreateDecoration()
{
	//Gel�nder
  CreateObject(RAI1, 1733, 1010, -1)->SetRail([1,3,1,3,1,3,1,3,1,3,1,3,1]);
  CreateObject(RAI1, 403, 1030, -1)->SetRail([1,3,1,3,1,3,1,3,1,3,1,3,1]);
  CreateObject(RAI1, 828, 780, -1)->SetRail([1,3,1,3,1,3,1,3,1,3,1,3,1]);

  CreateObject(RAI1, 600, 400, -1)->SetRail([1,3,1,3,1,3,1,3,1,3,1]);
  CreateObject(RAI1, 503, 400, -1)->SetRail([1,3,1]);
  CreateObject(RAI1, 403, 400, -1)->SetRail([1,3,1]);
  CreateObject(RAI1, 289, 400, -1)->SetRail([1,3,1,3,1]);

  //Regale
  CreateObject(FRAM, 325, 1010, -1);
  CreateObject(FRAM, 345, 1010, -1);

  //Sounds

  //Hallen
  CreateObject(SE4K, 1700, 900, -1)->Set("Interior*.ogg",665,105);
  CreateObject(SE4K, 700, 900, -1)->Set("Interior*.ogg",670,105);
  CreateObject(SE4K, 920, 640, -1)->Set("Interior*.ogg",665,105);
  CreateObject(SE4K, 1020, 270, -1)->Set("Interior*.ogg",670,105);
  CreateObject(SE4K, 390, 330, -1)->Set("Interior*.ogg",665,105);
  
  return true; 
}

/* Ablauf */

public func Script1()
{
  Sound("RadioConfirm*.ogg");
  TutorialMessage("$Txt01$");

  SetArrow(1555, 970);
}

public func Script5()
{
	if(FindObject2(Find_ID(PCMK), Find_InRect(400,995,160,35)))
  {
    Sound("RadioConfirm*.ogg");
    TutorialMessage("$Txt02$");

    SetArrow(830, 1010);
  }
  else
    FakeGoto(4);
}

func Script10()
{
  if(FindObject2(Find_ID(PCMK), Find_InRect(800,980,30,50)))
  {
    Sound("Alarm.ogg");
    Sound("RadioConfirm*.ogg");
    TutorialMessage("$Txt03$");

    SetArrow(345, 990);

    CreateObject(RSHL, 335, 1010, -1);
  }
  else
    FakeGoto(9);
}

func Script15()
{
  if(FindObject2(Find_ID(PCMK), FindContents(RSHL)))
  {
    Sound("RadioConfirm*.ogg");
    TutorialMessage("$Txt04$");

    SetArrow(970, 1010);
  }
  else
    FakeGoto(14);
}

func Script20()
{
  if(FindObject2(Find_ID(PCMK), Find_InRect(950,980,30,50)))
  {
    Sound("RadioConfirm*.ogg");
    TutorialMessage("$Txt05$");

    SetArrow(930, 750);
  }
  else
    FakeGoto(19);
}

func Script25()
{
  if(FindObject2(Find_ID(PCMK), Find_InRect(1010,400,60,100)))
  {
    Sound("RadioConfirm*.ogg");
    TutorialMessage("$Txt06$");

    SetArrow(1085, 335);
  }
  else
    FakeGoto(24);
}

func Script30()
{
  if(FindObject2(Find_ID(PCMK), Find_InRect(1060,310,40,40)))
  {
    Sound("RadioConfirm*.ogg");
    TutorialMessage("$Txt07$");

    SetArrow(695, 380);
  }
  else
    FakeGoto(29);
}

func Script35()
{
  if(FindObject2(Find_ID(PCMK), Find_Container(FindObject(ROOM))))
  {
    Sound("Cheer.ogg");
    TutorialMessage("$WellDone3$");
  }
  else
    FakeGoto(34);
}
