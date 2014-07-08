/*-- fEMP-Schock --*/

#strict 2

#appendto BKHK

local fEMP;

func EMP()
{
	fEMP = !fEMP;
	if(fEMP)
		Message("fEMP an!", this);
	else
		Message("fEMP aus!", this);
}

protected func ContainedUp(object ByObj)
{
  [$CtrlUp$]

  //Pilot
  if(ByObj == GetPilot() && !fEMP)
  {
    if(GetY() < 80)
      return;
    //Autopilot aus
    ResetAutopilot();
    //Startup-Sequence
    if(!throttle && (GetAction() == "Stand" || GetAction() == "EngineShutDown"))
      SetAction("EngineStartUp");
    //Schub geben
    if(!GetPlrCoreJumpAndRunControl(GetOwner(GetPilot())))
    {
      if(GetAction() == "Fly" || GetAction() == "Turn")
        throttle = BoundBy(throttle + BKHK_ThrottleSpeed, 0, BKHK_MaxThrottle);
    }
    else
    {
      AddEffect("BlackhawkChangeThrottle", this, 50, 3, this, GetID(), BKHK_ThrottleSpeed);
    }
  }

  //Schütze
  if(ByObj == GetGunner())
    pMGStation->~ControlUp(ByObj);
  //Koordinator
  if(ByObj == GetCoordinator())
    pRocketStation->~ControlUp(ByObj);

  return true;
}

protected func ContainedDown(object ByObj)
{
  [$CtrlDown$]

  //Pilot
  if(ByObj == GetPilot() && !fEMP)
  {
    //Autopilot aus
    ResetAutopilot();
    //Motor aus
    if(!throttle && (GetAction() == "Fly" || GetAction() == "EngineStartUp") && GetContact(0, -1, CNAT_Bottom))
      SetAction("EngineShutDown");
    //Vom Gas weg
    if(GetAction() == "Fly" || GetAction() == "Turn")
      if(GetPlrCoreJumpAndRunControl(GetOwner(GetPilot())))
        AddEffect("BlackhawkChangeThrottle", this, 50, 3, this, GetID(), -BKHK_ThrottleSpeed);
      else
        throttle = BoundBy(throttle - BKHK_ThrottleSpeed, 0, BKHK_MaxThrottle);
  }

  //Schütze
  if(ByObj == GetGunner())
    pMGStation->~ControlDown(ByObj);
  //Koordinator
  if(ByObj == GetCoordinator())
    pRocketStation->~ControlDown(ByObj);

  return true;
}

protected func ContainedUpDouble(object ByObj)
{
  [$CtrlUpD$]
  
  //Pilot
  if(ByObj == GetPilot() && !fEMP)
  {
    if(GetY() < 80)
      return;
    //Autopilot aus
    ResetAutopilot();
    if(throttle == 0 && (GetAction() == "Stand" || GetAction() == "EngineShutDown"))
      SetAction("EngineStartUp");  
    if(GetAction() == "Fly")
      throttle = BoundBy(throttle + BKHK_ThrottleSpeed * 2, 0, BKHK_MaxThrottle);
    return true;
  }
}

protected func ContainedDownDouble(object ByObj)
{
  [$CtrlDownD$]

  //Pilot
  if(ByObj == GetPilot() && !fEMP)
  {
    //Autopilot aus
    ResetAutopilot();
    //Motor aus
    if(throttle == 0 && (GetAction() == "Fly" || GetAction() == "EngineStartUp") && GetContact(0, -1, CNAT_Bottom))
      SetAction("EngineShutDown");
    //Vom Gas weg
    if(GetAction() == "Fly")
      throttle = BoundBy(throttle - BKHK_ThrottleSpeed*2, 0, 170);
  }

  //Passagiere
  if(ByObj == GetPassenger1() || ByObj == GetPassenger2())
    SetCommand(ByObj,"Exit");
  return true;
}

protected func ContainedLeft(object ByObj)
{
  [$CtrlLeft$]

  //Pilot
  if(ByObj == GetPilot() && !fEMP)
  {
    //Autopilot aus
    ResetAutopilot();
    if(GetAction() == "Fly" || GetAction() == "Turn")
      if(GetPlrCoreJumpAndRunControl(GetController(ByObj)))
        rotation = -BKHK_MaxRotation;
      else
        rotation = BoundBy(rotation - BKHK_ControlSpeed, -BKHK_MaxRotation, BKHK_MaxRotation);
  }

  //Schütze
  if(ByObj == GetGunner())
    pMGStation->~ControlLeft(ByObj);
  //Koordinator
  if(ByObj == GetCoordinator())
    pRocketStation->~ControlLeft(ByObj);

  return true;
}

