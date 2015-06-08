/*-- Laserpunkt --*/

#strict 2

local fActive;

public func Active()	{return fActive;}


/* Initialisierung */

func Initialize()
{
  SetVisibility(VIS_None);
}

/* Leuchten */

func Start()
{
  fActive = true;
  LaserLight(RandomX(3,6), GetColorDw(this),0,0,10);
  if(!GetEffect("Activity",this))
    AddEffect("Activity", this, 1, 1 ,this);
}

func Stop()
{
  fActive = false;
  if(GetEffect("Activity",this))
    RemoveEffect("Activity",this);
}

public func FxActivityTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
  LaserLight(RandomX(3,6), GetColorDw(this),0,0,10);
}