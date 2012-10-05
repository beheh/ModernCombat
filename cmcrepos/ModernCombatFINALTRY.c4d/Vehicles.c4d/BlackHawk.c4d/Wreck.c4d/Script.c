/*-- Wrack --*/

#strict 2


/* Initialisierung */

protected func Initialize()
{
  SetAction("Destroyed");
  Schedule("Smoke(0, 0, 30)", 5, 40, this);
}

/* Aufschlag */

protected func Hit3()
{
  if(GetEffectData(EFSM_ExplosionEffects) > 1)
  {
    CastParticles("GunSmoke", 15, 35, -20, 0, 300, 500);
    CastParticles("MetalSplinter", 8, 200, 0, 0, 100, 50, RGB(40, 20, 20));
  }
  Sound("WreckHit*.ogg");
}

protected func Hit()
{
  Sound("WreckHit*.ogg");
}

/* Zerst�rung */

func DestroyWreck()
{
  //Effekte
  CastParticles("GunSmoke", 15, 35, -20, 0, 300, 500);
  CastParticles("GunSmoke", 15, 35, 20, 0, 300, 500);
  if(GetEffectData(EFSM_ExplosionEffects) > 1) CastParticles("MetalSplinter", 8, 200, 0, 0, 100,50, RGB(40, 20, 20));
  if(GBackLiquid())
    Sound("C4EX_WaterDetonation.ogg");
  else
    Sound("C4EX_Detonation*.ogg");
  Sound("StructuralDamage*.ogg");
  Sound("Debris*.ogg");

  //Explosion
  Explode(40);
}

func BlastPar(int X, int Y, int Size, int Color)
{
  CreateParticle("Blast", X, Y, 0, 0, Size, Color, this);
}