/*-- Lobby --*/

#strict 2

#include TSYS

public func UsedRules() { return [NOFD, NOFF, NOAM]; } 
public func SectionName() { return "ArtilleryDart"; } 
public func HideInMenu() { return true; } 
public func IsCMCMinigame() { return true; }

local container;
local player_won, player_fail, iteration, player, playerindex;

public func Initialize()
{ 
  player_won = [];
  player_fail = [];
  container = CreateObject(TIM1);
  g_Gamemaster->Enter(container);
  
  return _inherited(...);
}

public func InitializePlayer(int iPlr)
{
	if(GetType(arRespawn[iPlr]) != C4V_Array)
		arRespawn[iPlr] = [];
	
	SetRespawnPosition(50, 100, iPlr);
	
	return _inherited(iPlr, ...);
}

public func OnClonkRelaunch(object pCrew) { pCrew->Enter(container); }

local artillery;

public func CreateInterior()
{
	artillery = CreateObject(ATBY, LandscapeWidth()/2, 300, -1);
	Indestructible(artillery);
  return true;
}

public func LoadingFinished()
{
	playerindex = -1;
	Wait(35*5, -1, true, "StartGame()");
	
	return true;
}

public func StartGame() { FakeScriptGo(true); NextPlayer(); }

public func NextPlayer()
{
	no_end = true;

	if(++playerindex < GetPlayerCount())
		player = GetPlayerByIndex(playerindex);
	else
		return EvalIteration();
		
	if(GetIndexOf(player, player_fail) > -1)
		return NextPlayer();
	
	GetCrew(player)->Exit();
	GetCrew(player)->SetPosition(LandscapeWidth()/2, 250);
	Indestructible(GetCrew(player));
	
	RemoveAll(BIRD);
	RemoveAll(ABLT);
	BindTrigger(CreateObject(BIRD, Random(LandscapeWidth()), 50, -1), TRIGGER_Death, [], "EvalPlayer", this);
	artillery->NewAmmo();
	if(GetEffect("ArtilleryDartTmr", this))
		RemoveEffect("ArtilleryDartTmr", this);
	if(GetEffect("IntScheduleCall", artillery))
		RemoveEffect("IntScheduleCall", artillery);
		
	LocalN("iCooldown", artillery) = 0;
	
	AddEffect("ArtilleryDartTmr", this, 1, 20*35, this);
	
	return true;
}

public func EvalPlayer()
{
	var bird;
	if(!(bird = FindObject(BIRD)) || !(bird->GetAlive()))
		player_won[GetLength(player_won)] = player;
	else
		player_fail[GetLength(player_fail)] = player;
	
	GetCrew(player)->Enter(container);
	NextPlayer();
	
	return true;
}

public func FxArtilleryDartTmrStop(object target, int nr, int reason)
{
	if(LocalN("loaded_ammo", artillery) == 3 && !LocalN("iCooldown", artillery))
		artillery->ControlThrow();
	
	return true;
}

public func EvalIteration()
{
	iteration++;

	if(iteration == 3)
	{
		g_aLastRoundWinners = player_won;
		return LoadMinigame(MLBW);
	}
	
	player_won = [];
	playerindex = -1;
	
	NextPlayer();
}

local no_end, projectile;

public func LastProjectile(object pProjectile)
{
	no_end = false;
	projectile = pProjectile;
	return true;
}

public func Script1()
{
	if(!no_end && !projectile)
		EvalPlayer();
		
	FakeGoto(0);
}


