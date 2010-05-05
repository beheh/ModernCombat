/*-- CTF --*/

//Punkte für Flaggen stehlen und zurückbringen.

#strict 2

#appendto GCTF


public func FlagScored(int iTeam, int iCTeam, object pClonk)
{
  DoPlayerPoints(BonusPoints("CTFSteal"), RWDS_BattlePoints, GetOwner(pClonk), pClonk, IC08);
  return _inherited(...);
}

public func FlagReturned(int iTeam, object pClonk)
{
  DoPlayerPoints(BonusPoints("CTFRegain"), RWDS_BattlePoints, GetOwner(pClonk), pClonk, IC09);
  return _inherited(...);
}
