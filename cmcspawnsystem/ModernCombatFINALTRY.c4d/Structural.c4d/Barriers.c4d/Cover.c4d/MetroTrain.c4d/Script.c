/*-- U-Bahnzug --*/

#strict 2

local damaged;


/* Zerstörung */

func Damage(int iChange, int iPlr)
{
  if(!this)
    return;
  if(damaged)
    return;
  SetController(iPlr);

  if(!this)
    return;
  if(GetDamage() < 250) return;
  Explode(iPlr);
}

func Explode(int iPlr)
{
  if(damaged) return;
  damaged = true;

  //Effekte
  if(GetEffectData(EFSM_ExplosionEffects) > 0) CastSmoke("Smoke3",30,40,0,0,500,200,RGBa(255,255,255,100),RGBa(255,255,255,130));
  if(GetEffectData(EFSM_ExplosionEffects) > 1) CastParticles("MetalSplinter",8,150,0,0,150,100);
  if(GetEffectData(EFSM_ExplosionEffects) > 1) CastParticles("MetalSplinter",20,150,0,0,40,15,RGB(40,20,20));
  AddFireEffect(this,50,RGB(80,80,80),true,30);
  Sound("StructuralDamage*.ogg");

  //Splitter verschleudern
  for(var i = 10; i > 0; --i)
  {
    var angle = Interpolate4K(Random(360),Angle(GetXDir(),GetYDir()),0,40,BoundBy(Distance(GetXDir(),GetYDir()),0,40)) - 180;
    var ammo = CreateObject(SHRP,0,0,iPlr);
    SetController(iPlr, ammo);
    ammo->Launch(angle,70+Random(30),100+Random(100),5,15,20,0,1);
  }

  //Explosion
  FakeExplode(40, iPlr+1);

  //Aussehen verändern
  SetGraphics("Destroyed");
}