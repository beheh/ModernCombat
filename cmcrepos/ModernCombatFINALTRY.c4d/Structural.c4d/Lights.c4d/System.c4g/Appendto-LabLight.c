/*-- Laborlicht --*/

//Kompatibilität zum neuen Lichtsystem.

#strict 2
#appendto LLGH

public func LightSize()	{return 35;}	//Lichtgröße


/* Initialisierung */

protected func Initialize()
{
  //Licht erstellen
  CreateLight();
  //Einschalten
  TurnOn(1);
  bOn = true;
}