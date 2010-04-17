/*-- Minigun-Aufsatz --*/

#strict 2

#include WEPN

public func HandSize() { return 1000; }
public func HandX()    { return 7000; }
public func HandY()    { return 0; }

public func FMData1(int data)
{
  if(data == FM_AmmoID)    return STAM;
  if(data == FM_AmmoLoad)  return 9999;

  if(data == FM_Reload)    return 1;
  if(data == FM_Recharge)  return 2;

  //if(data == FM_AmmoUsage) return(0);
  //if(data == FM_AmmoRate)  return(2);
  if(data == FM_Auto)      return 1;

  return Default(data);
}

public func FMData2(int data)
{
  if(data == FM_Recharge)  return 25;

  return FMData1(data);
}

public func Fire1()    // Projektil-Dauerfeuer
{
  var user = GetUser();
  var angle = user->AimAngle()+RandomX(-3,+3);
  var dir = GetDir(user)*2-1;
  
  var iX;
  var iY;
  
  user->WeaponEnd(iX,iY);
  
  var ammo = CreateObject(SHTX, iX, iY, GetController(user));
  //int iAngle, int iSpeed, int iDist, int iSize, int iTrail, int iDmg
  ammo->Launch(angle, 300, 600, 2, 100, 5,1);

  // Effekte
  MuzzleFlash(40+Random(60),user,iX,iY,angle);
  BulletCasing(dir*1,0,-dir*14*(Random(1)+1),-(13+Random(2)),7,0,true);

  // Sound
  Sound("MISA_Fire.ogg",0,ammo);
}

func OnAutoStop(int iFM)
{
  Sound("MISA_FireEnd.ogg");
}

public func Fire2()
{
  Burst();
  ScheduleCall(this,"Burst",4,2); 
}

private func Burst()
{
  var user = GetUser();
  var angle = user->AimAngle()+RandomX(-2,+2);
  var dir = GetDir(user)*2-1;
  
  var iX;
  var iY;
  
  user->WeaponEnd(iX,iY);
  
  var ammo = CreateObject(SHTX, iX, iY, GetController(user));
  ammo->Launch(angle, 300, 600, 2, 100, 5,1);

  // Effekte
  MuzzleFlash(20+Random(30),user,iX,iY,angle);
  BulletCasing(dir*1,0,-dir*14*(Random(1)+1),-(13+Random(2)),7,0,true);

  // Sound
  Sound("MISA_Fire.ogg",0,ammo);
}

public func NoWeaponChoice() { return true; }
