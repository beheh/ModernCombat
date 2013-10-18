/*-- Phosphorgranate --*/

#strict 2
#include NADE

public func Color()		{return RGB(235,199,54);}	//Farbe
public func ContainedDamage()	{return 100;}			//Schaden bei Detonation innerhalb eines Objekts
public func BlastRadius()	{return 10;}			//Explosionsradius


/* Aktivierung */

public func Fuse()
{
  Sound("SGRN_Activate.ogg");
  SetGraphics("Active");
  return AddEffect ("IntFuse",this,200,1,this);
}

public func Fused()
{
  var helper = CreateObject(TIM1,0,0,-1);
  AddEffect("IntShockWave",helper,10,1,0,GetID()); 

  //Schaden
  Explode(BlastRadius()*2/3);
  DamageObjects(BlastRadius()*3/2,BlastRadius()*2,this);
  for(var i = 0; i < 8; i++)
  {
    var pPhosphor = CreateObject(PSPR, 0, 0, GetOwner());
    SetController(GetController(), pPhosphor);
    SetXDir(Random(2*50+1)-50, pPhosphor);
    SetYDir(Random(2*50+1)-50, pPhosphor);
  }

  //Effekte
  CreateParticle("Blast",0,0,0,0,10*BlastRadius(),RGB(20,128,255));

  if(GetEffectData(EFSM_ExplosionEffects) > 0) CastSmoke("Smoke3",50,20,0,0,100,200,RGBa(255,255,255,100),RGBa(255,0,255,130));
  if(GetEffectData(EFSM_ExplosionEffects) > 1) CastParticles("MetalSplinter",8,200,0,0,45,20,RGB(40,20,20));
  Sound("PhosphorExplosion.ogg");
  Sound("GrenadeExplosion*.ogg");
}

/* Schockwelle */

public func FxIntShockWaveStart(object pTarget, int iEffectNumber, int iTemp)
{}

public func FxIntShockWaveTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
  pTarget->CreateParticle("ShockWave",0,0,0,0,iEffectTime*(10*(PGRN->BlastRadius()*3/2))/10,RGB(255,255,128));
  if(iEffectTime >= 10) return -1;
}

public func FxIntShockWaveStop(object pTarget, int iEffectNumber, int iReason, bool fTemp)
{
  RemoveObject(pTarget);
}

/* Aufprall */

func HitSound()
{
  Sound("SGRN_Hit*.ogg");
}