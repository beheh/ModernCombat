/*-- Fallschirmrucksack --*/

#strict 2

#include GEAR

local pTarget;

public func GetGearType()	{return GEAR_Backpack;}
public func IsEquipment()	{return true;}
public func NoArenaRemove()	{return true;}


public func GearEffect(object pClonk)
{
  pTarget = pClonk;
}

public func GearEffectReset(object pClonk)
{
  pTarget = 0;
}

/* Steuerung */

public func ControlUpDouble()
{
  if(!pTarget) return false;
  if(GetAction(pTarget) == "Tumble") return false;

  //Kein Fallschirm, wenn bereits vorhanden
  if(FindObject2(Find_ID(PARA), Find_ActionTarget(pTarget)))
    return false;

  var ydir = GetYDir(pTarget, 100);
  //Unterhalb genug Platz, Fallgeschwindigkeit hoch genug, nicht verschachtelt?
  if(!Contained() && ydir > 200 && !GetEffect("Flying", pTarget))
  {
    var x = GetX(pTarget), y = GetY(pTarget), xdir = GetXDir(pTarget, 100);
    SimFlight(x, y, xdir, ydir, 0, 0, 0, 100);
    if(Distance(xdir, ydir) > 700)
    {
      //Fallschirm erstellen und anhängen
      CreateObject(PARA, 0, 0, GetOwner(pTarget))->Set(pTarget);
      AddEffect("Flying", pTarget, 101, 5, this);
      Sound("PPAR_Activate.ogg", false, pTarget,75);
    }
    return true;
  }

  return false;
}

global func FxFlyingTimer(object target)
{
  if(GetProcedure(target) != "FLOAT" && GetProcedure(target) != "FLIGHT")  return -1;
}

public func ControlRightDouble()	{return 0;}
public func ControlLeftDouble()		{return 0;}

/* Sonstiges */

protected func Hit()
{
  Sound("AmmoBoxHit*.ogg");
}

protected func Selection()
{
  Sound("FAPK_Charge.ogg");
}