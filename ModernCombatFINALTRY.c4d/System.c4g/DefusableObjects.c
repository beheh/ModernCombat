/*-- Entschärfbare Objekte --*/

#strict 2

#appendto BBTP
#appendto SRBL
#appendto C4EX

local rt_defusecnt;

public func IsDefusable()	{return true;}

public func RTDefuse()
{
  rt_defusecnt++;
  if(rt_defusecnt > 10)
  {
    Sound("Limitation.ogg");
    DecoExplode(10);

    return true;
  }

  return false;
}
