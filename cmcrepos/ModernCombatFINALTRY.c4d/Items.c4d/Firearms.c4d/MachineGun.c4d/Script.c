/*-- Maschinengewehr --*/

#strict
#include WPN2

public func HandSize()      { return(800); }
public func HandX()         { return(5000); }
public func HandY()         { return(1000); }

public func BarrelXOffset() { return(-1000); }
public func BarrelYOffset() { return(-2000); }
public func SelectionTime() { return(12*3); }


//Kugeln - Automatikfeuer

public func FMData1(int data)
{
  if(data == FM_Name)               return("$Bullets$");
    
  if(data == FM_AmmoID)             return(STAM);
  if(data == FM_AmmoLoad)           return(60);
  
  if(data == FM_Reload)             return(280);
  if(data == FM_Recharge)           return(5);

  if(data == FM_Auto)               return(true);
  
  if(data == FM_Damage)             return(8);
  
  if(data == FM_SpreadAdd) return(30);
  if(data == FM_StartSpread) return(250);

  return(Default(data));
}

public func FMData1T1(int data)
{
  if(data == FT_Name)                 return("$Auto$");
  if(data == FT_IconFacet)            return(FICO_Auto);
  return(FMData1(data));
}

public func Fire1T1()
{
  Fire1();
}

public func BotData1(int data)
{
  if(data == BOT_Range)    return(600);
  if(data == BOT_Power)    return(BOT_Power_LongLoad);

  return(Default(data));
}

public func Fire1()
{
  var user = GetUser();
  var dir = GetDir(user)*2-1;
  var angle = user->AimAngle(10,0,true);
  var x,y;
  user->WeaponEnd(x,y);
  var ammo = SALaunchBullet(x,y,GetController(user),angle+RandomX(-2,+2),250,700,GetFMData(FM_Damage));
  ammo->Sound("MNGN_Fire.ogg");

  // Effekte
  SAMuzzleFlash(RandomX(35,50),user,x,y,angle);
  SABulletCasing(x/3,y/3,-dir*14*(Random(1)+1),-(13+Random(2)),5);
}


//Kugeln - Stoßfeuer

public func FMData1T2(int data)
{
  if(data == FT_Name)                 return("$Burst$");
  if(data == FM_Recharge)             return(30);
  if(data == FT_IconFacet)            return(FICO_Burst);
  if(data == FM_Auto)                 return(false);
  if(data == FM_BurstAmount)          return(4);
  if(data == FM_BurstRecharge)        return(5);
  if(data == FM_SpreadAdd)            return(55);
  return(FMData1(data));
}

public func Fire1T2()
{
  var user = GetUser();
  var dir = GetDir(user)*2-1;
  var angle = user->AimAngle(10,0,true);
  var x,y;
  user->WeaponEnd(x,y);
  var ammo = SALaunchBullet(x,y,GetController(user),angle+RandomX(-1,+1),250,700,GetFMData(FM_Damage));
  ammo->Sound("MNGN_Fire.ogg");

  // Effekte
  SAMuzzleFlash(45,user,x,y,angle);
  SABulletCasing(x/3,y/3,-dir*14*(Random(1)+1),-(13+Random(2)),5);
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