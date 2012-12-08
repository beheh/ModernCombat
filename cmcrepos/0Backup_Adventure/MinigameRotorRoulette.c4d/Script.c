/*-- Lobby --*/

#strict 2

#include TSYS

public func UsedRules() { return [NOFD, NOFF, NOAM]; } 
public func SectionName() { return "RotorRoulette"; } 
public func HideInMenu() { return true; } 
public func IsCMCMinigame() { return true; }

local container;
local blackhawk;

public func Initialize()
{  
	container = CreateObject(TIM1, 170, 220, NO_OWNER);
  g_Gamemaster->Enter(container);
  BindTrigger(container, TRIGGER_Find, [[Find_ID(PCMK), Find_NoContainer(), container->Find_InRect(0, 0, 40, 30)]], "EvalRound", this, false, true, 1);
  
	blackhawk = CreateObject(BKHK, 200, 150, -1);
	blackhawk->MGStartBlackhawk(DIR_Right, 100, 0);
	var effect = AddEffect("WindChanger", 0, 101, 100, 0, GetID());
	EffectVar(0, 0, effect) = this;
	
  return _inherited(...);
}

public func FxWindChangerTimer(object target, int nr)
{
	if(g_TutorialObject != EffectVar(0, target, nr))
		return -1;
	
	SetWind(RandomX(-100, 100));
}

public func InitializePlayer(int iPlr)
{
	if(GetType(arRespawn[iPlr]) != C4V_Array)
		arRespawn[iPlr] = [];
	
	SetRespawnPosition(740, 90, iPlr);
	
	return _inherited(iPlr, ...);
}

public func OnClonkRelaunch(object pCrew) 
{ 
	
}

public func CreateInterior()
{
  return true;
}

public func LoadingFinished()
{
	FakeScriptGo(true);
	Wait(35*5, -1, true, "StartGame()");
	
	return true;
}

public func StartGame()
{
	StartTimer(35*20, "EvalRound", this, false);
	return true;
}

public func EvalRound(array aObjects)
{
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

public func Script1()
{
	if(!Inside(GetX(blackhawk), 300, 550) || !Inside(GetY(blackhawk), 170, 470))
		blackhawk->SetAutopilot(0, 420, 240);
	
	return FakeGoto(0);
}

