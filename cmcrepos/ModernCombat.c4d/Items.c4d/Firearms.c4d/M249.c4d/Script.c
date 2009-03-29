/*-- M249 --*/

#strict
#include WPN2

public func HandSize()      { return(800); }
public func HandX()         { return(5000); }
public func HandY()         { return(1000); }

public func BarrelXOffset() { return(-1000); }
public func BarrelYOffset() { return(-2000); }

func OnReload()
{
  Sound("M249_Reload");
}

func OnSelect()
{
   Sound("M249_Charge");
}

public func FMData1(int data)
{
  if(data == FM_Name)               return("$Bullets$");
    
  if(data == FM_AmmoID)             return(STAM);
  if(data == FM_AmmoLoad)           return(60);
  
  if(data == FM_Reload)             return(280);
  if(data == FM_Recharge)           return(5);

  if(data == FM_Auto)               return(true);
  
  if(data == FM_Damage)             return(9);
  
  if(data == FM_SpreadAdd) return(35);
  if(data == FM_StartSpread) return(200);

  return(Default(data));
}

public func BotData1(int data)
{
  if(data == BOT_Range)    return(600);
  if(data == BOT_Power)    return(BOT_Power_LongLoad);

  return(Default(data));
}

//Feuertechniken
public func FMData1T1(int data)//Automatik
{
  if(data == FT_Name)                 return("$Auto$");
  if(data == FT_IconFacet)            return(FICO_Auto);
  return(FMData1(data));
}

public func FMData1T2(int data)//Einzelfeuer
{
  if(data == FT_Name)                 return("$Single$");
  if(data == FM_Recharge)             return(30);
  if(data == FM_Auto)                 return(false);
  if(data == FT_IconFacet)            return(FICO_Single);
  if(data == FM_Damage)               return(FMData1(FM_Damage)*2);
  return(FMData1(data));
}

//Feuer!
public func Fire1()
{
  var user = GetUser();
  var dir = GetDir(user)*2-1;
  var angle = user->AimAngle(10,0,true);
  var x,y;
  user->WeaponEnd(x,y);
  var ammo = SALaunchBullet(x,y,GetController(user),angle+RandomX(-2,+2),250,600,GetFMData(FM_Damage));
  ammo->Sound("M249_Fire");

  // Effekte
  SAMuzzleFlash(RandomX(35,50),user,x,y,angle);
  SABulletCasing(x/3,y/3,-dir*14*(Random(1)+1),-(13+Random(2)),5);
}

public func Fire1T1()
{
  Fire1();
}

public func Fire1T2()
{
  var user = GetUser();
  var dir = GetDir(user)*2-1;
  var angle = user->AimAngle(10,0,true);
  var x,y;
  user->WeaponEnd(x,y);
  var ammo = SALaunchBullet(x,y,GetController(user),angle+RandomX(-1,+1),250,600,GetFMData(FM_Damage));
  ammo->Sound("M249_Fire");

  // Effekte
  SAMuzzleFlash(45,user,x,y,angle);
  SABulletCasing(x/3,y/3,-dir*14*(Random(1)+1),-(13+Random(2)),5);
}