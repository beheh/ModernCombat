/*-- Spawnen --*/

//Beim Spawnen wird �berpr�ft, ob Objekte "IsSpawnBlocker" den Ort blockieren und zerst�ren diese.

#strict

#appendto TIM2


public func Spawn()
{
  for(var obj in FindObjects(Find_Distance(25), Find_Exclude(this()), Find_Func("IsSpawnBlocker")))
  {
   DoDmg(10000, DMG_Explosion, obj);
  }
  return(_inherited());
}