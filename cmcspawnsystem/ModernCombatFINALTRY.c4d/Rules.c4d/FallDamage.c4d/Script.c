/*-- Fallschaden --*/

#strict 2

public func IsChooseable()	{return 1;}	//Kann mittels des Spielzielausw�hlers ausgew�hlt werden


protected func Activate(iPlr)
{
  MessageWindow(GetDesc(),iPlr);
}