#strict

public func IsAmmoPacket() { return(true); }
public func AmmoID()       { return(STAM); }
public func AmmoCount()    { return(100); }

public func RejectEntrance(object pObj)
{
  if(pObj->GetAlive())
  {
    TransferAmmo(pObj);
    return(1);
  }
}

private func OnTransfer() {}

protected func Activate(object pObj)
{
  return(TransferAmmo(pObj));
}

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
  

  if(!MayTransfer(pObj))
  {
    PlayerMessage(GetOwner(pObj),"$NotMoreAmmo$",pObj,AmmoID());
    return();
  }

  HelpMessage(GetOwner(pObj),"$Collected$",pObj,AmmoCount(),AmmoID());
  DoAmmo(AmmoID(),AmmoCount(),pObj);
  pObj->~AmmoTransferred();
  pObj->Sound("H2AP_Pickup.ogg");
  
  if(!OnTransfer()) RemoveObject();

  return(true);
}
