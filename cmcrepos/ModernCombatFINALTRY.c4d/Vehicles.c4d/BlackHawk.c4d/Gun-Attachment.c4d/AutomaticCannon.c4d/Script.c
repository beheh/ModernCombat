/*-- Maschinenkanone --*/

#strict 2
#include WEPN

public func HandSize()		{return 1000;}
public func HandX()		{return 5000;}
public func HandY()		{return 600;}
public func NoWeaponChoice()	{return true;}


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
  var ammo = SALaunchBullet(x,y,GetController(user),angle+RandomX(-2,+2),250,750,GetFMData(FM_Damage));
  Sound("ACCN_Fire.ogg", 0, ammo);

  // Effekte
  SAMuzzleFlash(RandomX(35,50),user,x,y,angle);
  SABulletCasing(x/3,y/3,-dir*14*(Random(1)+1),-(13+Random(2)),5);
}

/* Allgemein */

func OnReload()
{
  Sound("ACCN_Reload.ogg");
}

public func StopAutoFire()
{
  //Nur wenn er auch wirklich feuert
  if (IsRecharging())
    return _inherited(...);
}