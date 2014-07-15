/*-- Waffengestell --*/

#strict 2


local cur_Attachment;
local aim_angle;
local pController;
local last_id;
local last_r, last_owner_r;
local iPat_Dir;
local pOwner,vis;
local Crosshair;
local Rad,Ang;
local rot_left,rot_right;
local blinkspeed;

public func GetAttWeapon()		{return cur_Attachment;}						//Waffe
public func MaxRotLeft()		{return rot_left+GetDir(pOwner)*(180-rot_right+180-rot_left)+GetR();}	//Maximaler Winkel links
public func MaxRotRight()		{return rot_right+GetDir(pOwner)*(180-rot_right+180-rot_left)+GetR();}	//Maximaler Winkel rechts
public func AimAngle()			{return aim_angle+GetR();}						//Winkel auf Ziel
public func ReadyToFire()		{return 1;}								//Allzeit bereit
public func IsAiming()			{return true;}								//Geschütz immer am Zielen
public func IsThreat()			{return pController;}							//Status
public func UpdateCharge()		{return 1;}
public func BonusPointCondition()	{return false;}								//Bonuspunkte macht der BlackHawk 
public func IsWeaponRack()		{return 1;}
public func IsRepairable()		{return false;}

/* Aufrufe */

public func Set(object pTarget, int iRad, int iAng, int iRotLeft, int iRotRight)
{
  Rad = iRad;
  Ang = iAng;
  rot_left = iRotLeft;
  rot_right = iRotRight;
  pOwner = pTarget;
  SetObjectOrder(pTarget);
  blinkspeed = RandomX(27,33);
  SetAction("Floating");
  AddEffect("IntTimer", this, 1, 1, this);
}

public func SetGunner(object pObj)
{
  //Zielen & HUD
  if(pController)
    pController->SetHUDTarget(0);
  if(!pObj)
    EndAim();
  else
  {
    SetController(GetOwner(pObj));
    InitAim();
    pObj->SetHUDTarget(GetAttWeapon());
  }
  
  //Besitzer, Vorbereitung, etc...
  pController = pObj;
  cur_Attachment->~StopAutoFire();
  iPat_Dir = 0;
}

public func Arm(id idWeapon)
{
  //Crash-Vermeidung
  if(!idWeapon) return;
  if(!GetName(0, idWeapon)) return;

  //Ausrüsten mit idWeapon
  var pWeapon = CreateObject(idWeapon, 0, 0, GetOwner());
  Enter(this,pWeapon);
   
  //Ordnern
  SetObjectOrder(this, pWeapon, 1);
  aim_angle = 180;
  cur_Attachment = pWeapon;
  LocalN("controller", pWeapon) = this;
  Reload();
  
  //Effekt
  AddEffect("ShowWeapon", this, 20, 1, this);
}

public func Disarm()
{
  while(Contents())
  {
   last_id = GetID(Contents());
   RemoveObject(Contents());
  }
  RemoveEffect("ShowWeapon", this);
}

public func StopAutoFire()
{
  //An Waffe weiterleiten
  if (cur_Attachment)
    return cur_Attachment->~StopAutoFire(...);
}

/* Steuerung */

public func ControlUp(pByObj)
{
  Reload();
  return true;
}

public func ControlDown(pByObj)
{
  iPat_Dir = 0;
  return true;
}

public func ControlLeft(pByObj)
{
  iPat_Dir = 1;
  return true;
}

public func ControlLeftReleased(pByObj)
{
  iPat_Dir = 0;
  return true;
}

public func ControlRight(pByObj)
{
  iPat_Dir = -1;
  return true;
}

public func ControlRightReleased(pByObj)
{
  iPat_Dir = 0;
  return true;
}

public func ControlLeftDouble(pByObj)
{
  iPat_Dir = 2;
  return true;
}

public func ControlRightDouble(pByObj)
{
  iPat_Dir = -2;
  return true;
}

public func ControlThrow(pByObj)
{
  if(!GetAttWeapon())
    return true;
  SetController(GetController(pByObj), GetAttWeapon());
  if(GetAttWeapon()->IsShooting() && !GetPlrCoreJumpAndRunControl(GetController(pByObj)))
    GetAttWeapon()->StopAutoFire();
  else
    GetAttWeapon()->ControlThrow(this);
  return true;
}

public func ControlDig()
{
  if (GetAttWeapon())
    GetAttWeapon()->~ControlDig(...);
}

