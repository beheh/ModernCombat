/*-- Waffenwahl --*/

//Die Waffenwahl Regel ist aus technischen Gr�nden nicht mehr w�hlbar, wenn bereits die Klassenwahl aktiv ist.

#strict
#appendto WPCH


public func IsChooseable()
{
  if(!ObjectCount(MCSL))
    return(1);
}

//Alle Waffenmodi auff�llen
func Finish(id unused, object pClonk, bool bRight) {
  if(bRight)
    return(MenuQueryCancel(0,pClonk));

  PlayerMessage(GetOwner(spawnclonk),"");
  spawnclonk = 0;
  spawntimer = 0;
  
  var iPlayer = GetOwner(pClonk);
  
  //Auswahl l�schen
  WeaponChoice[iPlayer] = CreateArray();
  //Men� schlie�en
  CloseMenu(GetCursor(iPlayer));
  //Sound! :D
  Sound("Cash",1,0,0,iPlayer);
  
  //Alle Waffen auff�llen
  for(var wpn in FindObjects(Find_Container(pClonk), Find_Func("IsWeapon")))
  {
   while(wpn->CycleFM(+1)) {
    var ammo = wpn->GetFMData(FM_AmmoID);
    var load = wpn->GetFMData(FM_AmmoLoad);
    if(wpn->GetAmmo(ammo) == load) break;
    //erst entladen
    DoAmmo(ammo,-load, wpn);
    //dann neu reinladen
    DoAmmo(ammo, load, wpn);
      //und noch zus�tzliche Muni f�r den Clonk
    DoAmmo(ammo, load, pClonk);
   }
   wpn->CycleFM(+1); //Noch ein letztes Mal
  }
  
  pClonk->~UpdateCharge();
  
  //Wieder angreifbar machen
  RemoveObject(Contained(pClonk),1);
}
