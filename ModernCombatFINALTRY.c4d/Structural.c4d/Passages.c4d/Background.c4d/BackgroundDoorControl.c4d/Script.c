/*-- Hintergrundtürsteuerung --*/

#strict 2
#include DOOR

local lock;
local target;

public func IsBackDoor()	{return true;}


/* Initalisierung */

public func Initialize()
{
  Unlock();
  return _inherited();
}

/* Abschließfunktion */

public func Lock()
{
  lock = 1;
}

public func Unlock()
{
  lock = 0;
}

/* Türverbindung */

public func GetTarget()
{
  return target;
}

public func Connect(pTarget)
{
  if(!pTarget->~IsBackDoor()) return ;
  target = pTarget;

  if(pTarget->GetTarget() != this)
    pTarget->Connect(this);
}

func Collection2(object pObj)
{
  if(!GetEffect("Move2Door",pObj))
    if(target)
    {
      AddEffect("Move2Door", pObj, 200, 15, this);
      //return();
    }
  return _inherited(...);
}

protected func FxMove2DoorStart(object pTarget)
{
  var cont = Contained(pTarget);
  if(cont)
    if(cont->~IsBackDoor())
      if(cont->GetTarget())
        Enter(cont->GetTarget(), pTarget);
  return 1;
}

protected func FxMove2DoorTimer(pTarget)
{
  if (Contained(pTarget) == GetTarget())
    AddCommand(pTarget,"Exit");
  else
    return -1;
}

private func SoundDoorLocked()
{
  Sound("MetalHit1");
}

public func ContainedLeft()	{if(target){return true;} else return _inherited();}
public func ContainedRight()	{if(target){return true;} else return _inherited();}
public func RejectContents()	{if(target){return true;} else return _inherited();}