/*-- CTF --*/

//Punkte für Flaggen stehlen und zurückbringen.

#strict 2
#appendto RWDS


public func FlagScored(int iTeam, int iCTeam, object pClonk)
{
  //Punkte bei Belohnungssystem (Flagge gestohlen)
  if(pClonk)
    DoPlayerPoints(BonusPoints("CTFTheft"), RWDS_TeamPoints, GetOwner(pClonk), pClonk, IC08);
}

public func FlagReturned(int iTeam, object pClonk)
{
  //Punkte bei Belohnungssystem (Flagge geborgen)
  if(pClonk)
    DoPlayerPoints(BonusPoints("CTFRecovery"), RWDS_TeamPoints, GetOwner(pClonk), pClonk, IC09);
}
