/*-- Raketenwerfer --*/

#strict 2

#include WPN2

public func HandSize()		{return 850;}
public func HandX()		{return 1000;}
public func HandY()		{return -2500;}

public func SelectionTime()	{return 45;}

local pRocket, bGuiding;


/* Raketen - Optische Steuerung */

public func FMData1(int data)
{
  if(data == FM_Name)		return "$Missiles$";

  if(data == FM_AmmoID)		return MIAM;
  if(data == FM_AmmoLoad)	return 1;

  if(data == FM_Reload)		return 180;
  if(data == FM_Recharge)	return 200;

  if(data == FM_Aim)		return 1;
  if(data == FM_Damage)		return 25;

  if(data == FM_Slot)		return 1;

  if(data == FM_SpreadAdd)	return 300;

  return Default(data);
}

public func FMData1T1(int data)
{
  if(data == FT_Name)		return "$Optical$";
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
  SetController(GetController(user), rocket);
  
  SetPlrView(GetController(user), rocket);
  pRocket = rocket;

  //Effekte
  Sound("RTLR_Launch*.ogg");
  var ax, ay, xdir, ydir;
  user->WeaponBegin(ax,ay);
  xdir = ax-x;
  ydir = ay-y;

  CreateParticle("Thrust",ax,ay,xdir/2,ydir/2,80,RGBa(255,200,200,0),0,0);

  for(var i=0; i<20; ++i)
  {
   var rand = RandomX(-10,+10);
   CreateParticle("Smoke2",ax+Sin(angle,rand),ay-Cos(angle,rand),
                  RandomX(0,2*xdir),RandomX(0,2*ydir),
                  RandomX(80,140),RGBa(220,200,180,0),0,0);
  }
}

/* Handeffekt */

public func HandR()
{
  var effect = IsReloading();
  if(effect)
   return Max(Sin(GetEffect(0,this,effect,6)*90/50,20),0);
  return;
}

/* Raketenverfolgung */

private func Check()
{
  if(!Contained()) return;
  
  if(Contained()->~IsAiming())
    if(pRocket)
      SetPlrView(GetOwner(Contained()), pRocket);
}

/* Allgemein */

public func OnReload()
{
  Sound("RTLR_Reload.ogg");
}

public func OnSelect()
{
  Sound("RTLR_Charge.ogg");
}