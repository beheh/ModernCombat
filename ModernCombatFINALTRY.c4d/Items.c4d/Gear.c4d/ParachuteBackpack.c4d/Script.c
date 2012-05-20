/*-- Fallschirmrucksack --*/

#strict 2

#include GEAR

local pTarget;

public func GetGearType()	{return GEAR_Backpack;}
public func IsEquipment()	{return true;}
public func TriggerHigh()	{return 100;}
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

  /*for(var obj in FindObjects(Find_ID(PARA), Find_ActionTarget())
    if(GetActionTarget(0, obj) == pTarget)
      return false;*/

  //Unterhalb genug Platz, Fallgeschwindigkeit hoch genug, nicht verschachtelt?
  if(!Contained() && GetYDir(pTarget) > 15 && !GetEffect("Flying", pTarget))
  {
    if(PathFree(GetX(pTarget), GetY(pTarget), GetX(pTarget), GetY(pTarget)+TriggerHigh()))
    {
      //Fallschirm erstellen und anhängen
      CreateObject(PARA, 0, 0, GetOwner(pTarget))->Set(pTarget);
      AddEffect("Flying", pTarget, 101, 5, this);
      Sound("PPAR_Activate.ogg", false, pTarget,50);
    }
    return true;
  }

  return false;
}

public func FxFlyingTimer(object target)
{
  if(WildcardMatch(GetAction(target), "*Walk*"))  return -1;
}

public func ControlRightDouble()	{return 0;}
public func ControlLeftDouble()		{return 0;}

/* Sonstiges */

protected func Hit()
{
  Sound("AmmoBoxHit*.ogg", false, this);
}

protected func Selection()
{
  Sound("FAPK_Charge.ogg", false, this);
}