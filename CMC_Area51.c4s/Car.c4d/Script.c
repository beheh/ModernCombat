/*-- Auto --*/

#strict 2

local damaged;

public func IsCraneGrabable()		{return !damaged;}
public func IgnoreFriendlyFire()	{return 1;}


/* Initialisierung */

public func Initialize()
{
  //Zufällige Richtung
  if(Random(2))
    SetAction("Left");
  else
    SetAction("Right");

  //Zufälliges Aussehen
  var rnd = Random(3)+2;
  SetGraphics(Format("%d", rnd));

  //Zufällige Farbe
  var dwRGB = HSL(RandomX(50,38),RandomX(50,255),RandomX(50,160));
  SetColorDw(dwRGB);
}

/* Zerstörung */

func Damage(int iChange, int iPlr)
{
  if(!this)
    return;
  if(damaged)
    return;
  SetController(iPlr);
  if(GetDamage() > 60)
    Incinerate(this, iPlr+1);

  if(!this)
    return;
  if(GetDamage() < 100) return;
  InstaExplode(iPlr);
}

func InstaExplode(int iPlr)
{
  if(damaged) return;
  damaged = true;

  //Effekte
  if(GetEffectData(EFSM_ExplosionEffects) > 0) CastParticles("MetalSplinter", 10+Random(5), 100, 0,0, 60,100,RGB(50,50,50));
  if(GetEffectData(EFSM_ExplosionEffects) > 1) CastParticles("Glas", 5+Random(5), 100, 0,0, 60,100, RGBa(200,200,200), RGBa(200,200,200));
  CastParticles("MetalCrateSplinter", 5+Random(5), 100, 0,0, 60,100,RGB(50,50,50));
  Sound("StructuralDamage*.ogg");
  Sound("Debris*.ogg");

  //Explosion
  BlowUp(iPlr);
}

func BlowUp(int iPlr)
{
  SetAction("Destroyed");
  FakeExplode(40, iPlr+1);
  Extinguish();
  SetSolidMask();
  AddFireEffect(this,50,RGB(80,80,80),true,30);

  //Nach oben schleudern
  SetRDir(RandomX(-10,10));
  SetXDir(Sin(GetR(),30));
  SetYDir(-Cos(GetR(),30));

  FadeOut();
}

/* Schaden */

public func OnDmg(int iDmg, int iType)
{
  if(iType == DMG_Bio)		return 100;	//Säure und biologische Schadstoffe
}

/* Aufschlag */ 

protected func Hit3()
{
  DoDamage(40);
}

protected func Hit()
{
  Sound("HeavyHit*.ogg");
  return 1;
}