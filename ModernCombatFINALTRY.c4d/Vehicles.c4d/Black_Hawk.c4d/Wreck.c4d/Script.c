/*-- Wrack --*/

#strict


/* Initalisierung */

protected func Initialize()
{
  SetAction("Destroyed");
  Schedule("Smoke(0, 0, 30)", 5, 40, this());
}

/* Aufschlag */

protected func Hit2()
{
  DestroyWreck();
}

protected func Hit()
{
  Sound("Collision*");
}

/* Schaden */

func Damage(int iChange, int iPlr)
{
  SetController(iPlr);
  if(GetDamage() >= 60)
   DestroyWreck();
}

/* Zerstörung */

func DestroyWreck()
{
  //Effekte
  CastParticles("GunSmoke",15,35,-20,0,300,500);
  CastParticles("GunSmoke",15,35,20,0,300,500);
  if(GetEffectData(EFSM_ExplosionEffects) > 1) CastParticles("MetalSplinter",8,200,0,0,100,50,RGB(40,20,20));
  Sound("C4EX_Detonation*.ogg", false, this());
  Sound("StructuralDamage*.ogg", false, this());
  Explode(30);
  return(RemoveObject());
}

func BlastPar(int X, int Y, int Size, int Color)
{
  CreateParticle("Blast", X, Y, 0, 0, Size, Color, this());
}