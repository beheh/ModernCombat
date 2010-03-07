/*-- Splittergranate --*/

#strict
#include NADE

public func Color(){return(RGB(255,255,0));}

public func ContainedDamage(){return(120);}

func Fused()
{
  for(var i = 40; i > 0; --i)
  {
    var angle = Interpolate4K(Random(360),Angle(GetXDir(),GetYDir()),0,80,BoundBy(Distance(GetXDir(),GetYDir()),0,80)) - 180;
    var ammo = CreateObject(SHRP,0,0,GetController());
    ammo->Launch(angle,70+Random(30),200+Random(100),5,15,20);
  }
  
  CreateParticle("Blast",0,0,0,0,10*15,RGB(255,255,128));
  Sparks(15,RGB(255,128));
  CastParticles("Smoke3",6,8,0,0,80,200,RGBa(255,255,255,120),RGBa(255,255,255,150));
  AddLightFlash(50,0,0,RGB(255,255,128),this());
  Sound("GrenadeExplosion*.ogg");
  RemoveObject();
}
