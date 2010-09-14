/*-- Wrack --*/

#strict


/* Initialisierung */

protected func Initialize()
{
  SetAction("Destroyed");
  Schedule("Smoke(0, 0, 30)", 5, 40, this());
}

/* Aufschlag */

protected func Hit()
{
  if(GetEffectData(EFSM_ExplosionEffects) > 1) CastParticles("GunSmoke",15,35,-20,0,300,500);
  Sound("HeavyHit*.ogg");
}

/* Zerst�rung */

func DestroyWreck()
{
  //Effekte
  CastParticles("GunSmoke",15,35,-20,0,300,500);
  CastParticles("GunSmoke",15,35,20,0,300,500);
  if(GetEffectData(EFSM_ExplosionEffects) > 1) CastParticles("MetalSplinter",8,200,0,0,100,50,RGB(40,20,20));
  Sound("C4EX_Detonation*.ogg", false, this());
  Sound("StructuralDamage*.ogg", false, this());

  //Explosion
  Explode(40);

  //L�schen und ausblenden
  Extinguish();
  FadeOut();
}

func BlastPar(int X, int Y, int Size, int Color)
{
  CreateParticle("Blast", X, Y, 0, 0, Size, Color, this());
}