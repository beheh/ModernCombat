/*-- Denkmal --*/

#strict 2

local iStatus, damaged;


/* Intialisierung */

func Initialize()
{
  SetAction("Be");
}

/* Zerstörung */

func Damage()
{
  if(damaged) return ;

  if(GetDamage() > 300)
  {
    //Effekte
    if(GetEffectData(EFSM_ExplosionEffects) > 1) CastParticles("ConcreteSplinter",10,100,-20,30,20,100);
    if(GetEffectData(EFSM_ExplosionEffects) > 1) CastParticles("ConcreteSplinter",10,100,20,30,20,100);
    CastSmoke("Smoke3",15,20,-20,30,100,200,RGBa(255,255,255,50),RGBa(255,255,255,130));
    CastSmoke("Smoke3",10,20,20,30,100,200,RGBa(255,255,255,50),RGBa(255,255,255,130));
    Sound("RockBreak*");
    Sound("Blast2");
    Sound("StructureDebris*.ogg");

    //Aussehen verändern
    SetPhase(6);

    damaged = true;
    return(1);
  }
  else
  if(GetDamage() > 250 && iStatus < 5)
  {
    //Effekte
    if(GetEffectData(EFSM_ExplosionEffects) > 1) CastParticles("ConcreteSplinter",10,100,0,-30,20,100);
    if(GetEffectData(EFSM_ExplosionEffects) > 1) CastParticles("ConcreteSplinter",10,100,0,0,20,100);
    CastSmoke("Smoke3",15,20,0,-30,100,200,RGBa(255,255,255,50),RGBa(255,255,255,130));
    CastSmoke("Smoke3",10,20,0,0,100,200,RGBa(255,255,255,50),RGBa(255,255,255,130));
    Sound("RockBreak*");
    Sound("Blast2");
    Sound("StructureDebris*.ogg");

    //Aussehen verändern
    SetPhase(5);

    iStatus = 5;
    return(1);
  }
  else
  if(GetDamage() > 200 && iStatus < 4)
  {
    //Effekte
    if(GetEffectData(EFSM_ExplosionEffects) > 1) CastParticles("ConcreteSplinter",10,100,-20,20,20,100);
    CastSmoke("Smoke3",15,20,-20,20,100,200,RGBa(255,255,255,50),RGBa(255,255,255,130));
    Sound("RockBreak*");
    Sound("Blast2");
    Sound("StructureDebris*.ogg");

    //Aussehen verändern
    SetPhase(4);

    iStatus = 4;
    return(1);
  }
  else
  if(GetDamage() > 150 && iStatus < 3)
  {
    //Effekte
    if(GetEffectData(EFSM_ExplosionEffects) > 1) CastParticles("ConcreteSplinter",10,100,0,-45,20,100);
    CastSmoke("Smoke3",15,20,0,-45,100,200,RGBa(255,255,255,50),RGBa(255,255,255,130));
    Sound("RockBreak*");
    Sound("Blast2");
    Sound("StructureDebris*.ogg");

    //Aussehen verändern
    SetPhase(3);

    iStatus = 3;
    return(1);
  }
  else
  if(GetDamage() > 100 && iStatus < 2)
  {
    //Effekte
    if(GetEffectData(EFSM_ExplosionEffects) > 1) CastParticles("ConcreteSplinter",10,100,20,5,20,100);
    CastSmoke("Smoke3",15,20,20,5,100,200,RGBa(255,255,255,50),RGBa(255,255,255,130));
    Sound("RockBreak*");
    Sound("Blast2");
    Sound("StructureDebris*.ogg");

    //Aussehen verändern
    SetPhase(2);

    iStatus = 2;
    return(1);
  }
  else
  if(GetDamage() > 50 && iStatus < 1)
  {
    //Effekte
    if(GetEffectData(EFSM_ExplosionEffects) > 1) CastParticles("ConcreteSplinter",10,100,-20,-30,20,100);
    if(GetEffectData(EFSM_ExplosionEffects) > 1) CastParticles("ConcreteSplinter",6,100,20,-40,20,100);
    CastSmoke("Smoke3",15,20,-20,-30,100,200,RGBa(255,255,255,50),RGBa(255,255,255,130));
    CastSmoke("Smoke3",10,20,20,-40,100,200,RGBa(255,255,255,50),RGBa(255,255,255,130));
    Sound("RockBreak*");
    Sound("Blast2");
    Sound("StructureDebris*.ogg");

    //Aussehen verändern
    SetPhase(1);

    iStatus = 1;
    return(1);
  }
}

/* Schaden */

public func OnDmg(int iDmg, int iType)
{
  if(iType == DMG_Bio)		return 100;	//Säure und biologische Schadstoffe
}