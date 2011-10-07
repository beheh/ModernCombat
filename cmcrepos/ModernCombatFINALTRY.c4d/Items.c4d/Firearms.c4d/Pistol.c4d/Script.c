/*-- Pistole --*/

#strict 2
#include WPN2

local casing;

public func HandSize()		{return 800;}
public func HandX()		{return 4000;}
public func HandY()		{return -1200;}
public func BarrelYOffset()	{return -5000;}
public func SelectionTime()	{return 10;}


/* Nahkampfangriff */

public func GetMCData(int data)
{
  if(data == MC_CanStrike)	return 1;
  if(data == MC_Damage)		return 10;
  if(data == MC_Recharge)	return 35;
  if(data == MC_Power)		return 18;
  if(data == MC_Angle)		return 45;
}

/* Kugeln */

public func FMData1(int data)
{
  if(data == FM_Name)		return "$Bullets$";
    
  if(data == FM_AmmoID)		return STAM;
  if(data == FM_AmmoLoad)	return 15;
  
  if(data == FM_Reload)		return 40;
  if(data == FM_Recharge)	return 5;

  if(data == FM_Auto)		return false;

  if(data == FM_Damage)		return 13;
  
  if(data == FM_Slot)		return 1;
  
  if(data == FM_SpreadAdd)	return 60;
  if(data == FM_StartSpread)	return 10;
  if(data == FM_MaxSpread)	return 200;

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
  var angle = user->AimAngle(20,0,true);
  var x,y;
  user->WeaponEnd(x,y);

  //Kugel abfeuern
  var ammo = SALaunchBullet(x,y,GetController(user),angle+RandomX(-1,+1),250,400,GetFMData(FM_Damage));

  //Effekte
  MuzzleFlash(RandomX(35,40),user,x,y,angle,0, 4);
  SABulletCasing(x/3,y/3,-dir*14*(Random(1)+1),-(13+Random(2)),4);
  Sound("PSTL_Fire*.ogg", 0, ammo);
  Echo("PSTL_Echo.ogg");
}

/* Peilsender */

public func FMData2(int data)
{
  if(data == FM_Name)		return "$TracerDart$";
    
  if(data == FM_AmmoID)		return STAM;
  if(data == FM_AmmoLoad)	return 1;

  if(data == FM_Reload)		return 90;

  if(data == FM_Auto)		return false;
  
  if(data == FM_Damage)		return;
  
  if(data == FM_Slot)		return 2;

  if(data == FM_SpreadAdd) return 60;
  if(data == FM_StartSpread) return 10;
  if(data == FM_MaxSpread) return 200;

  return Default(data);
}

/* Peilsender - Einzelfeuer */

public func FMData2T1(int data)
{
  if(data == FT_Name)		return "$Single$";
  return FMData2(data);
}

public func Fire2T1()
{
  Fire2();
}

/* Peilsender - Schuss */

public func Fire2()
{
  //Austritt bestimmen
  var user = GetUser();
  var angle = user->AimAngle(20,0,true);
  var x,y;
  user->WeaponEnd(x,y);

  //Kugel abfeuern
  var ammo = CreateObject(TRDT,x,y,GetController(user));
  ammo->Launch(angle,180,800,20,1000);
  Sound("PSTL_TracerFire.ogg", 0, ammo);

  //Patronenhülse vorhanden
  casing = 1;
}

/* Allgemein */

func OnSelect()
{
  Sound("PSTL_Charge.ogg");
}

public func OnReload(i)
{
  if(i == 1)
  {
    Sound("PSTL_Reload.ogg");
  }
  if(i == 2)
  {
    Sound("PSTL_TracerReload.ogg");
    if(casing)
    {
      var user = GetUser();
      var dir = GetDir(user)*2-1;
      SABulletCasing(dir*1,0,-dir*14*(Random(1)+1),-(13+Random(2)),6,RGB(150,150,150));
      casing = 0;
    }
  }
}