/*--- Stacheldraht ---*/

#strict 2
#include CSTR
#include FDSE

local iHits;
local flag;

public func MaxDamage()			{return 20;}	//Maximalschaden
public func AutoRepairDuration()	{return 0;}	//Keine Selbstreparatur
public func BuildCost()			{return 25;}	//Baukosten
public func IsRepairable()	{return !fDestroyed;}


/* Initialisierung */

public func Construction()
{
  //Maximalzahl an Treffern
  iHits = 3;

	flag = CreateObject(MFLG, 0, 1, GetOwner());
  flag->Set(this);
  SetDir(DIR_Right, flag);

  return _inherited(...);
}

/* Timer */

protected func Check()
{
  if(!iHits || fDestroyed || GetCon() < 100)
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
    //AddEffect("BarbedWireHit", obj, 30, 75, 0, GetID(this));
    AddEffect("BarbedWireHit", obj, 30, 15, 0, GetID(this));

    //Effekte
    if(GetEffectData(EFSM_ExplosionEffects) > 1) CastSmoke("Smoke3",4,5,0,0,40,80);
    if(GetEffectData(EFSM_ExplosionEffects) > 1) CastParticles("MetalSplinter",4,40,0,0,20,40);
    Sound("BarbwireCut*.ogg",0,obj);
  }
}

/* L�hmungseffekt 
0 = X
1 = Y
2 = Aufrufzahl
*/

func FxBarbedWireHitStart(object pTarget, int iEffectNumber, int iTemp)
{
  //L�hmung
  SetPhysical("Walk", GetPhysical("Walk", 2, pTarget)/4, 3, pTarget);
  SetPhysical("Jump", GetPhysical("Jump", 2, pTarget)/4, 3, pTarget);
  SetPhysical("Scale", GetPhysical("Scale", 2, pTarget)/4, 3, pTarget);
  SetPhysical("Hangle", GetPhysical("Hangle", 2, pTarget)/4, 3, pTarget);
  SetPhysical("Swim", GetPhysical("Swim", 2, pTarget)/4, 3, pTarget);
}

func FxBarbedWireHitTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
	EffectVar(2, pTarget, iEffectNumber)++;
	
	if(EffectVar(2, pTarget, iEffectNumber) == 1)
	{
		EffectVar(0, pTarget, iEffectNumber) = GetX(pTarget);
		EffectVar(1, pTarget, iEffectNumber) = GetY(pTarget);
	}
	
	if(EffectVar(2, pTarget, iEffectNumber) > 4 && (EffectVar(0, pTarget, iEffectNumber) != GetX(pTarget) || EffectVar(1, pTarget, iEffectNumber) != GetY(pTarget)))
	 return -1;
}

public func FxBarbedWireHitStop(object pTarget, no, reason, temp)
{
  //L�hmung r�ckg�ngig machen
  ResetPhysical(pTarget, "Walk");
  ResetPhysical(pTarget, "Jump");
  ResetPhysical(pTarget, "Scale");
  ResetPhysical(pTarget, "Hangle");
  ResetPhysical(pTarget, "Swim");
}

/* Zerst�rung */

public func Destroyed()
{
  fDestroyed = true;
  RemoveObject(flag);

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
  RemoveObject(flag);

  //Verschwinden
  FadeOut(this);
}
