/*-- Rauchgranate --*/

#strict 2
#include ESHL

public func Color()		{return RGB(255,255,255);}	//Kennfarbe
protected func SecureDistance()	{}				//Keine Sicherungsreichweite
func ExplodeDelay()		{return 120;}			//Verzögerung bis zu automatischer Zündung


/* Treffer */

func HitObject(object pObj)
{
  if(pObj)
  {
    DoDmg(30,DMG_Projectile,pObj, 0, 0, 0, iAttachment);
    if(Hostile(GetOwner(), GetOwner(pObj)))
      if(pObj->~IsClonk() && (!GetAlive(pObj) || IsFakeDeath(pObj)))
        //Achievement-Fortschritt (Smoke 'em up)
        DoAchievementProgress(1, AC26, GetController());

    if(GetEffectData(EFSM_ExplosionEffects) > 0) CastSmoke("Smoke3",12, 10, 0, 0, 130, 160, RGBa(255,255,255,100), RGBa(255,255,255,130));
    if(GetOCF(pObj) & OCF_Living)
    {
      Sound("SharpnelImpact*.ogg");
    }
    else
    {
      Sound("BulletHitMetal*.ogg");
      Sparks(30,RGB(255,128));
    }
  }

  if(Hostile(iLastAttacker, GetController()))
    //Punkte bei Belohnungssystem (Granatenabwehr)
    DoPlayerPoints(BonusPoints("Protection"), RWDS_TeamPoints, iLastAttacker, GetCursor(iLastAttacker), IC16);

  Trigger();
}

func Trigger()
{
  //Effekte
  CreateParticle("Blast",0,0,0,0,5*10,RGB(255,255,128));
  if(GetEffectData(EFSM_ExplosionEffects) > 0) CastSmoke("Smoke3",8, 50, 0, 0, 120, 140, RGBa(255,255,255,100), RGBa(255,255,255,130));
  Sound("SmokeShellExplosion*.ogg");

  //Rauch erzeugen
  CastObjects(SM4K,3,10);

  //Verschwinden
  RemoveObject();
}