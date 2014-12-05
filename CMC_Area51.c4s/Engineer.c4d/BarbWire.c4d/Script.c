/*--- Stacheldraht ---*/

#strict 2
#include CSTR

local iHits;

public func MaxDamage()			{return 30;}	//Maximalschaden
public func AutoRepairDuration()	{return 0;}	//Keine Selbstreparatur
public func BuildCost()			{return 10;}	//Baukosten


/* Initialisierung */

public func Initialize()
{
  //Maximalzahl an Treffern
  iHits = 4;

	var flag = CreateObject(MFLG, 0, 1, GetOwner());
  flag->Set(this);
  SetDir(DIR_Right, flag);

  return _inherited(...);
}

/* Timer */

protected func Check()
{
  if(!iHits || fDestroyed)
  	return;

  //Clonks suchen
  for(var obj in FindObjects(Find_InRect(-13, -10, 26, 15), Find_OCF(OCF_Alive), Find_Hostile(GetOwner()), Find_NoContainer()))
  {
    if(GetEffect("BarbedWireHit", obj))
      continue;

    //Schaden verursachen und abnutzen
    DoDmg(20, 0, obj, 0, 0, GetID(this));
    iHits--;
    if(!iHits)
      Destroyed();

    //Clonk beeinflussen
    AddEffect("BarbedWireHit", obj, 30, 75, 0, GetID(this));

    //Effekte
    if(GetEffectData(EFSM_ExplosionEffects) > 1) CastSmoke("Smoke3",4,5,0,0,40,80);
    if(GetEffectData(EFSM_ExplosionEffects) > 1) CastParticles("MetalSplinter",4,40,0,0,20,40);
    Sound("BarbwireCut*.ogg",0,obj);
  }
}

/* Lähmungseffekt */

func FxBarbedWireHitStart(object pTarget, int iEffectNumber, int iTemp)
{
  //Lähmung
  SetPhysical("Walk", GetPhysical("Walk", 2, pTarget)/4, 3, pTarget);
  SetPhysical("Jump", GetPhysical("Jump", 2, pTarget)/4, 3, pTarget);
  SetPhysical("Scale", GetPhysical("Scale", 2, pTarget)/4, 3, pTarget);
  SetPhysical("Hangle", GetPhysical("Hangle", 2, pTarget)/4, 3, pTarget);
  SetPhysical("Swim", GetPhysical("Swim", 2, pTarget)/4, 3, pTarget);
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

/* Zerstörung */

public func Destroyed()
{
  fDestroyed = true;

  //Effekte
  if(GetEffectData(EFSM_ExplosionEffects) > 1) CastSmoke("Smoke3",4,20,0,0,100,300);
  if(GetEffectData(EFSM_ExplosionEffects) > 1) CastParticles("MetalSplinter",8,60,0,0,40,60);
  Sound("FenceDestruct.ogg");

  //Verschwinden
  FadeOut(this);
}

public func Replaced()
{
  fDestroyed = true;

  //Verschwinden
  FadeOut(this);
}
