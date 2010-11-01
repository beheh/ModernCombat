/*-- Zaun --*/

#strict 2


/* Zerst�rung */

func Damage(int iChange)
{
  if(!this)
    return;
  if(GetDamage() < 70) return;
  Destruct();
}

func Destruct()
{
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

  //Entfernung
  RemoveObject();
}

/* Aussehen festlegen */

public func Set(int i)
{
  SetGraphics(Format("%d", i));
}

/* Schaden */

public func OnDmg(int iDmg, int iType)
{
  if(iType == DMG_Bio)		return 100;	//S�ure und biologische Schadstoffe
}