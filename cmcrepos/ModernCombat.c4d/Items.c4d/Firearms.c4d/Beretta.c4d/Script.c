/*-- Beretta --*/

#strict
#include WPN2

public func HandSize() { return(800); }
public func HandX()    { return(7000); }
public func HandY()    { return(0000); }

//public func BarrelYOffset() { return(-1600); }
public func BarrelYOffset() { return(-3200); }

func OnReload(i)
{
  Sound("92FS_Reload");
}

//Einzelfeuer - Kugeln
public func FMData1(int data)
{
  if(data == FM_Name)                 return("$Bullets$");
    
  if(data == FM_AmmoID)             return(STAM);
  if(data == FM_AmmoLoad)           return(15);
  
  if(data == FM_Reload)             return(35);
  if(data == FM_Recharge)           return(7);

  if(data == FM_Auto)               return(false);
  
  if(data == FM_Damage)    return(15);
  
  if(data == FM_Slot)    return(1);
  
  if(data == FM_SpreadAdd) return(80);
  if(data == FM_StartSpread) return(0);
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
  var ammo = SALaunchBullet(x,y,GetController(user),angle+RandomX(-1,+1),230,400,GetFMData(FM_Damage));
  ammo->Sound("92FS_Fire");

  // Effekte
  SAMuzzleFlash(RandomX(25,30),user,x,y,angle);
  SABulletCasing(x/3,y/3,-dir*5,-20,4);
}

public func Fire1T1()
{
  Fire1();
}


//Silencer

public func FMData1T2(int data)
{
  if(data == FT_Name) return("$Single$ [SIL]");
  if(data == FM_Damage) return(9);
  return(FMData1T1(data));
}

public func Fire1T2()
{
  var user = GetUser();
  var dir = GetDir(user)*2-1;
  var angle = user->AimAngle(20,0,true);
  var x,y;
  user->WeaponEnd(x,y);
  var ammo = SALaunchBullet(x,y,GetController(user),angle+RandomX(-1,+1),230,400,GetFMData(FM_Damage),35);
  ammo->Sound("92FS_FireSil",false,0,0,0,0,0,300);

  // Effekte
  SAMuzzleFlash(4,user,x,y,angle);
  SABulletCasing(x/3,y/3,-dir*5,-20,4);
}


//System-Gedöhns

public func OnSelectFT(int i, int j)
{
  if(j == 1)//Abschrauben?
  {
    Sound("WPN2_UnSil");
    SetAction("NoSil");
  }
  else//Aufschrauben!
  {
    Sound("WPN2_Sil");
    SetAction("Sil");
  }
}

public func Sil()
{
  SetGraphics(0,this(),SILE,1,GFXOV_MODE_Base); 
}

public func NoSil()
{
  SetGraphics(0,this(),0,1);
}