/*-- Insta GIB --*/

#strict

protected func Activate(iPlr)
{
  MessageWindow(GetDesc(),iPlr);
}

protected func Initialize()
{
  ScheduleCall(this(), "Initialized", 1);
}

protected func Initialized()
{
  // Keine-Munition-Regel muss da sein
  if(!FindObject(NOAM)) CreateObject(NOAM, 0,0, -1);
}

public func OnClonkRecruitment(object pClonk, int iPlr)
{
  CreateContents(APLO, pClonk)->DoAmmo(STAM, 1);
}

// Kann mittels des Spielzielauswählers ausgewählt werden
public func IsChooseable() { return(1); }
