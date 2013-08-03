/*-- Schutzt¸r --*/

#strict
#include GBDR

local Status;

public func IsMeleeTarget()	{if(lock && !destroyed && GetAction() eq "Close") return true;}


/* Initialisierung */

public func Initialize()
{
  SetAction("Close");
  _inherited();
  //Maximalschaden festlegen
  SetMaxDamage(80);
  //Standardfarbe: Rot
  SetColorDw(HSL(250, 210, 100, 127));
}

/* Schaden */

public func OnDmg(int iDmg, int iType)
{
  if(iType == DMG_Projectile)	return 50;	//Projektile
  if(iType == DMG_Melee)	return 50;	//Nahkampf
  if(iType == DMG_Energy)	return 50;	//Energie
  if(iType == DMG_Bio)		return 100;	//S‰ure und biologische Schadstoffe
}

public func MeleeHit()
{
  //Effekte
  CastSmoke("Smoke3",5,10,0,-10,20,150);
  CastSmoke("Smoke3",5,10,0,10,20,150);
  Sound("DoorKick.ogg");

  DoDmg(27);
  
  return true;
}

public func OnDestroyed(iType)
{
  //Effekte
  if(GetEffectData(EFSM_ExplosionEffects) > 1) CastParticles("WoodSplinter", 3, 80, 0,0, 70, 155);
  Sound("CrateDestruct*.ogg");

  SetAction("Destroyed");
}

/* ÷ffnen und Schlieﬂen */

public func OnOpen()
{
  //Nichts tun wenn bereits offen
  if( GetAction() eq "Open" || Status) return(0);

  //Aktion setzen und SolidMask erstellen
  SetAction("Open");
  SetSolidMask(0);
  Status = true;

  Sound("GDDR_Open*.ogg");
}

public func OnClose()
{
  //Nichts tun wenn bereits zu
  if( GetAction() eq "Close"|| !Status) return(0);  

  //Aktion setzen und SolidMask entfernen
  SetAction("Close");
  Status = false;
  SetSolidMask(12, 0, 10, 30, 12, 0);

  Sound("GDDR_Close*.ogg");
}

/* Serialisierung */

public func Serialize(array& extra)
{
  if (destroyed)
    extra[GetLength(extra)] = "SetAction(\"Destroyed\")";
}