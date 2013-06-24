/*-- Satellitenschüssel --*/

#strict 2

local damaged;


/* Zerstörung */

func Damage()
{
  if(damaged) return ;
  if(GetDamage() < 150) return;
  InstaExplode();
}

func InstaExplode()
{
  if(damaged) return ;
  damaged = true;

  //Effekte
  if(GetEffectData(EFSM_ExplosionEffects) > 1)
  {
    CastParticles("MetalSplinter",5,40,-10,0,0,200,20);
    CastParticles("MetalSplinter",4,100,0,-15,140,70);
  }
  CastSmoke("Smoke3",8,15,0,-5,250,200,RGBa(255,255,255,100),RGBa(255,255,255,130));
  AddFireEffect(this,50,RGB(80,80,80),true,30);
  Sound("StructureHit*.ogg");

  SetGraphics("Destroyed");
}

/* Schaden */

public func OnDmg(int iDmg, int iType)
{
  if(iType == DMG_Bio)		return 100;	//Säure und biologische Schadstoffe
}