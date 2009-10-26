#strict 2

#appendto CLNK

#appendto MONS
#appendto FMNS
#appendto WIPF
#appendto FISH
#appendto SHRK
#appendto BIRD
#appendto SNKE


public func IsBulletTarget()
{
  return 1;
}

protected func Death()
{
  FadeOut(this);
  return _inherited(...);
}