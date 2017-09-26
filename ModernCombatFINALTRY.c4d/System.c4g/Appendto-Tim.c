/*-- Spawnen --*/

//Beim Spawnen wird überprüft, ob Objekte "IsSpawnBlocker" den Ort blockieren und zerstören diese. Spawnverzögerung- und Votekick-Mechanik wird in das Spawnen integriert.

#strict 2

#appendto TIM1
#appendto TIM2

local Respawn_KillMsg, Respawn_Position;


public func IsSpawnObject()	{return true;}

public func Initialize()
{
  ScheduleCall(0, "CheckForRespawn", 1);
  return _inherited(...);
}

public func CheckForRespawn()
{
  if(GetEffect("WaitingObject", this))
    return;

  var clonk = FindObject2(Find_Container(this), Find_OCF(OCF_CrewMember));
  if(clonk && GameCall("GetPlayerRespawnTime", GetOwner(clonk)))
  {
    if(Respawn_Position)
      //An den Todesort verschieben
      SetPosition(Respawn_Position[0], Respawn_Position[1], this);

    AddEffect("WaitingObject", this, 100, 10, this, 0, Respawn_KillMsg);
  }
}

/* Spawnverzögerung */

public func FxWaitingObjectStart(object pTarget, int iNr, bool fTemp, string szKillmsg)
{
  if(fTemp)
    return;

  EffectVar(0, pTarget, iNr) = szKillmsg;
  FxWaitingObjectTimer(pTarget, iNr, 0);
  var clonk = FindObject2(Find_Container(this), Find_OCF(OCF_CrewMember));
  FKDT->SetFakeDeathEffects(clonk, GameCall("GetPlayerRespawnTime", GetOwner(clonk)) != FKDT_SuicideTime*35, GameCall("GetPlayerRespawnTime", GetOwner(clonk)));
}

public func FxWaitingObjectTimer(object pTarget, int iNr, int iTime)
{
  var clonk = FindObject2(Find_Container(this), Find_OCF(OCF_CrewMember));
  var time = GameCall("GetPlayerRespawnTime", GetOwner(clonk));

  if(time-iTime <= 0)
    return -1;

  FKDT->DeathMenu(clonk, this, FKDT_DeathMenu_FullSettings, time-iTime, FKDT_SuicideTime, EffectVar(0, pTarget, iNr), 0, TIM1);

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

  Respawn_KillMsg = 0;
  Respawn_Position = 0;
  GameCall("RelaunchPlayer", GetOwner(clonk), clonk, info[0]);
  this->~SpawnOk();
  return true;
}

public func Suicide(object pTarget)
{
  if(GetEffect("WaitingObject", Contained(pTarget)))
    RemoveEffect("WaitingObject", Contained(pTarget));
}

public func Spawn()
{
  //Warteobjekt oder kein Inhalt: Abbruch
  if(GetEffect("WaitingObject", this))
    return;
  if(!Contents())
    return(RemoveObject());

  //Sichtweite zurücksetzen
  GameCall("ResetSpawnViewRange", Contents());
  //Bei Bedarf Autospawneffekt entfernen
  RemoveEffect("Spawntimer", Contents());

  //Spawn-Blocker im Umfeld zerstören
  for(var obj in FindObjects(Find_Distance(25), Find_Exclude(this), Find_Func("IsSpawnBlocker")))
    DoDmg(10000, DMG_Explosion, obj);

  //Sound
  Sound("ClonkSpawn*.ogg");

  RemoveObject(0,1);

  return true;
}

public func MenuQueryCancel(int iSelection, object pMenuObject)
{
  if(GetMenu(pMenuObject) == SPEC)
  {
    SPEC->SpectateObject(pMenuObject);
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

protected func ContainedDown()
{
  _inherited(...);
  return true;
}