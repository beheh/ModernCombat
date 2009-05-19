/*-- Pumpgun --*/

#strict
#include WPN2

public func HandSize()      { return(1000); }
public func HandX()         { return(5000); }
public func HandY()         { return(2000); }
public func BarrelYOffset() { return(-2000); }

public func HandR()
{
  //if(IsReloading() || IsRecharging()) return(-17);

  var effect = IsReloading();
  if(effect)
    return(-BoundBy(GetEffect(0,this(),effect,6)*2,0,17));
    
  effect = IsRecharging();
  if(effect)
    return(-BoundBy(GetEffect(0,this(),effect,6)*2,0,17));
}	

public func OnSelectFT(int iFireMode, int iFireTec)
{
  //...
}

func OnSelect()
{
  if(GetAmmo())
    Sound("PPGN_Pump*.ogg");
}

func OnFinishReloadStart()
{
  AddEffect("Pump", this(), 1, 1+GetFMData(FM_Recharge, 1)-25, this());
}

func OnSingleReloadStart()
{
  Sound("PPGN_Reload.ogg");
}

public func FMData1(int data)
{
  if(data == FM_Name)               return("$Pellets$");
    
  if(data == FM_AmmoID)             return(STAM);
  if(data == FM_AmmoLoad)           return(20);
  if(data == FM_AmmoUsage)          return(4);
  
  if(data == FM_SingleReload)       return(1);
  if(data == FM_PrepareReload)      return(25);
  if(data == FM_FinishReload)       return(20);
    
  if(data == FM_Reload)             return(75);
  if(data == FM_Recharge)           return(50);
  
  if(data == FM_Damage)             return(40);
  
  if(data == FM_SpreadAdd) return(170);
  if(data == FM_StartSpread) return(60);

  return(Default(data));
}

public func BotData1(int data)
{
  if(data == BOT_Range)    return(400);
  return(Default(data));
}

public func Fire1()
{
  var user = GetUser();
  var dir = GetDir(user)*2-1;
  var angle = user->AimAngle(10,0,true);
  var x,y;
  user->WeaponEnd(x,y);

  var ammo;
  var j = GetFMData(FM_Damage, 1)/10;
  for(var i; i < j; i++)
  {
    ammo = SALaunchBullet(x,y,GetController(user),angle+RandomX(-6,+6),220+Random(60),260+Random(40),10);
  }
  ammo->Sound("PPGN_Fire*.ogg");

  // Effekte
  MuzzleFlash(RandomX(40,55),user,x,y,angle,RGB(0,128,255));
  //SAMuzzleFlash(RandomX(40,45),user,x,y,angle,RGB(255,255,255));
  SAMuzzleFlash(25,user,x,y,angle);
  AddEffect("Pump", this(), 1, 1+GetFMData(FM_Recharge, 1)-25, this());
}

public func FxPumpStop(object pTarget)
{
  Sound("PPGN_Pump*.ogg");

  if(!GetAmmo()) return();

  var user = GetUser();
  var dir = GetDir(user)*2-1;
  SABulletCasing(dir*1,0,-dir*14*(Random(1)+1),-(13+Random(2)),6,RGB(255));
}