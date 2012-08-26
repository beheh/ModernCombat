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

  //Farbe des Besitzers �bernehmen
  SetOwner(GetOwner(pHost));

  //Sichtbarkeit gegen�ber Verb�ndeten
  SetVisibility(VIS_Allies | VIS_Owner);
}

public func Update()
{
  if(pTarget && GetCategory(pTarget) & C4D_Living && !GetAlive(pTarget))
    return RemoveObject();

  //Verschwinden, wenn kein Ziel/Host oder Ziel verschachtelt oder Host nicht in Sensorn�he/inaktiv
  if(pTarget && pHost && GetOwner(pHost) != NO_OWNER && pHost->~IsActive() && !Contained(pTarget) && ObjectDistance(this, pHost) <= pHost->~SensorDistance())
    return;

  RemoveObject();
}

/* Aufnahme */

public func RejectEntrance(object pObj)
{
  RemoveObject();
}