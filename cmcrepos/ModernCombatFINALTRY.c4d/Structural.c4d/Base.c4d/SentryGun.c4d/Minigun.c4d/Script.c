/*-- Minigun --*/

#strict 2

#include WEPN

public func HandSize()		{return 1000;}
public func HandX()		{return 7000;}
public func HandY()		{return 0;}
public func NoWeaponChoice()	{return true;}

local fNoSpread = false;

/* Kugeln - Dauerfeuer */

public func FMData1(int data)
{
  if(data == FM_AmmoID)		return STAM;	//ID der Munition
  if(data == FM_AmmoLoad)	return 9999;	//Magazingröße

  if(data == FM_Reload)		return 2;	//Zeit für Nachladen
  if(data == FM_Recharge)	return 5;	//Zeit bis erneut geschossen werden kann

  if(data == FM_Auto)		return 1;	//Slot des Feuermodus

  if(data == FM_Damage)		return 7;	//Schadenswert

  return Default(data);
}

public func Fire1()
{
  //Austritt bestimmen
  var user = GetUser();
  var angle = user->AimAngle()+(!fNoSpread)*RandomX(-3,+3);
  var dir = GetDir(user)*2-1;
  var iX;
  var iY;
  user->WeaponEnd(iX,iY);

  //Kugel abfeuern
  var ammo = CreateObject(SHTX, iX, iY, GetController(user));
  ammo->CustomLaunch(angle, 350, 650, 2, GetFMData(FM_Damage) * 10, GetFMData(FM_Damage));

  //var ammo = CreateObject(SHTX, iX, iY, GetController(user));
  //ammo->Launch(angle, 300, 600, 2, 100, 3, 1);

  //Effekte
  MuzzleFlash(40+Random(60),user,iX,iY,angle);
  BulletCasing(dir*1,0,-dir*14*(Random(1)+1),-(13+Random(2)),7,0,true);
  Sound("MISA_Fire.ogg",0,ammo);
  Echo("ACCN_Echo.ogg");
}

func OnAutoStop(int iFM)
{
  Sound("MISA_FireEnd.ogg");
}
