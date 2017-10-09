/*-- Rauchgranate --*/

#strict 2
#include NADE

local time;

public func Color()		{return RGB(128,255,0);}	//Farbe
public func FuseTime()		{return 2*35;}			//Zeit bis zur Zündung
public func ContainedDamage()	{return 20;}			//Schaden bei Detonation innerhalb eines Objekts

public func IsFadingOut()	{return time == 10;}


/* Aktivierung */

public func Fuse()
{
  Sound("SGRN_Activate.ogg");
  SetGraphics("Active");

  return AddEffect("IntFuse",this,200,1,this);
}

public func Fused()
{
  //Rauch erzeugen
  ScheduleCall(this, "Smoke", 3, 10);

  //Effekte
  CastSmoke("Smoke3",10,50,0,0,120,140,RGBa(255,255,255,100), RGBa(255,255,255,130));
  Sound("SGRN_Fused.ogg");

  //Verschwinden
  if(OnFire())
    RemoveObject();
  else
    FadeOut();
}

/* Rauchen */

func Smoke()
{
  CastSmoke("Smoke3",2,10,0,0,80,120);

  var smoke = CreateObject(SM4K, 0, 0, GetOwner(GetUser()));
  SetXDir((GetXDir() / 3 * 2) + (time * RandomX(-30, +30)),smoke,100);
  SetYDir((GetYDir() / 3 * 2) - (time * (10 + Random(20))),smoke,100);

  time++;
}

/* Schaden */

protected func Damage(int iChange) 
{
  //Kein Schaden nehmen wenn gehalten und eventuelles Feuer löschen
  if(Contained())
  {
    if(OnFire())
    {
      Extinguish();
      return;
    }
    else
    return;
  }

  //Ansonsten Zündung durch Schaden
  if(GetDamage() < MaxDamage() || activated) return;

  //Effekte
  if(GetEffectData(EFSM_ExplosionEffects) > 0)
  {
    CastParticles("BlastDirt",2,10,0,0,400,100);
    CastParticles("BlastFlame",2,10,0,0,150,100);
  }
  Sparks(5,RGB(250,100));
  Sound("SGRN_Fused.ogg");

  //Rauch erzeugen
  Smoke();

  //Verschwinden
  RemoveObject();
}

/* Aufprall */

func HitSound()
{
  Sound("SGRN_Hit*.ogg");
}