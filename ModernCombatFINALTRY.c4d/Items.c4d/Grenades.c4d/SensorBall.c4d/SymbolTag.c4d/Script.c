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

  //Farbe des Besitzers übernehmen
  SetOwner(GetOwner(pHost));

  //Sichtbarkeit gegenüber Verbündeten
  SetVisibility(VIS_Allies | VIS_Owner);

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

/* Aufnahme */

public func RejectEntrance()		{return RemoveObject();}
public func AttachTargetLost()		{return RemoveObject();}