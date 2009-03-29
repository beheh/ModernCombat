/*-- MG-Aufsatz --*/

#strict

#include WEPN

public func HandSize() { return(1000); }
public func HandX()    { return(7000); }
public func HandY()    { return(0); }

public func FMData1(int data)
{
  if(data == FM_AmmoID)    return(STAM);
  if(data == FM_AmmoLoad)  return(500);

  if(data == FM_Reload)    return(1);
  if(data == FM_Recharge)  return(4);

  if(data == FM_AmmoUsage) return(0);
  if(data == FM_AmmoRate)  return(2);
  if(data == FM_Auto)      return(1);

//  return(Default(data));
}

public func Fire1()    // Projektil-Dauerfeuer
{
  var user = GetUser();
  var angle = user->AimAngle();
  var dir = GetDir(user)*2-1;
  
  var iX;
  var iY;
  
  user->WeaponEnd(iX,iY);
  
  var ammo = CreateObject(SHTX, iX, iY, GetController(user));
  ammo->Launch(angle, 300, 600, 3, 100, 33,1);

  // Effekte
  MuzzleFlash(20,user,iX,iY,angle);
  BulletCasing(dir*1,0,-dir*14*(Random(1)+1),-(13+Random(2)),5,0,true);

  // Sound
  ammo->Sound("MG2A_Fire");
}

public func NoWeaponChoice() { return(true); }
