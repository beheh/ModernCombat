/*-- Keine Attachments --*/

#strict 2

public func IsChooseable()	{return 1;}	//Kann mittels des Spielzielausw�hlers ausgew�hlt werden


protected func Activate(iByPlayer)
{
  MessageWindow(GetDesc(), iByPlayer);
  return 1;
}

global func NoAttachments() {return FindObject2(Find_ID(NOAT));}
