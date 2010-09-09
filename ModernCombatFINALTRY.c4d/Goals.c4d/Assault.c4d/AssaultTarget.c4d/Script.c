/*-- Assault-Ziel --*/

#strict 2

local aDamage;			//Schaden jedes Spielers
local iLastDamager;		//Der letzte der Schaden verursacht hat
local idImitation;		//Welche Definition imitiert wird

public func IsBulletTarget() 		{return IsAssaultTarget();}
public func AttractTracer(pTracer)	{return IsAssaultTarget() && GetPlayerTeam(GetController(pTracer)) != GetTeam();}
public func GetKillIcon()			{return idImitation;}

protected func Initialize() {
  aDamage = [];
}

public func OnDmg(int iDmg, int iType) {
  //Explosionen sind böse.
  if (iType == DMG_Explosion)
    return 20;
  //Standard, damit die Ziele was aushalten
  return 65;
}

public func OnHit(int iDamage, int iType, object pFrom) {
  var iPlr = GetOwner(pFrom);
  if (iPlr == -1)
    return;
  iLastDamager = iPlr;
  SetController(iPlr);
  aDamage[iPlr] += iDamage;
}

/* Assault-Calls */

public func GetDamager() {
  return [aDamage, iLastDamager];
}

public func SetImitationID(id id) {
  return idImitation = id;
}

public func GetImitationID() {
  return idImitation;
}