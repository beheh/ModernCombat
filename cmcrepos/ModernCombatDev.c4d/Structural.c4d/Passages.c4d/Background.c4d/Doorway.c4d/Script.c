/*-- Türsteuerung --*/

#strict
#appendto ROOM
#appendto ROM2

local lock;
local target;


public func Initialize()
{
  Unlock();
  return(_inherited());
}

public func Lock()
{
	lock = 1;
}

public func Unlock()
{
  lock = 0;
}

public func GetTarget()
{
  return(target);
}

public func Connect(pTarget)
{
  if(!pTarget->~IsBackDoor()) return();
  target = pTarget;
  
  if(pTarget->GetTarget() != this())
    pTarget->Connect(this());
}

func Collection2(object pObj)
{
  if(!GetEffect("Move2Door",pObj))
    if(target)
    {
      AddEffect("Move2Door",pObj,200,15);
      //return();//:S
    }
  return(_inherited(...));
}

global func FxMove2DoorStart(object pTarget)
{
  //:O lol
  var cont = pTarget->Contained();
  if(cont)
    if(cont->~IsBackDoor())
      if(cont->GetTarget())
        pTarget->Enter(cont->GetTarget());
  return(1);
}

global func FxMove2DoorTimer(pTarget)
{
  pTarget->AddCommand(0,"Exit");
  
  /*
  if(pTarget->Contained())
    pTarget->Contained()->Collection2(pTarget);
  */
  return(-1);
}

private func SoundDoorLocked()
{
  Sound("MetalHit1");
}

public func IsBackDoor() { return(true); }