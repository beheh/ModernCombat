/*--- MAV ---*/

#strict 2
#include CVHC

local cur_Attachment;
local iXDir;
local iYDir;
local iXTendency;
local iYTendency;
local iSpeed;
local fIsAiming;
local iAimAngle;
local iPat_Dir;
local crosshair;
local pLaser;
local pBeam;
local iC4Count;

public func AttractTracer(object pTracer)	{return GetPlayerTeam(GetController(pTracer)) != GetTeam() && !fDestroyed;}
public func IsBulletTarget()			{return !fDestroyed;}
public func IsDestroyed()			{return fDestroyed;}
public func AimAngle()				{return iAimAngle;}	//Winkel auf Ziel
public func ReadyToFire()			{return 1;}
public func IsMachine()				{return true;}
public func IsThreat()				{return !IsDestroyed();}
public func UpdateCharge()			{return 1;}
public func GetAttWeapon()			{return cur_Attachment;}
public func IsAiming() 				{return fIsAiming;}
public func GetLaser()				{return pLaser;}
public func Sgn(int x)				{if (x < 0) return x / x * -1; return x / x;}
public func IsMAV()				{return true;}
public func MaxDamage()				{return 40;}
public func MaxRotLeft()
{
  return 120;
}

public func MaxRotRight()
{
  return 240;
}


/* Initialisierung */

protected func Initialize()
{
  //Standardwerte setzen
  iSpeed = 40;
  iAimAngle = 180;

  //Ausrüsten
  Arm(LRDR);

  Sound("MAVE_Engine.ogg", 0, 0, 70, 0, +1);

  SetAction("Idle");
}

/* Zerstörung */

public func OnDestruction()
{
  //Explosion
  FakeExplode(4, GetController()+1, this);

  //Deaktivieren
  Idle();
  SetAction("Destroyed");

  //Verschwinden
  FadeOut();
}

/* Positionsbestimmung */

