/*-- Raketenwerfer --*/

#strict 2

#include WPN2

public func HandSize()		{return 1000;}
public func HandX()		{return 1000;}
public func HandY()		{return -2500;}
public func NoWeaponChoice()	{return true;}

local pRocket, bGuiding;


/* Raketen - Optische Steuerung */

public func FMData1(int data)
{
  if(data == FM_Name)		return "$Missiles$";

  if(data == FM_AmmoID)		return MIAM;
  if(data == FM_AmmoLoad)	return 4;

  if(data == FM_Reload)		return 190;

  if(data == FM_BurstAmount)	return 4;
  if(data == FM_BurstRecharge)	return 10;

  if(data == FM_Damage)		return 10;

  if(data == FM_Slot)		return 1;

  if(data == FM_SpreadAdd)	return 300;
  if(data == FM_StartSpread)	return 100;
  if(data == FM_MaxSpread)	return 400;

  return Default(data);
}

public func FMData1T1(int data)
{
  if(data == FT_Name)		return "$Optical$";
  return FMData1(data);
}

public func Fire1T1()
{
  LaunchRocket(ROKT,Contained()->~AimAngle(10), GetFMData(FM_Damage,1));
}

public func BotData1(int data)
{
  if(data == BOT_Range)		return 600;
  if(data == BOT_DmgType)	return DMG_Explosion;

  return Default(data);
}

/* Raketen - Abschuss */

public func LaunchRocket(id rid, int angle, int dmg)
{
  var user = Contained();
  var x,y;
  user->WeaponEnd(x,y);

  var rocket = CreateObject(rid,x,y+10,GetController(user));
  rocket->Launch(angle, dmg, user);
  rocket->Sound("RLSA_Fire*.ogg");
  SetController(GetController(user), rocket);
  
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

/* Raketenverfolgung */

private func Check()
{
  if(!Contained()) return;
  
  if(Contained()->~IsThreat()) //F�r Waffengesch�tz
    if(pRocket)
      SetPlrView(GetController(Contained()), pRocket);
}

/* Allgemein */

public func OnReload()
{
  Sound("RLSA_Reload.ogg");
}