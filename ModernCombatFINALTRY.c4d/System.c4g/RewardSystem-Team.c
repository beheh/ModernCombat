/*-- Auswertung der Punkte --*/

#strict 2
#appendto TEAM
#appendto GOCC

private func InitScoreboard()
{
	if(FindObject(CHOS)) return;
	return _inherited()
}

public func IsFulfilled() {
  var end = _inherited(...);
  if(end) RewardEvaluation();
  return end;
}
