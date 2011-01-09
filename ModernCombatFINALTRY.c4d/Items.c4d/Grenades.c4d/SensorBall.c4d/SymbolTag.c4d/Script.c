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

  //Farbe des Zielobjekts �bernehmen
  SetColorDw(GetPlrColorDw(GetOwner(pTarget)));

  //Sichtbarkeit gegen�ber Verb�ndeten
  SetVisibility(VIS_Allies | VIS_Owner);
}

public func Update()
{
  //Es fehlt: Ein Check, ob der Besitzerspieler (pHost) noch existiert.

  //Verschwinden, wenn kein Ziel/Host oder Ziel verschachtelt oder Host nicht in Sensorn�he/inaktiv
  if(Contained(pTarget) || !pTarget || !pHost || ObjectDistance(this, pHost) > pHost->~SensorDistance())
    RemoveObject();
}

/* Aufnahme */

public func RejectEntrance(object pObj)
{
  RemoveObject();
}