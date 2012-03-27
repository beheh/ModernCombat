/*-- HUB Station --*/

#strict

public func IsMachine()	{return(1);}


/* Timer */

protected func Activity(pClonk)
{
  //Blinkeffekt
  if(Random(2)) 
  {CreateParticle("NoGravSpark", 3, 1, 0, 0, 50, RGBa(250, 10, 10, 50));
   CreateParticle("NoGravSpark", -10, 1, 0, 0, 50, RGBa(250, 10, 10, 50));}
  else
  {CreateParticle("NoGravSpark", 3, 3, 0, 0, 50, RGBa(10, 10, 250, 50));
   CreateParticle("NoGravSpark", -10, 3, 0, 0, 50, RGBa(10, 10, 250, 50));}
}

/* Schaden */

public func OnDestruction()
{
  //Effekte
  CreateParticle("Blast",0,-10,-10,0,5*50,RGB(55,55,255));
  CreateParticle("Blast",0,-10,10,0,5*50,RGB(55,55,255));
  if(GetEffectData(EFSM_ExplosionEffects) > 0) CastParticles("Smoke3",15,15,0,-10,100,200,RGBa(50,50,255,100));
  if(GetEffectData(EFSM_ExplosionEffects) > 1) CastParticles("MetalSplinter", 15+Random(5), 100, 0,0, 35,50,RGB(40,20,20));
  if(GetEffectData(EFSM_ExplosionEffects) > 1) CastParticles("Glas", 10+Random(5), 100, 0,0, 60,100, RGBa(200,200,200), RGBa(200,200,200));
  Sound("StructuralDamage*.ogg");
}

public func OnDmg(int iDmg, int iType)
{
  if(iType == DMG_Fire)		return(60);	//Feuer
  if(iType == DMG_Explosion)	return(0);	//Explosionen und Druckwellen
  if(iType == DMG_Bio)		return(100);	//Säure und biologische Schadstoffe
  return(80);
}