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
  if(rt_defusecnt > 8)
  {
    Sound("MISL_ShotDown.ogg");
    DecoExplode(10);

    return true;
  }

  return false;
}