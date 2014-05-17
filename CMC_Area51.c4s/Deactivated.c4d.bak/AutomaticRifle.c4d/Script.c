/*-- Schnellfeuergewehr --*/

#strict 2
#include WPN2

local casing;

public func HandSize()		{return 1000;}
public func HandX()		{return 5000;}
public func HandY()		{return 1000;}
public func BarrelYOffset()	{return -2500;}
public func SelectionTime()	{return 36;}
public func IsPrimaryWeapon()	{return true;}

/* Nahkampfangriff */

public func GetMCData(int data)
{
  if(data == MC_CanStrike)	return 1;
  if(data == MC_Damage)		return 20;
  if(data == MC_Recharge)	return 40;
  if(data == MC_Power)		return 20;
  if(data == MC_Angle)		return 45;
}

/* Kugeln */

public func FMData1(int data)
{
  if(data == FM_Name)		return "$Bullets$";
    
  if(data == FM_AmmoID)		return STAM;
  if(data == FM_AmmoLoad)	return 30;
  
  if(data == FM_Reload)		return 90;
  if(data == FM_Recharge)	return 4;

  if(data == FM_Auto)		return true;
  
  if(data == FM_Damage)		return 7;
  
  if(data == FM_SpreadAdd)	return 30;
  if(data == FM_StartSpread)	return 100;
  if(data == FM_MaxSpread)	return 450;

  return Default(data);
}

/* Kugeln - Dauerfeuer */

public func FMData1T1(int data)
{
  if(data == FT_Name)		return "$Auto$";
  return FMData1(data);
}

public func Fire1T1()
{
  Fire();
}

public func BotData1(int data)
{
  if(data == BOT_Range)		return 250;
  if(data == BOT_Power)		return BOT_Power_LongLoad;

  return(Default(data));
}

/* Kugeln - Stoßfeuer */

public func FMData1T2(int data)
{
  if(data == FT_Name)		return "$Burst$";

  if(data == FM_Recharge)	return 15;

  if(data == FM_BurstAmount)	return 3;
  if(data == FM_BurstRecharge)	return 3;

  return FMData1(data);
}

public func Fire1T2()
{
  Fire();
}

/* Kugeln - Einzelfeuer */

public func FMData1T3(int data)
{
  if(data == FT_Name)		return "$Single$";

  if(data == FM_Auto)		return false;

  if(data == FM_Damage)		return 15;

  return FMData1(data);
}

public func Fire1T3()
{
  Fire();
}

/* Kugeln - Schuss */

public func Fire()
{
  //Austritt bestimmen
  var user = GetUser();
  var dir = GetDir(user)*2-1;
  var angle = user->AimAngle(10,0,true);
  var x,y;
  user->WeaponEnd(x,y);

  //Kugel abfeuern
  var ammo = SALaunchBullet(x,y,GetController(user),angle,300,800,GetFMData(FM_Damage));

  //Effekte
  MuzzleFlash(RandomX(30,40),user,x,y,angle,0, 0);
  SABulletCasing(x/3,y/3,-dir*14*(Random(1)+1),-(13+Random(2)),5);
  Sound("ATCR_Fire.ogg", 0, ammo);
  Echo("ATCR_Echo.ogg");
}

/* Handeffekt */

public func HandR()
{
  var effect = IsReloading();
  if(effect)
    return -BoundBy(GetEffect(0,this,effect,6)*2,0,10);

  effect = IsRecharging();
  if(effect)
    return -BoundBy(GetEffect(0,this,effect,6)*1,0,1);
}

/* Allgemein */

func OnReload(i)
{
  if(i == 1)
  {
    Sound("ASTR_Reload.ogg");
  }
  if(i == 2)
  {
    Sound("ASTR_LauncherReload.ogg");
    if(casing)
    {
      var user = GetUser();
      var dir = GetDir(user)*2-1;
      SABulletCasing(dir*1,0,-dir*14*(Random(1)+1),-(13+Random(2)),6,RGB(150,150,150));
      casing = 0;
    }
  }
}

func OnSelect()
{
   Sound("ASTR_Charge.ogg");
}