/*-- Markierung --*/

#strict 2

local pTarget, pHost, iRemoveTime, iEffectRemoveTime;


/* Einstellung */

public func Set(object target, object host, bool fClonk, int remove_time)
{
  pHost = host;
  pTarget = target;
  iRemoveTime = iEffectRemoveTime = remove_time;

  //Ziel kein Clonk? Andere Grafik setzen
  if(!fClonk)
    SetGraphics("Object", this);

  //Positionieren
  SetAction("Attach", pTarget);

  //Besitzer setzen
  SetOwner(GetOwner(pHost));

  //Farbe
  SetColor(RGB(255,255,255), this);

  //Sichtbarkeit gegenüber Verbündeten
  SetVisibility(VIS_Allies | VIS_Owner);

  //Effekt zur Lebensanzeige
  if(GetOCF(pTarget) & OCF_Alive)
    AddEffect("ShowEnergyBar", this, 1, 1, this, 0, GetOwner(host), pTarget);

  AddEffect("Remover", this, 1, 1, this);
}

protected func FxRemoverTimer(object pTarget, int iNr, int iReason) 
{
  if(!--iEffectRemoveTime)
    return -1;
}

protected func FxRemoverStop()
{
  return RemoveObject();
}

public func RefreshRemoveTimer()
{
  iEffectRemoveTime = iRemoveTime;
}

/* Statusbalkeneffekt für feindliche Energie */

public func FxShowEnergyBarStart(object target, int nr, int temp, int owner, object actTarget)
{
  if(temp) return;
  var bar = CreateObject(SBAR, 0, 0, owner);
  bar->Set(GetActionTarget(0,target), RGB(255,255,255), BAR_Energybar);
  SetVisibility(VIS_Owner|VIS_Allies, bar);
  EffectVar(0, target, nr) = bar;
  EffectVar(1, target, nr) = owner;
  EffectVar(2, target, nr) = actTarget;
  return true;
}

public func FxShowEnergyBarTimer(object target, int nr)
{
  var bar = EffectVar(0, target, nr);
  var actTarget = EffectVar(2, target, nr);
  var percent = BoundBy(100000 * GetEnergy(actTarget) / GetPhysical("Energy", PHYS_Current, actTarget), 0, 100);
  if(bar) bar->Update(percent, 0);
  SetVisibility(VIS_Owner|VIS_Allies, bar);
  return true;
}

public func FxShowEnergyBarStop(object target, int nr, int reason, bool temp)
{
  if(EffectVar(0, target, nr))
    RemoveObject(EffectVar(0, target, nr));
}

/* Aufnahme */

public func RejectEntrance()	{return RemoveObject();}
public func AttachTargetLost()	{return RemoveObject();}