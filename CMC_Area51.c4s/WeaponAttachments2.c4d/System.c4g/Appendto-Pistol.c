/*-- Pistolen-Erweiterung für Waffenupgrades --*/

#strict 2
#appendto PSTL

func Initialize()
{
   iPermittedAtts = AT_Silencer | AT_TracerDart;
   return _inherited(...);
}

public func FMData2(int data)
{
  if(iAttachment != AT_TracerDart) return 0;
  return _inherited(data);
}
