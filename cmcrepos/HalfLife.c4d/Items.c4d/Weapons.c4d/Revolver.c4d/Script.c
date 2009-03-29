/*-- Revolver --*/

#strict

#include H2WP

// Anzeige in der Hand
public func HandSize() { return(900); }
public func HandX()    { return(5500); }
public func HandY()    { return(-2000); }
public func BarrelYOffset() { return(-2700); }

local has_shells;

public func OnReload(int i)
{
  Sound("H2RW_Reload.ogg");
  if(has_shells)  AddEffect("ShellOut",this(),10,45,this());
  has_shells = true;
}

public func FxShellOutTimer(object pTarget)
{
  var dir = GetDir(GetUser())*2-1;
  for(var i = GetFMData(FM_AmmoLoad); i > 0; i--)
    SABulletCasing(dir*6,3,-dir*Random(2),-Random(4),4,0,RSHT);
    
  return(-1);
}

public func FMData1(int data)
{
  if(data == FM_Name)      return(GetName());
  if(data == FM_AmmoID)    return(REAM);
  if(data == FM_AmmoLoad)  return(6);

  if(data == FM_Reload)    return(35*3);
  if(data == FM_Recharge)  return(25);
  if(data == FM_AmmoWarning) return(1);

  if(data == FM_Damage)    return(40);
  
  if(data == FM_SpreadAdd) return(CH_MaxSpread/4*2);
  if(data == FM_StartSpread) return(0);
  if(data == FM_MaxSpread) return(CH_MaxSpread/4*3);

  return(Default(data));
}

public func BotData1(int data)
{
  if(data == BOT_Range)    return(600);
  if(data == BOT_Power)    return(BOT_Power_1);

  return(Default(data));
}

public func Fire1()    // Projektilschuss
{ 
  var user = GetUser();
  var dir = GetDir(user)*2-1;
  var angle = user->AimAngle(0,0,true);
  var x,y;
  user->WeaponEnd(x,y);
  var ammo = SALaunchBullet(x,y,GetController(user),angle,230,600,GetFMData(FM_Damage),0,RSHT);
  ammo->Sound("H2RW_Fire*.ogg");

  // Effekte
  SAMuzzleFlash(RandomX(40,45),user,x,y,angle,0,RSHT);
}