/*-- Baustelle --*/

#strict


/* Initialisierung */

protected func Initialize()
{
  //Blackhawk erstellen
  CreateObject(BKHK, 0, 30, GetOwner());

  //Verschwinden
  RemoveObject();
  return(true);
}