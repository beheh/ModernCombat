/*-- Bohrkopf --*/

#strict 2

#appendto PIPH

local pDerrick;

public func Set(object pObj)
{
	pDerrick = pObj;
	return true;
}

private func FindDerrick()
{
 return pDerrick;
}

private func Transfer()
{
  // Kein Zielobjekt  
  if(!pDerrick) return;
  // Zielobjekt erlaubt keinen Transfer
  if(!pDerrick->~AcceptTransfer()) return;
  // Zielobjekt hat nicht genug Energie  
  if(!pDerrick->~HasEnergy()) return;
  // Material soll nicht gepumt werden
  if(GetIndexOf(GetMaterial(), pDerrick->~AcceptedMaterials()) == -1) return;
  // Materialtransfer
  for (var i = 0; i < 3; i++)  
    pDerrick->~OnMaterialTransfer(ExtractLiquid());
    //ObjectInsertMaterial(ExtractLiquid(0, 0), pPumpTarget);
  // Blubbern
  if(!Random(5)) Bubble(0, 0);
  // Fertig
  return true;
}
