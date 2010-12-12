/*-- Rauchgranate --*/

#strict 2
#include ESHL

public func Color()		{return RGB(255,255,255);}
protected func SecureDistance()	{}
func ExplodeDelay()		{return 120;}


/* Treffer */

func HitObject(object pObj)
{
  if(pObj)
  {
    DoDmg(30,DMG_Projectile,pObj);
    if(GetEffectData(EFSM_ExplosionEffects) > 0)
      CastSmoke(150, 10, 12, 0, 0, RGBa(255,255,255,100), RGBa(255,255,255,130));
    if(GetOCF(pObj) & OCF_Living)
    {
      Sound("SharpnelImpact*.ogg");
    }
    else
    {
      Sound("BlockOff*.ogg");
      Sparks(30,RGB(255,128));
    }
  }
  Trigger();
}

func Trigger()
{
  //Effekte
  CreateParticle("Blast",0,0,0,0,5*10,RGB(255,255,128));
  if(GetEffectData(EFSM_ExplosionEffects) > 0)
    CastSmoke(130, 50, 8, 0, 0, RGBa(255,255,255,120), RGBa(255,255,255,150));
  Sound("SmokeShellExplosion*.ogg");

  //Rauch erzeugen
  CastObjects(SM4K,3,10);

  //Verschwinden
  RemoveObject();
}
