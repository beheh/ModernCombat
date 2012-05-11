/*-- Entsch�rfbare Objekte --*/

//Das Appendto macht bestimmte Objekte durch Schwei�brenner entsch�rfbar.

#strict 2

#appendto BBTP	//Sprengfallen
#appendto SRBL	//Sensorb�lle
#appendto C4EX	//C4 S�tze

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