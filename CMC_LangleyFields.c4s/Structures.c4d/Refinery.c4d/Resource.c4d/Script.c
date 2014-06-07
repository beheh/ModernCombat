/*-- Ware --*/

#strict 2

local value, duration;

public func IsDrawable()	{return true;}	//Wird sichtbar getragen
public func HandSize()		{return 800;}
public func HandX()		{return 6000;}
public func HandY()		{return -1000;}


/* Inhalt */

protected func Set(int iValue, int iDuration)
{
  value = iValue;
  duration = iDuration;
}

public func GetVal()		{return value;}
public func GetDuration()	{return duration;}

/* Aktivierung */

protected func Activate(object pClonk)
{
  SetWealth(GetOwner(pClonk), GetWealth(GetOwner(pClonk))+value/2);
  Sound("GetCash*.ogg");
}

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