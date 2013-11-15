/*-- Neues Script --*/

#strict 2

#appendto SPNP


public func RejectEntrance(object pClonk)
{
  // Objekt ist ein Clonk?
  if(!(GetOCF(pClonk) & OCF_CrewMember) && !(pClonk->~CanCollectFromSpawnpoints())) return 1;
  // Ich hab Contents?
  if(!Contents() || !initialized) return 1;
  // Darf einsammeln
  if(CheckCollect(GetOwner(pClonk), pClonk))
  {
    var pObj = CreateContents(GetID(Contents()));
    // Kann der Clonk einsammeln?
    if(pObj->~IsAmmoPacket())
    {
      if(!(pObj->MayTransfer(pClonk)))
        return 1;
      if(NoAmmo()) return 1;
    }

    Collect(pObj, pClonk);
    if(Contained(pObj) == this)
	  RemoveObject(pObj);
    else
    {
    	pObj->~OnSpawnpointCollect(this, pClonk);
      Sound("Grab", 0, pClonk, 0, GetOwner(pClonk)+1);
      Collected(GetOwner(pClonk), pClonk);
      // Munition
      if(pObj && pObj->~IsAmmoPacket())
	    pObj->~TransferAmmo(pClonk);
      // Waffe, gleich einsatzbereit!
      else if(pObj && pObj->~IsWeapon())
        DoAmmo(pObj->GetFMData(FM_AmmoID),pObj->GetFMData(FM_AmmoLoad),pObj);
    }
  }
  return 1;
}
