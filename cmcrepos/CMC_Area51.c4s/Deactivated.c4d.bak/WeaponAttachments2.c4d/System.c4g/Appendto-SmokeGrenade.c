/*-- Rauchgranate --*/

#strict 2
#appendto SSHL

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
      Sound("MetalHit*.ogg");
      Sparks(30,RGB(255,128));
    }
  }

  if(Hostile(iLastAttacker, GetController()))
    //Punkte bei Belohnungssystem (Granatenabwehr)
    DoPlayerPoints(BonusPoints("Protection"), RWDS_TeamPoints, iLastAttacker, GetCursor(iLastAttacker), IC16);

  Trigger();
}
