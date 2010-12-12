/*-- Splittergranate --*/

#strict 2
#include NADE

public func Color()		{return RGB(255,255,0);}
public func ContainedDamage()	{return 120;}


/* Explosion */

func Fused()
{
  //Splitter verschleudern
  for(var i = 40; i > 0; --i)
  {
    var angle = Interpolate4K(Random(360),Angle(GetXDir(),GetYDir()),0,80,BoundBy(Distance(GetXDir(),GetYDir()),0,80)) - 180;
    var ammo = CreateObject(SHRP,0,0,GetController());
    ammo->Launch(angle,70+Random(30),200+Random(100),5,15,20);
  }

  //Effekte
  Sparks(30,RGB(255,128));
  CreateParticle("Blast",0,0,0,0,200,RGB(255,255,128));
  if(GetEffectData(EFSM_ExplosionEffects) > 0) CastSmoke("Smoke3",6,8,0,0,80,200,RGBa(255,255,255,120),RGBa(255,255,255,150));
  if(GetEffectData(EFSM_ExplosionEffects) > 1) CastParticles("MetalSplinter",8,200,0,0,45,20,RGB(40,20,20));
  Sound("GrenadeExplosion*.ogg");
  RemoveObject();
}