/*-- Rakete --*/

//Korrigiert den hazard-eigenen Raketenwerfer für CMC.

#strict 2
#appendto MISS


private func HitObject(object pObject)
{
  exploding = true;
  BlastObjects(GetX(), GetY(), iDamage/2);
  BlastObjects(GetX(), GetY(), iDamage/2);
  Explode(iDamage);
}