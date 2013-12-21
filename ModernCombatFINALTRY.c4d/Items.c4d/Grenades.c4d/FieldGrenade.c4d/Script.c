/*-- Feldgranate --*/

#strict 2
#include NADE

public func Color()		{return RGB(255,0,0);}	//Farbe
public func ContainedDamage()	{return 120;}		//Schaden bei Detonation innerhalb eines Objekts
public func BlastRadius()	{return 40;}		//Explosionsradius


/* Aktivierung */

public func Fused(object pContainer)
{
  var helper = CreateObject(TIM1,0,0,-1);
  AddEffect("IntShockWave",helper,10,1,0,GetID()); 

  //Schaden
  if(!GetContact(this, -1) && !pContainer)
    for(var obj in FindObjects(Find_Distance(BlastRadius()*3/2), Find_Func("IsClonk")))
      AddEffect("Ribbon_TheBugfixer", obj, 1, 10, 0, GetID(), GetController());

  Explode(BlastRadius()*2/3);
  DamageObjects(BlastRadius()*3/2,BlastRadius()*2,this);

  //Effekte
  CreateParticle("Blast",0,0,0,0,10*BlastRadius(),RGB(255,255,128));
  if(GetEffectData(EFSM_ExplosionEffects) > 1) CastParticles("MetalSplinter",8,200,0,0,45,20,RGB(40,20,20));
  Sound("GrenadeExplosion*.ogg");
}

public func FxRibbon_TheBugfixerStart(object pTarget, int iNr, int iTmep, int iKiller)
{
  EffectVar(0, pTarget, iNr) = iKiller;
}

/* Schockwelle */

public func FxIntShockWaveStart(object pTarget, int iEffectNumber, int iTemp)
{}

public func FxIntShockWaveTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
  pTarget->CreateParticle("ShockWave",0,0,0,0,iEffectTime*(10*(FGRN->BlastRadius()*3/2))/10,RGB(255,255,128));
  if(iEffectTime >= 10) return -1;
}

public func FxIntShockWaveStop(object pTarget, int iEffectNumber, int iReason, bool fTemp)
{
  RemoveObject(pTarget);
}
