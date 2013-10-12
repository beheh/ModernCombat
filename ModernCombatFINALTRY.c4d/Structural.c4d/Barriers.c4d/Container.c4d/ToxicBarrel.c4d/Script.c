/*-- Giftfass --*/

#strict 2
#include XBRL


/* Zerstörung */

func Damage(int iChange, int iPlr)
{
  if(GetDamage() < 50) return ;
  InstaExplode(iPlr);
}

func BlowUp(int iPlr)
{
  //Achievement-Fortschritt (Barrel Roll)
  DoAchievementProgress(1, AC47, iPlr);

  //Effekte
  if(GetEffectData(EFSM_ExplosionEffects) > 1) CastParticles("MetalSplinter",4,100,0,0,20,70,RGB(0,250,0));
  Sound("SGRN_Fused.ogg");
  Sound("Crackle.ogg");

  SetAction("Wreck");
  SetController(iPlr);
  Extinguish();
  SetRDir(RandomX(-40,+40));

  AddEffect("GasEffect", this, 1, 1, 0, GetID());
}

global func FxGasEffectTimer(object target, int effect, int time)
{
  SetClrModulation(RGBa(255,255,255,time*255/175), target);

  if(!(time % 8))
  {
    var flame = target->CreateObject(FFLM,0,0,GetOwner());
    flame->SetController(GetController(target));
    flame->Launch(10-Random(20),10-Random(20),true,30-(time*30/175));
  }

  target->CreateParticle("FrSprk",0,0,15-Random(30),15-Random(30),200, RGBa(160,255,0,time*255/175));

  if(time >= 175)
    RemoveObject(target);
}

/* Aufprall */

protected func Hit3()
{
  DoDamage(50);
}