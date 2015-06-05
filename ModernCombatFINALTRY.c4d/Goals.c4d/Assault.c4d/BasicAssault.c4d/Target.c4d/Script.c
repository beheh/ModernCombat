/*-- Zielobjekt --*/

#strict 2

local aDamage;					//Schaden jedes Spielers
local iLastDamager;				//Der letzte der Schaden verursacht hat
local idImitation;				//Welche Definition imitiert wird

public func GetKillIcon()				{return idImitation;}
public func IsBulletTarget(id idBullet, object pBullet)	{return !FindObject2(Find_Func("IsCMCAssaultGoal"));}
public func BulletCheckObjectHitbox()			{return true;}


/* Initialisierung */

protected func Initialize()
{
  aDamage = [];
  iLastDamager = -1;
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