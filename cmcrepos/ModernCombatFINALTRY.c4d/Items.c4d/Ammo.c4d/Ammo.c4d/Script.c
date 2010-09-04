/*-- Munition --*/

#strict 2

public func IsAmmoPacket()	{return GetID() != MCAM;}
public func AmmoID()		{return STAM;}
public func AmmoCount()		{return 100;}
public func NoArenaRemove()	{return true;}


/* Initalisierung */

protected func Initialize()
{
  //Keine Munition Regel: Kein Existenzgrund
  if(FindObject(NOAM)) Schedule("RemoveObject()", 1);

  return 1;
}

/* Aktivierung */

protected func Activate(object pObj)
{
  return TransferAmmo(pObj);
}

private func OnTransfer()	{}

public func MayTransfer(object pObj)
{
  if(!pObj) return false;
  var MaxAmmo = AmmoID()->~MaxAmmo();
  if(MaxAmmo)
    if(GetAmmo(AmmoID(),pObj) + AmmoCount() > MaxAmmo)
      return false;
  return true;
}

public func TransferAmmo(object pObj)
{
  if(!pObj) return false;
  if(NoAmmo()) return false;
  
  //nicht wenn er schon zu viel hat
  if(!MayTransfer(pObj))
  {
    PlayerMessage(GetOwner(pObj),"$NotMoreAmmo$",pObj,AmmoID());
    return;
  }

  //Nachricht ausgeben
  HelpMessage(GetOwner(pObj),"$Collected$",pObj,AmmoCount(),AmmoID());
  DoAmmo(AmmoID(),AmmoCount(),pObj);
  pObj->~AmmoTransferred();
  Sound("Resupply.ogg");
  if(!OnTransfer()) RemoveObject();

  return true;
}

/* Aufschlag */

protected func Hit()
{
  Sound("AmmoBoxHit*.ogg");
  return 1;
}

protected func Selection()
{
  Sound("FAPK_Charge.ogg");
  return 1;
}