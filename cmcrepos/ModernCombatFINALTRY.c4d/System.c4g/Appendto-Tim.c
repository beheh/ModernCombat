/*-- Spawnen --*/

//Beim Spawnen wird überprüft, ob Objekte "IsSpawnBlocker" den Ort blockieren und zerstören diese.

#strict 2

#appendto TIM1
#appendto TIM2

public func RejectCollect(id idObject)
{
  if(GetCategory(0, idObject) & C4D_Living) return false;
  return true;
}

public func Spawn()
{
  for(var obj in FindObjects(Find_Distance(25), Find_Exclude(this), Find_Func("IsSpawnBlocker")))
  {
    DoDmg(10000, DMG_Explosion, obj);
  }
  return _inherited();
}

protected func ContainedDown()
{
  _inherited(...);
  return true;
}