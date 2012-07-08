/*-- LaserDesignator --*/

#strict 2

#include WPN2

public func HandSize()		{return 1000;}
public func HandX()		{return 1000;}
public func HandY()		{return -2500;}
public func NoWeaponChoice()	{return true;}


protected func Construction()
{
  AddEffect("IntNoSound", this, 1, 5);
  return _inherited(...);
}

/* Raketen - Optische Steuerung */

public func FMData1(int data)
{
  if(data == FM_Name)		return "$Missiles$";

  if(data == FM_AmmoID)		return MIAM;
  if(data == FM_AmmoLoad)	return 1;

  if(data == FM_Reload)		return 300;

  if(data == FM_Recharge)	return 10;

  if(data == FM_Slot)		return 1;

  if(data == FM_SpreadAdd)	return 300;
  if(data == FM_StartSpread)	return 100;
  if(data == FM_MaxSpread)	return 400;

  return Default(data);
}

public func FMData1T1(int data)
{
  if(data == FT_Name)		return "$Laser$";
  return FMData1(data);
}

public func Fire1T1()
{
  LaunchRocket(AGMS,Angle(GetX(), 0, GetX(Contained()->GetLaser()), GetY(Contained()->GetLaser())));
}

public func BotData1(int data)
{
  if(data == BOT_Range)		return 600;
  if(data == BOT_DmgType)	return DMG_Explosion;

  return Default(data);
}

/* Raketen - Abschuss */

public func LaunchRocket(id rid, int angle)
{
  //Austritt bestimmen
  var user = Contained();
  var x,y;
  user->WeaponEnd(x,y);

  //Rakete abfeuern
  var rocket = CreateObject(rid,x,-GetY()-200,GetController(user));
  AddEffect("IntHeliProtection", rocket, 1, 20, 0, BKHK, LocalN("heli", Contained()));
  rocket->Launch(angle, user);
  rocket->Sound("SATW_Launch.ogg");
  SetController(GetController(), rocket);

  //Effekte
  /*
  var ax, ay, xdir, ydir;
  ax = x;
  ay = -GetY();
  xdir = Sin(angle, 10);
  ydir = -Cos(angle, 10);

  if(GetEffectData(EFSM_ExplosionEffects) > 1) Sparks(5,RGB(255,128),ax-x,ay-y);
  if(GetEffectData(EFSM_ExplosionEffects) > 0)
  {
    CreateParticle("Thrust",ax,ay,xdir/2,ydir/2,80,RGBa(255,200,200,0),0,0);

    for(var i=0; i<20; ++i)
    {
      var rand = RandomX(-10,+10);
      CreateParticle("Smoke2",ax+Sin(angle,rand),ay-Cos(angle,rand),
      		RandomX(0,2*xdir),RandomX(0,2*ydir),
      		RandomX(80,140),RGBa(220,200,180,0),0,0);

      var rand = RandomX(-10,+10);
      CreateParticle("BlastSpark1",ax+Sin(angle,rand),ay-Cos(angle,rand),
      		RandomX(0,2*xdir),RandomX(0,2*ydir),
      		RandomX(40,70),RGBa(220,200,180,0),0,0);
    }
  }
  */

  Echo("RTLR_Echo.ogg");
}

/* Allgemein */

public func OnEmpty()
{
  if(Contained() && Contained()->~IsWeaponRack())
    Contained()->~OnEmpty();
}