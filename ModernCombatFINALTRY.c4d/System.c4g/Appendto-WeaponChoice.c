/*-- Waffenwahl --*/

//Die Waffenwahl Regel ist aus technischen Gründen nicht mehr wählbar, wenn bereits die Klassenwahl aktiv ist.

#strict 2
#appendto WPCH

public func IsChooseable()	{return;}


public func Initialize()
{
  SetPosition();
  WeaponChoice = CreateArray();
  aWare = CreateArray();
  aCount = CreateArray();
  AddWares("IsWeapon2",-1);
  AddWares("IsGrenade",-1);
  AddWares("IsCMCEquipment",-1);
  SortWare("IsWeapon2","IsGrenade","IsCMCEquipment");
}

public func RelaunchPlayer(int iPlr, object pClonk)
{
  if(!pClonk)
    if(!(pClonk = GetCursor(iPlr)))
      return(ScheduleCall(this,"RelaunchPlayer",1,0,iPlr,pClonk));
  if(!GetAlive(pClonk))
    return(ScheduleCall(this,"RelaunchPlayer",1,0,iPlr));

  if(FindObject(GLMS))
  {
    spawntimer = 10;
    spawnclonk = pClonk;
  }

  var obj;
  while(obj = Contents(0, pClonk))
  {
    if(obj) RemoveObject(obj);
  }

  if(GetLength(WeaponChoice[iPlr]))
  {
    var WeaponChoice2 = CreateArray();
    for(var ID in WeaponChoice[iPlr])
    {
      if(!ID) continue;
      if(GetDefValue(ID) > GetWealth(iPlr))
        continue;
      //testen ob wir das Objekt überhaupt noch aufnehmen dürfen
      var tmp = CreateObject(ID,0,0,iPlr);
      if(pClonk->~RejectCollect(ID,tmp))
      {
        RemoveObject(tmp);
        continue;
      }
      else
      {
        Enter(pClonk,tmp);
        AddToArray(WeaponChoice2,ID);
      }
    }

    //Auswahl löschen
    WeaponChoice[iPlr] = WeaponChoice2;
  }
  else
  {
    WeaponChoice[iPlr] = CreateArray();
  }

  CreateWpnMenu(pClonk);
  return;
}

func Finish(id unused, object pClonk, bool bRight)
{
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
   while(wpn->~CycleFM(+1)) {
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
   wpn->~CycleFM(+1); //Noch ein letztes Mal
  }

  pClonk->~UpdateCharge();

  GameCallEx("OnClassSelection", pClonk);

  //Wieder angreifbar machen
  RemoveObject(Contained(pClonk),1);
}