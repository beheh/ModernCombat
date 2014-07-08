/*-- Keine Waffenaufsätze --*/

#strict 2

global func NoAttachments()	{return FindObject2(Find_ID(NOAT));}
public func IsChooseable()	{return 1;}				//Kann mittels des Spielzielauswählers ausgewählt werden


protected func Activate(iByPlayer)
{
  MessageWindow(GetDesc(), iByPlayer);
  return 1;
}