/*-- Verbrannte Geschützstellung --*/

#strict 2
#include CSTR

local cur_Attachment;
local aim_angle;
local iPat_Dir;
local last_id;
local crosshair;
local controller;
local iRotation;
local pShield;
local fShield;
local iTurningSpeed;

public func MaxDamage()		{return 100;}
public func IsMachine()		{return true;}
public func StartRepair()	{return true;}


public func OnRepair()
{
  //Lokale Variablen funktionslokal speichern (Auf lokale Variablen darf nach ChangeDef nicht zugegriffen werden)
  var attachment = last_id;
  var shield = fShield;
  var rotation = iRotation;
  var speed = iTurningSpeed;

  //Definitionswechsel zum funktionstüchtigen Geschütz
  ChangeDef(GNET);
  
  this->~Initialize();
  this->~Set(attachment, rotation, shield, speed);
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