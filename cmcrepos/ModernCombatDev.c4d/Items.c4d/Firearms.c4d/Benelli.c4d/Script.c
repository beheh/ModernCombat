/*-- Benelli --*/

#strict
#include WPN2

public func HandSize()      { return(1000); }
public func HandX()         { return(5000); }
public func HandY()         { return(2000); }

public func BarrelYOffset() { return(-3000); }

public func HandR()
{
  //if(IsReloading() || IsRecharging()) return(-20);
  
  var effect = IsReloading();
  if(effect)
    return(-BoundBy(GetEffect(0,this(),effect,6)*2,0,20));
    
  effect = IsRecharging();
  if(effect)
    return(-BoundBy(GetEffect(0,this(),effect,6)*2,0,20));
}		

/*
  Wird aufgerufen, wenn eine Feuertechnik ausgewählt wurde.
*/
public func OnSelectFT(int iFireMode, int iFireTec)
{
  //...
}

func OnSelect()
{
  if(GetAmmo())
    Sound("BNEL_Pump");
}

func OnFinishReloadStart()
{
  AddEffect("Pump", this(), 1, 1+GetFMData(FM_Recharge, 1)-25, this());
}

func OnSingleReloadStart()
{
  Sound("BNEL_Reload");
}

//Automatikfeuer - Kugeln
public func FMData1(int data)
{
  if(data == FM_Name)               return("$Pellets$");
    
  if(data == FM_AmmoID)             return(STAM);
  if(data == FM_AmmoLoad)           return(32);
  if(data == FM_AmmoUsage)          return(4);
  
  if(data == FM_SingleReload)       return(1);
  if(data == FM_PrepareReload)      return(0);
  if(data == FM_FinishReload)       return(25);
  
  if(data == FM_Reload)             return(75);
  if(data == FM_Recharge)           return(65);
  
  if(data == FM_Damage)             return(90);
  
  if(data == FM_SpreadAdd) return(170);
  if(data == FM_StartSpread) return(90);

  return(Default(data));
}

public func BotData1(int data)
{
  if(data == BOT_Range)    return(300);
  return(Default(data));
}

public func Fire1()
{
  var user = GetUser();
  var dir = GetDir(user)*2-1;
  var angle = user->AimAngle(0,0,true);
  var x,y;
  user->WeaponEnd(x,y);

  var ammo;
  var j = GetFMData(FM_Damage, 1)/5;
  for(var i; i < j; i++)
  {
    ammo = SALaunchBullet(x,y,GetController(user),angle+RandomX(-8,+8),220+Random(60),200+Random(50),5);
  }
  ammo->Sound("BNEL_Fire");

  // Effekte
  SAMuzzleFlash(RandomX(40,50),user,x,y,angle);
  SAMuzzleFlash(55,user,x,y,angle);
  AddEffect("Pump", this(), 1, 1+GetFMData(FM_Recharge, 1)-25, this());//Typ übergeben?
}

public func FxPumpStop(object pTarget)
{
  Sound("BNEL_Pump");

  if(!GetAmmo()) return();
  
  var user = GetUser();
  var dir = GetDir(user)*2-1;
  SABulletCasing(dir*1,0,-dir*14*(Random(1)+1),-(13+Random(2)),6,RGB(255));
}