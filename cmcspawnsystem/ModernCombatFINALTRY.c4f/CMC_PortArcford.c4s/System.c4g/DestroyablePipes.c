/*-- Rohr --*/

//Rohre sind zerstörbar.

#strict 2
#appendto PIPL

local damaged;


/* Initialisierung */

public func Initialize()
{
  SetAction("Pipe");
  SetClrModulation(RGB(80,80,80));
  efflux = CreateArray();

  if(!GBackLiquid()) noliquid = true;

  //Standardstärke
  power = 1;
}

/* Zerstörung */

func Damage(int iChange, int iPlr)
{
  if(damaged) return ;
  SetController(iPlr);
  if(GetDamage() >= 100)
   BlowUp();
}

func BlowUp(int iPlr)
{
  if(damaged) return ;
  damaged = true;
  SetController(iPlr);

  //Effekte
  if(GetEffectData(EFSM_ExplosionEffects) > 0) CastParticles("MetalCrateSplinter", 15+Random(5), 100, 0,0, 60,100,RGB(250,0,0));
  if(GetEffectData(EFSM_ExplosionEffects) > 1) CastParticles("MetalSplinter", 15+Random(5), 100, 0,0, 35,50,RGB(40,20,20));
  if(GetEffectData(EFSM_ExplosionEffects) > 1) CastParticles("Glas", 10+Random(5), 100, 0,0, 60,100, RGBa(200,200,200), RGBa(200,200,200));
  Sound("StructuralDamage*.ogg");

  //Transparent
  SetClrModulation(RGB(80,80,80));
  SetSolidMask();

  //Explosion
  FakeExplode(25, iPlr+1);
}