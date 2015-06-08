/*-- Zaun --*/

#strict 2

local damaged;


/* Zerstörung */

func Damage(int iChange)
{
  if(damaged) return ;
  if(GetDamage() < 70) return;
  Destruct();
}

func Destruct()
{
  if(damaged) return ;
  damaged = true;

  //Effekte
  Sound("FenceDestruct.ogg");
  if(GetEffectData(EFSM_ExplosionEffects) > 1)
  {
    CastParticles("FencePart",1,80,-20,-10,80,150);
    CastParticles("FencePart",1,10,-10,-10,80,150);
    CastParticles("FencePart",1,10,10,-10,80,150);
    CastParticles("FencePart",1,80,20,-10,80,150);
    CastParticles("FencePart",1,40,-20,10,80,150);
    CastParticles("FencePart",1,10,-10,10,80,150);
    CastParticles("FencePart",1,10,10,10,80,150);
    CastParticles("FencePart",1,40,20,10,80,150);
    CastParticles("MetalSplinter",4,100,0,0,20,70);
  }

  SetGraphics("Destroyed");
}

/* Aussehen festlegen */

public func Set(int i)
{
  SetGraphics(Format("%d", i));
}

/* Schaden */

public func OnDmg(int iDmg, int iType)
{
  if(iType == DMG_Bio)		return 100;	//Säure und biologische Schadstoffe
}