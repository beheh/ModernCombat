/*-- Schalldämpfer --*/

#strict
#include KLAS


protected func Hit()
{
  Sound("MetalHit*");
}

public func UpgradeObject(object pObj)
{
  if(pObj->~Upgrade(GetID()))
  {
   //Sound
   Sound("SilencerOn.ogg");
   //Nur entfernen wenn Upgrade() von pObj "true" wiedergibt
    return(RemoveObject());
  }
}