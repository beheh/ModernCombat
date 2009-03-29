/*-- Semtexgranate --*/

#strict
#include NADE

local time;//:X

public func FuseTime(){return(4*35);}
public func Color(){return(RGB(255,128,0));}//grün
public func ContainedDamage(){return(0);}

func HitSound()
{
  Sound("SGRN_Hit*");
}

public func Fused()
{
  Sound("SXGR_SemtexBlast.ogg");
  ScheduleCall(this(),"DoFlames",3,20); 
  FadeOut();
}

func DoFlames()
{
  var flame, x,y, a = Random(360), s = 20+Random(20);
  x = +Cos(a,s);
  y = -Sin(a,s);
  var flame = CreateObject(SFLM,0,0,GetController());
  flame->SetMaxLifetime(35*4+Random(35));
  flame->Launch(x,y,0,80);
}