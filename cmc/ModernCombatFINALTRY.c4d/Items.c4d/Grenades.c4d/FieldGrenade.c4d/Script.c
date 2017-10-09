/*-- Feldgranate --*/

#strict 2
#include NADE

public func Color()		{return RGB(255,0,0);}	//Farbe
public func ContainedDamage()	{return 120;}		//Schaden bei Detonation innerhalb eines Objekts
public func BlastRadius()	{return 40;}		//Explosionsradius


/* Aktivierung */

public func Fused(object pContainer)
{
  //Effekt für Ehrenbandvergabe
  if(!GetContact(this, -1) && !pContainer)
    for(var obj in FindObjects(Find_Distance(BlastRadius()*3/2), Find_Func("IsClonk")))
      AddEffect("Ribbon_TheBugfixer", obj, 1, 10, 0, GetID(), GetController());

  //Explosion und Zusatzschaden
  Explode(BlastRadius()*2/3);
  DamageObjects(BlastRadius()*3/2,BlastRadius()*2,this);

  //Effekte
  if(GetEffectData(EFSM_ExplosionEffects) > 1) CastParticles("MetalSplinter",10,150,0,0,25,50);
  CreateParticle("ShockWave",0,0,0,0,12*BlastRadius(),RGB(255,255,55));
  CreateParticle("Blast",0,0,0,0,10*BlastRadius(),RGB(255,255,128));
  Sound("GrenadeExplosion*.ogg");
}

public func FxRibbon_TheBugfixerStart(object pTarget, int iNr, int iTmep, int iKiller)
{
  EffectVar(0, pTarget, iNr) = iKiller;
}