/*-- Gestein kann zerschossen werden --*/

#strict 2
#appendto RCK2

public func IsBulletTarget() { return 1; }


/* Zerstörung */

func Damage()
{
  if(GetDamage() > 5)
  {
   var i;
   while (++i * 20 < GetCon()) CastSplitRock();
   Sound("RockBreak*");
   Smoke(Random(11) -5, Random(11) -5, Random(25));
   Smoke(Random(11) -5, Random(11) -5, Random(25));
   Smoke(Random(11) -5, Random(11) -5, Random(25));
   Smoke(Random(11) -5, Random(11) -5, Random(25));
   RemoveObject();
   return 1;
  }
}