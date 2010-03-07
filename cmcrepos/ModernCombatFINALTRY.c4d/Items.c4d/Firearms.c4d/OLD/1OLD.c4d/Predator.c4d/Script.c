/*-- Predator --*/

#strict
#include WPN2

public func HandSize() { return(800); }
public func HandX()    { return(5500); }
public func HandY()    { return(-1000); }
public func BarrelYOffset() { return(-5000); }
public func SelectionTime() { return(3*3); }


//Kugeln - Einzelfeuer

public func FMData1(int data)
{
  if(data == FM_Name)               return("$Bullets$");
    
  if(data == FM_AmmoID)             return(STAM);
  if(data == FM_AmmoLoad)           return(15);
  
  if(data == FM_Reload)             return(40);
  if(data == FM_Recharge)           return(5);

  if(data == FM_Auto)               return(false);
  
  if(data == FM_Damage)    return(13);
  
  if(data == FM_Slot)    return(1);
  
  if(data == FM_SpreadAdd) return(60);//Kaum Rückstoß
  if(data == FM_StartSpread) return(0);//Pistolen sind leicht
  if(data == FM_MaxSpread) return(CH_MaxSpread/3*2);

  return(Default(data));
}

public func FMData1T1(int data)
{
  if(data == FT_Name)                 return("$Single$");
  return(FMData1(data));
}

public func Fire1T1()
{
  Fire1();
}

public func BotData1(int data)
{
  if(data == BOT_Range)    return(500);

  return(Default(data));
}

public func Fire1()
{
  var user = GetUser();
  var dir = GetDir(user)*2-1;
  var angle = user->AimAngle(20,0,true);
  var x,y;
  user->WeaponEnd(x,y);
  var ammo = SALaunchBullet(x,y,GetController(user),angle+RandomX(-1,+1),230,350+Random(50),GetFMData(FM_Damage));
  ammo->Sound("PDTW_Fire*.ogg");

  // Effekte
  SAMuzzleFlash(RandomX(25,30),user,x,y,angle);
  SABulletCasing(x/3,y/3,-dir*14*(Random(1)+1),-(13+Random(2)),4);
}


//Monochrom - Doppelschuss

public func FMData2(int data)
{
  if(data == FM_Name)           return("$Monochrom$");
  if(data == FM_BurstAmount)    return(2);
  if(data == FM_BurstRecharge)  return(4);
  if(data == FM_SpreadAdd) return(50);
  return(FMData1(data));
}

public func FMData2T1(int data)
{
  if(data == FT_Name)                 return("$MonochromSingle$");
  return(FMData2(data));
}

public func Fire2T1()
{
  Fire2();
}

public func BotData2(int data)
{
  if(data == BOT_Range)    return(300);
  return(Default(data));
}

public func Fire2()
{
  var user = GetUser();
  var dir = GetDir(user)*2-1;
  var x,y;
  GetUser()->WeaponEnd(x,y);
  
  var angle = user->AimAngle(20,0,true) + RandomX(-3,+3);
  var ammo = CreateObject(MSHT,x,y,GetController(user));
  ammo -> Launch(angle, 200+Random(70), 320+Random(50), 4, 150, GetFMData(FM_Damage, 2), 10, 1, 7,1);

  BulletCasing(x/3,y/3,-dir*17*(Random(1)+1),-(15+Random(2)),4);

  ammo->Sound("PDTW_FireMonochrom*.ogg");
  
  MuzzleFlash(RandomX(30,40),user,x,y-2,angle,RGB(0,128,255));
  MuzzleFlash(RandomX(10,20),user,x,y-2,angle,RGB(0,70+Random(30),255));
}

/* Allgemein */

func OnSelect(int iFM)
{
   Sound("PDTW_Charge.ogg");
}

public func OnReload(int i)
{
  Sound("PDTW_Reload.ogg");
}