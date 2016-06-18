/*-- Maschinenkanone --*/

#strict 2
#include WPN2

public func HandSize()		{return 1000;}
public func HandX()		{return 5000;}
public func HandY()		{return 600;}
public func NoWeaponChoice()	{return true;}


protected func Construction()
{
  AddEffect("IntNoSound", this, 1, 5);
  return _inherited(...);
}

/* Kugeln */

public func FMData1(int data)
{
  if(data == FM_Name)		return "$Bullets$";

  if(data == FM_AmmoID)		return STAM;	//ID der Munition
  if(data == FM_AmmoLoad)	return 100;	//Magazingröße

  if(data == FM_Reload)		return 200;	//Zeit für Nachladen
  if(data == FM_Recharge)	return 6;	//Zeit bis erneut geschossen werden kann

  if(data == FM_Auto)		return true;	//Automatikfeuer

  if(data == FM_Damage)		return 9;	//Schadenswert

  if(data == FM_Slot)		return 1;	//Slot des Feuermodus

  if(data == FM_SpreadAdd)	return 25;	//Bei jedem Schuss hinzuzuaddierende Streuung
  if(data == FM_StartSpread)	return 100;	//Bei Auswahl der Waffe gesetzte Streuung
  if(data == FM_MaxSpread)	return 400;	//Maximaler Streuungswert

  return Default(data);
}

/* Kugeln - Automatikfeuer */

public func FMData1T1(int data)
{
  if(data == FT_Name)		return "$Auto$";

  return FMData1(data);
}

public func Fire1T1()
{
  Fire1();
}

public func BotData1(int data)
{
  if(data == BOT_Range)		return 500;

  return Default(data);
}

/* Kugeln - Schuss */

public func Fire1()
{
  //Austritt bestimmen
  var user = GetUser();
  var dir = GetDir(user)*2-1;
  var angle = user->AimAngle(10,0,true);
  var x,y;
  user->WeaponEnd(x,y);

  //Kugel abfeuern
  var ammo = CreateObject(SHTX, x, y, GetController(user));
  AddEffect("IntHeliProtection", ammo, 5, 0, 0, BKHK, LocalN("heli", Contained()));
  ammo->CustomLaunch(angle + Random(5) - 2, 250, 750, 2, GetFMData(FM_Damage) * 10, GetFMData(FM_Damage));

  //Effekte
  MuzzleFlash(RandomX(35,50),user,x,y,angle,0, 0);
  SABulletCasing(x/3,y/3,-dir*14*(Random(1)+1),-(13+Random(2)),7);
  Sound("ACCN_Fire.ogg", 0, ammo);
  Echo("ACCN_Echo.ogg");

  //Klickgeräusch bei wenig Munition
  if(Inside(GetAmmo(GetFMData(FM_AmmoID)), 0, 1))
    Sound("MNGN_Empty.ogg", 0, this, 0, GetOwner(user)+1);
  else
    if(Inside(GetAmmo(GetFMData(FM_AmmoID)), 1, GetFMData(FM_AmmoLoad)/3))
      Sound("MNGN_Click.ogg", 0, ammo, 0, GetOwner(user)+1);
}

/* Nachladen */

public func OnEmpty()
{
  if(Contained() && Contained()->~IsWeaponRack())
    Contained()->~OnEmpty();
}

public func OnReload()
{
  if(!GetEffect("IntNoSound", this))
    Sound("ACCN_Reload.ogg", false, this);
}

/* Allgemein */

public func StopAutoFire()
{
  //Nur wenn er auch wirklich feuert
  if(IsRecharging() && !IsReloading())
    return _inherited(...);
}