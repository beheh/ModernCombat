/*-- MP7 --*/

#strict 2
#include WPN2

public func HandSize()	{ return 800; }
public func HandX()	{ return 5000; }
public func HandY()	{ return -2000; }

public func BarrelYOffset() { return -5500; }
public func SelectionTime() { return 10*3; }


//Kugeln - Automatikfeuer

public func FMData1(int data)
{
  if(data == FM_Name)                 return "$Bullets$";
    
  if(data == FM_AmmoID)             return STAM;
  if(data == FM_AmmoLoad)           return 30;
  
  if(data == FM_Reload)             return 70;
  if(data == FM_Recharge)           return 4;

  if(data == FM_Auto)               return true;
  
  if(data == FM_Damage)    return 8;
  
  if(data == FM_Slot)    return 1;
  
  if(data == FM_SpreadAdd) return 25;
  if(data == FM_StartSpread) return 20;

  return Default(data);
}

public func FMData1T1(int data)
{
  if(data == FT_Name)                 return "$Auto$";
  if(data == FT_IconFacet)            return FICO_Auto;
  return FMData1(data);
}

public func Fire1T1()
{
  Fire1();
}

public func BotData1(int data)
{
  if(data == BOT_Range)    return 500;

  return Default(data);
}

public func Fire1()
{
  var user = GetUser();
  var dir = GetDir(user)*2-1;
  var angle = user->AimAngle(15,0,true);
  var x,y;
  user->WeaponEnd(x,y);
  var ammo = SALaunchBullet(x,y,GetController(user),angle+RandomX(-3,+3),230,350+Random(100),GetFMData(FM_Damage));
  ammo->Sound("MP7R_Fire.ogg");

  // Effekte
  SAMuzzleFlash(RandomX(15,25),user,x,y,angle);
  SABulletCasing(x/3,y/3,-dir*14,-(14),4);
}


//Kugeln - Stoßfeuer

public func FMData1T2(int data)
{
  if(data == FT_Name)                 return "$Burst$";
  if(data == FM_Recharge)             return 20;
  if(data == FT_IconFacet)            return FICO_Burst;
  if(data == FM_Auto)                 return false;
  if(data == FM_Damage)               return(FMData1(FM_Damage)*3/2);
  if(data == FM_BurstAmount)          return 3;
  if(data == FM_BurstRecharge)        return 3;
  if(data == FM_SpreadAdd)            return 42;
  return FMData1(data);
}

public func Fire1T2()
{ 
  var user = GetUser();
  var dir = GetDir(user)*2-1;
  var angle = user->AimAngle(15,0,true);
  var x,y;
  user->WeaponEnd(x,y);
  var ammo = SALaunchBullet(x,y,GetController(user),angle+RandomX(-2,+2),230,350+Random(100),GetFMData(FM_Damage));
  ammo->Sound("MP7R_Fire.ogg");

  // Effekte
  SAMuzzleFlash(RandomX(15,25),user,x,y,angle);
  SABulletCasing(x/3,y/3,-dir*14,-(14),4);
}


//Kugeln - Einzelfeuer

public func FMData1T3(int data)
{
  if(data == FT_Name)                 return "$Single$";
  if(data == FM_Recharge)             return 9;
  if(data == FT_IconFacet)            return FICO_Single;
  if(data == FM_Auto)                 return false;
  if(data == FM_Damage)               return FMData1(FM_Damage)*2;
  return FMData1(data);
}

public func Fire1T3()
{
  var user = GetUser();
  var dir = GetDir(user)*2-1;
  var angle = user->AimAngle(15,0,true);
  var x,y;
  user->WeaponEnd(x,y);
  var ammo = SALaunchBullet(x,y,GetController(user),angle+RandomX(-1,+1),220,350+Random(100),GetFMData(FM_Damage));
  ammo->Sound("MP7R_Fire.ogg");

  // Effekte
  SAMuzzleFlash(RandomX(15,25),user,x,y,angle);
  SABulletCasing(x/3,y/3,-dir*14,-(14),4);
}

/* Allgemein */

func OnSelect()
{
  Sound("MP7R_Charge.ogg");
}

func OnReload()
{
  Sound("MP7R_Reload.ogg");
}
