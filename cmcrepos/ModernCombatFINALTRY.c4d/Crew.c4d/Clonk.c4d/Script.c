/*--- Der Clonk ---*/

#strict 2
#include PCMK

public func SkinCount() { return 2; }

public func WeaponCollectionLimit() { return 1; }
public func ObjectCollectionLimit() { return 4; }

public func Death()
{
  if(!this->~Redefine2(CLNK))
    this->~Redefine(CLNK);
  return _inherited(...);
}