/*-- Piloten-HUD --*/

#strict 2
#include BHUD

static const APCE_Overlay_RocketPod = 5;

local fRocketPod;


/* Initialisierung */

public func Initialize() 
{
  SetVisibility(VIS_None);
  iState = -1;
  Schedule("SetVisibility(VIS_Owner)", 1, 0, this);
  SetGraphics("Flares", this, BHUD, BHUD_Overlay_Flares, GFXOV_MODE_Base);
  SetGraphics("Rockets", this, AHUD, APCE_Overlay_RocketPod, GFXOV_MODE_Base);
  SetGraphics("Warning", this, BHUD, BHUD_Overlay_Warning, GFXOV_MODE_Base);
  SetGraphics("Failure", this, BHUD, BHUD_Overlay_Failure, GFXOV_MODE_Base);
  SetClrModulation(RGBa(255,255,255,255), this, BHUD_Overlay_Flares);
  SetClrModulation(RGBa(255,255,255,255), this, APCE_Overlay_RocketPod);
  SetClrModulation(RGBa(255,255,255,255), this, BHUD_Overlay_Warning);
  SetClrModulation(RGBa(255,255,255,255), this, BHUD_Overlay_Failure);
  SetState(BHUD_Ready);
  return true;
}

public func SetState(int iNewState, bool fKeepSound)
{
  //if(iState == iNewState) return;
  iState = iNewState;
  var dwArrowColor;
  if(iState == BHUD_Off)
  {
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
  else
  {
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
  else
  {
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
  if(fFlares)
  {
    SetClrModulation(dwArrowColor, this, BHUD_Overlay_Flares);
  }
  else
  {
    SetClrModulation(RGBa(255,255,255,255), this, BHUD_Overlay_Flares);
  }
  if(fRocketPod)
  {
    SetClrModulation(dwArrowColor, this, APCE_Overlay_RocketPod);
  }
  else
  {
    SetClrModulation(RGBa(255,255,255,255), this, APCE_Overlay_RocketPod);
  }

  if(pRotation) pRotation->SetClrModulation(dwArrowColor);
  if(pThrottle) pThrottle->SetClrModulation(dwArrowColor);
  if(pAltitude) pAltitude->SetClrModulation(dwArrowColor);
  if(pWind) pWind->SetClrModulation(dwArrowColor);
  return true;
}

protected func Timer()
{
  if(!pHelicopter || !pHelicopter->GetPilot() || GetOwner(this) != GetOwner(pHelicopter->GetPilot()) || GetOwner() == NO_OWNER)
  {
    RemoveObject();
    return true;
  }
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
  SetPosition(GetX()-180, GetY()+70-BoundBy((((140*1000)/BKHK_MaxThrottle)*pHelicopter->GetThrottle())/1000, 0, 140), pThrottle);
  pThrottle->SetVisibility(GetVisibility());
  //Höhe anzeigen
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

  //Flares
  fFlares = pHelicopter->CanDeployFlares();

  //Raketenpods
  fRocketPod = pHelicopter->~RocketPodsReady();

  //Status setzen
  SetObjDrawTransform(1000,0,0,0,1000,0);
  if(fDamage || pHelicopter->GetDamage() >= pHelicopter->MaxDamage()*3/4)
  {
    var fDisable = false;
    if(iDamageRemaining == 0 && Random(5))
    {
      SetState(BHUD_Error);
    }
    else
    {
      if(iDamageRemaining == 0) fDisable = true;
      if(!Random(5) && fDisable) {
        SetState(BHUD_Off, true);
      }
      else if(!Random(2))
      {
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
    {
      SetState(BHUD_Warning);
    }
    else
    {
      if(pHelicopter->GetAutopilot())
      {
        SetState(BHUD_Disabled);
      }
      else
      {
        SetState(BHUD_Ready);
      }
    }
  }
  if(iDamageRemaining > 0) iDamageRemaining--;
  if(!iDamageRemaining) fDamage = false;
  return true;
}