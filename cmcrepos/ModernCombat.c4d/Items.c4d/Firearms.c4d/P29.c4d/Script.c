/*-- P29 --*/

#strict
#include WPN2

public func HandSize()      { return(730); }
public func HandX()         { return(5000); }
public func HandY()         { return(2000); }

public func BarrelYOffset() { return(+2000); }
public func BarrelXOffset() { return(-1500); }
public func SelectionTime() { return(14*3); }

public func OnReload(int i)
{
  Sound("P29W_Reload.ogg");
}

//Automatikfeuer - Kugeln
public func FMData1(int data)
{
  if(data == FM_Name)                 return("$Bullets$");
    
  if(data == FM_AmmoID)             return(STAM);
  if(data == FM_AmmoLoad)           return(35);
  
  if(data == FM_Reload)             return(65);
  if(data == FM_Recharge)           return(3);

  if(data == FM_Auto)               return(true);
  
  if(data == FM_Damage)    return(6);
  
  if(data == FM_Slot)    return(1);
  
  if(data == FM_SpreadAdd) return(28);
  if(data == FM_StartSpread) return(80);
  if(data == FM_MaxSpread) return(CH_MaxSpread/3*2);

  return(Default(data));
}

public func BotData1(int data)
{
  if(data == BOT_Range)    return(400);

  return(Default(data));
}

public func FMData1T1(int data)
{
  if(data == FT_Name)                 return("$Auto$");
  if(data == FT_IconFacet)            return(FICO_Auto);
  return(FMData1(data));
}

public func FMData1T2(int data)//Einzelfeuer
{
  if(data == FT_Name)                 return("$Single$");
  if(data == FT_IconFacet)            return(FICO_Single);
  if(data == FM_Recharge)             return(9);
  if(data == FM_Auto)                 return(false);
  if(data == FM_Damage)               return(FMData1(FM_Damage)*2);
  return(FMData1(data));
}

public func Fire1()
{
  var user = GetUser();
  var dir = GetDir(user)*2-1;
  var angle = user->AimAngle(15,0,true);
  var x,y;
  user->WeaponEnd(x,y);
  var ammo = SALaunchBullet(x,y,GetController(user),angle+RandomX(-2,+2),230,350+Random(50),GetFMData(FM_Damage));
  ammo->Sound("P29W_Fire.ogg");

  // Effekte
  SAMuzzleFlash(RandomX(25,30),user,x,y-2,angle);
  SABulletCasing(x/3,y/3,-dir*14*(Random(1)+1),-(13+Random(2)),4);
}

public func Fire1T1()
{
  Fire1();
}

public func Fire1T2()
{
  var user = GetUser();
  var dir = GetDir(user)*2-1;
  var angle = user->AimAngle(15,0,true);
  var x,y;
  user->WeaponEnd(x,y);
  var ammo = SALaunchBullet(x,y,GetController(user),angle+RandomX(-1,+1),230,400,GetFMData(FM_Damage));
  ammo->Sound("P29W_Fire.ogg");

  // Effekte
  SAMuzzleFlash(RandomX(25,30),user,x,y-2,angle);
  SABulletCasing(x/3,y/3,-dir*14*(Random(1)+1),-(13+Random(2)),4);
}

//Monochrom
public func FMData2(int data)
{
  if(data == FM_Name)           return("$Monochrom$");
  if(data == FM_BurstAmount)    return(2);
  if(data == FM_BurstRecharge)  return(1);
  if(data == FM_SpreadAdd)      return(FMData1(data)/3*2);
  if(data == FM_Recharge)       return(6);
  
  return(FMData1(data));
}

public func BotData2(int data)
{
  if(data == BOT_Range)    return(300);

  return(Default(data));
}

public func FMData2T1(int data)
{
  if(data == FT_Name)                 return("$Auto$");
  if(data == FT_IconFacet)            return(FICO_Auto);
  return(FMData2(data));
}

public func FMData2T2(int data)//Einzelfeuer
{
  if(data == FT_Name)                 return("$Single$");
  if(data == FT_IconFacet)            return(FICO_Single);
  if(data == FM_Recharge)             return(9);
  if(data == FM_Auto)                 return(false);
  if(data == FM_Damage)               return(FMData2(FM_Damage)*2);
  
  if(data == FM_BurstRecharge)  return(3);
  
  return(FMData2(data));
}

public func Fire2()
{
  var user = GetUser();
  var dir = GetDir(user)*2-1;
  var x,y;
  GetUser()->WeaponEnd(x,y);

  var angle = user->AimAngle(15,0,true) + RandomX(-2,+2);
  var ammo = CreateObject(MSHT,x,y,GetController(user));
  ammo -> Launch(angle, 230+Random(40), 380+Random(70), 3, 150, GetFMData(FM_Damage, 2), 10, 1, 7,1);

  BulletCasing(x/3,y/3,-dir*17*(Random(1)+1),-(15+Random(2)),4);

  ammo->Sound("P29W_MonochromFire.ogg");

  MuzzleFlash(RandomX(40,50),user,x,y-2,angle,RGB(0,128,255));
  MuzzleFlash(RandomX(20,30),user,x,y-2,angle,RGB(0,70+Random(30),255));
}

public func Fire2T1()
{
  Fire2();
}

public func Fire2T2()
{
  var user = GetUser();
  var dir = GetDir(user)*2-1;
  var x,y;
  GetUser()->WeaponEnd(x,y);
    
  var angle = user->AimAngle(15,0,true) + RandomX(-2,+2);
  var ammo = CreateObject(MSHT,x,y,GetController(user));
  ammo -> Launch(angle, 230+Random(40), 400+Random(70), 3, 250, GetFMData(FM_Damage, 2), 10, 1, 7,1);

  BulletCasing(x/3,y/3,-dir*17*(Random(1)+1),-(15+Random(2)),4);
  
  ammo->Sound("P29W_MonochromFire.ogg");
  
  MuzzleFlash(RandomX(40,50),user,x,y-2,angle,RGB(0,128,255));
  MuzzleFlash(RandomX(20,30),user,x,y-2,angle,RGB(0,70+Random(30),255));
}

public func OnSelect(int iFM)
{
  Sound("P29W_Charge.ogg");
}

/* Da Michael Code Geass süchtig ist. ; ) */
