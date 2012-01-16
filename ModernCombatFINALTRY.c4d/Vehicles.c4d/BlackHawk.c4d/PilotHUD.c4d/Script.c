/*-- Piloten-HUD --*/

#strict 2

static const BHUD_Off = -1;
static const BHUD_Ready = 0;
static const BHUD_Warning = 1;
static const BHUD_Error = 2;
static const BHUD_Disabled = 3;

static const BHUD_Overlay_Flares = 1;
static const BHUD_Overlay_SmokeWall = 2;
static const BHUD_Overlay_Warning = 3;
static const BHUD_Overlay_Failure = 4;

local pHelicopter;
local iState;
local pRotation, pThrottle, pAltitude, pWind;
local fDamage, iDamageRemaining;
local fFlares, fSmokeWall;

/* Initialisierung */

public func Initialize() 
{
  SetVisibility(VIS_None);
  iState = -1;
  fFlares = true;
  fSmokeWall = true;
  Schedule("SetVisibility(VIS_Owner)", 1, 0, this);
  SetGraphics("Flares", this, GetID(), BHUD_Overlay_Flares, GFXOV_MODE_Base);
  SetGraphics("SmokeWall", this, GetID(), BHUD_Overlay_SmokeWall, GFXOV_MODE_Base);
  SetGraphics("Warning", this, GetID(), BHUD_Overlay_Warning, GFXOV_MODE_Base);
  SetGraphics("Failure", this, GetID(), BHUD_Overlay_Failure, GFXOV_MODE_Base);
  SetClrModulation(RGBa(255,255,255,255), this, BHUD_Overlay_Flares);
  SetClrModulation(RGBa(255,255,255,255), this, BHUD_Overlay_SmokeWall);
  SetClrModulation(RGBa(255,255,255,255), this, BHUD_Overlay_Warning);
  SetClrModulation(RGBa(255,255,255,255), this, BHUD_Overlay_Failure);
  SetState(BHUD_Ready);
  return true;
}

protected func SetState(int iNewState, bool fKeepSound, bool fUpdate)
{
  if(iState == iNewState && !fUpdate) return;
  iState = iNewState;
  var dwArrowColor;
  if(iState == BHUD_Off) {
    SetClrModulation(RGBa(0,0,0,255));
    dwArrowColor = RGBa(0,0,0,255);
  }
  if(iState == BHUD_Error)
  {
    SetClrModulation(RGBa(255,0,0,50));
    SetClrModulation(RGBa(255,0,0,50), this, BHUD_Overlay_Failure);
    dwArrowColor = RGBa(255,0,0,50);
    Sound("WarningDamage.ogg", false, pHelicopter, 100, GetOwner()+1, +1);
  }
  else {
    SetClrModulation(RGBa(255,255,255,255), this, BHUD_Overlay_Failure);
    if(!fKeepSound)
    {
      Sound("WarningDamage.ogg", false, pHelicopter, 100, GetOwner()+1, -1);
    }
  }
  if(iState == BHUD_Warning)
  {
    SetClrModulation(RGBa(255,153,0,50));
    SetClrModulation(RGBa(255,153,0,50), this, BHUD_Overlay_Warning);
    dwArrowColor = RGBa(255,153,0,50);
    Sound("WarningLockon.ogg", false, pHelicopter, 100, GetOwner()+1, +1);
  }
  else {
    SetClrModulation(RGBa(255,255,255,255), this, BHUD_Overlay_Warning);
    if(!fKeepSound)
    {
      Sound("WarningLockon.ogg", false, pHelicopter, 100, GetOwner()+1, -1);
    }
  }
  if(iState == BHUD_Ready)
  {
    SetClrModulation(RGBa(0,153,255,50));
    dwArrowColor = RGBa(255,204,0,50);
  }
  if(iState == BHUD_Disabled)
  {
    SetClrModulation(RGBa(122,122,122,50));
    dwArrowColor = RGBa(122,122,122,50);
  }
  if(fFlares) {
    SetClrModulation(dwArrowColor, this, BHUD_Overlay_Flares);
  }
  else {
    SetClrModulation(RGBa(255,255,255,255), this, BHUD_Overlay_Flares);
  }
  if(fSmokeWall) {
    SetClrModulation(dwArrowColor, this, BHUD_Overlay_SmokeWall);
  }
  else {
    SetClrModulation(RGBa(255,255,255,255), this, BHUD_Overlay_SmokeWall);
  }
  if(pRotation) pRotation->SetClrModulation(dwArrowColor);
  if(pThrottle) pThrottle->SetClrModulation(dwArrowColor);
  if(pAltitude) pAltitude->SetClrModulation(dwArrowColor);
  if(pWind) pWind->SetClrModulation(dwArrowColor);
  return true;
}

protected func GetState() {
  return iState;
}

public func SetHelicopter(object pNewHelicopter)
{
  pHelicopter = pNewHelicopter;
  Align();
  return true;
}

public func DamageReceived()
{
  fDamage = true;
  iDamageRemaining = 10;
  return true;
}

