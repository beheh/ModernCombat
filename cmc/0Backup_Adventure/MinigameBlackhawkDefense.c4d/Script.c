/*-- Lobby --*/

#strict 2

#include TSYS

public func UsedRules() { return [NOFD, NOFF, NOAM]; } 
public func SectionName() { return "BlackhawkDefense"; } 
public func HideInMenu() { return true; } 
public func IsCMCMinigame() { return true; }

local container;
local blackhawks;
local plrscore;

public func Initialize()
{
	container = CreateObject(TIM1, 0, 0, NO_OWNER);
  g_Gamemaster->Enter(container);
	blackhawks = [];
	plrscore = [];
	
  return _inherited(...);
}

public func InitializePlayer(int iPlr)
{
	if(GetType(arRespawn[iPlr]) != C4V_Array)
		arRespawn[iPlr] = [];
	
	SetRespawnPosition(445, 20, iPlr);
	
	return _inherited(iPlr, ...);
}

public func OnClonkRelaunch(object pCrew, bool fInitPlr) 
{
	if(!fInitPlr)
		pCrew->Enter(container);
	else
	{
		//In ein stationäres Geschütz packen.
		var rX = Random(110)+20;
		SetPosition(rX, 200, pCrew);
		var gun_emplacement = CreateObject(GNET, rX, 200, -1);
		gun_emplacement->~Arm(SATW);
		gun_emplacement->~ActivateEntrance(pCrew);
	}
}

public func LoadingFinished()
{
	//FakeScriptGo(true);
	Wait(35*5, -1, true, "StartGame()");
	AddEffect("BasejumperBlackhawks", this, 101, 3, this);
	
	return true;
}

public func StartGame()
{
	FakeScriptGo(true);
	return true;
}

public func FxBasejumperBlackhawksTimer(object target, int nr)
{
	// Blackhawks bewegen
	for(var blackhawk in blackhawks)
	{
		if(!blackhawk)
			continue;
		
		if(Inside(GetX(blackhawk), 0, 200))
			blackhawk->SetAutopilot(0, LandscapeWidth()-180, 100);
		else if(Inside(GetX(blackhawk), LandscapeWidth()-200, LandscapeWidth()))
			blackhawk->SetAutopilot(0, 180, 100);
	}
}

public func EvalRound()
{
	var players = [];
	for(var i = 0; i < GetLength(plrscore); i++)
		if(plrscore[i] > 5)
			players[GetLength(players)] = i;
	
	g_aLastRoundWinners = players;
	return LoadMinigame(MLBW);
}

public func Script1()
{
	if(ObjectCount(BKHK) < 4)
		CreateBlackhawk();
	
	return FakeGoto(0);
}

public func CreateBlackhawk()
{
	var blackhawk = CreateObject(BKHK, Random(LandscapeWidth()-400)+200, -100, -1);
	blackhawks[GetLength(blackhawks)] = blackhawk;
	
	blackhawk->MGStartBlackhawk(DIR_Left, 100, 0);
	var rnd = Random(2);
	if(!rnd)
		blackhawk->SetAutopilot(0, 180, 100);  
	if(!--rnd)
		blackhawk->SetAutopilot(0, LandscapeWidth()-180, 100);
		
	SetController(-1, blackhawk);
	LocalN("rotation", blackhawk) = 5;
	DoDamage(199, blackhawk);
	
	return true;
}

public func BlackhawkDestroyed(object pByObj)
{
	plrscore[GetController(pByObj)]++;
	for(var score in plrscore)
		if(score > 5)
			return EvalRound();

	return true;
}

