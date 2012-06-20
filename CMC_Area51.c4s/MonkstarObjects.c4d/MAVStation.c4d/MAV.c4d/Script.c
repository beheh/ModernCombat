/*--- MAV ---*/

#strict 2
#include CVHC

local iXDir;
local iYDir;
local iXTendency;
local iYTendency;
local iSpeed;

public func AttractTracer(object pTracer)	{return GetPlayerTeam(GetController(pTracer)) != GetTeam() && !fDestroyed;}
public func IsBulletTarget()			{return !fDestroyed;}
public func IsDestroyed()			{return fDestroyed;}


/* Initialisierung */

protected func Initialize()
{
  iSpeed = 20;
  SetAction("Idle");
  Sound("MAVE_Engine.ogg", true, 0, 80);
}

public func OnDestruction()
{
  SetAction("Idle");
  FakeExplode(5, GetController()+1, this);
  FadeOut();
}

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

  SetXDir(iXDir);
  SetYDir(iYDir);

  SetPlrView(GetController(), this);

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
}

public func Idle(object pClonk)
{
  iXTendency = 0;
  iYTendency = 0;
  iXDir = 0;
  iYDir = 0;

  SetPlrView(GetController(), pClonk);
}

/* Steuerung */

public func ControlLeft(pByObj)
{
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