/*-- Verbrannte MAV Station --*/

#strict 2
#include CSTR

local crosshair;
local controller;
local pMAV, fMAVExistence;
local iXSpawnOff;
local iYSpawnOff;
local iBuyCooldown;

public func MaxDamage()		{return 150;}
public func IsMachine()		{return true;}
public func StartRepair()	{return true;}


/* Initialisierung */

public func Initialize() 
{
  _inherited();

  fDestroyed = true;
  SetAction("Destroyed");
}

public func OnRepair()
{
  //Aktion und Grafik setzen
  ChangeDef(MVSN);
  this->~Initialize();
}

/* Schaden */

public func OnDmg(int iDmg, int iType)
{
  if(iType == DMG_Projectile)	return 40;	//Projektile
  if(iType == DMG_Fire)		return 60;	//Feuer
  if(iType == DMG_Explosion)	return;		//Explosionen und Druckwellen
  if(iType == DMG_Energy)	return 50;	//Energiewaffen
  if(iType == DMG_Bio)		return 100;	//Säure und biologische Schadstoffe

  return 50;
}
