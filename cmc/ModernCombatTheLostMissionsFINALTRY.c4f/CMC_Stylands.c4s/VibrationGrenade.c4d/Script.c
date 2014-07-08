/*-- Vibrationsgranate --*/

#strict 2
#include NADE

public func FuseTime()		{return 2*35;}
public func Color()		{return RGB(0,255,0);}
public func ContainedDamage()	{return 0;}


/* Aktivierung */

public func Fused()
{
  Sound("Fuse");
  SetAction("Activated");
  ShakeFree(GetX(this),GetY(this),35);
}

protected func Active()
{
  //Material freirütteln
  ShakeFree(GetX(this),GetY(this),35-GetActTime());

  //Raucheffekt
  CreateParticle("GunSmoke",0,0,0,0,10*(35-GetActTime()),RGBa(0,255,0,64));
}

protected func Stop()
{
  Sound("Pshshsh");
  RemoveObject();
}

/* Aufschlag */

func HitSound()
{
  Sound("SGRN_Hit*.ogg");
}