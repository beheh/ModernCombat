/*-- Kein FakeDeath --*/

#strict

public func IsChooseable()	{return(1);}	//Kann mittels des Spielzielausw�hlers ausgew�hlt werden


/* Infos */

protected func Activate(iByPlayer)
{
  MessageWindow(GetDesc(), iByPlayer);
  return(1);
}