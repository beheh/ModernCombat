/*-- Satellitenschüssel --*/

#strict 2

local damaged;


/* Zerstörung */

func Damage(int iChange, int iPlr)
{
  if(!this)
    return;
  SetController(iPlr);
  if(GetDamage() < 150) return;
  InstaExplode(iPlr);
}

func InstaExplode(int iPlr)
{
  if(damaged) return ;
  damaged = true;

  //Effekte
  Sound("StructuralDamage*.ogg");
  if(GetEffectData(EFSM_ExplosionEffects) > 1) CastParticles("MetalSplinter",5,40,-10,0,0,200,20);

  //Explosion
  SetAction("Wreck");
  SetController(iPlr);
  FakeExplode(30, iPlr+1);

  SetRDir(RandomX(-40,+40));
  AddFireEffect(this,50,RGB(80,80,80),true,30);
}

/* Schaden */

public func OnDmg(int iDmg, int iType)
{
  if(iType == DMG_Bio)		return 100;	//Säure und biologische Schadstoffe
}