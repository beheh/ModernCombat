/*-- Waffenautomat --*/

//Automaten können auch getragene Objekte verkaufen.

#strict 2

#appendto WPVM


/*local WeaponChoice;

func Initialize()
{
  SetPosition();
  WeaponChoice = CreateArray();
  WPCH::inherited();
  AddWares("IsWeapon",-1);
  AddWares("IsGrenade",-1);
  AddWares("IsEquipment",-1);
  AddWares("IsUpgrade",-1);
  SortWare("IsWeapon","IsGrenade","IsEquipment","IsUpgrade");
}*/

func ControlThrowDouble(object pByObj)
{
  var pItem = Contents(0, pByObj);
  if(!pItem) return;
  if(!FindWare(GetID(pItem))) return;

  var iPlr = GetController(pByObj);

  //Geld abziehen
  DoWealth(iPlr,GetValue(pItem,0,0,iPlr)/2);
  Sound("UnCash",0,0,0,iPlr+1);

  DoWare(GetID(pItem), +1);

  RemoveObject(pItem);
}