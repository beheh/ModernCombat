/*-- Teamsteuerung --*/

//Erweitert die Teamsteuerung um die Aktivierung der Punkteauswertung.

#strict 2
#appendto TEAM


public func IsFulfilled()
{
  var end = _inherited(...);
  if(end) RewardEvaluation();
  return end;
}