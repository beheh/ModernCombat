/*-- Kokons sind Verkaufsware bei Waffenautomaten --*/

#strict
#appendto WPVM

func ControlThrowDouble(object pByObj)
{
  var pItem = pByObj->Contents();
  if(!pItem) return();
  if(!FindWare(GetID(pItem)) && (GetID(pItem) != SK5B)) return();
  
  var iPlr = GetController(pByObj);

  //Geld abziehn
  if(GetID(pItem) == SK5B)
  {
    DoWealth(iPlr,5);
    Sound("UnCash",0,0,0,iPlr+1);
    RemoveObject(pItem);
    return();
  }
  
  DoWealth(iPlr,GetValue(pItem,0,0,iPlr)/2);
  //*Ca-Ching*
  Sound("UnCash",0,0,0,iPlr+1);

  DoWare(GetID(pItem), +1);
  
  RemoveObject(pItem);
}