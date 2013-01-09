/*-- Markierung --*/

#strict 2

local pTarget, pHost;


/* Einstellung */

public func Set(object target, object host, bool fClonk)
{
  pHost = host;
  pTarget = target;

  //Clonk? Dann andere Grafik
  if(!fClonk)
    SetGraphics("Object", this);

  //Positionieren
  SetAction("Attach", pTarget);

  //Farbe des Besitzers übernehmen
  SetOwner(GetOwner(pHost));

  //Sichtbarkeit gegenüber Verbündeten
  SetVisibility(VIS_Allies | VIS_Owner);
}

public func Update()
{
  if(pTarget && GetCategory(pTarget) & C4D_Living && !GetAlive(pTarget))
    return RemoveObject();

  //Verschwinden, wenn kein Ziel/Host oder Ziel verschachtelt/Host nicht in Sensornähe/inaktiv oder Ziel am ausblenden
  if(pTarget && pHost && GetOwner(pHost) != NO_OWNER && !Contained(pTarget) && ObjectDistance(this, pHost) <= pHost->~SensorDistance() && (GetCategory(pTarget) & C4D_Living || pTarget->~IsDetectable()))
    return;

  RemoveObject();
}

/* Aufnahme */

public func RejectEntrance(object pObj)
{
  RemoveObject();
}
