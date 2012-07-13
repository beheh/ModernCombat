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
  AddFireEffect(this,50,RGB(80,80,80),true,30);
}

public func SetGraphic(int iGraphic)
{
  if(iGraphic == 2)
    SetPhase(2);
  if(iGraphic == 1)
    SetPhase(1);
  if(iGraphic == 0)
    SetPhase(0);
  else
    return;
}