/*-- Palette --*/

#strict 2

local iStatus;


/* Zerstörung */

func Damage()
{
  if(GetDamage() > 290)
  {
    //Effekte
    if(GetEffectData(EFSM_ExplosionEffects) > 1)
    {
      CastParticles("WoodSplinter",4,100,-20,-15,100,80);
      CastParticles("WoodSplinter",4,100,20,-15,100,80);
    }
    CastSmoke("Smoke3",3,10,-20,0,100,200,RGBa(255,255,255,100),RGBa(255,255,255,130));
    CastSmoke("Smoke3",4,10,0,0,150,200,RGBa(255,255,255,100),RGBa(255,255,255,130));
    CastSmoke("Smoke3",3,10,20,0,100,200,RGBa(255,255,255,100),RGBa(255,255,255,130));
    Sound("CrateDestruct*.ogg");

    //Verschwinden
    RemoveObject();
  }
  else
  if(GetDamage() > 240 && iStatus < 4)
  {
    //Aussehen verändern
    SetGraphics("5");

    //Effekte
    if(GetEffectData(EFSM_ExplosionEffects) > 1) CastParticles("MetalSplinter",4,150,0,-15,140,70,RGB(0,250,0));
    CastSmoke("Smoke3",4,13,0,-10,250,100,RGBa(255,255,255,100),RGBa(255,255,255,130));
    Sound("BarrelImpact*.ogg");

    iStatus = 4;
    return(1);
  }
  else
  if(GetDamage() > 180 && iStatus < 3)
  {
    //Aussehen verändern
    SetGraphics("4");

    //Effekte
    if(GetEffectData(EFSM_ExplosionEffects) > 1) CastParticles("WoodSplinter",4,100,-22,-12,100,80);
    CastSmoke("Smoke3",3,10,-20,-10,100,200,RGBa(255,255,255,100),RGBa(255,255,255,130));
    Sound("WoodCrack*.ogg");

    iStatus = 3;
    return(1);
  }
  else
  if(GetDamage() > 120 && iStatus < 2)
  {
    //Aussehen verändern
    SetGraphics("3");

    //Effekte
    if(GetEffectData(EFSM_ExplosionEffects) > 1) CastParticles("MetalSplinter",4,100,-10,-25,80,50);
    CastSmoke("Smoke3",3,10,-10,-20,100,200,RGBa(255,255,255,100),RGBa(255,255,255,130));
    Sound("MetalHit*");

    iStatus = 2;
    return(1);
  }
  else
  if(GetDamage() > 60 && iStatus < 1)
  {
    //Aussehen verändern
    SetGraphics("2");

    //Effekte
    if(GetEffectData(EFSM_ExplosionEffects) > 1) CastParticles("WoodSplinter",4,100,20,-15,100,80);
    CastSmoke("Smoke3",3,10,20,-10,100,200,RGBa(255,255,255,100),RGBa(255,255,255,130));
    Sound("CrateDestruct*.ogg");

    iStatus = 1;
    return(1);
  }
}