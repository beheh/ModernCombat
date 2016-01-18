/*-- Neues Script --*/

#strict 2

#appendto CSTD

public func OnClonkEquip(object pClonk) {
  //Falls kein Clonk, nicht ausf�hren
  if(!pClonk)
    return;

  //Falls Clonk bereits ausger�stet, nicht nochmal ausr�sten
  if(Contents(0,pClonk))
    return;

  //Keine Klassenwahl: Standardausr�stung vergeben: Pistole und Granaten
  if(!FindObject2(Find_Category(C4D_Rule|C4D_Goal|C4D_Environment), Find_Func("CustomClonkEquip"))) {
    var wpn = CreateContents(PSTL, pClonk);
    wpn->DoAmmo(wpn->GetFMData(FM_AmmoID),wpn->GetFMData(FM_AmmoLoad));
    CreateContents(FGRN, pClonk);
    CreateContents(FRAG, pClonk);
    CreateContents(STUN, pClonk);

    //Zusatzmunition f�r die Pistole sofern ben�tigt
    if(!FindObject(NOAM))
      pClonk->DoAmmo(wpn->GetFMData(FM_AmmoID),wpn->GetFMData(FM_AmmoLoad)*2);
  }
  
  return;
}
