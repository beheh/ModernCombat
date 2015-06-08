/*-- Splittergranate --*/

#strict 2
#include NADE

public func Color()		{return RGB(255,255,0);}	//Farbe
public func ContainedDamage()	{return 120;}			//Schaden bei Detonation innerhalb eines Objekts


/* Aktivierung */

func Fused()
{
  //Splitter verschleudern
  var i, iAngle, pAmmo;
  var iR = Angle(GetXDir(),GetYDir()), iSpeed = BoundBy(Distance(GetXDir(),GetYDir()),0,80);
  for(i = 40; i > 0; i--)
  {
    iAngle = Interpolate4K(Random(360),iR,0,80,iSpeed);
    pAmmo = CreateObject(SHRP,0,0,GetController());
    SetController(GetController(), pAmmo);
    pAmmo->Launch(iAngle,70+Random(30),200+Random(100),5,15,20);
  }

  //Effekte
  Sparks(30,RGB(255,128));
  CreateParticle("Blast",0,0,0,0,200,RGB(255,255,128));
  if(GetEffectData(EFSM_ExplosionEffects) > 0) CastSmoke("Smoke3",6,8,0,0,80,200,RGBa(255,255,255,120),RGBa(255,255,255,150));
  if(GetEffectData(EFSM_ExplosionEffects) > 1) CastParticles("MetalSplinter",8,200,0,0,45,20,RGB(40,20,20));
  Sound("GrenadeExplosion*.ogg");
  RemoveObject();
}