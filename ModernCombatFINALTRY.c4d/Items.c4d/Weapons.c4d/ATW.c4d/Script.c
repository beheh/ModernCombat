/*-- APW --*/

#strict 2

#include WPN2
#include RTLR

local pRocket, fired, guided;

public func HandSize()		{return 850;}
public func HandX()		{return 2000;}
public func HandY()		{return -1000;}
public func BarrelXOffset()	{return -3500;}
public func BarrelYOffset()	{return 4000;}
public func IsSecondaryWeapon()	{return true;}
public func GetPathFreeY()	{return -2;}

public func SelectionTime()	{return 55;}	//Anwahlzeit


/* Initialisierung */

public func Initialize()
{
  _inherited();

  //Waffe laden
  DoAmmo(GetFMData(FM_AmmoID), GetFMData(FM_AmmoLoad));
}

/* Raketen */

public func FMData1(int data)
{
  if(data == FM_Name)		return "$Missiles$";

  if(data == FM_AmmoID)		return MIAM;	//ID der Munition
  if(data == FM_ProjectileID)	return AAMS;	//ID des Projektils
  if(data == FM_AmmoLoad)	return 1;	//Magazingröße

  if(data == FM_Aim)		return 1;	//Waffe kann nur zielend abgefeuert werden

  if(data == FM_Slot)		return 1;	//Slot des Feuermodus

  if(data == FM_SpreadAdd)	return 300;	//Bei jedem Schuss hinzuzuaddierende Streuung

  if(data == FM_NoAmmoModify)	return 1;	//Waffe nicht nach- oder entladbar

  return Default(data);
}

/* Raketen - Optische Steuerung */

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
  if(data == BOT_Precision)	return 3;
  return Default(data);
}

public func Fire1()
{
  LaunchRocket(AAMS,Contained()->~AimAngle(10));
}

/* Raketen - Ungelenkt */

public func FMData1T2(int data)
{
  if(data == FT_Name)	return "$Unguided$";

  return FMData1(data);
}

public func Fire1T2()
{
  LaunchRocket(AAMS,Contained()->~AimAngle(10), true);
}

/* Raketen - Schuss */

public func LaunchRocket(id rid, int angle, bool unguided)
{
  //Austritt bestimmen
  var user = GetUser();
  var x,y;
  user->WeaponEnd(x,y);

  //Rakete abfeuern
  var rocket = CreateObject(rid,x,y+10,GetController(user));
  rocket->Launch(angle, user, unguided, ((GetX()-lastX) * 10), ((GetY()-lastY) * 10));
  Sound("AT4R_Launch.ogg", 0, rocket);
  SetController(GetController(user), rocket);
  
  //Sicht auf Rakete  
  if(!unguided) SetPlrView(GetController(user), rocket);
  pRocket = rocket;

  //Effekte
  var ax, ay, xdir, ydir;
  user->WeaponBegin(ax,ay);
  xdir = ax-x + (GetX()-lastX);
  ydir = ay-y + (GetY()-lastY);

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
  guided = !unguided;
  fired = true;
  Echo("RTLR_Echo.ogg");
}

/* Raketenverfolgung */

private func Check()
{
  //Positionsänderungen ermitteln
  lastX = lastXTemp;
  lastY = lastYTemp;
  lastXTemp = GetX();
  lastYTemp = GetY();

  //Wegwurf wenn Rakete abgefeuert und/oder explodiert
  if(fired)
    if(!pRocket || !guided)
      if(Contained() && Contained()->~IsClonk())
      {
        if(Contained()->~IsAiming() && Contents(0, Contained()) == this) return;
        ThrowAway();
      }

  if(!Contained() || Contents(0, Contained()) != this || !Contained()->~IsClonk()) return;

  //Sicht auf existierende Rakete setzen
  if(Contained()->~IsAiming())
    if(pRocket && pRocket->Guideable())
      SetPlrView(GetOwner(Contained()), pRocket);
}

/* Wegwurf */

public func ThrowAway()
{
  //Kategorie wechseln
  SetCategory(C4D_Vehicle); 
  //Waffe auswerfen sofern verschachtelt
  if(Contained())
  {
    //Schützen verlassen
    var dir = +1, rot = 0;
    if(GetDir(GetUser()) == DIR_Right)
    {
      dir = -1;
      rot = 180;
    }

    Exit(0, 0, 0, rot, dir,-3, RandomX(-8,8));

    Sound("AT4R_ThrowAway.ogg");
  }

  //Verschwinden
  FadeOut();
  
  return true;
}

/* Nachladen unmöglich */

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
    if(!pRocket || !guided)
      return true;
}

/* Allgemein */

public func OnEmpty()
{
  fired = true;
}

public func OnSelect()
{
  Sound("AT4R_Charge.ogg");
}