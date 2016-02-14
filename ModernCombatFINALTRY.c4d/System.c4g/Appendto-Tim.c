/*-- Spawnen --*/

//Beim Spawnen wird überprüft, ob Objekte "IsSpawnBlocker" den Ort blockieren und zerstören diese. Spawnverzögerung- und Votekick-Mechanik wird in das Spawnen integriert.

#strict 2

#appendto TIM1
#appendto TIM2


/* Spawnverzögerung */

public func FxWaitingObjectStart(object pTarget, int iNr, bool fTemp, string szKillmsg)
{
  if(fTemp)
    return;

  EffectVar(0, pTarget, iNr) = szKillmsg;
  FxWaitingObjectTimer(pTarget, iNr, 0);
  var clonk = FindObject2(Find_Container(this), Find_OCF(OCF_CrewMember));
  FKDT->SetFakeDeathEffects(clonk, GameCall("GetPlayerRespawnTime", GetOwner(clonk)) != FKDT_SuicideTime*35);
}

public func FxWaitingObjectTimer(object pTarget, int iNr, int iTime)
{
  var clonk = FindObject2(Find_Container(this), Find_OCF(OCF_CrewMember));
  var time = GameCall("GetPlayerRespawnTime", GetOwner(clonk));

  if(time-iTime <= 0)
    return -1;

  if(!g_FallbackDeathmenu)
    FKDT->DeathMenu(clonk, this, FKDT_DeathMenu_FullSettings, time-iTime, FKDT_SuicideTime, EffectVar(0, pTarget, iNr), 0, TIM1);
  else
  {
    CloseMenu();
    CreateMenu(FKDT, clonk, this, 0, Format("$RemainingRespawnTime$", (time-iTime)/35+!!((time-iTime)%35)), 0, C4MN_Style_Context, true);
    AddMenuItem("<Platzhalter fuer DeathMenu>", 0, FKDT, clonk);
  }

  //Effekt verzögert löschen (ChangeEffect und ähnliches funktionieren nicht wie gewollt, daher Schedule)
  if(time-iTime < 10)
    Schedule(Format("RemoveEffect(0, Object(%d), %d)", ObjectNumber(pTarget), iNr), time-iTime);

  return true;
}

public func FxWaitingObjectStop(object pTarget, int iNr, bool fTemp)
{
  if(fTemp)
    return;

  var clonk = FindObject2(Find_Container(this), Find_OCF(OCF_CrewMember));
  FKDT->ResetFakeDeathEffects(clonk);
  CloseMenu(clonk);
  GameCall("SetPlayerRespawnTime", GetOwner(clonk), 0);
  
  var info = GameCall("GetPlayerRelaunchInformation", GetOwner(clonk));
  if(!info)
  	info = [];

  GameCall("RelaunchPlayer", GetOwner(clonk), clonk, info[0]);
  return true;
}

public func Suicide(object pTarget)
{
  if(GetEffect("WaitingObject", Contained(pTarget)))
    RemoveEffect("WaitingObject", Contained(pTarget));
}

public func Spawn()
{
  if(GetEffect("WaitingObject", this))
    return;

  return _inherited(...);
}

public func MenuQueryCancel(int iSelection, object pMenuObject)
{
  if(GetMenu(pMenuObject) == SPEC)
  {
    SPEC->SpectateObject(pMenuObject, GetOwner(pMenuObject));
    return false;
  }

  return true;
}

public func OnMenuSelection(int iSelection, object pMenuObject)
{
  if(GetMenu(pMenuObject) == SPEC)
    return SPEC->SpectateMenuSelection(iSelection, pMenuObject);
  return true;
}

/* Votekick */

local votekickVars;

public func Initialize()
{
  votekickVars = [];
  return _inherited(...);
}

public func InputCallback(string reason, int plr)
{
  StartVotekick(plr, votekickVars[0], votekickVars[1], reason, true);
  return true;
}

public func RejectCollect(id idObject)
{
  if(GetCategory(0, idObject) & C4D_Living) return false;
  return true;
}

public func Spawn()
{
  //Spawn-Blocker im Umfeld zerstören
  for(var obj in FindObjects(Find_Distance(25), Find_Exclude(this), Find_Func("IsSpawnBlocker")))
  {
    DoDmg(10000, DMG_Explosion, obj);
  }

  //Sound
  Sound("ClonkSpawn*.ogg");

  return _inherited(...);
}

protected func ContainedDown()
{
  _inherited(...);
  return true;
}