protected func FxIntTimerTimer(object pTarget, int iEffect, int iTime)
{
  //Waffe vorhanden?
  if(!GetAttWeapon())
    return;

  //Hostobjekt vorhanden?
  if(!pOwner)
  {
    OnDestruction();
    return RemoveObject();
  }

  
  //Rotation des Besitzers abfragen
  var rot = GetR(pOwner) + (GetDir(pOwner) * 2 - 1) * (90 + Ang);

  //Und in die Positionsbestimmung einfließen lassen
  SetPosition(GetX(pOwner) + Sin(rot, Rad),
              GetY(pOwner) - Cos(rot, Rad), this());
  SetR(GetR(pOwner));

  //Besitzer aktualisieren
  SetOwner(GetOwner(pOwner));
  cur_Attachment->SetOwner(GetOwner());

  //Transparenz anpassen
  SetClrModulation(GetClrModulation(pOwner));

  //Alle X Frames
  if(!(iTime % blinkspeed) && pController)
  {
    if(pOwner->GetTeam())
      var rgb = GetTeamColor(pOwner->GetTeam());
    else if(GetOwner(pOwner) != NO_OWNER)
      var rgb = GetPlrColorDw(pOwner->GetOwner());
    else
      var rgb = RGB(255, 255, 255);
    CreateParticle("FapLight", 0, 4, 0, 0, 60, LightenColor(rgb), this);
  }

  /* Geschütz fahren */

  //Rotation
  
	aim_angle += iPat_Dir;
	//Links?
	if(AimAngle() < MaxRotLeft())
	{
	  aim_angle = MaxRotLeft()-GetR();
	  iPat_Dir = 0; //Anhalten
	}
	//Rechts?
	else if(AimAngle() > MaxRotRight())
	{
	  aim_angle = MaxRotRight()-GetR();
	  iPat_Dir = 0; //Anhalten
	}

  //Crosshair nachziehen
  if(Crosshair) {
    var r = AimAngle()-GetR();
    if(last_r != r || GetR() != last_owner_r)
    {
      Crosshair->SetAngle(r);
      last_owner_r = GetR();
      last_r = r;
    }
  }

  //Sichtbarkeit prüfen
  if(pOwner->~HideWeaponRack(this))
  {
    if(vis)
    {
      SetVisibility(VIS_None, GetAttWeapon());
      SetVisibility(VIS_None);
      vis = false;
    }
  }
  else
    if(!vis)
    {
      SetVisibility(VIS_All, GetAttWeapon());
      SetVisibility(VIS_All);
      vis = true;
    }
}

public func OnEmpty() {
	Reload();
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

/* Fadenkreuz */

private func InitAim()
{
  if(Crosshair)
    RemoveObject(Crosshair);

  //Besitzer wird in Init gesetzt
  Crosshair = CreateObject(HCRH);
  Crosshair->Init(this);
  Crosshair->SetAngle(AimAngle());
} 

private func EndAim()
{
  if(Crosshair)
    RemoveObject(Crosshair);
}

/* Zerstörung */

public func OnDestruction()
{
  //Waffe entfernen
  Disarm();

  //Effekte
  if(GetEffectData(EFSM_ExplosionEffects) > 0) CastParticles("Smoke3",3,20,0,0,220,500);
  if(GetEffectData(EFSM_ExplosionEffects) > 1) CastParticles("ConcreteSplinter",4,100,0,0,40,15,RGB(40,20,20));
}

public func Destruction()
{
  Disarm();
  RemoveEffect("ShowWeapon", this);
}

/* Schaden */

public func Damage()
{
  return;
}

/* Waffenende */

public func WeaponAt(&x, &y, &r)
{
  x = Sin(GetR()-180,7000);
  y = -Cos(GetR()-180,7000);
  r = aim_angle+270+GetR();
  return 1;
}

public func WeaponBegin(&x, &y)
{
  var number = GetEffect("ShowWeapon",this);
  if(!number)
    return;
  x = EffectVar(2, this, number)/1000;
  y = EffectVar(3, this, number)/1000;
}

public func WeaponEnd(&x, &y)
{
  var number = GetEffect("ShowWeapon",this);
  if(!number)
    return;
  x = EffectVar(4, this, number)/1000;
  y = EffectVar(5, this, number)/1000;
}

public func GetWeaponR()
{
  var number = GetEffect("ShowWeapon",this);
  if(!number)
    return;
  return EffectVar(1, this, number);
}
