/*-- APW --*/

#strict 2

#include WPN2
#include RTLR

public func HandSize()		{return 850;}
public func HandX()		{return 2000;}
public func HandY()		{return -1000;}
public func BarrelXOffset()	{return -3500;}
public func BarrelYOffset()	{return 4000;}
public func SelectionTime()	{return 75;}
public func IsSecondaryWeapon() {return true;}

local pRocket, fired;


/* Initialisierung */

public func Initialize()
{
  _inherited();

  //Waffe laden
  DoAmmo(GetFMData(FM_AmmoID), GetFMData(FM_AmmoLoad));
}

/* Rakete - Optische Steuerung */

public func FMData1(int data)
{
  if(data == FM_Name)		return "$Missiles$";

  if(data == FM_AmmoID)		return MIAM;
  if(data == FM_AmmoLoad)	return 1;

  if(data == FM_Aim)		return 1;

  if(data == FM_Slot)		return 1;

  if(data == FM_SpreadAdd)	return 300;

  if(data == FM_NoAmmoModify)	return 1;

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
  if(data == BOT_Range)		return 1000;
  if(data == BOT_DmgType)	return DMG_Explosion;
  if(data == BOT_Power)		return(BOT_Power_LongLoad);
  return Default(data);
}

public func Fire1()
{
  LaunchRocket(AAMS,Contained()->~AimAngle(10));
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
  Sound("AT4R_Launch.ogg", 0, rocket);
  SetController(GetController(user), rocket);

  //Sicht auf Rakete  
  SetPlrView(GetController(user), rocket);
  pRocket = rocket;

  //Effekte
  var ax, ay, xdir, ydir;
  user->WeaponBegin(ax,ay);
  xdir = ax-x;
  ydir = ay-y;

  if(GetEffectData(EFSM_ExplosionEffects) > 1) Sparks(7,RGB(255,128),ax-x,ay-y);
  if(GetEffectData(EFSM_ExplosionEffects) > 0)
  {
    CreateParticle("Thrust",ax,ay,xdir/2,ydir/2,80,RGBa(255,200,200,0),0,0);

    for(var i=0; i<20; ++i)
    {
      var rand = RandomX(-10,+10);
      CreateParticle("Smoke2",ax+Sin(angle,rand),ay-Cos(angle,rand),
      		RandomX(0,2*xdir),RandomX(0,2*ydir),
      		RandomX(100,160),RGBa(220,200,180,0),0,0);

      var rand = RandomX(-10,+10);
      CreateParticle("BlastSpark1",ax+Sin(angle,rand),ay-Cos(angle,rand),
      		RandomX(0,2*xdir),RandomX(0,2*ydir),
      		RandomX(60,90),RGBa(220,200,180,0),0,0);
    }
  }

  //Waffe abgefeuert
  fired = true;
  Echo("RTLR_Echo.ogg");
}

/* Raketenverfolgung */

private func Check()
{
  /*//Wenn Waffe leer, gefeuert annehmen
  if(!pRocket)
  {
    var ammoid = GetFMData(FM_AmmoID);
    var ammousage = GetFMData(FM_AmmoUsage);
    if(!CheckAmmo(ammoid,ammousage,this()))
    {
      fired = true;
    }
  }*/

  //Wegwurf wenn Rakete abgefeuert und/oder explodiert
  if(fired)
    if(!pRocket)
      if(Contained() && Contained()->~IsClonk())
      {
        if(Contained()->~IsAiming() && Contents(0, Contained()) == this) return;
        ThrowAway();
      }
      else {
        ThrowAway();
      }

  if(!Contained() || Contents(0, Contained()) != this || !Contained()->~IsClonk()) return;

  //Sicht auf existierende Rakete setzen
  if(Contained()->~IsAiming())
    if(pRocket)
      SetPlrView(GetOwner(Contained()), pRocket);
}

/* Wegwurf */

public func ThrowAway()
{
  //Kategorie wechseln
  SetCategory(C4D_Vehicle); 
  //Sch�tze das zielen einstellen lassen
  if(Contained())
  {
    //Sch�tzen verlassen
    var dir = +1;
    if(GetDir(GetUser()) == DIR_Right)
      dir = -1;

    Exit(0, 0, 0, Random((360)+1), dir,-3, Random(11)-5);

    Sound("AT4R_ThrowAway.ogg");
  }

  //Verschwinden
  FadeOut();
  
  return true;
}

/* Nachladen unm�glich */

public func ControlThrow(caller)
{
  SetUser(caller);

  if(GetFMData(FM_Aim)>0 && !(GetUser()->~IsAiming()) && !(GetUser()->~AimOverride()))
  {
    if(GetUser()->~ReadyToAim())
      GetUser()->StartAiming();

    var ammoid = GetFMData(FM_AmmoID);
    var ammousage = GetFMData(FM_AmmoUsage);
    //Waffe leer? Nicht nachladen
    if(!CheckAmmo(ammoid,ammousage,this()))
      Sound("AT4R_Empty.ogg");
    return(1);
  }

  if(!Fire())
  {
    var ammoid = GetFMData(FM_AmmoID);
    var ammousage = GetFMData(FM_AmmoUsage);
    //Waffe leer? Nicht nachladen
    if(!CheckAmmo(ammoid,ammousage,this()))
      Sound("AT4R_Empty.ogg");
  }
  return(1);
}

/* Aufnahme */

public func RejectEntrance()
{
  if(fired)
    if(!pRocket)
      return true;
}

/* Allgemein */

public func OnEmpty() {
	fired = true;
}

public func OnSelect()
{
  Sound("AT4R_Charge.ogg");
}
