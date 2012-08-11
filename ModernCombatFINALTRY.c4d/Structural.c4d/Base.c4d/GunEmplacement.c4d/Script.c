/*-- Gesch�tzstellung --*/

#strict 2
#include CSTR

local cur_Attachment;
local aim_angle;
local iPat_Dir;
local last_id;
local crosshair;
local controller;
local iRotation;
local pShield;
local fShield;
local iTurningSpeed;

public func GetAttWeapon()			{return cur_Attachment;}
public func AimAngle()				{return aim_angle + GetR();}	//Winkel auf Ziel
public func ReadyToFire()			{return 1;}			//St�ndige Feuerbereitschaft
public func RemoveTracer()			{return IsDestroyed();}		//Tracer entfernen, wenn zerst�rt
public func DisableCH()				{return true;}			//Eventuelles Fadenkreuz des Clonks ausblenden
public func MaxDamage()				{return 100;}
public func IsMachine()				{return true;}
public func IsBulletTarget()			{return false;}
public func IsThreat()				{return !IsDestroyed();}
public func UpdateCharge()			{return 1;}
public func AttractTracer(object pTracer)	{return GetPlayerTeam(GetController(pTracer)) != GetTeam() && !IsDestroyed();}
public func IsStill()				{return true;}
public func IsAiming()				{return true;}
public func VaryingDir()			{if(AimAngle() > 0) return 3; else return 2;}
public func StartRepair()			{return true;}
public func IsGunEmplacement()			{return true;}
protected func RejectContents()			{return true;}
public func BonusPointCondition()		{return false;}
public func MaxRotLeft()
{
  if(iRotation==-90)
    return -20+iRotation;
  else
  return -75+iRotation;
}

public func MaxRotRight()
{
  if(iRotation==90)
    return 20+iRotation;
  else
   return 75+iRotation;
}


/* Initialisierung */

public func Initialize() 
{
  AddEffect("ShowWeapon", this, 1, 1, this, GetID());

  //Standardwerte setzen
  iRotation = 0;
  iTurningSpeed = 2;
  aim_angle = iRotation;
  iPat_Dir = 0;

  //Standardwaffe: Maschinenkanone
  Arm(ACCN);

  _inherited();

  SetAction("Ready");
}

/* Station�res Gesch�tz konfigurieren */

public func Set(id idWeapon, int iRotationParam , bool fShieldParam, int iTurningSpeedParam)
{
  Arm(idWeapon);
  SetRotation(iRotationParam);
  iTurningSpeed = iTurningSpeedParam;

  if(fShieldParam)
    GetShield();
}

/* Rotationsbereich festlegen */

public func SetRotation(int iRot)
{
  if(iRot == 90)
    SetGraphics("Right");
  else if(iRot == -90)
    SetGraphics("Left");
  else if(iRot == 0)
    SetGraphics(0);
  else
    return;

  iRotation = iRot;
  aim_angle = iRotation;

  Arm(last_id);

  if(fShield)
    GetShield();
}

/* Zerst�rung */

public func OnDestruction()
{
  //Waffe entfernen
  Disarm();

  //Schild entfernen, sofern vorhanden
  if(pShield)
    RemoveObject(pShield);

  var iRot = iRotation;

  //Definitions- und Aktionswechsel
  ChangeDef(_GTB);
  SetAction("Destroyed");

  if(iRot == 90)
    SetPhase(1);
  else if(iRot == -90)
    SetPhase(2);
  else if(iRot == 0)
    SetPhase(0);

  //Effekte
  if(GetEffectData(EFSM_ExplosionEffects) > 0) CastSmoke("Smoke3",8,15,0,5,250,200,RGBa(255,255,255,100),RGBa(255,255,255,130));
  if(GetEffectData(EFSM_ExplosionEffects) > 0) CastParticles("ConcreteSplinter", 8, 100, 0, 0, 40, 15, RGB(40, 20, 20));
  CastParticles("Sandbag", 10, 70, 0,0, 35, 45, RGBa(228,228,228,0), RGBa(250,250,250,50));
}

public func Destruction()
{
  RemoveEffect("ShowWeapon", this);
  EndAim();

  if(GetUser())
  {
    var pUser = GetUser();

    ObjectSetAction(pUser, "Walk");
    SetActionTargets(0, 0, pUser);
    pUser->SetHUDTarget(0);
    pUser->~ShowCH();
  }
}

/* Schaden */

public func OnDmg(int iDmg, int iType)
{
  if(iType == DMG_Projectile)	return 40;	//Projektile
  if(iType == DMG_Fire)		return 60;	//Feuer
  if(iType == DMG_Explosion)	return;		//Explosionen und Druckwellen
  if(iType == DMG_Energy)	return 50;	//Energiewaffen
  if(iType == DMG_Bio)		return 100;	//S�ure und biologische Schadstoffe

  return 50;
}

/* Bewaffnung */

