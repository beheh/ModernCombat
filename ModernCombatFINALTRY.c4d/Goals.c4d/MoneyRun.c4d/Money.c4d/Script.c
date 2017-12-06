/*-- Geldspawn --*/

#strict 2

local Money;	//Array oder Int
local iCurrent;	//Momentanes Geld


/* Initialisierung */

protected func Initialize()
{
  var i = AddEffect("IntShow", this, 1, 3, this);
  EffectCall(this, i, "Timer");
  AddEffect("IntSignalMoney", this, 1, 100, this);
}

/* Calls */

public func Set(par)
{
  return Money = par;
}

public func Refill()
{
  if(!Money)
    return false;
  iCurrent = Money;
  if(GetType(Money) == C4V_Array)
    iCurrent = iCurrent[Random(GetLength(iCurrent))];
  return true;
}

public func GetCurrent()
{
  return iCurrent;
}

/* Effekt */

protected func FxIntShowTimer(object pTarget, int iEffect, int iTime)
{
  //Effekte
  if(!Random(3)) CreateParticle("NoGravSpark", 5 - Random(11), 5 + Random(6), 0, -2 -Random(3), 25, RGBa(255, 224, 160, 96));

  //Sichtbarkeit nur wenn Geld vorhanden ist
  if(!pTarget->~GetCurrent())
    return SetVisibility(VIS_None, pTarget);
  SetVisibility(VIS_All, pTarget);

  SetObjDrawTransform(1000, 0, 0, 0, 1000, 1000 * Sin(2 * iTime, GetDefCoreVal("Height", "DefCore", GetID(pTarget)) / 2));
}

protected func FxIntSignalMoneyTimer(object pTarget, int iEffect)
{
  if(!pTarget->~GetCurrent())
    return false;

  CreateParticle("NoGravSpark", 0, 0, 0, 0, 400, RGBa(255, 250, 150, 200), this);
}

/* Einsammeln */

protected func RejectEntrance(object pInto)
{
  //Nichts geben wenn nichts da
  if(!iCurrent)
    return true;
  //Nur von Clonks einsammelbar
  if(!pInto || !pInto->~IsClonk())
    return true;
  //Das Ganze übernimmt das Spielziel
  AddEffect("IntMoneyCollected", 0, 1, GMNR_MoneySpawnTime, 0, 0, iCurrent, pInto);
  Sound("GetCash*.ogg", false, pInto);
  iCurrent = 0;
  return true;
}