protected func ContainedRight(object ByObj, fRelease)
{
  [$CtrlRight$]
  
  //Pilot
  if(ByObj == GetPilot() && !fEMP)
  {
    //Autopilot aus
    ResetAutopilot();
    if(fRelease)
      rotation = GetR();
    else 
      if(GetAction() == "Fly" || GetAction() == "Turn")
        if(GetPlrCoreJumpAndRunControl(GetController(ByObj)))
          rotation = BKHK_MaxRotation;
        else
          rotation = BoundBy(rotation + BKHK_ControlSpeed, -BKHK_MaxRotation, BKHK_MaxRotation);
  }

  //Schütze
  if(ByObj == GetGunner())
    pMGStation->~ControlRight(ByObj);
  //Koordinator
  if(ByObj == GetCoordinator())
    pRocketStation->~ControlRight(ByObj);

  return true;
}

protected func ContainedLeftDouble(object ByObj)
{
  [$CtrlLeftD$]
  //Pilot
  if(ByObj == GetPilot() && !fEMP)
  {
    //Autopilot aus
    ResetAutopilot();
    if(GetDir() && GetAction() == "Fly")
      if(GetAction() == "Turn" || GetContact(this, -1))
        return true;
      else
        SetAction("Turn");
  }

  //Schütze
  if(ByObj == GetGunner())
    pMGStation->~ControlLeftDouble(ByObj);
  //Koordinator
  if(ByObj == GetCoordinator())
    pRocketStation->~ControlLeftDouble(ByObj);

  return true;
}

protected func ContainedRightDouble(object ByObj)
{
  [$CtrlRightD$]
  
  //Pilot
  if(ByObj == GetPilot() && !fEMP)
  {
    //Autopilot aus
    ResetAutopilot();
    if(!GetDir() && GetAction() == "Fly")
      if(GetAction() == "Turn" || GetContact(this, -1))
        return true;
      else
        SetAction("Turn");
  }

  //Schütze
  if(ByObj == GetGunner())
    pMGStation->~ControlRightDouble(ByObj);
  //Koordinator
  if(ByObj == GetCoordinator())
    pRocketStation->~ControlRightDouble(ByObj);

  return true;
}

protected func ContainedThrow(object ByObj)
{
  [Image=KOKR|$CtrlThrow$]
  
  //nicht wenn kaputt
  if(GetDamage() > MaxDamage())
    return true;
  //Piloten-Speedmenü
  if(ByObj == GetPilot() && !fEMP)
  {
    var ring = CreateSpeedMenu(this, ByObj);

    var overlay;

    //Flareabwurf
    //Nur wenn geladen
    if(CanDeployFlares())
    {
      overlay = ring->AddThrowItem("$Flares$", "DeployFlares", ByObj, SMIN);
      SetGraphics("6", ring, SMIN, overlay, GFXOV_MODE_IngamePicture);
    }

    //Rauchwand
    //Nur wenn geladen
    if(CanDeploySmokeWall())
    {
      overlay = ring->AddLeftItem("$Smoke$", "DeploySmoke", ByObj, SMIN);
      SetGraphics("7", ring, SMIN, overlay, GFXOV_MODE_IngamePicture);
    }

    //HUD ein- oder ausblenden
    overlay = ring->AddRightItem("$HUD$", "SwitchHUD", ByObj, SMIN);
    SetGraphics("8", ring, SMIN, overlay, GFXOV_MODE_IngamePicture);

    //Scheinwerfer ein- oder ausschalten
    overlay = ring->AddDownItem("$Spotlight$", "SwitchSpotlights", ByObj, SMIN);
    SetGraphics("9", ring, SMIN, overlay, GFXOV_MODE_IngamePicture);

    //Radio ein- oder ausschalten
    overlay = ring->AddUpItem("$Radio$", "SwitchRadio", ByObj, SMIN);
    SetGraphics("10", ring, SMIN, overlay, GFXOV_MODE_IngamePicture);

    return Sound("BKHK_Switch.ogg", false, this, 100, GetOwner(ByObj) + 1);
  }

  //Schütze: Feuer eröffnen/einstellen
  if(ByObj == GetGunner())
  {
    if(!GetPlrCoreJumpAndRunControl(GetController(ByObj)))
    {
      if(GetPilot())
        pMGStation->~ControlThrow(ByObj);
      else
        RefuseLaunch(ByObj);
    }
  }
  //Koordinator
  if(ByObj == GetCoordinator())
    if(GetPilot())
      pRocketStation->~ControlThrow(ByObj);
    else
      RefuseLaunch(ByObj);

  return true;
}
