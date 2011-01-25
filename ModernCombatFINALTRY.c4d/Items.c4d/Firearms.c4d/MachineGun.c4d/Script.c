/*-- Maschinengewehr --*/

#strict 2
#include WPN2

public func HandSize()		{return 800;}
public func HandX()		{return 5000;}
public func HandY()		{return 800;}
public func BarrelXOffset()	{return -1000;}
public func BarrelYOffset()	{return -2000;}
public func SelectionTime()	{return 36;}


/* Nahkampfangriff */

public func GetMCData(int data)
{
  if(data == MC_CanStrike)	return 1;
  if(data == MC_Damage)		return 20;
  if(data == MC_Recharge)	return 45;
  if(data == MC_Power)		return 20;
  if(data == MC_Angle)		return 45;
}

/* Kugeln */

public func FMData1(int data)
{
  if(data == FM_Name)		return "$Bullets$";
    
  if(data == FM_AmmoID)		return STAM;
  if(data == FM_AmmoLoad)	return 60;
  
  if(data == FM_Reload)		return 220;
  if(data == FM_Recharge)	return 4;

  if(data == FM_Auto)		return true;
  
  if(data == FM_Damage)		return 8;
  
  if(data == FM_SpreadAdd)	return 30;
  if(data == FM_StartSpread)	return 200;
  if(data == FM_MaxSpread)	return 400;

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
  Fire1();
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

  if(data == FM_Recharge)	return 30;
  if(data == FM_Auto)		return false;

  if(data == FM_BurstAmount)	return 4;
  if(data == FM_BurstRecharge)	return 4;

  if(data == FM_Damage)		return 10;

  if(data == FM_SpreadAdd)	return 40;

  return FMData1(data);
}

public func Fire1T2()
{
  Fire1();
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
  var ammo = SALaunchBullet(x,y,GetController(user),angle+RandomX(-1,+1),270,750,GetFMData(FM_Damage));

  //Effekte
  SAMuzzleFlash(RandomX(35,50),user,x,y,angle);
  SABulletCasing(x/3,y/3,-dir*14*(Random(1)+1),-(13+Random(2)),5);
  Sound("MNGN_Fire.ogg", 0, ammo);
}

/* Allgemein */

func OnReload()
{
  Sound("MNGN_Reload.ogg");
}

func OnSelect()
{
  Sound("MNGN_Charge.ogg");
}