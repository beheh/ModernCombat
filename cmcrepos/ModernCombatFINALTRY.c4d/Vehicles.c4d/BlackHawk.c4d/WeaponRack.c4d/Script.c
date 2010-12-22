/*-- Waffengestell --*/

#strict 2
#include CSTR

local cur_Attachment;
local aim_angle;
local pController;
local last_id;
local iPat_Dir;
local heli,vis;
local Crosshair;
local Rad,Ang;
local rot_left,rot_right;
local blinkspeed;

public func GetAttWeapon()	{return cur_Attachment;}						//Waffe
public func MaxRotLeft()	{return rot_left+GetDir(heli)*(180-rot_right+180-rot_left)+GetR();}	//Maximaler Winkel links
public func MaxRotRight()	{return rot_right+GetDir(heli)*(180-rot_right+180-rot_left)+GetR();}	//Maximaler Winkel rechts
public func AimAngle()		{return aim_angle+GetR();}						//Winkel auf Ziel
public func ReadyToFire()	{return 1;}								//Allzeit bereit
public func IsAiming()		{return true;}								//Gesch�tz immer am Zielen
public func IsThreat()		{return pController;}							//Status
public func UpdateCharge()	{return 1;}


/* Aufrufe */

public func Set(pTarget, iRad, iAng, iRotLeft, iRotRight)
{
  Rad = iRad;
  Ang = iAng;
  rot_left = iRotLeft;
  rot_right = iRotRight;
  heli = pTarget;
  blinkspeed = RandomX(27,33);
}

public func SetGunner(pObj)
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

  //Ausr�sten mit idWeapon
  var pWeapon = CreateObject(idWeapon, 0, 0, GetOwner());
  Enter(this,pWeapon);
   
  //Ordnern
  SetObjectOrder(this, pWeapon, 1);
  aim_angle = 250;
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

public func ControlRight(pByObj)
{
  iPat_Dir = -1;
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
  if(!cur_Attachment)
    return true;
  if(GetAttWeapon()->IsShooting())
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


public func TimerCall()
{
  //Waffe vorhanden?
  if(!GetAttWeapon()) return;

  //Hosthelikopter vorhanden?
  if(!heli)
  {
    OnDestruction();
    return RemoveObject();
  }
  
  //Rotation des Heli abfragen
  var rot = GetR(heli)+(GetDir(heli)*2-1)*(90)+(GetDir(heli)*2-1)*(Ang);
  //Und in die Positionsbestimmung einflie�en lassen
  SetPosition(GetX(heli) + Sin(rot, Rad),
              GetY(heli) - Cos(rot, Rad), this());
  SetXDir(GetXDir(heli));
  SetYDir(GetYDir(heli));
  SetR(GetR(heli));
  //SetRDir(GetRDir(heli));

  //Besitzer aktualisieren
  SetOwner(GetOwner(heli));
  cur_Attachment->SetOwner(GetOwner());

  //Transparenz anpassen
  SetClrModulation(GetClrModulation(heli));

  //Alle X Frames
  if(!(GetActTime()%blinkspeed) && pController)
  {
    if(heli->GetTeam())
      var rgb = GetTeamColor(heli->GetTeam());
    else if(heli->GetOwner() != NO_OWNER)
      var rgb = GetPlrColorDw(heli->GetOwner());
    else
      var rgb = RGB(255,255,255);
    CreateParticle("FlashLight",0,4,0,0,3*15,rgb,this);
  }

  //N�tig nachzuladen?
  if(GetAmmo(GetAttWeapon()->GetFMData(FM_AmmoID), GetAttWeapon()) < GetAttWeapon()->GetFMData(FM_AmmoUsage))
    Reload();

  /* Gesch�tz fahren */

  // jedes Frame
  //links?
  if( AimAngle() < MaxRotLeft())
  {
    aim_angle = MaxRotLeft()-GetR();
    iPat_Dir = 0; //Anhalten
  }
	    
  //rechts?
  else if( AimAngle() > MaxRotRight())
  {
    aim_angle = MaxRotRight()-GetR();
    iPat_Dir = 0; //Anhalten
  }
  aim_angle += iPat_Dir;

  //Crosshair nachziehen
  if(Crosshair)
    Crosshair->SetAngle(AimAngle()-GetR());

  //An Helikopterrotation anpassen

  //Nur sichtbar, wenn Helikopter nicht drehend
  if (GetAction(heli) == "Turn")
  {
    if (vis)
    {
      SetVisibility(VIS_None,GetAttWeapon());
      SetVisibility(VIS_None);
      vis = false;
    }
  }
  else
    if (!vis)
    {
      SetVisibility(VIS_All,GetAttWeapon());
      SetVisibility(VIS_All);
      vis = true;
    }
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
  Crosshair->Init(this());
  Crosshair->SetAngle(AimAngle());
} 

private func EndAim()
{
  if(Crosshair)
    RemoveObject(Crosshair);
}

/* Zerst�rung */

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
