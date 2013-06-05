/*-- CTF --*/

//Punkte f�r Flaggen stehlen und zur�ckbringen.

#strict 2
#appendto RWDS


public func FlagScored(int iTeam, int iCTeam, object pClonk)
{
  //Punkte bei Belohnungssystem (Flaggendiebstahl)
  if(pClonk)
    DoPlayerPoints(BonusPoints("CTFSteal"), RWDS_TeamPoints, GetOwner(pClonk), pClonk, IC08);
}

public func FlagReturned(int iTeam, object pClonk)
{
  //Punkte bei Belohnungssystem (Flaggenr�ckeroberung)
  if(pClonk)
    DoPlayerPoints(BonusPoints("CTFRegain"), RWDS_TeamPoints, GetOwner(pClonk), pClonk, IC09);
}