/*-- Desert Eagle --*/

#strict
#include WPN2

public func HandSize() { return(800); }
public func HandX()    { return(7000); }
public func HandY()    { return(0000); }

//public func BarrelYOffset() { return(-1600); }
public func BarrelYOffset() { return(-3200); }
public func SelectionTime() { return(3*8); }

func OnSelect(int iFM)
{
   Sound("DEGL_Charge.ogg");
}

func OnReload(i)
{
  Sound("DEGL_Reload.ogg");
}

//Einzelfeuer - Kugeln
public func FMData1(int data)
{
  if(data == FM_Name)                 return("$Bullets$");
    
  if(data == FM_AmmoID)             return(STAM);
  if(data == FM_AmmoLoad)           return(9);
  
  if(data == FM_Reload)             return(58);
  if(data == FM_Recharge)           return(22);

  if(data == FM_Auto)               return(false);
  
  if(data == FM_Damage)    return(23);
  
  if(data == FM_Slot)    return(1);
  
  if(data == FM_SpreadAdd) return(95);
  if(data == FM_StartSpread) return(5);
  if(data == FM_MaxSpread) return(CH_MaxSpread/2);

  return(Default(data));
}

public func BotData1(int data)
{
  if(data == BOT_Range)    return(100);

  return(Default(data));
}

public func FMData1T1(int data)
{
  if(data == FT_Name) return("$Single$");
  return(FMData1(data));
}

public func Fire1()
{
  var user = GetUser();
  var dir = GetDir(user)*2-1;
  var angle = user->AimAngle(20,0,true);
  var x,y;
  user->WeaponEnd(x,y);
  var ammo = SALaunchBullet(x,y,GetController(user),angle+RandomX(-1,+1),230,500,GetFMData(FM_Damage));
  ammo->Sound("DEGL_Fire*.ogg");

  // Effekte
  SAMuzzleFlash(RandomX(30,60),user,x,y,angle);
  SABulletCasing(x/3,y/3,-dir*14*(Random(1)+2),-(13+Random(2)),5);
}

public func Fire1T1()
{
  Fire1();
}