protected func Align()
{
  SetPosition(BoundBy(GetX(pHelicopter), GetDefWidth(GetID())/2+GetDefWidth(BARW)+10, LandscapeWidth()-GetDefWidth(GetID())/2-GetDefWidth(BARW)-10),
    BoundBy(GetY(pHelicopter), GetDefHeight(GetID())/2+GetDefHeight(BARW)+10, LandscapeHeight()-GetDefHeight(GetID())/2-GetDefHeight(BARW)-10));
  return true;
}

protected func Timer()
{
  if(!pHelicopter || !pHelicopter->GetPilot() || GetOwner(this) != GetOwner(pHelicopter->GetPilot()) || GetOwner() == NO_OWNER)
  {
    RemoveObject();
    return true;
  }
  if(iState != BHUD_Off) {
    //Ausrichten
    Align();
    //Rotation anzeigen
    if(!pRotation)
    {
      pRotation = CreateObject(BRTN,0,0,GetOwner());
      pRotation->SetOwner(GetOwner());
      pRotation->SetClrModulation(RGBa(255,204,0,50));
    }
    SetPosition(GetX(), GetY()+56, pRotation);
    pRotation->SetVisibility(GetVisibility());
    pRotation->SetR(GetR(pHelicopter));
    //Throttle anzeigen
    if(!pThrottle)
    {
      pThrottle = CreateObject(BARW,0,0,GetOwner());
      pThrottle->SetOwner(GetOwner());
      pThrottle->SetClrModulation(RGBa(255,204,0,50));
    }
    SetPosition(GetX()-180, GetY()+70-BoundBy((((140*1000)/max_throttle)*pHelicopter->GetThrottle())/1000, 0, 140), pThrottle);
    pThrottle->SetVisibility(GetVisibility());
    //H�he anzeigen
    if(!pAltitude)
    {
      pAltitude = CreateObject(BARW,0,0,GetOwner());
      pAltitude->SetR(180);
      pAltitude->SetOwner(GetOwner());
      pAltitude->SetClrModulation(RGBa(255,204,0,50));
    }
    SetPosition(GetX()+180, GetY()+64-BoundBy(140-((((140*1000)/LandscapeHeight())*GetY(pHelicopter))/1000), 0, 140), pAltitude);
    pAltitude->SetVisibility(GetVisibility());
    //Wind anzeigen
    if(!pWind)
    {
      pWind = CreateObject(BARW,0,0,GetOwner());
      pWind->SetR(90);
      pWind->SetOwner(GetOwner());
      pWind->SetVisibility(VIS_Owner);
      pWind->SetClrModulation(RGBa(255,204,0,50));
    }
    SetPosition(GetX()-4+BoundBy((1400*GetWind(AbsX(GetX(pHelicopter)), AbsY(GetY(pHelicopter))))/1000, -69, 71), GetY()-98, pWind);
    pWind->SetVisibility(GetVisibility());
    //Status setzen
    SetObjDrawTransform(1000,0,0,0,1000,0);
  }
  //Flares und SmokeWall
  var fUpdate = false;
  var tFlares = pHelicopter->CanDeployFlares();
  var tSmokeWall = pHelicopter->CanDeploySmokeWall();
  if(fFlares != tFlares) {
      fUpdate = true;
      if(!fFlares) Sound("WarningFlaresReloaded.ogg", false, pHelicopter, 100, GetOwner()+1);
      fFlares = tFlares;
  }
  if(fSmokeWall != tSmokeWall) {
      fUpdate = true;
      if(!fSmokeWall) Sound("WarningSmokeWallReloaded.ogg", false, pHelicopter, 100, GetOwner()+1);
      fSmokeWall = tSmokeWall;
  }
  if(fUpdate) {
    SetState(GetState(), false, true);
  }
  //Schadensverhalten
  if(fDamage || pHelicopter->GetDamage() >= pHelicopter->MaxDamage()*3/4)
  {
    var fDisable = false;
    if(iDamageRemaining == 0 && Random(5)) {
      SetState(BHUD_Error);
    }
    else {
      if(iDamageRemaining == 0) fDisable = true;
      if(!Random(2) && fDisable) {
        SetState(BHUD_Off, true);
      }
      else if(!Random(5)) {
        SetState(BHUD_Error);
        if(!Random(2)) {
          var val = RandomX(0,300);
          if(!Random(2)) val *= -1;
          SetObjDrawTransform(RandomX(800, 1200),val,RandomX(-5,5)*1000,val,RandomX(800, 1200),RandomX(-5,5)*1000);
        }
      }
    }
  }
  else
  {
    if(pHelicopter->GetRocket())
      SetState(BHUD_Warning);
    else
      if(pHelicopter->GetAutopilot())
        SetState(BHUD_Disabled);
      else
        SetState(BHUD_Ready);
  }
  if(iDamageRemaining > 0) iDamageRemaining--;
  if(!iDamageRemaining) fDamage = false;
  return true;
}

public func Destruction()
{
  SetState(BHUD_Ready);
  if(pRotation) RemoveObject(pRotation);
  if(pThrottle) RemoveObject(pThrottle);
  if(pAltitude) RemoveObject(pAltitude);
  if(pWind) RemoveObject(pWind);
  return true;
}
