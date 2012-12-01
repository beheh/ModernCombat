/*-- Lobby --*/

#strict 2

#include TSYS

public func UsedRules() { return [NOFD, NOAM]; } 
public func SectionName() { return "CrawlForYourLife"; } 
public func HideInMenu() { return true; } 
public func IsCMCMinigame() { return true; }

local container;
local peacemaker;

public func Initialize()
{  
	container = CreateObject(TIM1, 895, 10, NO_OWNER);
  g_Gamemaster->Enter(container);
  BindTrigger(container, TRIGGER_Find, [[Find_ID(PCMK), Find_NoContainer(), container->Find_InRect(0, 0, 40, 30), Find_Action("Walk"), Find_Not(Find_Owner(-1))]], "EvalRound", this, false, true, 1);
	
	peacemaker = CreateObject(PCMK, 890, 40, NO_OWNER);
	peacemaker->SetDir(DIR_Left);
	Indestructible(peacemaker);
	
	var rifle = CreateContents(ASTR, peacemaker);
	rifle->CycleFT(1);
	DoAmmo2(rifle->GetSlot(), STAM, 9999, rifle);
	
  return _inherited(...);
}

public func InitializePlayer(int iPlr)
{
	if(GetType(arRespawn[iPlr]) != C4V_Array)
		arRespawn[iPlr] = [];
	
	SetRespawnPosition(10, 30, iPlr);
	
	return _inherited(iPlr, ...);
}

public func OnClonkRelaunch(object pCrew) 
{ 
	SetFoW(false, GetOwner(pCrew));
	return DoEnergy(-149, pCrew); // InstaDeath rulez.
}

public func CreateInterior()
{
  return true;
}

public func LoadingFinished()
{
	Wait(35*5, -1, true, "StartGame()");
	
	return true;
}

public func StartGame()
{
	FakeScriptGo(true);
	return true;
}

public func EvalRound(array aObjects)
{
	if(peacemaker) // Der soll nicht bei Sektionswechsel mitgenommen werden!
		RemoveObject(peacemaker);

	// FoW wieder für alle Spieler aktivieren
	for(var i = 0; i < GetPlayerCount(); i++)
		SetFoW(true, GetPlayerByIndex(i));

	if(!aObjects)
		aObjects = [];

	var players = [];
	for(var obj in aObjects)
	{
		players[GetLength(players)] = GetOwner(obj);
	}
	
	g_aLastRoundWinners = players;
	return LoadMinigame(MLBW);
}

public func Script7()
{
	LocalN("spread", peacemaker) = 0;
	var rnd = Random(3);
	if(!rnd)
		peacemaker->StartAiming();
	if(!--rnd)
		peacemaker->StartSquatAiming();
	if(!--rnd)
	{
		peacemaker->StartCrawling();
		ScheduleCall(peacemaker, "StartAiming",55);
		ScheduleCall(peacemaker, "DoAiming", 56, 0, 1);
		ScheduleCall(peacemaker, "StopCrawling",61);
	}
	ScheduleCall(peacemaker, "ControlThrow", 60);
	ScheduleCall(peacemaker, "StopAiming", 62);
	
	return FakeGoto(0);
}

