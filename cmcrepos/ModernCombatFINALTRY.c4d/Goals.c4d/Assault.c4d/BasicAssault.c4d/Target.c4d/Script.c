/*-- Zielobjekt --*/

#strict 2

local aDamage;					//Schaden jedes Spielers
local iLastDamager;				//Der letzte der Schaden verursacht hat
local idImitation;				//Welche Definition imitiert wird

public func IsBulletTarget()			{return IsAssaultTarget();}
public func IsMeleeTarget(object pByObj)	{return (GetPlayerTeam(GetOwner(pByObj->~GetUser())) != GetTeam());}
public func AttractTracer(pTracer)		{return IsAssaultTarget() && GetPlayerTeam(GetController(pTracer)) != GetTeam();}
public func GetKillIcon()			{return idImitation;}


/* Initialisierung */

protected func Initialize()
{
  aDamage = [];
  iLastDamager = -1;
}

/* Schaden */

public func OnDmg(int iDmg, int iType)
{
  if(iType == DMG_Explosion)	return 20;	//Explosionen und Druckwellen
  return 65;
}

public func OnHit(int iDamage, int iType, object pFrom)
{
  var iPlr = GetOwner(pFrom);
  if (iPlr == -1)
    return;
  iLastDamager = iPlr;
  SetController(iPlr);
  aDamage[iPlr] += iDamage;
}

public func MeleeHit()
{
  //Effekte
  CastSmoke("Smoke3",5,13,-5,-5,50,150);
  CastSmoke("Smoke3",5,13,5,5,50,150);
  Sound("DoorKick.ogg");

  DoDmg(14);
  
  return true;
}

/* Assault-Calls */

public func GetDamager()
{
  return [aDamage, iLastDamager];
}

public func SetImitationID(id id)
{
  return idImitation = id;
}

public func GetImitationID()
{
  return idImitation;
}