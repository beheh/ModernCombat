#strict 2
#appendto MISS

//Keine Railgunrockets

private func HitObject(object pObject) {
  exploding = true;
  BlastObjects(GetX(), GetY(), iDamage/2);
  BlastObjects(GetX(), GetY(), iDamage/2);
  Explode(iDamage);
}