/*--- Stacheldraht ---*/

#strict 2
#include CSTR

local iHits;

public func MaxDamage()			{return 120;}
public func AutoRepairDuration()	{return 0;}
public func BuildCost()			{return 10;}

public func Initialize()		{iHits = 5; return _inherited(...);}

protected func Check()
{
  for(var obj in FindObjects(Find_InRect(-10, -10, 20, 15), Find_OCF(OCF_Alive), Find_Hostile(GetOwner()), Find_NoContainer()))
  {
  	if(GetEffect("BarbedWireHit", obj))
  		continue;
  	
  	DoDmg(20, 0, obj, 0, 0, GetID(this));
  	iHits--;
  	if(!iHits)
  		Destroyed();
  	
  	AddEffect("BarbedWireHit", obj, 30, 100, 0, GetID(this));
  }
}

func FxBarbedWireHitStart(object pTarget, int iEffectNumber, int iTemp)
{
  //Lähmung
  SetPhysical("Walk", GetPhysical("Walk", 2, pTarget)/3, 3, pTarget);
  SetPhysical("Jump", GetPhysical("Jump", 2, pTarget)/3, 3, pTarget);
  SetPhysical("Scale", GetPhysical("Scale", 2, pTarget)/3, 3, pTarget);
  SetPhysical("Hangle", GetPhysical("Hangle", 2, pTarget)/3, 3, pTarget);
  SetPhysical("Swim", GetPhysical("Swim", 2, pTarget)/3, 3, pTarget);
}

public func FxBarbedWireHitStop(object pTarget, no, reason, temp)
{
  //Lähmung rückgängig machen
  ResetPhysical(pTarget, "Walk");
  ResetPhysical(pTarget, "Jump");
  ResetPhysical(pTarget, "Scale");
  ResetPhysical(pTarget, "Hangle");
  ResetPhysical(pTarget, "Swim");
}

public func Destroyed()
{
  fDestroyed = true;
	FadeOut(this);
}
