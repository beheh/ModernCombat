/*-- Metall --*/

//Bei Erstellung von Br�cken wird Stein verwendet statt Granit.

#strict 2
#appendto METL


public func BridgeMaterial()
{
  return(Material("Rock"));
}