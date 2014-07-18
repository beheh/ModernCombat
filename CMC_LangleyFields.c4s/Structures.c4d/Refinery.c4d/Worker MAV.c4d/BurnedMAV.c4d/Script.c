/*--- Verbranntes MAV ---*/

#strict 2
#include CVHC

local cur_Attachment;
local iXDir, iYDir;
local iXTendency, iYTendency;
local iBank;
local iBankMax;
local iSpeed;
local fIsAiming;
local iAimAngle;
local iPat_Dir;
local crosshair;
local pLaser;
local pBeam;
local iC4Count;
local pItem;
local iItemType;		//0: Standard (Sensorball), 1: MTP, 2: EHP, 3: Schweißbrenner, 4: Einsatzschild, 5: Sprengfalle, 6: Defibrillator
local iHKShots;
local living_dmg_cooldown;	//Gehört zum Schweißbrenner, ist aber praktischer, wenn direkt hier gespeichert
local ChargeBar;

public func IsDestroyed()	{return true;}
public func IsMachine()		{return true;}
public func IsMAV()		{return true;}
public func IsRepairable()	{return false;}


/* Initialisierung */

protected func Initialize()
{
  SetAction("Destroyed");

  while(GetEffect("TracerDart", this))
    RemoveEffect("TracerDart", this);

  FadeOut();
}

/* Aufschlag */

protected func Hit2()
{
  //Effekte
  Sound("HeavyHit*.ogg");
}