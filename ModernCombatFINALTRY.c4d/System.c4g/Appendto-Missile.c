/*-- Rakete --*/

//Raketenfehler in Hazardgegenst�ck �berladen und korrigiert.

#strict 2
#appendto MISS

private func HitObject(object pObject)
{
  exploding = true;
  BlastObjects(GetX(), GetY(), iDamage/2);
  BlastObjects(GetX(), GetY(), iDamage/2);
  Explode(iDamage);
}