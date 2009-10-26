/*-- P90 --*/

#strict
#include WPN2

public func HandSize()      { return(900);  }
public func HandX()         { return(5000); }
public func HandY()         { return(1000); }
public func BarrelXOffset() { return(-1500);} 

func OnReload()
{
  Sound("FP90_Reload");
}

func OnSelect()
{
   Sound("FP90_Charge");
}

public func FMData1(int data)
{
  if(data == FM_Name)               return("$Bullets$");
    
  if(data == FM_AmmoID)             return(STAM);
  if(data == FM_AmmoLoad)           return(25);
  if(data == FM_AmmoRate)           return(2);
  
  if(data == FM_Reload)             return(120);
  if(data == FM_Recharge)           return(3);

  if(data == FM_Auto)               return(true);
  
  if(data == FM_Damage)             return(5);
  
  if(data == FM_SpreadAdd) return(34);
  if(data == FM_StartSpread) return(50);

  return(Default(data));
}

public func BotData1(int data)
{
  if(data == BOT_Range)    return(400);

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
  if(data == FT_IconFacet)            return(FICO_Single);
  if(data == FM_Recharge)             return(15);
  if(data == FM_Auto)                 return(false);
  if(data == FM_Damage)               return(FMData1(FM_Damage)*3/2);
  return(FMData1(data));
}

//Feuer!
public func Fire1()
{
  var user = GetUser();
  var dir = GetDir(user)*2-1;
  var angle = user->AimAngle(15,0,true);
  var x,y;
  user->WeaponEnd(x,y);
  var ammo = SALaunchBullet(x,y,GetController(user),angle+RandomX(-2,+2),230,350+Random(50),GetFMData(FM_Damage));
  ammo->Sound("FP90_Fire");

  // Effekte
  SAMuzzleFlash(RandomX(15,25),user,x,y,angle);
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
  var ammo = SALaunchBullet(x,y,GetController(user),angle+RandomX(-1,+1),230,350+Random(50),GetFMData(FM_Damage));
  ammo->Sound("FP90_Fire");

  // Effekte
  SAMuzzleFlash(RandomX(15,25),user,x,y,angle);
  SABulletCasing(x/3,y/3,-dir*14*(Random(1)+1),-(13+Random(2)),4);
}

//Silencer

public func FMData1T3(int data)
{
  if(data == FT_Name)                 return("$Auto$ [SIL]");
  if(data == FT_Condition)            return(SilEnabled());
  return(FMData1T1(data));
}

public func Fire1T3()
{
  var user = GetUser();
  var dir = GetDir(user)*2-1;
  var angle = user->AimAngle(15,0,true);
  var x,y;
  user->WeaponEnd(x,y);
  var ammo = SALaunchBullet(x,y,GetController(user),angle+RandomX(-2,+2),230,350+Random(50),GetFMData(FM_Damage),35);
  ammo->Sound("WPN2_FireSil",false,0,0,0,0,0,300);

  // Effekte
  SAMuzzleFlash(4,user,x,y,angle);
  SABulletCasing(x/3,y/3,-dir*14*(Random(1)+1),-(13+Random(2)),4);
}

public func FMData1T4(int data)
{
  if(data == FT_Name)                 return("$Single$ [SIL]");
  if(data == FT_Condition)            return(SilEnabled());
  return(FMData1T2(data));
}

public func Fire1T4()
{
  var user = GetUser();
  var dir = GetDir(user)*2-1;
  var angle = user->AimAngle(15,0,true);
  var x,y;
  user->WeaponEnd(x,y);
  var ammo = SALaunchBullet(x,y,GetController(user),angle+RandomX(-1,+1),230,350+Random(50),GetFMData(FM_Damage),35);
  ammo->Sound("WPN2_FireSil",false,0,0,0,0,0,300);

  // Effekte
  SAMuzzleFlash(4,user,x,y,angle);
  SABulletCasing(x/3,y/3,-dir*14*(Random(1)+1),-(13+Random(2)),4);
}

//System-Gedöhns

public func OnSelectFT(int iFM, int iFT, int iLastFT)
{
  if((iFT <= 2) && (iLastFT >= 3))
  {
    Sound("WPN2_UnSil");
    SetAction("NoSil");
  }
  
  if((iFT >= 3) && (iLastFT <= 2))
  {
    Sound("WPN2_Sil");
    SetAction("Sil");
  }
}

public func SilEnabled()
{
  if(GetUser())
    if(GetUser()->FindContents(GOLD))//Gold bringt Glück? o,O
      return(true);
  return(false);
}

public func NoSil()
{
  SetGraphics(0,this(),0,1);
}

public func Sil()
{
  SetGraphics(0,this(),SILE,1,GFXOV_MODE_Base); 
}