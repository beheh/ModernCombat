/*-- Waffenwahl --*/

//Die Waffenwahl Regel ist aus technischen Gr�nden nicht mehr w�hlbar, wenn bereits die Klassenwahl aktiv ist.

#strict
#appendto WPCH

public func IsChooseable()
{
  if(!ObjectCount(MCSL))
    return(1);
}

func Initialize() {
  SetPosition();
  WeaponChoice = CreateArray(0);
  aWare = CreateArray();
  aCount = CreateArray();
  AddWares("IsWeapon2",-1);
  AddWares("IsEquipment",-1);
  SortWare("IsWeapon2","IsEquipment");
}

func RelaunchPlayer(int iPlr, object pClonk) {
  if(!pClonk)
    if(!(pClonk = GetCursor(iPlr)))
      return(ScheduleCall(this(),"RelaunchPlayer",1,0,iPlr,pClonk));
  if(!GetAlive(pClonk))
    return(ScheduleCall(this(),"RelaunchPlayer",1,0,iPlr));

  if(FindObject(GLMS)) {
	spawntimer = 10;
	spawnclonk = pClonk;
  }

	var obj;
	while(obj = pClonk->Contents()) {
	  if(obj) RemoveObject(obj);
	}
	
	if(GetLength(WeaponChoice[iPlr])) {

		var WeaponChoice2 = CreateArray();
		for(var ID in WeaponChoice[iPlr]) {
			if(!ID) continue;
			if(GetDefValue(ID) > GetWealth(iPlr))
		    continue;
		  //testen ob wir das Objekt �berhaupt noch aufnehmen d�rfen
		  var tmp = CreateObject(ID,0,0,iPlr);
		  if(pClonk->~RejectCollect(ID,tmp)) {
		    RemoveObject(tmp);  
		    continue;
		  }
		  else {
		    Enter(pClonk,tmp);
			  AddToArray(WeaponChoice2,ID);
			}
		}
			
		//Auswahl l�schen
		WeaponChoice[iPlr] = WeaponChoice2;
	}
	else {
	  WeaponChoice[iPlr] = CreateArray();
	}
	
  CreateWpnMenu(pClonk);
  return();
}

//Alle Waffenmodi auff�llen
func Finish(id unused, object pClonk, bool bRight) {
  _inherited(...);
  
  GameCallEx("OnWeaponChoice", pClonk);
  
  //Alle Waffen auff�llen
  for(var wpn in FindObjects(Find_Container(pClonk), Find_Func("IsWeapon")))
  {
   while(wpn->~CycleFM(+1)) {
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
   wpn->~CycleFM(+1); //Noch ein letztes Mal
  }
}
