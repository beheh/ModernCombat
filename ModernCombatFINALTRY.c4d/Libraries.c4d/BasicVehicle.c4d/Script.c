/*-- Grundfahrzeug --*/

#strict 2

local fDestroyed;
local iLastAttacker;
local aDealers;

public func OnDamage()			{}							//Beim Erhalten von Schaden
public func OnDestruction()		{}							//Bei der Zerstörung des Fahzeugs
public func MaxDamage()			{return 100;}						//Maximalschaden
public func BonusPointCondition()	{return Hostile(GetLastAttacker(),GetController());}			//Ob bei der Zerstörung Punkte vergeben werden

public func GetLastAttacker()		{return iLastAttacker;}
public func IsDestroyed()		{return fDestroyed;}
public func IsCMCVehicle()		{return true;}


/* Initialisierung */

public func Initialize()
{
  fDestroyed = false;
  iLastAttacker = -1;
  aDealers = CreateArray();
  AddEffect("BasicVehicleUnused", this, 50, 1, this, 0);
  return true;
}

global func FxBasicVehicleUnusedStart(object pTarget, int iEffectNumber) {
	EffectVar(0, pTarget, iEffectNumber) = 38*15;
}

global func FxBasicVehicleUnusedTimer(object pTarget, int iEffectNumber, int iTime) {
	if(GetOwner(pTarget) == NO_OWNER) return FX_OK;
	var pClonk = FindObject2(Find_Container(pTarget), Find_Or(Find_Distance(50, AbsX(GetX(pTarget)), AbsY(GetY(pTarget))), Find_ActionTarget(pTarget), Find_ActionTarget(GetActionTarget(0, pTarget))), Find_Not(Find_Func("IsFakeDeath")), Find_Func("IsClonk"), Find_OCF(OCF_Alive));
	if(!pClonk) {
		EffectVar(0, pTarget, iEffectNumber)--;
	}
	else {
		SetOwner(GetOwner(pClonk), pTarget);
		EffectVar(0, pTarget, iEffectNumber) = 38*15;
	}
	if(EffectVar(0, pTarget, iEffectNumber) > 0) return FX_OK;
	SetOwner(NO_OWNER, pTarget);
	return FX_OK;
}

/* Schaden */

public func Damage()
{
  if(GetDamage() > MaxDamage() && !IsDestroyed())
    Destroyed();
  OnDamage();
}

public func OnDmg(int iDmg, int iType)
{
  return 50;	//Standardwert
}

public func OnHit(int iDmg, int iType, object pBy)
{
  var iPlr = GetController(pBy);
  if(!IsDestroyed())
    iLastAttacker = iPlr;
  if(!aDealers)
    aDealers = CreateArray();
    if(Hostile(iPlr, GetController()))
    {
      if(!aDealers[iPlr]) aDealers[iPlr] = 0;
        aDealers[iPlr] += iDmg;
        while(aDealers[iPlr] >= 50) {
					DoPlayerPoints(BonusPoints("VehicleDamage"), RWDS_BattlePoints, iLastAttacker, GetCursor(iLastAttacker), IC03);
        	aDealers[iPlr] -= 50;
        }
    }
  return true;
	var iPlr = GetController(pBy);
	if(!IsDestroyed())
	  iLastAttacker = iPlr;
	if(!aDealers)
		aDealers = CreateArray();
	if(Hostile(iPlr, GetController())) {
		if(!aDealers[iPlr]) aDealers[iPlr] = 0;
		aDealers[iPlr] += iDmg;
		while(aDealers[iPlr] >= 50) {
			DoPlayerPoints(BonusPoints("VehicleDamage"), RWDS_BattlePoints, iLastAttacker, GetCursor(iLastAttacker), IC03);
			aDealers[iPlr] -= 50;
		}
	}
	return true;
}

/* Zerstörung */

public func Destroyed()
{
  //Status setzen
  fDestroyed = true;

  //Punkte bei Belohnungssystem
  if(BonusPointCondition() && iLastAttacker != -1)
    if((GetOwner() != -1 && Hostile(GetOwner(), iLastAttacker)) || (GetOwner() == -1 && !GetTeam(this)) || (GetTeam(this) != GetPlayerTeam(iLastAttacker)))
		  DoPlayerPoints(BonusPoints("Destruction"), RWDS_BattlePoints, iLastAttacker, GetCursor(iLastAttacker), IC03);

  //Sound
  Sound("Blast2", false, this);

  //Callback
  OnDestruction();

  //Letzen Angreifer zurücksetzen
  iLastAttacker = -1;
}
