/*-- Kein FakeDeath --*/

#strict

public func IsChooseable()	{return(1);}	//Kann mittels des Spielzielauswählers ausgewählt werden


/* Infos */

protected func Activate(iByPlayer)
{
  MessageWindow(GetDesc(), iByPlayer);
  return(1);
}