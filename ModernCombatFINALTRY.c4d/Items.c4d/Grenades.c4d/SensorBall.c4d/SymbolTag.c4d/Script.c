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
  SetColorDw(GetPlrColorDw(GetOwner(pHost)));

  //Sichtbarkeit gegen�ber Verb�ndeten
  SetVisibility(VIS_Allies | VIS_Owner);
}

public func Update()
{
  //Es fehlt:
  // - Ein Check, ob der Besitzerspieler (pHost) noch existiert.
  // - Ein Check, ob der gefundene, passende Hostsensorball �berhaupt aktiv ist.

  //Verschwinden, wenn kein Ziel/Host oder Ziel verschachtelt oder Host nicht in Sensorn�he/inaktiv
  if(pTarget && pHost && pHost->~IsActive() && !Contained(pTarget) && ObjectDistance(this, pHost) <= pHost->~SensorDistance())
    return;
    
  RemoveObject();
}

/* Aufnahme */

public func RejectEntrance(object pObj)
{
  RemoveObject();
}