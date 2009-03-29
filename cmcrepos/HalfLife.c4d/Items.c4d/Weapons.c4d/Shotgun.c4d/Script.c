/*-- Shotgun --*/

#strict
#include H2WP

public func HandSize()      { return(1000); }
public func HandX()         { return(5000); }
public func HandY()         { return(2000); }
public func BarrelYOffset() { return(-2000); }

public func HandR()
{
  if(IsReloading()) return(-17);
    
  var effect = IsRecharging();
  if(effect)
    return(-BoundBy(GetEffect(0,this(),effect,6)*2,0,17));
}	

func OnSelect()
{
  if(GetAmmo())
    Sound("H2SG_Pump.ogg");
}

func OnFinishReloadStart()
{
  AddEffect("Pump", this(), 1, 1+GetFMData(FM_Recharge, 1)-25, this());
}

func OnSingleReloadStart()
{
  Sound("H2SG_Reload*.ogg");
}

public func FMData1(int data)
{
  if(data == FM_Name)               return(GetName(0,SGAM));
    
  if(data == FM_AmmoID)             return(SGAM);
  if(data == FM_AmmoLoad)           return(6);
  
  if(data == FM_SingleReload)       return(1);
  if(data == FM_PrepareReload)      return(0);
  if(data == FM_FinishReload)       return(25);
    
  if(data == FM_Reload)             return(75);
  if(data == FM_Recharge)           return(35);
  
  if(data == FM_Damage)             return(60);
  
  if(data == FM_SpreadAdd) return(150);
  if(data == FM_StartSpread) return(70);

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
  var j = GetFMData(FM_Damage, 1)/6;
  for(var i; i < j; i++)
  {
    ammo = SALaunchBullet(x,y,GetController(user),angle+RandomX(-9,+9),220+Random(60),150+Random(150),6);
  }
  ammo->Sound("H2SG_Fire*.ogg");

  // Effekte
  SAMuzzleFlash(RandomX(30,40),user,x,y,angle);
  AddEffect("Pump", this(), 1, 1+GetFMData(FM_Recharge, 1)-25, this());
}

public func FxPumpStop(object pTarget)
{
  Sound("H2SG_Pump.ogg");

  if(!GetAmmo()) return();
  
  var user = GetUser();
  var dir = GetDir(user)*2-1;
  SABulletCasing(dir*1,0,-dir*14*(Random(1)+1),-(13+Random(2)),6,RGB(255));
}