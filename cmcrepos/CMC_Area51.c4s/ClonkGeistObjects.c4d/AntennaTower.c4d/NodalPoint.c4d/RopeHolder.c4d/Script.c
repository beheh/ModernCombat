/*-- Seilhalterung --*/

#strict 2


/* Initalisierung */

protected func Initialize()
{
  SetAction("Stand");
}

public func NodeDestroyed(object pRopeAttachment)
{
  //Grafik setzen
  SetGraphics("Destroyed");

  //Effekte
  if(GetEffectData(EFSM_ExplosionEffects) > 1) CastParticles("MetalSplinter",4,100,0,0,20,70,RGB(250,0,0));
}

public func SetGraphic(int iGraphic)
{
  SetPhase(iGraphic);
}
