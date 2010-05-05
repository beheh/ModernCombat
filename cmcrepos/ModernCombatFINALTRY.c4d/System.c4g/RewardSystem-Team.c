/*-- Auswertung der Punkte --*/

#strict 2
#appendto TEAM
#appendto GOCC
#appendto GPL_

public func IsFulfilled() {
  var end = _inherited(...);
  if(end) RewardEvaluation();
  return end;
}
