/*-- Laborlicht --*/

//Kompatibilit�t zum neuen Lichtsystem.

#strict 2
#appendto LLGH

public func LightSize()	{return 35;}	//Lichtgr��e


/* Initialisierung */

protected func Initialize()
{
  //Licht erstellen
  CreateLight();
  //Einschalten
  TurnOn();
  bOn = true;
}