public func WeaponAt(&x, &y, &r)
{
  x = Sin(GetR()-180, 7000);
  y = -Cos(GetR()-250, 7000);
  r = iAimAngle+630+GetR();

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

/* Timer */

private func FlyingTimer()
{
  if(iXDir < iXTendency)
    iXDir++;
  if(iXDir > iXTendency)
    iXDir--;

  if(iYDir < iYTendency)
    iYDir++;
  if(iYDir > iYTendency)
    iYDir--;

  if(GetY() <= GetDefCoreVal("Offset", "DefCore", MAVE, 1) * -1 && iYTendency < 0)
  {
    iYTendency = 0;
    iYDir = 0;
  }

  SetXDir(iXDir / (iC4Count+1));
  SetYDir(iYDir / (iC4Count+1));

  //Blinklicht (alle 30 Frames)
  if(!(GetActTime()%30))
  {
    if(GetTeam())
      var rgb = GetTeamColor(GetTeam());
    else if(GetOwner())
      var rgb = GetPlrColorDw(GetOwner());
    else
      var rgb = RGB(255, 255, 255);
    CreateParticle("FlashLight", 0, 4, 0, 0 , 45, rgb, this);
  }

  //Nachladen und C4 prüfen (alle 5 Frames)
  if(!(GetActTime()%5))
  {
    if((GetAmmo(GetAttWeapon()->GetFMData(FM_AmmoID), GetAttWeapon()) < GetAttWeapon()->GetFMData(FM_AmmoUsage)) && !GetAttWeapon()->IsReloading())
      Reload();

    //C4 beeinflusst die Fluggeschwindigkeit
    iC4Count = 0;

    for(var pC4 in FindObjects(Find_Distance(50, 0, 0), Find_Func("IsC4Explosive")))
    {
      if(LocalN("pStickTo",pC4) != this)
        continue;
      iC4Count++;
    }
  }

  if(fIsAiming)
  {
    //Waffe vorhanden?
    if(!GetAttWeapon()) return;
    //Funktionstüchtig?
    if(EMPShocked()) return;
    if(IsDestroyed()) return;
    
    cur_Attachment->SetTeam(GetTeam());
  
  
    //Überdrehung nach links und rechts verhindern
    if(AimAngle() <= MaxRotLeft() && iPat_Dir < 0)
    {
      iPat_Dir = 0;
    }
    else if(AimAngle() >= MaxRotRight() && iPat_Dir > 0)
    {
      iPat_Dir = 0;
    }
    
    iAimAngle += iPat_Dir;
  
    if(crosshair)
    {
      if(AimAngle()+GetR() <= 360)
        crosshair->SetAngle(AimAngle()-GetR()+360);
      else
        crosshair->SetAngle(AimAngle()-GetR());
    }
    
    if(!pLaser)
      pLaser = CreateObject(LRDT,0,0,GetOwner(this));
      
   var number = GetEffect("ShowWeapon", this);

  var xPos = GetX(), yPos = GetY(), x = GetX(), y = GetY(), xdir = Sin(AimAngle(), 3000), ydir = Cos(AimAngle(), -3000);
  var gravity = GetGravity();
 
  SetGravity(0);
  if (!SimFlight(x, y, xdir, ydir))
    pLaser->Stop();
  else 
    if(!pLaser->Active())
      pLaser->Start();
      
  SetGravity(gravity);

  var pEnemy;

  if(pLaser->Active())
    pEnemy = FindObject2(Find_OnLine(0, 0, x - xPos, y - yPos), Find_Hostile(GetOwner(this)), Find_NoContainer(), Find_Or(Find_OCF(OCF_Alive), Find_Func("IsBulletTarget"), Find_Func("IsCMCVehicle")), Sort_Distance(0, 0));
  else
    pEnemy = FindObject2(Find_OnLine(0, 0, xdir, ydir), Find_Hostile(GetOwner(this)), Find_NoContainer(), Find_Or(Find_OCF(OCF_Alive), Find_Func("IsBulletTarget"), Find_Func("IsCMCVehicle")), Sort_Distance(0, 0));
  
  //Es folgen episch awesome funktionierende Berechnungen by Monkstar
  if(pEnemy)
  {
    if(!pLaser->Active())
      pLaser->Start();
    
    x = GetX(pEnemy);
    y = GetY(pEnemy);
    
    var xOff, yOff;
    
    if(xPos > x)
      xOff = GetDefCoreVal("Width", "DefCore", GetID(pEnemy)) + GetDefCoreVal("Offset", "DefCore", GetID(pEnemy), 0) + x;
    else
      xOff = GetDefCoreVal("Offset", "DefCore", GetID(pEnemy), 0) + x;
      
    if(yPos > y)
      yOff = GetDefCoreVal("Height", "DefCore", GetID(pEnemy)) + GetDefCoreVal("Offset", "DefCore", GetID(pEnemy), 1) + y;
    else
      yOff = GetDefCoreVal("Offset", "DefCore", GetID(pEnemy), 1) + y;
    
    if(Inside(xPos, Min(x, xOff), Max(x, xOff)))
    {
      x = Sin(AimAngle(), (yOff - yPos) * 1000 / (-Cos(AimAngle(), 1000))) + xPos;
      y = -Cos(AimAngle(), (yOff - yPos) * 1000 / (-Cos(AimAngle(), 1000))) + yPos;
    }
    else
      if(Inside(yPos, Min(y, yOff), Max(y, yOff)))
      {
        x = Sin(AimAngle(), (xOff - xPos) * 1000 / (Sin(AimAngle(), 1000))) + xPos;
        y = -Cos(AimAngle(), (xOff - xPos) * 1000 / (Sin(AimAngle(), 1000))) + yPos;
      }
      else
        if((Angle(xPos, yPos, xOff, yOff) >= 180 && Angle(xPos, yPos, xOff, yOff) < AimAngle()) || (Angle(xPos, yPos, xOff, yOff) <= 180 && Angle(xPos, yPos, xOff, yOff) > AimAngle()))
        {
          x = Sin(AimAngle(), (yOff - yPos) * 1000 / (-Cos(AimAngle(), 1000))) + xPos;
          y = -Cos(AimAngle(), (yOff - yPos) * 1000 / (-Cos(AimAngle(), 1000))) + yPos;
        }
        else
        {
          x = Sin(AimAngle(), (xOff - xPos) * 1000 / (Sin(AimAngle(), 1000))) + xPos;
          y = -Cos(AimAngle(), (xOff - xPos) * 1000 / (Sin(AimAngle(), 1000))) + yPos;
        }

  }
  
  if(!pBeam)  
    pBeam = CreateObject(LRBM, 0, 0, GetOwner(this));
  else
    pBeam->SetPosition(xPos, yPos);
    
  pBeam->SetVisibility(VIS_Owner | VIS_Allies);
  
  if(pEnemy || pLaser->Active())
    pBeam->SetObjDrawTransform(100 * Distance(xPos, yPos, x, y), 0, -458 * Distance(xPos, yPos, x, y), 0, 1000, 0);
  else
    pBeam->SetObjDrawTransform(100 * Distance(xPos, yPos, xPos + xdir/3, yPos + ydir/3), 0, -458 * Distance(xPos, yPos, xPos + xdir/3, yPos + ydir/3), 0, 1000, 0);
  
  pBeam->SetR(AimAngle()+90);
  SetPosition(x, y, pLaser);
  
  //Find_OnLine(int x, int y, int x2, int y2)
   /*
   
   if (x > 0)
    x = 10000 * (LandscapeWidth() - posX) / x;
   else
    x = -10000 * posX / x;
   
   if (y > 0)
     y = 10000 * posY / y;
   else
     y = -10000 * (LandscapeHeight() - posY) / y;
   
   if (posX + Sin(AimAngle(), y) > LandscapeWidth() - 1)
   {
     x = posX + Sin(AimAngle(), x);
     y = posY - Cos(AimAngle(), x);
   }
   else
   {
      x = posX + Sin(AimAngle(), y);
     y = posY - Cos(AimAngle(), y);
   }
   if (AimAngle()<225)
  {
    PathFree2(posX, posY, x, y);
    SetPosition(posX, posY, pLaser);
  }
  else
  {
    PathFree2(x, y, posX, posY);
    SetPosition(x, y, pLaser);
  }
  */
  //Log(Format("angle: %d, xPrev %d, yPrev %d, xNew %d, yNew %d, Material %s", iAimAngle, x, y, posX, posY,MaterialName(GetMaterial(posX, posY))));
   //SetPosition(posX, posY, pLaser);
  }
}

public func Idle()
{
  iXTendency = 0;
  iYTendency = 0;
  iXDir = 0;
  iYDir = 0;

  Sound("MAVE_Engine.ogg", 0, 0, 70, 0, -1);

  EndAim();
}

public func Arm(id idWeapon)
{

  //Waffe erstellen
  var pLaser = CreateObject(idWeapon, 0, 0, GetOwner(this));
  Enter(this, pLaser);

  //Und konfigurieren
  SetObjectOrder(this, pLaser, 1);
  cur_Attachment = pLaser;
  LocalN("controller", pLaser) = this;
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

private func InitAim()
{
  //Fadenkreuz entfernen falls vorhanden
  if(crosshair)
    RemoveObject(crosshair);

  //Besitzer setzen
  crosshair = CreateObject(HCRH, 0, 0, GetOwner(this));
  crosshair->Init(this);

  if (pLaser)
    pLaser->SetOwner(GetOwner(this));

  if(AimAngle()+GetR() >= 360)
    crosshair->SetAngle(AimAngle()+GetR()-360);
  else
    crosshair->SetAngle(AimAngle()+GetR());

  AddEffect("ShowWeapon", this, 20, 1, this);
}

public func EndAim()
{
  if(crosshair)
    RemoveObject(crosshair);

  if (GetEffect("ShowWeapon",this))
    RemoveEffect("ShowWeapon", this);

  if (pLaser)
    pLaser->Stop();

  if(pBeam)
    RemoveObject(pBeam);

  fIsAiming = false;
  iPat_Dir = 0;
}

/* Steuerung */

public func ControlLeft(pByObj)
{
  if(fIsAiming)
  {
    iPat_Dir = 1;
    return true;
  }

  if(iXTendency > 0)
    iXTendency = 0;
  else
      iXTendency = -iSpeed;

  return true;
}

public func ControlLeftDouble(pByObj)
{
  iXTendency = -iSpeed;

  return true;
}

public func ControlLeftReleased(pByObj)
{
  iXTendency = 0;
  return true;
}

public func ControlRight(pByObj)
{

  if(fIsAiming)
  {
    iPat_Dir = -1;
    return true;
  }

  if(iXTendency < 0)
    iXTendency = 0;
  else
    iXTendency = iSpeed;

  return true;
}

public func ControlRightDouble(pByObj)
{
  iXTendency = iSpeed;

  return true;
}

public func ControlRightReleased(pByObj)
{
  iXTendency = 0;
  return true;
}

public func ControlDown(pByObj)
{

  if(fIsAiming)
  {
    iPat_Dir = 0;
    return true;
  }

  if(iYTendency < 0)
    iYTendency = 0;
  else
    iYTendency = iSpeed;

  return true;
}

public func ControlDownDouble(pByObj)
{
  iYTendency = iSpeed;

  return true;
}

public func ControlUp(object pByObj)
{
  if(iYTendency > 0)
    iYTendency = 0;
  else
    iYTendency = -iSpeed;

  return true;
}

public func ControlUpDouble(pByObj)
{
  iYTendency = -iSpeed;

  return true;
}

public func ControlThrow(pByObj)
{
  if(!fIsAiming)
  {
    InitAim();
    fIsAiming = true;
    
  }
  else
  {
    if(pLaser && pLaser->Active())
      GetAttWeapon()->ControlThrow(this);
    else
      (PlayerMessage(GetOwner(pByObj), "$MarkRequired$", this));
  }
}

/* Aufschlag */

protected func Hit()
{
  Sound("HeavyHit*.ogg");
}