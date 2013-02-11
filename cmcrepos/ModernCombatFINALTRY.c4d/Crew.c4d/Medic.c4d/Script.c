/*--- Der Sanitäter ---*/

#strict 2
#include PCMK

static const MDIC_PortraitVersion = 160;

public func WeaponCollectionLimit()	{return 2;}				//Anzahl Waffen im Inventar
public func ObjectCollectionLimit()	{return 3;}				//Anzahl Objekte im Inventar
public func DeathAnimationCount()	{return (GetID() == MDIC) && 6;}	//Anzahl Todesanimationen
protected func IsMedic()		{return 1;}				//Ist ein Sanitäter


/* Initialisierung */

func Initialize()
{
  AddEffect("EnergyBars", this, 1, 1, this);
  _inherited(...);
}

public func HazardGearSupported(object pGear)
{
  if(GetID(pGear) == HARM) return false;
  return true;
}

/* Statusbalkensystem */

public func FxEnergyBarsStart(object target, int nr)
{
  EffectVar(0, target, nr) = [];
  return true;
}

public func FxEnergyBarsTimer(object target, int nr)
{
  //Balken aktualisieren
  for(var bar in EffectVar(0, target, nr))
  {
    if(!bar)
      continue;

    var actTarget = GetActionTarget(0, bar);
    if(!(GetOCF(actTarget) & OCF_Alive) || Hostile(GetOwner(actTarget), GetOwner(target)) || !GetPlayerName(GetOwner(actTarget)))
      RemoveObject(bar);
    else if(Contained(actTarget) || !GetCursor(GetOwner(target)) || (GetCursor(GetOwner(target)) != target && GetCursor(GetOwner(target))->~GetRealCursor() != target))
      bar->Update(0, true);
    else
    {
      var percent = BoundBy(100000 * GetEnergy(actTarget) / GetPhysical("Energy", PHYS_Current, actTarget), 0, 100);

      bar->Update(percent, (percent >= 95));
    }
  }

  for(var clonk in FindObjects(Find_OCF(OCF_Alive), Find_OCF(OCF_CrewMember), Find_NoContainer(), Find_Exclude(target), Find_Not(Find_Hostile(GetOwner(target)))))
  {
    if(FindObject2(Find_ID(SBAR), Find_ActionTarget(clonk), Find_Owner(GetOwner(target)), Find_Func("HasBarType", BAR_Energybar))) //Hat schon einen Balken?
      continue;

    var bar = CreateObject(SBAR, 0, 0, GetOwner(target));
    bar->Set(clonk, RGB(255,0,0), BAR_Energybar, 0, 0, SM13);
    EffectVar(0, target, nr)[GetLength(EffectVar(0, target, nr))] = bar;
  }

  return true;
}

public func FxEnergyBarsStop(object target, int nr)
{
  for(var bar in EffectVar(0, target, nr))
    if(bar)
      RemoveObject(bar);
}

/* Tod */

public func Death()
{
  for(var bar in FindObjects(Find_ID(HBAR),Find_Owner(GetOwner())))
    RemoveObject(bar);
  _inherited(...);
}

/* Portrait-Updates */

protected func Recruitment()
{
  if(GetCrewExtraData(this, "CMC_Portrait") < MDIC_PortraitVersion)
  {
    SetCrewExtraData(this, "CMC_Portrait", MDIC_PortraitVersion);
    SetPortrait("random", this, GetID(), true, true);
  }
  return _inherited(...);
}