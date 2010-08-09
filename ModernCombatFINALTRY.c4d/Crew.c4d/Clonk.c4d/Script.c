/*--- Der Clonk ---*/

#strict 2
#include PCMK //inkludiert Peacemaker für CMC Agilität

public func WeaponCollectionLimit() { return 1; } //Der Clonk kann eine Waffe tragen
public func ObjectCollectionLimit() { return 4; } //Und 4 beliebige Zusatzobjekte


public func Death()
{
  /*if(!this->~Redefine2(CLNK))
    this->~Redefine(CLNK);*/
  return _inherited(...);
}
