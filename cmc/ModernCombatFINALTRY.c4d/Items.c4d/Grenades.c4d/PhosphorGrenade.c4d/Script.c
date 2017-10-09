/*-- Phosphorgranate --*/

#strict 2
#include NADE

public func Color()		{return RGB(235,199,54);}	//Farbe
public func ContainedDamage()	{return 100;}			//Schaden bei Detonation innerhalb eines Objekts
public func BlastRadius()	{return 10;}			//Explosionsradius


/* Aktivierung */

public func Fuse()
{
  Sound("SGRN_Activate.ogg");
  SetGraphics("Active");

  return AddEffect("IntFuse",this,200,1,this);
}

public func Fused()
{
  //Explosion und Zusatzschaden
  Explode(BlastRadius()*2/3);
  DamageObjects(BlastRadius()*3/2,BlastRadius()*2,this);

  //Phosphor verschleudern
  for(var i = 0; i < 8; i++)
  {
    var pPhosphor = CreateObject(PSPR, 0, 0, GetOwner());
    SetController(GetController(), pPhosphor);
    SetXDir(Random(2*50+1)-50, pPhosphor);
    SetYDir(Random(2*50+1)-50, pPhosphor);
  }

  //Effekte
  if(GetEffectData(EFSM_ExplosionEffects) > 1)
  {
    CastParticles("MetalSplinter",10,150,0,0,25,50);
    CastSmoke("Smoke3",20,20,0,0,100,200,RGBa(100,150,250,100));
  }
  CreateParticle("ShockWave",0,0,0,0,12*BlastRadius(),RGB(0,255,255));
  CreateParticle("Blast",0,0,0,0,10*BlastRadius(),RGB(20,128,255));
  Sound("PhosphorExplosion.ogg");
  Sound("GrenadeExplosion*.ogg");
}

/* Aufprall */

func HitSound()
{
  Sound("SGRN_Hit*.ogg");
}