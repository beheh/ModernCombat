/*-- Entschärfbare Objekte --*/

//Das Appendto macht bestimmte Objekte durch Schweißbrenner entschärfbar.

#strict 2

#appendto BBTP	//Sprengfallen
#appendto SRBL	//Sensorbälle
#appendto C4EX	//C4 Sätze

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