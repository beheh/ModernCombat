/*-- SMG --*/

#strict

#include H2WP

// Anzeige in der Hand
public func HandSize() { return(900); }
public func HandX()    { return(5500); }
public func HandY()    { return(-1000); }
public func BarrelYOffset() { return(-2700); }

public func OnReload(int i)
{
  Sound("H2MG_Reload.ogg");
}

public func FMData1(int data)
{
  if(data == FM_Name)      return(GetName());
  if(data == FM_AmmoID)    return(MGAM);
  if(data == FM_AmmoLoad)  return(45);

  if(data == FM_Reload)    return(46);
  if(data == FM_Recharge)  return(3);
  if(data == FM_AmmoWarning) return(11);
  if(data == FM_Auto) return(true);
  
  if(data == FM_Damage)    return(7);
  
  if(data == FM_SpreadAdd) return(22);
  if(data == FM_StartSpread) return(50);
  if(data == FM_MaxSpread) return(CH_MaxSpread/2);

  return(Default(data));
}

public func BotData1(int data)
{
  if(data == BOT_Range)    return(500);
  if(data == BOT_Power)    return(BOT_Power_2);

  return(Default(data));
}

public func Fire1()
{ 
  var user = GetUser();
  var dir = GetDir(user)*2-1;
  var angle = user->AimAngle(15,0,true);
  var x,y;
  user->WeaponEnd(x,y);
  var ammo = SALaunchBullet(x,y,GetController(user),angle+RandomX(-2,+2),230,400,GetFMData(FM_Damage));
  ammo->Sound("H2MG_Fire*.ogg");

  // Effekte
  SAMuzzleFlash(RandomX(25,30),user,x,y,angle);
  SABulletCasing(dir*6,3,-dir*5,-20,4);
}