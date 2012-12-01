/*-- Lobby --*/

#strict 2

#include TSYS

public func UsedRules() { return [NOFD, NOAM]; } 
public func SectionName() { return "RandomMissiles"; } 
public func HideInMenu() { return true; } 
public func IsCMCMinigame() { return true; }

local container;

public func Initialize()
{ 
  container = CreateObject(TIM1, 300, 0, -1);
  g_Gamemaster->Enter(container);
  
 	BindTrigger(container, TRIGGER_Find, [[Find_ID(MISL), container->Find_InRect(0, 0, 100, LandscapeHeight())]], "ControlMissile", this);
  
  return _inherited(...);
}

public func ControlMissile(array arMissile)
{
	var door = doors[Random(GetLength(doors))];
	var crosshair = LocalN("crosshair", peacemaker);
	var angle = -(Angle(GetX(arMissile[0]), GetY(arMissile[0]), GetX(door), GetY(door))-270);
	
	//Log("DoorCoordinates: (%d|%d), angle: %d, crosshair_Angle: %d, Angle_Door: %d, MissileCoordinates: (%d|%d)", GetX(door), GetY(door), angle, crosshair->GetAngle(), Angle(GetX(arMissile[0]), GetY(arMissile[0]), GetX(door), GetY(door)), GetX(arMissile[0]), GetY(arMissile[0]));
	
	peacemaker->DoAiming(angle);
	ScheduleCall(peacemaker, "StopAiming", 60);
	ScheduleCall(this, "MissileDown", 150);
	
	return true;
}

public func MissileDown()
{
	for(var door in doors)
		door->~Close();
	
	for(var obj in FindObjects(Find_ID(PCMK)))
		RemoveEffect("CMCWait", obj);
	
	if(ObjectCount2(Find_ID(PCMK), Find_NoContainer(), Find_Not(Find_Owner(-1))) <= 1)
		return EvalRound();
	else
		StartGame();
	
	peacemaker->~ControlThrow();
	
	return true;
}

public func InitializePlayer(int iPlr)
{
	if(GetType(arRespawn[iPlr]) != C4V_Array)
		arRespawn[iPlr] = [];
	
	SetRespawnPosition(40, 350, iPlr);
	
	return _inherited(iPlr, ...);
}

public func OnClonkRelaunch(object pCrew, bool fInitPlr) 
{ 
	if(!fInitPlr)
		pCrew->Enter(container);
	else
		DoEnergy(-149, pCrew);
	
	return true;
}

local peacemaker, doors;

public func CreateInterior()
{
	doors = [];
	doors[0] = CreateObject(SLDR, 195, 330, -1);
	doors[1] = CreateObject(SLDR, 195, 250, -1);
	doors[2] = CreateObject(SLDR, 195, 170, -1);
	doors[3] = CreateObject(SLDR, 195,  90, -1);
	for(var door in doors)
		door->Lock();
	
	CreateObject(LADR, 150, 360, -1)->Set(36);

	peacemaker = CreateObject(PCMK, 690, 230, -1);
	var rtlr = CreateContents(RTLR, peacemaker);
	DoAmmo2(rtlr->GetSlot(), MIAM, 9999, rtlr);
	peacemaker->SetDir(DIR_Left);
	Indestructible(peacemaker);
	

  return true;
}

public func CreateOptionalFeatures()
{
  //Hintergrund.
  CreateObject(BD05, LandscapeWidth()/2, LandscapeHeight()+1, -1);
}


public func EvalRound()
{
	var players = [];
	for(var obj in FindObjects(Find_ID(PCMK), Find_NoContainer(), Find_Not(Find_Owner(-1))))
		players[GetLength(players)] = GetOwner(obj);
	
	RemoveObject(peacemaker);
	
	g_aLastRoundWinners = players;
	return LoadMinigame(MLBW);
}

public func LoadingFinished()
{
	Wait(35*5, -1, true, "StartGame()");
	return true;
}

public func StartGame()
{
	return StartTimer(35*15, "StartMissile", this, false);
}

public func StartMissile()
{
	for(var obj in FindObjects(Find_ID(PCMK), Find_NoContainer(), Find_InRect(0, 0, 169, LandscapeHeight())))
		Kill(obj);
	
	Wait(-1, -1);
	
	for(var obj in FindObjects(Find_ID(PCMK), Find_NoContainer(), Find_InRect(170, 0, 100, LandscapeHeight())))
	{
		// Niemand springt oder hangelt.
		SetXDir(0, obj); 
		SetYDir(5, obj);
		
		// Es soll auch keiner laufen...
		SetComDir(COMD_Down, obj);
	}
	
	for(var door in doors)
		door->~Open();
	
	peacemaker->~StartAiming();
	ScheduleCall(peacemaker, "ControlThrow", 90);
	
	return true;
}

//public func StartGame() { FakeScriptGo(true); NextPlayer(); }



