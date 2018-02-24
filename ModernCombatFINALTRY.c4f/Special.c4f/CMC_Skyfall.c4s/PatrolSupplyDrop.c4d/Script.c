/*-- Benzinversorgungsabwurf --*/

#strict 2
#include ABLT


/* Initialisierung */

func Initialize()
{
  fHit = false;
  fAchievement = false;

  //Zufälliges Aussehen
  SetAction(Format("Look%d", RandomX(1,4)));

  return 1;
}

/* Objekte treffen */

func SearchObjects()
{
  if(fHit) return;
  var target = FindObject2(Find_AtPoint(), Find_Exclude(this), Find_Func("IsBulletTarget", GetID(), this, 0, GetX(), GetY()));
  if(target)
    Hit();
}

/* Schaden */

func Hit()
{
  if(fHit) return;

  //Schon getroffen
  fHit = true;

  //Explosion
  Explode(40);

  //Fässer verschleudern
  CastObjects(XBRL,RandomX(2,3),RandomX(50,80)); 

  //Effekte
  if(GetEffectData(EFSM_ExplosionEffects) > 0) CastSmoke("Smoke3",15,10,0,0,300,700, RGBa(255,255,255,0));
  Sound("C4EX_Detonation*.ogg");
}

public func Damage()
{
  if(GetDamage() > 20)
    Hit();
}