/*-- Stationäre APW --*/

#strict 2
#include WPN2

public func HandSize()		{return 1000;}
public func HandX()		{return 1000;}
public func HandY()		{return -2500;}
public func NoWeaponChoice()	{return true;}

local pRocket, fView;


protected func Construction()
{
  AddEffect("IntNoSound", this, 1, 5);
  return _inherited(...);
}

/* Raketen - Optische Steuerung */

public func FMData1(int data)
{
  if(data == FM_Name)		return "$Missiles$";

  if(data == FM_AmmoID)		return MIAM;	//ID der Munition
  if(data == FM_ProjectileID)	return AAMS;	//ID des Projektils
  if(data == FM_AmmoLoad)	return 1;	//Magazingröße

  if(data == FM_Reload)		return 205;	//Zeit für Nachladen
  if(data == FM_Recharge)	return 10;	//Zeit bis erneut geschossen werden kann

  if(data == FM_Slot)		return 1;	//Slot des Feuermodus

  if(data == FM_SpreadAdd)	return 300;	//Bei jedem Schuss hinzuzuaddierende Streuung
  if(data == FM_StartSpread)	return 100;	//Bei Auswahl der Waffe gesetzte Streuung
  if(data == FM_MaxSpread)	return 400;	//Maximaler Streuungswert

  return Default(data);
}

public func FMData1T1(int data)
{
  if(data == FT_Name)		return "$Optical$";

  return FMData1(data);
}

public func Fire1T1()
{
  LaunchRocket(AAMS,Contained()->~AimAngle(10));
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
  var rocket = CreateObject(rid,x,y+14,GetController(user));
  AddEffect("IntHeliProtection", rocket, 1, 20, 0, BKHK, LocalN("heli", Contained()));
  rocket->Launch(angle, user);
  rocket->Sound("SATW_Launch.ogg");
  SetController(GetController(), rocket);

  //Sicht auf Rakete
  SetPlrView(GetController(), rocket);
  pRocket = rocket;
  fView = true;

  //Effekte
  var ax, ay, xdir, ydir;
  user->WeaponBegin(ax,ay);
  xdir = ax-x;
  ydir = ay-y;

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
  Echo("RTLR_Echo.ogg");
}

/* Raketenverfolgung */

private func Check()
{
  if(!Contained() || !fView) return;

  if(Contained()->~IsThreat()) //Für Waffengeschütz
    if(pRocket && fView)
      SetPlrView(GetController(), pRocket);
}

public func ControlDig(object pBy)
{
  fView = !fView;
  if(!fView)
    SetPlrView(GetController(pBy), pBy);
  else
  {
    SetController(GetController(pBy));
    Check();
  }
}

/* Allgemein */

public func OnEmpty()
{
  if(Contained() && Contained()->~IsWeaponRack())
    Contained()->~OnEmpty();
}

public func OnReload()
{
  if(!GetEffect("IntNoSound", this))
    Sound("SATW_Reload.ogg", false, this);
}