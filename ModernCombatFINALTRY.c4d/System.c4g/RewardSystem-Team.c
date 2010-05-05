/*-- Auswertung der Punkte --*/

#strict 2
#appendto TEAM
#appendto GOCC //Weil eigenes IsFulfilled()

public func IsFulfilled() {
  var end = _inherited(...);
  if(end) RewardEvaluation();
  return end;
}
