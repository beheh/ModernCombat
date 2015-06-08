/*-- Lobby --*/

#strict 2

#include TSYS

public func UsedRules() { return [NOFD, NOFF, NOAM, FDMG]; } 
public func SectionName() { return "Basejumper"; } 
public func HideInMenu() { return true; } 
public func IsCMCMinigame() { return true; }

local container;
local artilleries, blackhawks;
local data;

public func Initialize()
{	
  artilleries = [];
  blackhawks = [];
  var length = Random(200)+100;
	var x = Random(LandscapeWidth()-length*2)+length;
	var height = 20;
	var y = 1610;
	DrawMaterialQuad("Wall", x, y, x, y+height, x+length, y+height, x+length, y);
	data = [x, y, length, height];
	
	container = CreateObject(TIM1, x, y-height+1, NO_OWNER);
  g_Gamemaster->Enter(container);
  
  BindTrigger(container, TRIGGER_Find, [[Find_ID(PCMK), Find_NoContainer(), container->Find_InRect(0, 0, length, height), Find_Action("Walk")]], "EvalRound", this, false, true, 1);
	var trigger;
	BindTrigger(trigger = CreateObject(TIM1, 0, 50, -1), TRIGGER_Find, [[Find_ID(PCMK), Find_NoContainer(), trigger->Find_InRect(0, 0, LandscapeWidth(), 100)]], "ActParachute", this);
	
  return _inherited(...);
}

public func InitializePlayer(int iPlr)
{
	if(GetType(arRespawn[iPlr]) != C4V_Array)
		arRespawn[iPlr] = [];
	
	SetRespawnPosition(445, 20, iPlr);
	
	return _inherited(iPlr, ...);
}

public func OnClonkRelaunch(object pCrew) 
{
	CreateObject(PPAR, 0, 0, GetOwner(pCrew))->Activate(pCrew);
	// Fallschirmrucksack geben.
}

public func CreateInterior()
{
	var rnd = Random(4)+4;
	for(var i = 0; i < rnd; i++)
		CreateBlackhawks();
		
	for(var i = 0; i < rnd; i++)
		CreateArtillery();
	
  return true;
}

public func LoadingFinished()
{
	//FakeScriptGo(true);
	Wait(35*5, -1, true, "StartGame()");
	AddEffect("BasejumperBlackhawks", this, 101, 3, this);
	//StartTimer(35*20, "EvalRound", this, false);
	
	return true;
}

public func StartGame()
{
	var pos = data[0]+Random(data[2]);
	var marker = CreateObject(ARW_, pos, 40, -1);
	return true;
}

public func FxBasejumperBlackhawksTimer(object target, int nr)
{
	// Blackhawks bewegen
	for(var blackhawk in blackhawks)
	{
		if(Inside(GetX(blackhawk), 0, 200))
			blackhawk->SetAutopilot(0, LandscapeWidth()-180, GetY(blackhawk));
		else if(Inside(GetX(blackhawk), LandscapeWidth()-200, LandscapeWidth()))
			blackhawk->SetAutopilot(0, 180, GetY(blackhawk));
	}
}

public func EvalRound(array aObjects)
{
	if(!aObjects)
		aObjects = [];

	var players = [];
	for(var obj in aObjects)
		players[GetLength(players)] = GetOwner(obj);
	
	g_aLastRoundWinners = players;
	return LoadMinigame(MLBW);
}

public func CreateArtillery()
{
	var randomY = Random(LandscapeHeight()-600)+500;
	if(!FindObject2(Find_Or(Find_ID(ATBY), Find_ID(BKHK)), Find_InRect(0,randomY-40, LandscapeWidth(), 80))) // Keine Blackhawks/Artilleriebatterien
	{
		var rdir = Random(2);
		rdir = rdir + (!rdir * -1);
		var x;
		if(rdir == 1)
			x = 50;
		if(rdir == -1)
			x = LandscapeWidth()-50;
		
		var artillery = CreateObject(ATBY, x, randomY, -1);
		artilleries[GetLength(artilleries)] = artillery;
		var aX = GetX(artillery); var aY = GetY(artillery);
		artillery->DrawMaterialQuad("Wall", aX-25, aY+11, aX-25, aY+21, aX+25, aY+21, aX+25, aY+11);
		//DrawMaterialQuad("Wall", GetX()-25, GetY()+11, GetX()-25, GetY()+21, GetX()+25, GetY()+21, GetX()+25, GetY()+11);
		artillery->SetRotation(RandomX(40, 100)*rdir);
		LocalN("plr_locked", artillery) = true;
		AddEffect("ArShoot", artillery, 101, 70, this);
		//artilleries[GetLength(artilleries)]->SetMinigame(Minigame(), rdir);
		
		return true;
	}
}

public func FxArShootTimer(object target) { target->~Shoot(); }

public func CreateBlackhawks()
{
	var randomY = Random(LandscapeHeight()-200)+100;
	if(!FindObject2(Find_ID(BKHK), Find_InRect(0, randomY-60, LandscapeWidth(), 40))) // Keine Blackhawks und tief genug.
	{
		var blackhawk = CreateObject(BKHK, LandscapeWidth()/2, randomY, -1);
		blackhawks[GetLength(blackhawks)] = blackhawk;
		
		blackhawk->MGStartBlackhawk(DIR_Left, 100, 0);
		var rnd = Random(2);
		if(!rnd)
			blackhawk->SetAutopilot(0, 180, GetY(blackhawk));  
		if(!--rnd)
			blackhawk->SetAutopilot(0, LandscapeWidth()-180, GetY(blackhawk));
			
		SetController(-1, blackhawk);
		LocalN("rotation", blackhawk) = 5;
		Indestructible(blackhawk);
		AddEffect("AIRocketShoot", blackhawk, 101, Random(300)+150, blackhawk);
		
		return true;
	}
	
	return CreateBlackhawks();
}

public func ActParachute(array aObjects)
{
	for(var obj in aObjects)
	{
		if(LocalN("parachute", obj))
			continue;
		
		var parachute = obj->~HasGear(0, PPAR);
		parachute->ControlUpDouble();
	}
	
	return true;
}


