/*-- Karabiner --*/

#strict
#include WPN2

public func HandSize()		{return 1000;}
public func HandX()		{return 5000;}
public func HandY()		{return 1000;}
public func BarrelYOffset()	{return -2500;}
public func SelectionTime()	{return(20*3);}

/*
public func HandSize()		{return(1000);}
public func HandX()		{return(8000);}
public func BarrelYOffset()	{return(-2000);}
*/

/* Nahkampfangriff */

public func GetMCData(int data)
{
  if(data == MC_CanStrike)	return 1;
  if(data == MC_Damage)		return 20;
  if(data == MC_Recharge)	return 50;
  if(data == MC_Power)		return 30;
  if(data == MC_Angle)		return 45;
}

/* Kugeln */

public func FMData1(int data)
{
  if(data == FM_Name)		return "$Bullets$";
    
  if(data == FM_AmmoID)		return STAM;
  if(data == FM_AmmoLoad)	return 10;
  
  if(data == FM_Reload)		return 130;
  if(data == FM_Recharge)	return 25;

  if(data == FM_Auto)		return false;

  if(data == FM_Slot)		return 1;

  if(data == FM_Damage)		return 21;
  
  if(data == FM_SpreadAdd)	return 150;
  if(data == FM_StartSpread)	return 100;
  if(data == FM_MaxSpread)	return 450;

  return Default(data);
}

/* Kugeln - Einzelfeuer */

public func FMData1T1(int data)
{
  if(data == FT_Name)		return "$Single$";

  return FMData1(data);
}

public func Fire1T1()
{
  Fire1();
}

public func BotData1(int data)
{
  if(data == BOT_Range)		return 400;
  if(data == BOT_Power)		return(BOT_Power_1);
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
  var ammo = SALaunchBullet(x,y,GetController(user),angle,270,900,GetFMData(FM_Damage));

  //Effekte
  MuzzleFlash(RandomX(60,70),user,x,y,angle,0, 0);
  AddEffect("Pump", this, 1, 1+GetFMData(FM_Recharge, 1)-25, this);
  Sound("MCBN_Fire.ogg", 0, ammo);
  SoundLevel("PPGN_Echo2.ogg", 0, this);
  Sound("PPGN_Echo2.ogg", false, this, 100, 0, 0, true, 1000);
}

/* Nachladen */

public func FxPumpStop(object pTarget)
{
  Sound("MCBN_Reloading.ogg");

  if(!GetAmmo()) return;

  var user = GetUser();
  var dir = GetDir(user)*2-1;
  SABulletCasing(dir*1,0,-dir*14*(Random(1)+1),-(13+Random(2)),6,RGB(255));
}

/* Allgemein */

func OnReload(i)
{
  Sound("MCBN_Reload.ogg");
}

func OnSelect()
{
   Sound("MNGN_Charge.ogg");
}