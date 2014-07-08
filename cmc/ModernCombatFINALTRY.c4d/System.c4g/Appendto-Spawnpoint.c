/*-- Spawnpoint --*/

//Spawnpoints sind nun als solche identifizierbar.

#strict 2
#appendto SPNP

public func IsSpawnpoint()	{return true;}


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

    //Munition sofort in Munitionsgürtel legen
    var fAmmo = pObj->~IsAmmoPacket();
    if(pObj && fAmmo)
      pObj->~TransferAmmo(pClonk);
    else
      Collect(pObj, pClonk);

    if(Contained(pObj) == this && !fAmmo)
      RemoveObject(pObj);
    else
    {
      Sound("Grab", 0, pClonk, 0, GetOwner(pClonk)+1);
      Collected(GetOwner(pClonk), pClonk);
      // Waffe, gleich einsatzbereit!
      if(pObj && pObj->~IsWeapon())
        DoAmmo(pObj->GetFMData(FM_AmmoID),pObj->GetFMData(FM_AmmoLoad),pObj);
    }
  }
  return 1;
}