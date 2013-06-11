/*-- Splittergranate --*/

#strict 2
#include ESHL

public func Color()	{return RGB(255,255,0);}
func ExplodeDelay()	{return 80;}


func Trigger()
{
  //Splitter erzeugen
  for(var i = 20; i > 0; --i)
  {
    var angle = Random(360);
    var ammo = CreateObject(SHRP,0,0,GetController());
    ammo->Launch(angle,70+Random(30),100+Random(100),5,15,20, iAttachment);
  }

  //Effekte
  CreateParticle("Blast",0,0,0,0,5*10,RGB(255,255,128));
  if(GetEffectData(EFSM_ExplosionEffects) > 0) CastSmoke("Smoke3",10,15,0,0,300,50,RGBa(255,255,255,120),RGBa(255,255,255,150));
  if(GetEffectData(EFSM_ExplosionEffects) > 1) CastParticles("MetalSplinter",5,100,0,0,20,40,RGB(40,20,20));
  Sparks(15,RGB(255,128));
  Sound("GrenadeExplosion*.ogg");
  AddLightFlash(50,0,0,RGB(255,255,128),this);

  //Verschwinden
  RemoveObject();
}