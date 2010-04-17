/*-- Pistole --*/

#strict
#include WPN2

public func HandSize() { return(800); }
public func HandX()    { return(5500); }
public func HandY()    { return(-1000); }

public func BarrelYOffset() { return(-5000); }
public func SelectionTime() { return(3*3); }


/* Kugeln */

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
  
  if(data == FM_SpreadAdd) return(60); //Kaum Rückstoß
  if(data == FM_StartSpread) return(0); //Pistolen sind leicht
  if(data == FM_MaxSpread) return(CH_MaxSpread/3*2);

  return(Default(data));
}

/* Kugeln - Einzelfeuer */

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
  ammo->Sound("PSTL_Fire*.ogg");

  // Effekte
  SAMuzzleFlash(RandomX(25,30),user,x,y,angle);
  SABulletCasing(x/3,y/3,-dir*14*(Random(1)+1),-(13+Random(2)),4);
}

/* Allgemein */

func OnSelect(int iFM)
{
   Sound("PSTL_Charge.ogg");
}

public func OnReload(int i)
{
  Sound("PSTL_Reload.ogg");
}
