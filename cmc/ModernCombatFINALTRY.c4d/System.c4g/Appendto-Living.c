/*-- Lebewesen --*/

//Alle inkludierten Lebewesen sind Projektilziele und faden nach dem Tod aus.

#strict 2

#appendto CLNK	//Clonk
#appendto MONS	//Monster
#appendto FMNS	//Feuermonster
#appendto WIPF	//Wipf
#appendto FISH	//Fisch
#appendto SHRK	//Hai
#appendto BIRD	//Vogel
#appendto SNKE	//Schlange


public func IsBulletTarget()
{
  return 1;
}

protected func Death()
{
  FadeOut(this);
  return _inherited(...);
}