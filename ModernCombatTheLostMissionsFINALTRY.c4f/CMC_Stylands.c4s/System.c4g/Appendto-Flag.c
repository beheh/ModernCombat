/*-- Flagge --*/

//Beim Einsammeln einer Flagge ist der Sammler der neue Besitzer.

#strict 2
#appendto FLAG


protected func Entrance(object container)
{
  if(GetAlive(container))
    SetOwner(GetOwner(container));

  return(_inherited(container, ...));
}