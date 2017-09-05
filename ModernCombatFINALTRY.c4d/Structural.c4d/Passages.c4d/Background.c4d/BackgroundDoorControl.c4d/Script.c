/*-- Hintergrundt�rsteuerung --*/

#strict 2

#include DOOR

local lock;
local target;
local fNoProcedureCheck;

public func IsBackDoor()			{return true;}
public func SwitchProcedureCheck(bool fInto)	{fNoProcedureCheck = fInto;}	//Wenn fNoProcedureCheck true, auch beim Hangeln etc. T�ren betretbar


/* Initalisierung */

public func Initialize()
{
  Unlock();
  return _inherited(...);
}

/* Sperren und �ffnen */

public func Lock()
{
  lock = true;
}

public func Unlock()
{
  lock = false;
}

/* Versiegelung */

public func SealEntrance(int iRemove, int iNoEffects)
{
  //Effekte
  if(!iNoEffects)
  {
    DecoExplode(20,0,1);
    Sound("StructureHit*.ogg");
  }

  //Inhalte evakuieren und Eingang entfernen oder sperren
  if(iRemove)
    RemoveObject(this, true);
  else
  {
    Lock();
    SetAction("Idle");
    SetClrModulation(RGBa(100,100,100,5));
    var pContent;
    while(pContent = Contents(0, this))
      Exit(pContent);
  }
}

/* T�rverbindung */

public func GetTarget()
{
  return target;
}

public func Connect(pTarget)
{
  if(!pTarget->~IsBackDoor()) return;
  target = pTarget;

  if(pTarget->GetTarget() != this)
    pTarget->Connect(this);
}

protected func Collection2(object pObj)
{
  if(!GetEffect("NoDoorEntrance", pObj))
    AddEffect("NoDoorEntrance", pObj, 101, 75, 0, GetID());

  if(!GetEffect("Move2Door",pObj))
    if(target)
    {
      AddEffect("Move2Door", pObj, 200, 15, this);
    }

  return _inherited(pObj, ...);
}

public func FxNoDoorEntranceStart()
{
  return true;
}

protected func ActivateEntrance(object pObj)
{
  //Objekt kann nicht eintreten wenn am hangeln, klettern oder in der Luft
  if(!fNoProcedureCheck && GetProcedure(pObj) == "FLIGHT" || GetProcedure(pObj) == "HANGLE" || GetProcedure(pObj) == "SCALE" || GetProcedure(pObj) == "KNEEL")
    return false;

  //Eintritt verweigern wenn der Ausgang in einem Sperrgebiet liegt
  if(FindObject2(Find_ID(BRDR), Find_Func("IsDangerous", pObj, GetX(target), GetY(target))) && !GetEffect("Move2Door", pObj))
  {
    PlayerMessage(GetOwner(pObj), "$Restricted$", pObj);
    return false;
  }

  //Bei Clonks auf deren Walk-Aktion wechseln, um Campen zu vermeiden
  if(pObj->~IsClonk())
    pObj->SetAction("Walk");

  if(lock) return false;
  return _inherited(pObj, ...);
}

protected func RejectCollect(id objID, object pObj)
{
  if(FindObject2(Find_ID(BRDR), Find_Func("IsDangerous", pObj, GetX(target), GetY(target))) && !GetEffect("Move2Door", pObj))
  {
    PlayerMessage(GetOwner(pObj), "$Restricted$", pObj);
    return true;
  }
  if(pObj->~IsWeapon() || pObj->~IsGrenade() || pObj->~IsEquipment()) return true;

  return false;
}

protected func FxMove2DoorStart(object pTarget, int iNr, int iTemp)
{
  if(iTemp)
    return false;

  var cont = Contained(pTarget);
  if(cont)
    if(cont->~IsBackDoor())
      if(cont->GetTarget())
        Enter(cont->GetTarget(), pTarget);
  return 1;
}

protected func FxMove2DoorTimer(pTarget)
{
  if(Contained(pTarget) == GetTarget())
    AddCommand(pTarget,"Exit");
  else
    return -1;
}

private func SoundDoorLocked()
{
  Sound("MetalHit1");
}

func Collection2(obj)
{
  if(!target)
  {
    var owner = GetOwner(obj);
    var ejected = false;
    for(var clonk in FindObjects(Find_Container(this)))
    {
      if(Hostile(owner, GetOwner(clonk)))
      {
        if(!ejected)
        {
          SetCommand(obj, "Exit");
          ejected = true;
        }
        SetCommand(clonk, "Exit");
      }
    }
  }
  return _inherited(obj, ...);
}

public func ContainedLeft(pCaller)	{if(target){return true;} else return SetCommand(pCaller, "Get", this, 0, 0, 0, 1);}
public func ContainedRight(pCaller)	{if(target){return true;} else return SetCommand(pCaller, "Get", this, 0, 0, 0, 1);}
public func RejectContents()		{if(target){return true;} else return _inherited(...);}