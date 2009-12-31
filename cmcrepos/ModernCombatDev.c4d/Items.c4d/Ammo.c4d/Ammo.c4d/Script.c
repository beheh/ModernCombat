/*-- Munition --*/

#strict 2

public func IsAmmoPacket() { return GetID() != MCAM; }
public func AmmoID()       { return STAM; }
public func AmmoCount()    { return 100; }
protected func NoArenaRemove() {return true;}

protected func Activate(object pObj)
{
  return TransferAmmo(pObj);
}

public func OnTransfer() { Sound("Resupply.ogg"); }


public func MayTransfer(object pObj)
{
  if(!pObj) return false;
  var MaxAmmo = AmmoID()->~MaxAmmo();
  if(MaxAmmo)
    if(GetAmmo(AmmoID(),pObj) + AmmoCount() > MaxAmmo)
      return false;
  return true;
}

public func TransferAmmo(object pObj)		// Ammo dem Clonk geben
{
  if(!pObj) return false;
  if(NoAmmo()) return false;
  
  // nicht wenn er schon zu viel hat
  if(!MayTransfer(pObj))
  {
    PlayerMessage(GetOwner(pObj),"$NotMoreAmmo$",pObj,AmmoID());
    return ;
  }

  // Nachricht ausgeben
  HelpMessage(GetOwner(pObj),"$Collected$",pObj,AmmoCount(),AmmoID());
  DoAmmo(AmmoID(),AmmoCount(),pObj);
  pObj->~AmmoTransferred();
  if(!OnTransfer()) RemoveObject();

  return true;
}

public func Hit()
{
  Sound("Ammobaghit*.ogg");
  return 1;
}
