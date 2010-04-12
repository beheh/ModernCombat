/*-- Waffenwahl --*/

//Die Waffenwahl Regel ist aus technischen Gründen nicht mehr wählbar, wenn bereits die Klassenwahl aktiv ist.

#strict
#appendto WPCH


public func IsChooseable()
{
  if(!ObjectCount(MCSL))
    return(1);
}

//Alle Waffenmodi auffüllen
func Finish(id unused, object pClonk, bool bRight) {
  if(bRight)
    return(MenuQueryCancel(0,pClonk));

  PlayerMessage(GetOwner(spawnclonk),"");
  spawnclonk = 0;
  spawntimer = 0;
  
  var iPlayer = GetOwner(pClonk);
  
  //Auswahl löschen
  WeaponChoice[iPlayer] = CreateArray();
  //Menü schließen
  CloseMenu(GetCursor(iPlayer));
  //Sound! :D
  Sound("Cash",1,0,0,iPlayer);
  
  //Alle Waffen auffüllen
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
      //und noch zusätzliche Muni für den Clonk
    DoAmmo(ammo, load, pClonk);
   }
   wpn->CycleFM(+1); //Noch ein letztes Mal
  }
  
  pClonk->~UpdateCharge();
  
  //Wieder angreifbar machen
  RemoveObject(Contained(pClonk),1);
}