public func Arm(id idWeapon)
{
  if(!idWeapon) return;
  if(!GetName(0, idWeapon)) return;

  //Eventuellen Vorg�nger entfernen
  Disarm();

  //Waffe erstellen
  var pWeapon = CreateObject(idWeapon, 0, 0, GetOwner());
  Enter(this, pWeapon);

  //Und konfigurieren
  SetObjectOrder(this, pWeapon, 1);
  aim_angle = iRotation;
  cur_Attachment = pWeapon;
  last_id = idWeapon;
  LocalN("controller", pWeapon) = this;
  Reload();

  //Effekt
  AddEffect("ShowWeapon", this, 20, 1, this);
}

public func Disarm()
{
  while(Contents())
  {
   RemoveObject(Contents());
  }
  RemoveEffect("ShowWeapon", this);
}

private func Reload()
{
  //Munitionsart bestimmen
  var AmmoID = GetAttWeapon()->~GetFMData(FM_AmmoID);
  //Munition erzeugen
  Local(0, CreateContents(AmmoID)) = GetAttWeapon()->~GetFMData(FM_AmmoLoad);
  //Feuer einstellen und nachladen
  GetAttWeapon()->~StopAutoFire();
  GetAttWeapon()->~Reload();
}

/* Schutzschild hinzuf�gen oder entfernen */

public func GetShield()
{
  //Schild entfernen sofern vorhanden
  if(pShield)
    RemoveObject(pShield);

  //Schild erstellen
  pShield = CreateObject(RSLH, 0, 0 ,GetOwner(GetUser()));
  pShield->Set(this, this);
  pShield->SetColorDw(RGB(255,255,255));

  SetOwner(GetOwner(GetUser()), pShield);

  //Und konfigurieren
  aim_angle = iRotation;
  fShield = true;
}

/* Positionsbestimmung */

public func WeaponAt(&x, &y, &r)
{
  x = Sin(GetR()-180, 7000);
  y = -Cos(GetR()-250, 7000);
  r = aim_angle+630+GetR();

  return 1;
}

public func WeaponBegin(&x, &y)
{
  var number = GetEffect("ShowWeapon", this);
  if(!number)
    return;
  x = EffectVar(2, this, number)/1000;
  y = EffectVar(3, this, number)/1000;
}

public func WeaponEnd(&x, &y)
{
  var number = GetEffect("ShowWeapon", this);
  if(!number)
   return;
  x = EffectVar(4, this, number)/1000;
  y = EffectVar(5, this, number)/1000;
}

public func GetWeaponR()
{
  var number = GetEffect("ShowWeapon", this);
  if(!number)
    return;
  return EffectVar(1, this, number);
}

public func FxActivityTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
  if(!GetUser()) 
  {
    RemoveEffect("Activity", this);
    EndAim();
    if(GetAttWeapon())
    {
      GetAttWeapon()->~StopAutoFire();
      SetOwner(-1, GetAttWeapon());
    }

    if(pShield)
    {
      SetOwner(-1, pShield);
      pShield->SetColorDw(RGB(255,255,255));
    }

    if(controller)
    {
      controller->SetHUDTarget(0);
      controller->~ShowCH();
      SetPlrView(GetOwner(controller), controller);
    }

    SetOwner(-1, this);
    controller=-1;
    iPat_Dir=0;
    return;
  }

  //Waffe vorhanden?
  if(!GetAttWeapon()) return;
  //Funktionst�chtig?
  if(EMPShocked()) return;
  if(IsDestroyed()) return;
  if(IsRepairing()) return;

  //Besitzer aktualisieren
  cur_Attachment->SetTeam(GetTeam());

  //Blinklicht (alle 30 Frames)
  if(!(iEffectTime%30))
  {
    if(GetTeam())
      var rgb = GetTeamColor(GetTeam());
    else if(GetOwner())
      var rgb = GetPlrColorDw(GetOwner());
    else
      var rgb = RGB(255, 255, 255);
    CreateParticle("FlashLight", 0, 4, 0, 0 , 45, rgb, this);
  }

  //Nachladen pr�fen (alle 5 Frames)
  if(!(iEffectTime%5))
  {
    if((GetAmmo(GetAttWeapon()->GetFMData(FM_AmmoID), GetAttWeapon()) < GetAttWeapon()->GetFMData(FM_AmmoUsage)) && !GetAttWeapon()->IsReloading())
      Reload();
  }

  //�berdrehung nach links und rechts verhindern
  if(AimAngle() <= MaxRotLeft() && iPat_Dir < 0)
  {
    iPat_Dir = 0;
  }
  else if(AimAngle() >= MaxRotRight() && iPat_Dir > 0)
  {
    iPat_Dir = 0;
  }

  //HUD aktualisieren
  var User = GetUser();
  if(User)
  {
    var UserHUD = User->GetHUD();
    if(UserHUD)
      UserHUD->Update(GetAttWeapon(), User->AmmoStoring(),User);
  }

  //Clonkposition anpassen
  UpdateDir();

  //Drehgeschwindigkeit
  if(iTurningSpeed>0)
  aim_angle += iPat_Dir*iTurningSpeed;
  else if(!(iEffectTime%-iTurningSpeed))
    aim_angle += iPat_Dir;

  if(crosshair)
  {
    if(AimAngle()+GetR() <= 360)
      crosshair->SetAngle(AimAngle()-GetR()+360);
    else
      crosshair->SetAngle(AimAngle()-GetR());
  }
}

