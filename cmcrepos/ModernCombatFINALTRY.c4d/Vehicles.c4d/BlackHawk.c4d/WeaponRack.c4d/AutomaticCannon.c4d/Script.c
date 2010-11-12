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

  if(data == FM_AmmoID)		return STAM;
  if(data == FM_AmmoLoad)	return 100;

  if(data == FM_Reload)		return 200;
  if(data == FM_Recharge)	return 6;

  if(data == FM_Auto)		return true;

  if(data == FM_Damage)		return 9;

  if(data == FM_Slot)		return 1;

  if(data == FM_SpreadAdd)	return 25;
  if(data == FM_StartSpread)	return 100;
  if(data == FM_MaxSpread)	return 400;

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
  var user = GetUser();
  var dir = GetDir(user)*2-1;
  var angle = user->AimAngle(10,0,true);
  var x,y;
  user->WeaponEnd(x,y);
  var ammo = CreateObject(SHTX, x, y, GetController(user));
  AddEffect("IntHeliProtection", ammo, 5, 0, 0, BKHK, LocalN("heli", Contained()));
  ammo->CustomLaunch(angle + Random(5) - 2, 250, 750, 2, GetFMData(FM_Damage) * 10, GetFMData(FM_Damage));
  Sound("ACCN_Fire.ogg", 0, ammo);
  

  // Effekte
  SAMuzzleFlash(RandomX(35,50),user,x,y,angle);
  SABulletCasing(x/3,y/3,-dir*14*(Random(1)+1),-(13+Random(2)),5);
}

/* Allgemein */

func OnReload()
{
  if (!GetEffect("IntNoSound", this))
    Sound("ACCN_Reload.ogg", false, this);
}

public func StopAutoFire()
{
  //Nur wenn er auch wirklich feuert
  if (IsRecharging())
    return _inherited(...);
}