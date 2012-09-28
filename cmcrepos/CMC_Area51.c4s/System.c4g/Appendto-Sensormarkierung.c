/*-- Sensorballmarkierung darf bei geparktem MAV nicht verschwinden --*/

#strict 2
#appendto SM08

public func Update()
{
  if(pTarget && GetCategory(pTarget) & C4D_Living && !GetAlive(pTarget))
    return RemoveObject();

  //Verschwinden, wenn kein Ziel/Host oder Ziel verschachtelt oder Host nicht in Sensornähe/inaktiv
  if(pTarget && pHost && GetOwner(pHost) != NO_OWNER && ((pHost->~IsMAV() && !pHost->IsDestroyed()) || pHost->~IsActive()) && !Contained(pTarget) && ObjectDistance(this, pHost) <= pHost->~SensorDistance())
    return;

  RemoveObject();
}
