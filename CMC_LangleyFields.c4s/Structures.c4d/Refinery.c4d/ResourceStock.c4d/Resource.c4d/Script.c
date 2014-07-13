/*-- Ware --*/

#strict 2

local value;

public func IsDrawable()	{return true;}	//Wird sichtbar getragen
public func HandSize()		{return 800;}
public func HandX()		{return 6000;}
public func HandY()		{return -1000;}


/* Inhalt */

protected func Set(int iValue)
{
  value = iValue;
}

public func CalcValue() { return value; }

/* Sonstiges */

protected func Hit()
{
  Sound("AmmoBoxHit*.ogg");
  return 1;
}

protected func Selection()
{
  Sound("FAPK_Charge.ogg");
  return 1;
}
