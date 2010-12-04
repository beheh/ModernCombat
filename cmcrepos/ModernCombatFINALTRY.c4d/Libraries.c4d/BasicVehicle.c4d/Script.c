/*-- Grundfahrzeug --*/

#strict 2

local fDestroyed;
local iLastAttacker;
local aDealers;

public func OnDamage()			{}							//Beim Erhalten von Schaden
public func OnDestruction()		{}							//Bei der Zerstörung des Fahzeugs
public func MaxDamage()			{return 100;}						//Maximalschaden
public func BonusPointCondition()	{return Hostile(GetLastAttacker(),GetController());}	//Ob bei der Zerstörung Punkte vergeben werden

public func GetLastAttacker()		{return iLastAttacker;}
public func IsDestroyed()		{return fDestroyed;}
public func IsCMCVehicle()		{return true;}


/* Initialisierung */

public func Initialize()
{
  fDestroyed = false;
  iLastAttacker = -1;
  aDealers = CreateArray();
  SetColorDw(RGB(255,255,255));
  //AddEffect("BasicVehicleUnused", this, 50, 1, this, 0);
  return true;
}

/*global func FxBasicVehicleUnusedStart(object pTarget, int iEffectNumber)
{
  EffectVar(0, pTarget, iEffectNumber) = 38*15;
}

global func FxBasicVehicleUnusedTimer(object pTarget, int iEffectNumber, int iTime)
{
  if(GetOwner(pTarget) == NO_OWNER) return FX_OK;
  var pClonk = FindObject2(Find_Container(pTarget), Find_Or(Find_Distance(50, AbsX(GetX(pTarget)), AbsY(GetY(pTarget))), Find_ActionTarget(pTarget), Find_ActionTarget(GetActionTarget(0, pTarget))), Find_Not(Find_Func("IsFakeDeath")), Find_Func("IsClonk"), Find_OCF(OCF_Alive));
  if(!pClonk)
  {
    EffectVar(0, pTarget, iEffectNumber)--;
  }
  else
  {
    SetOwner(GetOwner(pClonk), pTarget);
    EffectVar(0, pTarget, iEffectNumber) = 38*15;
  }
  if(EffectVar(0, pTarget, iEffectNumber) > 0) return FX_OK;
  SetOwner(NO_OWNER, pTarget);
  return FX_OK;
}*/

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
  {
    iLastAttacker = iPlr;
    if(Hostile(iPlr, GetController()))
    {
      if(!aDealers)
        aDealers = CreateArray();
      if(!aDealers[iPlr])
        aDealers[iPlr] = 0;
      aDealers[iPlr] += iDmg;
      while(aDealers[iPlr] >= 50)
      {
        DoPlayerPoints(BonusPoints("VehicleDamage"), RWDS_BattlePoints, iLastAttacker, GetCursor(iLastAttacker), IC18);
        aDealers[iPlr] -= 50;
      }
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
  if(BonusPointCondition() && iLastAttacker != -1 && GetOwner() != -1 && Hostile(GetOwner(), iLastAttacker))
    DoPlayerPoints(BonusPoints("Destruction"), RWDS_BattlePoints, iLastAttacker, GetCursor(iLastAttacker), IC03);

  //Sound
  Sound("Blast2", false, this);

  //Callback
  OnDestruction();

  //Letzen Angreifer zurücksetzen
  iLastAttacker = -1;
}

/* Umfärbung */

global func SetOwnerFade(int iPlr, object pTarget, int iDuration)
{
  if(!pTarget) pTarget = this;
  if(!pTarget) return 0;
  if(GetOwner(pTarget) == iPlr) return true;
    if(GetOwner(pTarget) == -1 && GetColorDw(pTarget) == 0) SetColorDw(RGB(0,0,255), pTarget);	
      var clrOld = GetColorDw(pTarget);
      SetColorDwFade(GetPlrColorDw(iPlr), pTarget, iDuration);
  SetOwner(iPlr, pTarget);
  SetColorDw(clrOld, pTarget);
  return true;
}

global func SetColorDwFade(int clrNew, object pTarget, int iDuration)
{
  if(!iDuration) iDuration = 50;
  if(!pTarget) pTarget = this;
  if(!pTarget) return 0;

  var clrOld = GetColorDw(pTarget);
  RemoveEffect("UpdateOwnerColor", pTarget);
  SetColorDw(clrOld, pTarget);
  AddEffect("UpdateOwnerColor", pTarget, 50, 1, pTarget, 0, clrOld, clrNew, iDuration);
  return true;
}

global func FxUpdateOwnerColorStart(pTarget, iNo, iTemp, clrOld, clrNew, iDuration)
{
  if(clrOld == clrNew) return -1;
  EffectVar(0, pTarget, iNo) = clrOld;
  EffectVar(1, pTarget, iNo) = clrNew;
  EffectVar(2, pTarget, iNo) = iDuration;
  return true;
}
    
global func FxUpdateOwnerColorTimer(pTarget, iNo, iTime)
{
  var clrOld = EffectVar(0, pTarget, iNo);
  var clrNew = EffectVar(1, pTarget, iNo);
  var iDuration = EffectVar(2, pTarget, iNo);
  var color = RGBa(Interpolate2(GetRGBaValue(clrOld, 1), GetRGBaValue(clrNew, 1), iTime, iDuration), Interpolate2(GetRGBaValue(clrOld, 2), GetRGBaValue(clrNew, 2), iTime, iDuration), Interpolate2(GetRGBaValue(clrOld, 3), GetRGBaValue(clrNew, 3), iTime, iDuration));
  SetColorDw(color, pTarget);
  if(iTime >= iDuration) return -1;
}

global func FxUpdateOwnerColorStop(pTarget, iNo, iReason, fTemp)
{
  SetColorDw(EffectVar(1, pTarget, iNo), pTarget);
}