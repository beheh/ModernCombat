/*-- Railgun --*/

#strict 2
#include WPN2

local laser, empty;

public func HandSize()		{return 1000;}
public func HandX()		{return 6000;}
public func HandY()		{return 1000;}
public func IsSecondaryWeapon()	{return true;}

public func SelectionTime()	{return 25;}	//Anwahlzeit


/* Kompatible Waffenaufsätze */

func PermittedAtts()
{
  return AT_Laserpointer | AT_Flashlight;
}

/* Nahkampfangriff */

public func GetMCData(int data)
{
  if(data == MC_CanStrike)	return 1;	//Waffe kann Kolbenschlag ausführen
  if(data == MC_Damage)		return 20;	//Schaden eines Kolbenschlages
  if(data == MC_Recharge)	return 50;	//Zeit nach Kolbenschlag bis erneut geschlagen oder gefeuert werden kann
  if(data == MC_Power)		return 20;	//Wie weit das Ziel durch Kolbenschläge geschleudert wird
  if(data == MC_Angle)		return 45;	//Mit welchem Winkel das Ziel durch Kolbenschläge geschleudert wird
}

/* Initialisierung */

public func Initialize()
{
  _inherited();

  //Waffe laden
  DoAmmo(GetFMData(FM_AmmoID), GetFMData(FM_AmmoLoad));
}

/* Kugeln */

public func FMData1(int data)
{
  if(data == FM_Name)		return "$Bullets$";

  if(data == FM_AmmoID)		return STAM;		//ID der Munition
  if(data == FM_AmmoLoad)	return 10;		//Magazingröße

  if(data == FM_Recharge)	return 100;		//Zeit bis erneut geschossen werden kann

  if(data == FM_Auto)		return false;		//Kein Automatikfeuer

  if(data == FM_Damage)		return 30;		//Schadenswert

  if(data == FM_Slot)		return 1;		//Slot des Feuermodus

  if(data == FM_SpreadAdd)	return 300;		//Bei jedem Schuss hinzuzuaddierende Streuung

  if(data == FM_NoAmmoModify)	return 1;		//Waffe nicht nach- oder entladbar

  return Default(data);
}

/* Kugeln - Einzelfeuer */

public func FMData1T1(int data)
{
  if(data == FT_Name)		return "$Single$";

  return FMData1(data);
}

public func Fire1T1()
{
  Fire1();
}

public func BotData1(int data)
{
  if(data == BOT_Range)		return 800;
  if(data == BOT_Power)		return(BOT_Power_3);
  return Default(data);
}

/* Kugeln - Schuss */

public func Fire1()
{
  //Austritt bestimmen
  var user = GetUser();
  var dir = GetDir(user)*2-1;
  var angle = user->AimAngle(10,0,true);
  var x,y;
  user->WeaponEnd(x,y);

  //Laser abfeuern
  laser = CreateObject(LASR,x,y,GetController(user));
  laser->SetClrModulation(GetPlrColorDw(GetController(user)));
  laser->Set(angle,8,800,10,this(),user);

  //Effekte
  SABulletCasing(x/3,y/3,-dir*14*(Random(1)+1),-(13+Random(2)),7);
  MuzzleFlash(200,user,x,y,angle,GetPlrColorDw(GetController(user)));
  MuzzleFlash(100,user,x,y,angle,RGBa(255,255,255,0),5);
  Sound("RLGN_Fire.ogg");
  Echo("RLGN_Echo.ogg");
  if(GetAmmo(GetFMData(FM_AmmoID)) >= 2)
    Sound("RLGN_Reload.ogg");
  else
    Sound("ASTR_Empty.ogg", 0, this, 0, GetOwner(user)+1);
}

/* Laser */

public func LaserStrike(object pObj, int iTime)
{
  //Effekte
  if(GetOCF(pObj) & OCF_Living)
  {
    AddFireEffect(pObj,100,RGB(80,80,80),true,50);
    Sound("RLGN_HitLiving*.ogg",0,pObj);
  }
  else
  {
    Sparks(30,GetPlrColorDw(GetController(pObj)),GetX(pObj), GetY(pObj));
    Sound("BulletHitMetal*.ogg",0,pObj);
  }

  //Einsatzschilder nicht penetrierbar
  if(GetID(pObj) == RSLH)
  {
    //Effekte
    SABulletCasing(GetX(pObj), GetY(pObj),RandomX(-5,5),RandomX(-5,5),7);

    laser->SetMaxDistance(ObjectDistance(GetUser(),pObj));

    return 1;
  }

  //Objekte im Umfeld umwerfen
  KnockDownObjects(GetX(pObj),GetY(pObj),10);

  //Objekt beschädigen
  DoDmg(GetFMData(FM_Damage, 1), DMG_Projectile, pObj);

  return;
}

public func LaserHitLandscape(int x, int y)
{
  var a = laser->GetAngle();
  var x2 = Sin(a,2);
  var y2 = -Cos(a,2);

  //Grabbares Material entfernen
  laser->DigFree(laser->GetX()+x-x2,laser->GetY()+y-y2,25);

  //Effekte
  laser->CreateParticle("Blast",x-x2,y-y2,0,0,300,GetPlrColorDw(GetController(laser)));
  laser->Sparks(30,GetPlrColorDw(GetController(laser)),x-x2,y-y2);

  return 1;
}

public func IsFiring()	{return(laser);}

/* Feuervorbereitung */

private func CheckToFire()
{
  //Timer-Anzeige entfernen
  if(GetUser())
    PlayerMessage(GetController(GetUser())," ", GetUser());

  //Schütze feuerbereit?
  if(!GetUser() || !GetUser()->~ReadyToFire() || !GetUser()->~IsClonk() || this != Contents(0, GetUser()))
  {
    SetAction("Idle");
    PlayerMessage(GetOwner(GetUser()), "$NotReady$", GetUser());
    Sound("RLGN_Abort.ogg");

    return;
  }

  //Feuer frei!
  Shoot();
}

public func ControlThrow(caller)
{
  var ammoid = GetFMData(FM_AmmoID);
  var ammousage = GetFMData(FM_AmmoUsage);
  if(!CheckAmmo(ammoid,ammousage,this()))
  {
    Sound("AT4R_Empty.ogg");
    return;
  }

  if(IsRecharging() || GetAction() == "Prepare") return 1;

  //Schuss vorbereiten
  SetAction("Prepare");

  Sound("RLGN_Click.ogg");

  return 1;
}

/* Handeffekt */

public func RechargeAnimation()		{return true;}
public func MaxRechargeRotation()	{return 10;}
public func ReloadAnimationSpeed()	{return 1 + !!IsReloading();}

/* Wegwurf */

private func Check()
{
  if(Contained() && Contained()->~IsClonk())
  {
    var c = GetUser();

    if(!CheckAmmo(GetFMData(FM_AmmoID),GetFMData(FM_AmmoUsage),this()))
    {
      if(c->~IsAiming() && Contents(0, c) == this) return;
      ThrowAway();
    }

    if(GetAction() == "Prepare" && Contents(0, c) == this)
      PlayerMessage(GetController(c),"<c %x>{{SM29}}</c>",c,InterpolateRGBa2(RGB(0,255),RGB(255,255),RGB(255,0),0,10,GetPhase()));
  }
}

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

/* Aufnahme */

public func RejectEntrance()
{
  if(empty)
    return true;
}

/* Allgemein */

public func OnEmpty()
{
  empty = true;
}

func OnSelect()
{
  Sound("RLGN_Charge.ogg");
}

public func OnDeselect()
{
  if(laser)
    RemoveObject(laser);
}