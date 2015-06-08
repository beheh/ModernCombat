/*-- Schild --*/

#strict 2

local iStatus, pLight;


/* Intialisierung */

func Initialize()
{
  SetAction("Sign1");
  SetMode();
}

/* Einstellungen */

func SetMode(int iMode)
{
  if(iMode == 1)
  {
    SetCategory(C4D_StaticBack);
    SetGraphics("Wall",0,GetID(),1,GFXOV_MODE_Base,0);
  }
  else
  {
    SetCategory(C4D_Structure);
    SetGraphics("Stand",0,GetID(),1,GFXOV_MODE_Base,0);
  }

  if(iStatus > 0)
    SetPhase(iStatus);
}

func Light()
{
  if(pLight)
  {
    if(pLight) RemoveObject(pLight);
  }
  else
  {
    pLight = AddLightCone(3000,RGBa(255,255,255,70),this());
    pLight -> ChangeOffset(15,-80);
    pLight -> ChangeR(180);
    pLight -> TurnOn();
  }
}

/* Zerstörung */

func Damage()
{
  if(GetDamage() > 300)
  {
    //Effekte
    if(GetEffectData(EFSM_ExplosionEffects) > 1)
    {
      CastParticles("WoodSplinter",4,100,-30,-30,100,80);
      CastParticles("WoodSplinter",4,100,-30,30,100,80);
      CastParticles("WoodSplinter",4,100,30,-30,100,80);
      CastParticles("WoodSplinter",4,100,30,30,100,80);
      CastParticles("FencePart",4,100,-30,-30,100,80);
      CastParticles("FencePart",4,100,30,-30,100,80);
      CastParticles("FencePart",2,50,-30,30,80,150);
      CastParticles("FencePart",2,50,30,30,80,150);
    }
    CastSmoke("Smoke3",8,30,0,0,600,200,RGBa(255,255,255,100),RGBa(255,255,255,130));
    CastSmoke("Smoke3",5,10,-30,-30,300,200,RGBa(255,255,255,100),RGBa(255,255,255,130));
    CastSmoke("Smoke3",5,10,-30,30,300,200,RGBa(255,255,255,100),RGBa(255,255,255,130));
    CastSmoke("Smoke3",5,10,30,-30,300,200,RGBa(255,255,255,100),RGBa(255,255,255,130));
    CastSmoke("Smoke3",5,10,30,30,300,200,RGBa(255,255,255,100),RGBa(255,255,255,130));
    Sound("WoodCrack*.ogg");
    Sound("FenceDestruct.ogg");

    //Verschwinden
    RemoveObject();
  }
  else
  if(GetDamage() > 250 && iStatus < 5)
  {
    //Effekte
    if(GetEffectData(EFSM_ExplosionEffects) > 1) CastParticles("WoodSplinter",4,100,-10,20,100,80);
    CastSmoke("Smoke3",5,10,-10,20,300,200,RGBa(255,255,255,100),RGBa(255,255,255,130));
    Sound("WoodCrack*.ogg");

    //Aussehen verändern
    SetPhase(5);

    iStatus = 5;
    return(1);
  }
  else
  if(GetDamage() > 200 && iStatus < 4)
  {
    //Effekte
    if(GetEffectData(EFSM_ExplosionEffects) > 1) CastParticles("WoodSplinter",4,100,30,20,100,80);
    CastSmoke("Smoke3",5,10,30,20,300,200,RGBa(255,255,255,100),RGBa(255,255,255,130));
    Sound("WoodCrack*.ogg");

    //Aussehen verändern
    SetPhase(4);

    iStatus = 4;
    return(1);
  }
  else
  if(GetDamage() > 150 && iStatus < 3)
  {
    //Effekte
    if(GetEffectData(EFSM_ExplosionEffects) > 1) CastParticles("WoodSplinter",4,100,-30,-30,100,80);
    CastSmoke("Smoke3",5,10,-20,-20,300,200,RGBa(255,255,255,100),RGBa(255,255,255,130));
    Sound("WoodCrack*.ogg");

    //Aussehen verändern
    SetPhase(3);

    iStatus = 3;
    return(1);
  }
  else
  if(GetDamage() > 100 && iStatus < 2)
  {
    //Effekte
    if(GetEffectData(EFSM_ExplosionEffects) > 1) CastParticles("WoodSplinter",4,100,-30,15,100,80);
    CastSmoke("Smoke3",5,10,-20,20,300,200,RGBa(255,255,255,100),RGBa(255,255,255,130));
    Sound("WoodCrack*.ogg");

    //Aussehen verändern
    SetPhase(2);

    iStatus = 2;
    return(1);
  }
  else
  if(GetDamage() > 50 && iStatus < 1)
  {
    //Effekte
    if(GetEffectData(EFSM_ExplosionEffects) > 1) CastParticles("WoodSplinter",4,100,30,-30,100,80);
    CastSmoke("Smoke3",5,10,20,-20,300,200,RGBa(255,255,255,100),RGBa(255,255,255,130));
    Sound("WoodCrack*.ogg");

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