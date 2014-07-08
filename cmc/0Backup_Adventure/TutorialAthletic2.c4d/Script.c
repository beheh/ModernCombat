/*-- Athletic 2 --*/

#strict 2

#include TSYS

public func UsedRules() { return [NOFD, NOFF]; }
public func SectionName() { return "Athletic2"; }
public func Condition(object pByObj) { return HasTutorialAccess(1, GetOwner(pByObj)); }

local terminals;

public func Initialize()
{
	SetPlayList("CMC_Showtime.ogg");
  Music("CMC_Showtime.ogg",1);
  
  // Ausgang
  CreateObject(EXIT, 1750, 310, NO_OWNER);
  
  // Nachricht
  Sound("RadioConfirm*.ogg");
  TutorialMessage("$Txt01$");
  
  terminals = [];
  
  // Terminals
  terminals[0] = CreateObject(TMNL, 625, 1050, -1);
  terminals[1] = CreateObject(TMNL, 820, 870, -1);
  terminals[2] = CreateObject(TMNL, 1095, 690, -1);
  terminals[3] = CreateObject(TMNL, 1790, 590, -1);
  terminals[4] = CreateObject(TMNL, 1790, 510, -1);
  
  return _inherited(...);
}

public func InitializePlayer(int iPlr)
{
	if(GetType(arRespawn[iPlr]) != C4V_Array)
		arRespawn[iPlr] = [];
	
	SetRespawnPosition(485, 1020, iPlr);
	SetArrow(850, 855, iPlr);
	
	for(var i = 0; i < 5; i++)
		terminals[i]->SetNewInfo(iPlr, Format("$Info%02d$", i));
	
	return _inherited(iPlr, ...);
}

public func CreateInterior()
{//Terminals
  CreateObject(TMNL, 625, 1050, -1)->SetNewInfo("$Info01$");
  CreateObject(TMNL, 820, 870, -1)->SetNewInfo("$Info02$");
  CreateObject(TMNL, 1095, 690, -1)->SetNewInfo("$Info03$");
  CreateObject(TMNL, 1790, 590, -1)->SetNewInfo("$Info04$");
  CreateObject(TMNL, 1790, 510, -1)->SetNewInfo("$Info05$");
	//Selbstschussanlage und Konsole
  var tmp = CreateObject (SEGU, 1760, 338, -1);
  tmp->Arm(MISA);
  tmp->TurnOn();
  CreateObject(CONS, 1715, 585, -1)->Set(tmp);

  //Sprungschanzen
  CreateObject(JMPD, 725, 1050, -1)->Set(90, 10);
  CreateObject(JMPD, 1230,505, -1)->Set(60, 65);

  //Orientierungslichter
  CreateObject(OLGH, 485, 1030, -1)->Set(3, 15, 1, 1, 40);
  CreateObject(OLGH, 625, 1050, -1)->Set(2, 15, 1, 1, 40);
  CreateObject(OLGH, 725, 1050, -1)->Set(2, 15, 1, 1, 40);

  CreateObject(OLGH, 930, 690, -1)->Set(3, 15, 1, 1, 40);
  CreateObject(OLGH, 1095, 690, -1)->Set(3, 15, 1, 1, 40);
  CreateObject(OLGH, 1260, 690, -1)->Set(3, 15, 1, 1, 40);

  CreateObject(OLGH, 1565, 310, -1)->Set(2, 15, 1, 1, 40);
  CreateObject(OLGH, 1755, 310, -1)->Set(3, 15, 1, 1, 40);

  //Glasscheibe
  CreateObject(_WIN, 1210, 880, -1);

  //Metallkiste
  CreateObject(MWCR, 920, 690, -1);
  CreateObject(MWCR, 940, 690, -1);
  
  return true;
}

public func CreateDecoration()
{
	//Geländer
  CreateObject(RAI1, 403, 1030, -1)->SetRail([1,3,1,3,1,3,1,3,1,3,1,3,1]);
  CreateObject(RAI1, 803, 870, -1)->SetRail([1,3,1,3,1,3,1]);
  CreateObject(RAI1, 1503, 670, -1)->SetRail([1,3,1,3,1,3,1]);
  CreateObject(RAI1, 1693, 310, -1)->SetRail([1,3,1,3,1,3,1,3,1,3,1]);

  //Regale
  CreateObject(FRAM, 1620, 510, -1);
  CreateObject(FRAM, 1640, 510, -1);

  //Stahlbrücken
  CreateObject(_HBR, 1015, 702, -1);
  CreateObject(_HBR, 1015, 792, -1);
  CreateObject(_HBR, 1175, 702, -1);

  //Sounds

  //Hallen
  CreateObject(SE4K, 590, 960, -1)->Set("Interior*.ogg",665,105);
  CreateObject(SE4K, 1175, 740, -1)->Set("Interior*.ogg",670,105);
  CreateObject(SE4K, 1265, 450, -1)->Set("Interior*.ogg",665,105);
  CreateObject(SE4K, 1685, 550, -1)->Set("Interior*.ogg",670,105);
  CreateObject(SE4K, 1665, 330, -1)->Set("Interior*.ogg",665,105);
}

