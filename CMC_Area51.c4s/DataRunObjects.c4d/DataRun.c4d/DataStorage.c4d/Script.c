/*-- Datenträger --*/

#strict 2

local iData;

public func IsSpecialGoalItem()	{return true;}


/* Initialisierung */

public func Initialize()
{
  //Kein Data Run-Spielziel: Verschwinden
  if(!FindObject(GDAR))
    RemoveObject(this);

  //Standardmenge
  iData = 10;
}

/* Datenmenge festlegen */

public func Set(int iAmount)	{return iData = iAmount;}

/* Aufnahme */

public func RejectEntrance(object pContainer)
{
  //Kann nur von Clonks aufgehoben werden
  if(!(GetOCF(pContainer) & OCF_CrewMember))
    return true;

  return false;
}

public func Entrance(object pClonk)
{
  //Data Run-Spielziel vorhanden: Daten übertragen, ansonsten verschwinden
  if(FindObject(GDAR))
    FindObject(GDAR)->GiveData(GetOwner(pClonk), iData);
  else
    RemoveObject(this);

  //Effekte
  Sound("GetCash*.ogg", false, pClonk);
  Sound("PaperFly*.ogg", false, pClonk);

  //Verschwinden
  RemoveObject(this);
  return true;
}

/* Allgemein */

protected func Hit()
{
  Sound("AmmoBoxHit*.ogg");
  Sound("CrateImpact*.ogg");
}