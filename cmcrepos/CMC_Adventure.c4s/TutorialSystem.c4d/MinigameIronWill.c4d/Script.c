/*-- Lobby --*/

#strict 2

#include TSYS

public func UsedRules() { return [NOFD, NOFF, NOAM]; } 
public func SectionName() { return "IronWill"; } 
public func HideInMenu() { return true; } 
public func IsCMCMinigame() { return true; }

local container;
local blackhawk;

public func Initialize()
{  
	container = CreateObject(TIM1, 0, 0, NO_OWNER);
  g_Gamemaster->Enter(container);
  MusicLevel(0);
	Music("@CMC_Valkyries.ogg");
	blackhawk = CreateObject(BKHK, 200, 150, -1);
	DoDamage(190, blackhawk); // Schaden!
	LocalN("BKHK_LockSeats", blackhawk) = true; // Jeder bekommt Passagiersitz. Sitzwechsel unmöglich.
	blackhawk->MGStartBlackhawk(DIR_Right, 100, 0);
	SetWind(0);
	
  return _inherited(...);
}

public func InitializePlayer(int iPlr)
{
	if(GetType(arRespawn[iPlr]) != C4V_Array)
		arRespawn[iPlr] = [];
	
	SetRespawnPosition(1, 1, iPlr);
	
	return _inherited(iPlr, ...);
}

public func OnClonkRelaunch(object pCrew) 
{ 
	if(blackhawk)
		Enter(blackhawk, pCrew); 
	else
		Enter(container, pCrew);
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
	var players = [];
	var hX;
	for(var obj in FindObjects(Find_ID(PCMK), Find_NoContainer()))
	{
		if(GetX(obj) == hX)
			players[GetLength(players)] = GetOwner(obj);
		else if(GetX(obj) > hX)
			players = [GetOwner(obj)];
	}
	
	g_aLastRoundWinners = players;
	return LoadMinigame(MLBW);
}

local musiclevel;

public func Script1()
{
	musiclevel += 5;
	if(musiclevel <= 100)
		MusicLevel(musiclevel);
	
	if(blackhawk)
	{
		blackhawk->DoRotation(1);
		if(!(LocalN("rotation", blackhawk) % 5))
			LocalN("throttle", blackhawk) += 1;
	}
	else
	{
		var effect = DelayedCall("EvalRound", 5);
		EffectVar(3, 0, effect) = this;
	}
	
	return FakeGoto(0);
}

