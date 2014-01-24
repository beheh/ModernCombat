/*-- Säule --*/

#strict 2

local iStatus,iName;


/* Einstellung */

public func Set(string szName)
{
  //Aufruf festlegen
  if(szName)
    iName = szName;
}

/* Zerstörung */

public func Damage(int iChange, int iPlr)
{
  if(GetDamage() > 100)
  {
    //Zusammensturz melden
    if(iName)
      GameCallEx(iName, iPlr);

    //Rauch erzeugen
    CastObjects(SM4K,2,20);

    //Effekte
    if(GetEffectData(EFSM_ExplosionEffects) > 1) CastParticles("ConcreteSplinter",8,110,-20,0,60,100);
    if(GetEffectData(EFSM_ExplosionEffects) > 1) CastParticles("ConcreteSplinter",8,110,20,0,60,100);
    if(GetEffectData(EFSM_ExplosionEffects) > 1) CastParticles("ConcreteSplinter",4,100,0,0,40,15,RGB(40,20,20));
    CastSmoke("Smoke3",10,20,0,-20,220,500);
    CastSmoke("Smoke3",10,20,0,20,220,500);
    Sound("StructureHeavyHit*.ogg");
    Sound("ExplosionHuge.ogg");
    Echo("TowerBreakingEcho.ogg",1);

    //Verschwinden
    RemoveObject();
  }
  else
  if(GetDamage() > 65 && iStatus < 2)
  {
    //Aussehen verändern
    SetGraphics("3");

    //Effekte
    if(GetEffectData(EFSM_ExplosionEffects) > 1) CastParticles("ConcreteSplinter",8,110,0,0,60,100);
    if(GetEffectData(EFSM_ExplosionEffects) > 1) CastParticles("ConcreteSplinter",10,100,0,0,40,15,RGB(40,20,20));
    CastSmoke("Smoke3",15,20,0,0,220,500);
    Sound("StructureHit*.ogg");
    Sound("StructureDebris*.ogg");

    iStatus = 2;
    return(1);
  }
  else
  if(GetDamage() > 30 && iStatus < 1)
  {
    //Aussehen verändern
    SetGraphics("2");

    //Effekte
    if(GetEffectData(EFSM_ExplosionEffects) > 1) CastParticles("ConcreteSplinter",8,110,0,0,60,100);
    if(GetEffectData(EFSM_ExplosionEffects) > 1) CastParticles("ConcreteSplinter",10,100,0,0,40,15,RGB(40,20,20));
    CastSmoke("Smoke3",15,20,0,0,220,500);
    Sound("StructureHit*.ogg");
    Sound("StructureDebris*.ogg");

    iStatus = 1;
    return(1);
  }
}