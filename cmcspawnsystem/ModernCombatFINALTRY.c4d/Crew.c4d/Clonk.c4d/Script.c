/*--- Der Clonk ---*/

#strict 2
#include PCMK

public func WeaponCollectionLimit()	{return 1;}	//Anzahl Waffen im Inventar
public func ObjectCollectionLimit()	{return 4;}	//Anzahl Objekte im Inventar


public func Death()
{
  /*if(!this->~Redefine2(CLNK))
    this->~Redefine(CLNK);*/
  return _inherited(...);
}