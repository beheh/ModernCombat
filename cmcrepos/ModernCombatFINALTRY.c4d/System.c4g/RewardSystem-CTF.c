/*-- CTF --*/

//Punkte für Flaggen stehlen und zurückbringen.

#strict 2
#appendto RWDS


public func FlagScored(int iTeam, int iCTeam, object pClonk)
{
  //Punkte bei Belohnungssystem (Flaggendiebstahl)
  DoPlayerPoints(BonusPoints("CTFSteal"), RWDS_TeamPoints, GetOwner(pClonk), pClonk, IC08);
}

public func FlagReturned(int iTeam, object pClonk)
{
  //Punkte bei Belohnungssystem (Flaggenrückeroberung)
  DoPlayerPoints(BonusPoints("CTFRegain"), RWDS_TeamPoints, GetOwner(pClonk), pClonk, IC09);
}