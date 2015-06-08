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

public func RejectEntrance(object pClonk)
{
  //Kann nur von Clonks aufgehoben werden
  if(!(GetOCF(pClonk) & OCF_CrewMember))
    return true;

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

  return false;
}

/*
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

  //Inventar wieder zurückswitchen
  //ShiftContents(pClonk, true);
  return true;
}*/

/* Allgemein */

protected func Hit()
{
  Sound("AmmoBoxHit*.ogg");
  Sound("CrateImpact*.ogg");
}