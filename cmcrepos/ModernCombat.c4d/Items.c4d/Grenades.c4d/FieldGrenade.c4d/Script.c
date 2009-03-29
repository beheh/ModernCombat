/*-- Feldgranate --*/

#strict
#include NADE

public func Color(){return(RGB(255,0,0));}
public func ContainedDamage(){return(120);}
public func BlastRadius(){return(40);}

public func Fused()
{
  //Kaboom!
  var helper = CreateObject(TIM1,0,0,-1);
  AddEffect("IntShockWave",helper,10,1,0,GetID()); 
  
  Explode(BlastRadius()*2/3);
  DamageObjects(BlastRadius()*3/2,BlastRadius()*2,this());
  CreateParticle("Blast",0,0,0,0,10*BlastRadius(),RGB(255,255,128));//FakeBlast :°
  Sound("GrenadeExplosion*");
}

//Schockwelle. ;)
public func FxIntShockWaveStart(object pTarget, int iEffectNumber, int iTemp)
{
  //...
}

public func FxIntShockWaveTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
  pTarget->CreateParticle("ShockWave",0,0,0,0,iEffectTime*(10*(FGRN->BlastRadius()*3/2))/10,RGB(255,255,128));
  if(iEffectTime >= 10) return(-1);
}

public func FxIntShockWaveStop(object pTarget, int iEffectNumber, int iReason, bool fTemp)
{
  RemoveObject(pTarget);
}