/* EMP */

public func EMPShock()
{
  EMPShockEffect(20*35);
  if(GetAttWeapon())
   GetAttWeapon()->StopAutoFire();
  return 1;
}

/* Eingangssteuerung */

protected func ActivateEntrance(pUser)
{
  //Nicht wenn bereits besetzt
  if(GetUser())
    return(0);

  //Nur lebende Clonks
  if(!pUser->~IsClonk())
    return(0);

  //Nur, wenn das Objekt noch steht
  if(IsDestroyed())
    return(0);

  pUser->SetAction("RideStill", this);
  if(iRotation == 90)
    SetDir(1, pUser);

  //Clonkposition anpassen
  SetOwner(GetOwner(pUser));
  InitAim();
  pUser->SetHUDTarget(GetAttWeapon());
  controller = pUser;
  pUser->HideCH();
  UpdateDir();

  //Sound
  Sound("RSHL_Deploy.ogg", true, this, 100, GetOwner(pUser) + 1);

  if(last_id == RLSA)
    LocalN("fView", GetAttWeapon()) = false;

  if(pShield)
    SetOwner(GetOwner(pUser), pShield);

  if(GetAttWeapon())
    SetOwner(GetOwner(pUser), GetAttWeapon());

  if(!GetEffect("Activity",this))
    AddEffect("Activity", this, 1, 1 ,this);

  return(1);
}

public func GetUser()
{
  var pUser;
  if(pUser = FindObject2(Find_OCF(OCF_CrewMember), Find_ActionTarget(this), Find_Func("IsRiding")))
    return(pUser);
  else
    pUser=0;
}

private func ExitClonk(object byObject)
{
  //Nutzer auswerfen
  if(byObject == GetUser())
  { 
    ObjectSetAction(byObject, "Walk"); 
    SetActionTargets(0, 0, byObject); 
    EndAim();
    byObject->SetHUDTarget(0);
    controller->~ShowCH();

    if(GetAttWeapon())
    {
      GetAttWeapon()->~StopAutoFire();
      SetOwner(-1, GetAttWeapon());
    }

    if(pShield)
    {
      SetOwner(-1, pShield);
      pShield->SetColorDw(RGB(255,255,255));
    }
    SetOwner(-1, this);
    return(1); 
  }
  return(0);
}

/* Steuerung */

public func ControlLeft(pByObj)
{
  iPat_Dir = -1;
  return true;
}

public func ControlLeftReleased(pByObj)
{
  iPat_Dir = 0;
  return true;
}

public func ControlRight(pByObj)
{
  iPat_Dir = 1;
  return true;
}

public func ControlRightReleased(pByObj)
{
  iPat_Dir = 0;
  return true;
}

public func ControlDown(pByObj)
{
  iPat_Dir = 0;
  return true;
}

protected func ControlUp(object ByObj)
{
  if(GetAttWeapon())
    Reload();
  return true;
}

public func ControlDigSingle()
{
  if(GetAttWeapon())
    GetAttWeapon()->~ControlDig(...);
  return true;
}

protected func ControlDigDouble(object byObj)
{
  ExitClonk(byObj);
  return true;
}

protected func ControlThrow(object byObj)
{
  if(!GetAttWeapon())
    return true;

  SetOwner(GetController(byObj), GetAttWeapon());

  if(GetAttWeapon()->IsShooting())
    GetAttWeapon()->StopAutoFire();
  else
    GetAttWeapon()->ControlThrow(this);
  return true;
}

public func ControlUpdate(object byObj, int comdir, bool dig, bool throw) 
{
  if(!throw)
    return GetAttWeapon()->StopAutoFire();
}

/* Sonstiges */

protected func UpdateDir()
{
  var pUser = GetUser();
  if(!pUser) return;

  if(AimAngle() > 0)
  {
    SetActionData(5, pUser);
    SetDir(1, pUser);
  }
  else
  {
    SetActionData(4, pUser);
    SetDir(0, pUser);
  }
}

private func InitAim()
{
  //Fadenkreuz entfernen falls vorhanden
  if(crosshair)
    RemoveObject(crosshair);

  //Besitzer setzen
  crosshair = CreateObject(HCRH, 0, 0, GetOwner(GetUser()));
  crosshair->Init(this);

  if(AimAngle()+GetR() >= 360)
    crosshair->SetAngle(AimAngle()+GetR()-360);
  else
    crosshair->SetAngle(AimAngle()+GetR());
}

private func EndAim()
{
  if(crosshair)
    RemoveObject(crosshair);
}

public func DoHit(int iDamage)
{
  return true;
}