/*-- Tavor --*/

#strict
#include WPN2

public func HandSize()      { return(800); }
public func HandX()         { return(5000); }
public func HandY()         { return(1000); }

public func OnReload(int i)
{
  Sound("TAVR_Reload");
}

//Automatikfeuer - Kugeln
public func FMData1(int data)
{
  if(data == FM_Name)                 return("$Bullets$");
    
  if(data == FM_AmmoID)             return(STAM);
  if(data == FM_AmmoLoad)           return(30);
  
  if(data == FM_Reload)             return(100);
  if(data == FM_Recharge)           return(6);

  if(data == FM_Auto)               return(true);
  
  if(data == FM_Damage)    return(9);
  
  if(data == FM_Slot)    return(1);
  
  if(data == FM_SpreadAdd) return(40);
  if(data == FM_StartSpread) return(80);

  return(Default(data));
}

public func BotData1(int data)
{
  if(data == BOT_Range)    return(600);

  return(Default(data));
}

public func FMData1T1(int data)
{
  if(data == FT_Name)                 return("$Auto$");
  if(data == FT_IconFacet)            return(FICO_Auto);
  return(FMData1(data));
}

public func FMData1T2(int data)
{
  if(data == FT_Name)                 return("$Burst$");
  if(data == FT_IconFacet)            return(FICO_Burst);
  if(data == FM_Auto)                 return(false);
  if(data == FM_SpreadAdd)            return(49);
  if(data == FM_Recharge)             return(25);
  if(data == FM_BurstAmount)          return(3);
  if(data == FM_BurstRecharge)        return(2);
  return(FMData1(data));
}

public func FMData1T3(int data)
{
  if(data == FT_Name)                 return("$Single$");
  if(data == FT_IconFacet)            return(FICO_Single);
  if(data == FM_Auto)                 return(false);
  if(data == FM_Recharge)             return(20);
  return(FMData1(data));
}

public func Fire1()
{
  var user = GetUser();
  var dir = GetDir(user)*2-1;
  var angle = user->AimAngle(10,0,true);
  var x,y;
  user->WeaponEnd(x,y);
  var ammo = SALaunchBullet(x,y,GetController(user),angle+RandomX(-1,+1),250,550+Random(50),GetFMData(FM_Damage));
  ammo->Sound("TAVR_Fire");

  // Effekte
  SAMuzzleFlash(RandomX(25,40),user,x,y-2,angle);
  SABulletCasing(x/3,y/3,-dir*14*(Random(1)+1),-(16+Random(2)),5);
}

public func Fire1T1()
{
  Fire1();
}

public func Fire1T2()
{
  Fire1();
}

public func Fire1T3()
{
  var user = GetUser();
  var dir = GetDir(user)*2-1;
  var angle = user->AimAngle(10,0,true);
  var x,y;
  user->WeaponEnd(x,y);
  var ammo = SALaunchBullet(x,y,GetController(user),angle,250,650+Random(50),GetFMData(FM_Damage));
  ammo->Sound("TAVR_Fire");

  // Effekte
  SAMuzzleFlash(RandomX(25,40),user,x,y-2,angle);
  SABulletCasing(x/3,y/3,-dir*14*(Random(1)+1),-(16+Random(2)),5);
}

func OnSelect()
{
  Sound("TAVR_Charge");
}