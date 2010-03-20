/*-- Feldgranate --*/

#strict 2
#include NADE

public func Color()		{return RGB(255,0,0);}
public func ContainedDamage()	{return 120;}
public func BlastRadius()	{return 40;}

/* Explosion */

public func Fused()
{
  var helper = CreateObject(TIM1,0,0,-1);
  AddEffect("IntShockWave",helper,10,1,0,GetID()); 

  //Schaden
  Explode(BlastRadius()*2/3);
  DamageObjects(BlastRadius()*3/2,BlastRadius()*2,this());

  //Effekte
  CreateParticle("Blast",0,0,0,0,10*BlastRadius(),RGB(255,255,128));
  CastParticles("MetalSplinter",8,200,0,0,45,20,RGB(40,20,20));
  Sound("GrenadeExplosion*.ogg");
}

/* Schockwelle */

public func FxIntShockWaveStart(object pTarget, int iEffectNumber, int iTemp)
{
  //...
}

public func FxIntShockWaveTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
  pTarget->CreateParticle("ShockWave",0,0,0,0,iEffectTime*(10*(FGRN->BlastRadius()*3/2))/10,RGB(255,255,128));
  if(iEffectTime >= 10) return -1;
}

public func FxIntShockWaveStop(object pTarget, int iEffectNumber, int iReason, bool fTemp)
{
  RemoveObject(pTarget);
}
