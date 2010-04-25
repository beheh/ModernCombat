/*-- CTF --*/

//Punkte für Flaggenklauen

#strict 2
#appendto GCTF

public func FlagScored(int iTeam, int iCTeam, object pClonk)
{
  DoPlayerPoints(BonusPoints("CTFSteal"), RWDS_BattlePoints, GetOwner(pClonk), pClonk, IC08);
  return _inherited(...);
}

public func FlagReturned(int iTeam)
{
  DoPlayerPoints(BonusPoints("CTFRegain"), RWDS_BattlePoints, aFlagCarrier[iTeam], GetCursor(aFlagCarrier[iTeam]), IC09);
  return _inherited(...);
}
