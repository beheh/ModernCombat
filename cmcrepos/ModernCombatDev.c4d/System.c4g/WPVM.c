/*-- Waffenautomat --*/
//Kann auch Dinge verkaufen.
#strict 2
#appendto WPVM

/*local WeaponChoice;

func Initialize()
{
  SetPosition();
  WeaponChoice = CreateArray(0);
  WPCH::inherited();
  AddWares("IsWeapon",-1);
  AddWares("IsGrenade",-1);
  AddWares("IsEquipment",-1);
  AddWares("IsUpgrade",-1);
  SortWare("IsWeapon","IsGrenade","IsEquipment","IsUpgrade");
}*/

func ControlThrowDouble(object pByObj)
{
  var pItem = pByObj->Contents();
  if(!pItem) return ;
  if(!FindWare(GetID(pItem))) return ;
  
  var iPlr = GetController(pByObj);

  //Geld abziehn
  DoWealth(iPlr,GetValue(pItem,0,0,iPlr)/2);
  //*Ca-Ching*
  Sound("UnCash",0,0,0,iPlr+1);

  DoWare(GetID(pItem), +1);
  
  RemoveObject(pItem);
}
