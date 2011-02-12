/*-- Markierung --*/

#strict 2

local pTarget, pHost;


/* Einstellung */

public func Set(object target, object host)
{
  pHost = host;
  pTarget = target;

  //Positionieren
  SetAction("Attach", pTarget);

  //Farbe des Besitzers übernehmen
  SetOwner(GetOwner(pHost));

  //Sichtbarkeit gegenüber Verbündeten
  SetVisibility(VIS_Allies | VIS_Owner);
}

public func Update()
{
  //Verschwinden, wenn kein Ziel/Host oder Ziel verschachtelt oder Host nicht in Sensornähe/inaktiv
  if(pTarget && pHost && GetOwner(pHost) != NO_OWNER && pHost->~IsActive() && !Contained(pTarget) && ObjectDistance(this, pHost) <= pHost->~SensorDistance())
    return;
    
  RemoveObject();
}

/* Aufnahme */

public func RejectEntrance(object pObj)
{
  RemoveObject();
}