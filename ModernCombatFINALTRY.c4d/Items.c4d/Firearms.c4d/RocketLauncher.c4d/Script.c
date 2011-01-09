/*-- Raketenwerfer --*/

#strict 2

#include WPN2

public func HandSize()		{return 850;}
public func HandX()		{return 1000;}
public func HandY()		{return -2500;}
public func BarrelXOffset()	{return -3500;}

public func SelectionTime()	{return 45;}

local pRocket;


/* Raketen - Optische Steuerung */

public func FMData1(int data)
{
  if(data == FM_Name)		return "$Missiles$";

  if(data == FM_AmmoID)		return MIAM;
  if(data == FM_AmmoLoad)	return 1;

  if(data == FM_Reload)		return 180;
  if(data == FM_Recharge)	return 200;

  if(data == FM_Aim)		return 1;

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
  if(data == BOT_Power)   return(BOT_Power_LongLoad);
  return Default(data);
}

public func Fire1()
{
  LaunchRocket(MISL,Contained()->~AimAngle(10));
}

public func LaunchRocket(id rid, int angle)
{
  //Austritt bestimmen
  var user = GetUser();
  var x,y;
  user->WeaponEnd(x,y);

  //Rakete abfeuern
  var rocket = CreateObject(rid,x,y+10,GetController(user));
  rocket->Launch(angle, user);
  Sound("RTLR_Launch*.ogg", 0, rocket);
  SetController(GetController(user), rocket);

  //Sicht auf Rakete  
  SetPlrView(GetController(user), rocket);
  pRocket = rocket;

  //Effekte
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

public func AimAngleChange(bool fJNR)
{
  //Schnelleres Zielen
  if(fJNR)
    return 4;
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
  if(!Contained() || Contents(0, Contained()) != this) return;

  //Sicht auf existierende Rakete setzen
  if(Contained()->~IsAiming())
    if(pRocket)
      SetPlrView(GetOwner(Contained()), pRocket);
}

public func RejectSwitch()
{
  //Sicht wegnehmen
  if (!_inherited(...) && pRocket)
    SetPlrView(GetOwner(Contained()), Contained());
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