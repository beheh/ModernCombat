/*-- Raketenwerfer --*/

#strict 2

#include WPN2

public func HandSize()	{return 850;}
public func HandX()	{return 1000;}
public func HandY()	{return -2000;}

public func SelectionTime() { return(3*3); }


/* Raketen */

public func FMData1(int data)
{
  if(data == FM_Name)		return "$Missiles$";

  if(data == FM_AmmoID)		return MIAM;
  if(data == FM_AmmoLoad)	return 1;

  if(data == FM_Reload)		return 200;
  if(data == FM_Recharge)	return 200;

  if(data == FM_Aim)		return 3;
  if(data == FM_Damage)		return 35;

  if(data == FM_Slot)    return(1);

  if(data == FM_SpreadAdd) return(100);

  return Default(data);
}

/* Raketen - Lasererfassung */

public func FMData1T1(int data)
{
  if(data == FT_Name)		return "$Laser$";
  return FMData1(data);
}

public func Fire1T1()
{
  Fire1();
}

public func BotData1(int data)
{
  if(data == BOT_Range)		return 600;
  if(data == BOT_DmgType)	return DMG_Explosion;

  return Default(data);
}

public func Fire1()
{
  LaunchRocket(ROKT,Contained()->~AimAngle(10), GetFMData(FM_Damage,1));
}

public func LaunchRocket(id rid, int angle, int dmg)
{
  var user = Contained();
  var x,y;
  user->WeaponEnd(x,y);

  var rocket = CreateObject(rid,x,y+10,GetController(user));
  rocket->Launch(angle, dmg, user);

  //Effekte
  var ax, ay, xdir, ydir;
  user->WeaponBegin(ax,ay);
  xdir = ax-x;
  ydir = ay-y;

  CreateParticle("Thrust",ax,ay,xdir/2,ydir/2,80,RGBa(255,200,200,0),0,0);

  for(var i = 0; i < 10; ++i)
  {
   var rand = RandomX(-7,+7);
   CreateParticle("Smoke2",ax+Sin(angle,rand),ay-Cos(angle,rand),
                  RandomX(0,xdir),RandomX(0,ydir),
                  RandomX(80,100),RGBa(255,255,255,128),0,0);
  }

  Sound("RTLR_Launch*.ogg");
}

/* Allgemein */

public func OnReload()
{
  Sound("RTLR_Reload.ogg");
}

public func HandR()
{
  var effect = IsReloading();
  if(effect)
   return Max(Sin(GetEffect(0,this(),effect,6)*90/50,20),0);
  return 0;
}
