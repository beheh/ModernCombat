/*-- Explosivgranate --*/

#strict 2
#appendto ESHL

local iAttachment;

func Launch(int xdir, int ydir, int iDmg,a,b,c, int attachment)
{
  iAttachment = attachment;
  SetCategory(C4D_Vehicle);
  active = true;
  sx = GetX();
  sy = GetY();
  start = FrameCounter();
  inherited(xdir,ydir,iDmg,a,b,c);
  if(Stuck()) Hit();
}

func HitObject(object pObj)
{
  if(Secure())
  {
    if(pObj)
    {
      DoDmg(20, DMG_Projectile, pObj, 0, 0, 0, iAttachment);

      if(GetEffectData(EFSM_ExplosionEffects) > 0) CastSmoke("Smoke3",12, 10, 0, 0, 100, 200, RGBa(255,255,255,100), RGBa(255,255,255,130));
      if(GetOCF(pObj) & OCF_Living)
      {
        Sound("SharpnelImpact*.ogg");
      }
      else
      {
        Sound("MetalHit*.ogg");
        Sparks(30,RGB(255,128));
      }
      RemoveObject();
      return;
    }
    else
    {
      Sound("GrenadeHit*.ogg");
      Sparks(30,RGB(255,128));
      if(GetEffectData(EFSM_ExplosionEffects) > 0) CastSmoke("Smoke3",12, 10, 0, 0, 100, 200, RGBa(255,255,255,100), RGBa(255,255,255,130));
      RemoveObject();
      return;
    }
  }
  if(Hostile(iLastAttacker, GetController()) && GetID() != ERND)
    //Punkte bei Belohnungssystem (Granatenabwehr)
    DoPlayerPoints(BonusPoints("Protection"), RWDS_TeamPoints, iLastAttacker, GetCursor(iLastAttacker), IC16);

  Trigger();
}

func Trigger(object pObj)
{
  Explode(BlastRadius()*2/3);
  DamageObjects(BlastRadius()*3/2,BlastRadius()/2,this, 0, 0, iAttachment);
  CreateParticle("Blast",0,0,0,0,10*BlastRadius(),RGB(255,255,128));
  Sound("ShellExplosion*.ogg");
}
