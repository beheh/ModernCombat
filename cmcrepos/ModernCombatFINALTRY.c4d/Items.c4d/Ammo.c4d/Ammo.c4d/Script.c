/*-- Munition --*/

#strict

public func IsAmmoPacket() { return(GetID() != MCAM); }
public func AmmoID()       { return(STAM); }
public func AmmoCount()    { return(100); }


/* Aktivierung */

protected func Activate(object pObj)
{
  return(TransferAmmo(pObj));
}

private func OnTransfer() {}

public func MayTransfer(object pObj)
{
  if(!pObj) return(false);
  var MaxAmmo = AmmoID()->~MaxAmmo();
  if(MaxAmmo)
    if(GetAmmo(AmmoID(),pObj) + AmmoCount() > MaxAmmo)
      return(false);
  return(true);
}

public func TransferAmmo(object pObj)
{
  if(!pObj) return(false);
  if(NoAmmo()) return(false);
  
  // nicht wenn er schon zu viel hat
  if(!MayTransfer(pObj))
  {
    PlayerMessage(GetOwner(pObj),"$NotMoreAmmo$",pObj,AmmoID());
    return();
  }

  // Nachricht ausgeben
  HelpMessage(GetOwner(pObj),"$Collected$",pObj,AmmoCount(),AmmoID());
  DoAmmo(AmmoID(),AmmoCount(),pObj);
  pObj->~AmmoTransferred();
  Sound("Resupply.ogg");
  if(!OnTransfer()) RemoveObject();

  return(true);
}

/* Aufschlag */

protected func Hit()
{
  Sound("AmmoBoxHit*.ogg");
  return(1);
}