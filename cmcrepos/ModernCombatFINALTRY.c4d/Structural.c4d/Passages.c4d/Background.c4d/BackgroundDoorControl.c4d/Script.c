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
  lock = true;
}

public func Unlock()
{
  lock = false;
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

protected func ActivateEntrance(pObj) {
  if(lock) return false;
  return inherited(pObj);
}

func Collection2(obj) {
  if(!target) {
    var owner = GetOwner(obj);
    var ejected = false;
    for(var clonk in FindObjects(Find_Container(this))) {
      if(Hostile(owner, GetOwner(clonk))) {
        if(!ejected) {
          SetCommand(obj, "Exit");
          ejected = true; 
        }
        SetCommand(target, "Exit");        
      }
    }
  }
  return _inherited(obj);
}

public func ContainedLeft(pCaller)	{if(target){return true;} else return SetCommand(pCaller, "Get", this, 0, 0, 0, 1);}
public func ContainedRight(pCaller)	{if(target){return true;} else return SetCommand(pCaller, "Get", this, 0, 0, 0, 1);}
public func RejectContents()	{if(target){return true;} else return _inherited();}