/*--- Der Sanitäter ---*/

#strict
#include PCMK

public func WeaponCollectionLimit() { return(2); } // max. Waffen im Inventar
public func ObjectCollectionLimit() { return(3); } // max. Objekte im Inventar

protected func IsMedic() { return(1); } //Diese eine Zeile macht den Clonk so besonders.

public func HazardGearSupported(object pGear)
{
  if(GetID(pGear) == HARM) return(false);
  return(true);
}
