/*-- Satellitensch�ssel --*/

#strict 2

local damaged;


/* Zerst�rung */

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
  Sound("StructuralDamage*.ogg");
  if(GetEffectData(EFSM_ExplosionEffects) > 0) CastSmoke("Smoke3",8,15,0,-5,250,200,RGBa(255,255,255,100),RGBa(255,255,255,130));
  if(GetEffectData(EFSM_ExplosionEffects) > 1)
  {
    CastParticles("MetalSplinter",5,40,-10,0,0,200,20);
    CastParticles("MetalSplinter",4,100,0,-15,140,70);
  }

  SetGraphics("Destroyed");
  AddFireEffect(this,50,RGB(80,80,80),true,30);
}

/* Schaden */

public func OnDmg(int iDmg, int iType)
{
  if(iType == DMG_Bio)		return 100;	//S�ure und biologische Schadstoffe
}