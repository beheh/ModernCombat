/*-- Automat --*/

#strict 2

local damaged;


/* Initialisierung */

protected func Initialize()
{
  //Soundeffekt
  Sound("FreezerLoop", 0,0,0,0, +1);

  //Aktionsschleife an zufälliger Stelle starten
  SetAction("Glow");
  SetPhase(RandomX(0,23));
}

/* Aussehen festlegen */

public func Set(int i)
{
  SetGraphics(Format("%d", i));
}

/* Zerstörung */

func Damage()
{
  if(damaged) return ;
  if(GetDamage() > 10)
  {
    //Effekte
    Sound("CokeCrash");
    if(GetEffectData(EFSM_ExplosionEffects) > 0) CastSmoke("Smoke3",12,10,0,0,100,200,RGBa(255,255,255,100),RGBa(255,255,255,130));
    Schedule(Format("CreateParticle(\"Tin\", -5, 12, RandomX(-10, 10), -1, 30, %d);", RGB(255, 25, 25)), 2, 10, this());
  }
  if(GetDamage() > 80)
  {
    damaged = true;

    //Effekte
    Sound("FreezerLoop", 0,0,0,0, -1);
    Sound("StructureHit*.ogg");
    Schedule("CastObjects(SPRK, 1, 15, RandomX(-10, 10), RandomX(-15, 15));", 3, 15, this());
    if(GetEffectData(EFSM_ExplosionEffects) > 1) CastParticles("MetalSplinter", 10, 100, 0,0, 80);
    AddFireEffect(this,100,RGB(80,80,80),true,30);
    SetAction("Destroyed");
  }
}

/* Schaden */

public func OnDmg(int iDmg, int iType)
{
  if(iType == DMG_Bio)		return 100;	//Säure und biologische Schadstoffe
}