public func CreateTriggers()
{
	// TRIGGER_Find
	BindTrigger(CreateObject(TIM1, 0, 0, NO_OWNER), TRIGGER_Find, [[Find_ID(PCMK), Find_InRect(810, 830, 40, 80)]], "InfoMessage02", this);
	BindTrigger(CreateObject(TIM1, 0, 0, NO_OWNER), TRIGGER_Find, [[Find_ID(PCMK), Find_InRect(800, 840, 100, 30), Find_Action("Crawl")]], "InfoMessage03", this);
	BindTrigger(CreateObject(TIM1, 0, 0, NO_OWNER), TRIGGER_Find, [[Find_ID(PCMK), Find_InRect(1140, 790, 80, 80)]], "InfoMessage04", this);
	BindTrigger(CreateObject(TIM1, 0, 0, NO_OWNER), TRIGGER_Find, [[Find_ID(PCMK), Find_InRect(950, 760, 30, 20)]], "InfoMessage05", this);
	BindTrigger(CreateObject(TIM1, 0, 0, NO_OWNER), TRIGGER_Find, [[Find_ID(PCMK), Find_Action("Scale"), Find_InRect(830, 740, 50, 50)]], "InfoMessage06", this);
	BindTrigger(CreateObject(TIM1, 0, 0, NO_OWNER), TRIGGER_Find, [[Find_ID(PCMK), Find_InRect(1220, 600, 80, 90)]], "InfoMessage07", this);
	BindTrigger(CreateObject(TIM1, 0, 0, NO_OWNER), TRIGGER_Find, [[Find_ID(PCMK), Find_InRect(1500, 640, 100, 30)]], "InfoMessage08", this);
	BindTrigger(CreateObject(TIM1, 0, 0, NO_OWNER), TRIGGER_Find, [[Find_ID(PCMK), Find_InRect(1610, 450, 40, 60)]], "InfoMessage09", this);
	BindTrigger(CreateObject(TIM1, 0, 0, NO_OWNER), TRIGGER_Find, [[Find_ID(PCMK), Find_InRect(1760, 470, 40, 60)]], "InfoMessage10", this);
	BindTrigger(CreateObject(TIM1, 0, 0, NO_OWNER), TRIGGER_Find, [[Find_ID(PCMK), Find_InRect(1500,250,300,100)]], "WellDone1", this);
}

public func InfoMessage02(array pObjects)
{
	for(var obj in pObjects)
	{
		var plr = GetOwner(obj);
		Sound("RadioConfirm*.ogg", true, 0, 100, plr);
		TutorialMessage("$Txt02$", plr+1);
		SetRespawnPosition(835, 860, plr);
	}
	RemoveArrow();
	
	return true;
}

public func InfoMessage03(array pObjects)
{
	for(var obj in pObjects)
	{
		var plr = GetOwner(obj);
		Sound("RadioConfirm*.ogg", true, 0, 100, plr);
		TutorialMessage("$Txt03$", plr+1);
		SetArrow(1155, 855, plr);
	}
	
	return true;
}

public func InfoMessage04(array pObjects)
{
	for(var obj in pObjects)
	{
		var plr = GetOwner(obj);
		Sound("RadioConfirm*.ogg", true, 0, 100, plr);
		TutorialMessage("$Txt04$", plr+1);
		SetRespawnPosition(1170, 860, plr);
		SetArrow(960, 770, plr);
	}
	
	return true;
}

public func InfoMessage05(array pObjects)
{
	for(var obj in pObjects)
	{
		var plr = GetOwner(obj);
		Sound("RadioConfirm*.ogg", true, 0, 100, plr);
		TutorialMessage("$Txt05$", plr+1);
		SetRespawnPosition(965, 770, plr);
		SetArrow(870, 770, plr);
	}
	
	return true;
}

public func InfoMessage06(array pObjects)
{
	for(var obj in pObjects)
	{
		var plr = GetOwner(obj);
		Sound("RadioConfirm*.ogg", true, 0, 100, plr);
		TutorialMessage("$Txt06$", plr+1);
		SetArrow(1250, 665, plr);
	}
	
	return true;
}

public func InfoMessage07(array pObjects)
{
	for(var obj in pObjects)
	{
		var plr = GetOwner(obj);
		Sound("RadioConfirm*.ogg", true, 0, 100, plr);
		TutorialMessage("$Txt07$", plr+1);
		SetRespawnPosition(1250, 680, plr);
		SetArrow(1240, 480, plr);
	}
	
	return true;
}

public func InfoMessage08(array pObjects)
{
	for(var obj in pObjects)
	{
		var plr = GetOwner(obj);
		Sound("RadioConfirm*.ogg", true, 0, 100, plr);
		TutorialMessage("$Txt08$", plr+1);
		SetRespawnPosition(1550, 660, plr);
		SetArrow(1630, 485, plr);
	}
	
	return true;
}

public func InfoMessage09(array pObjects)
{
	for(var obj in pObjects)
	{
		var plr = GetOwner(obj);
		Sound("RadioConfirm*.ogg", true, 0, 100, plr);
		TutorialMessage("$Txt09$", plr+1);
		SetRespawnPosition(1595, 580, plr);
		SetArrow(1790, 485, plr);
	}
	
	return true;
}

public func InfoMessage10(array pObjects)
{
	for(var obj in pObjects)
	{
		var plr = GetOwner(obj);
		Sound("RadioConfirm*.ogg", true, 0, 100, plr);
		TutorialMessage("$Txt09$", plr+1);
		SetRespawnPosition(1815, 500, plr);
		SetArrow(1770, 280, plr);
	}
	
	return true;
}

public func WellDone1(array pObjects)
{
	for(var obj in pObjects)
	{
		var plr = GetOwner(obj);
		Sound("Cheer.ogg", true, 0, 100, plr);
		TutorialMessage("$WellDone1$", plr+1);
	}
	
	return true;
}
