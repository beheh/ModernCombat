/*-- Splittergranate --*/

#strict 2
#include NADE

public func IsSpawnTrap()	{return IsFusing();}
public func Color()		{return RGB(255,255,0);}	//Farbe
public func ContainedDamage()	{return 80;}			//Schaden bei Detonation innerhalb eines Objekts


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
  if(GetEffectData(EFSM_ExplosionEffects) > 1)
  {
    CastParticles("MetalSplinter",10,150,0,0,25,50);
    CastSmoke("Smoke3",10,15,0,0,100,200);
  }
  CreateParticle("ShockWave",0,0,0,0,300,RGB(255,255,55));
  CreateParticle("Blast",0,0,0,0,200);
  Sparks(30,RGB(255,128));
  Sound("GrenadeExplosion*.ogg");

  RemoveObject();
}