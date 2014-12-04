/*-- EMP-Bolzen --*/

#strict 2
#include GREN

local active,sx,sy, start;
local iXDir, iYDir;
local iLastAttacker;
local iAttachment;

public func Color()		{return RGB(255,0,0);}
public func BlastRadius()	{return 30;}
protected func SecureDistance()	{return 0;}
public func IgnoreTracer()	{return true;}
public func IsRifleGrenade()	{return true;}		//Ist eine Gewehrgranate
public func AllowHitboxCheck()	{return true;}
public func RejectC4Attach()	{return true;}


/* Initialisierung */

protected func Initialize()
{
  iLastAttacker = NO_OWNER;
  return _inherited();
}

/* Start */

func Launch(int xdir, int ydir, int iDmg,a,b,c, int attachment)
{
  iAttachment = attachment;
  SetCategory(C4D_Vehicle);
  active = true;
  sx = GetX();
  sy = GetY();
  start = FrameCounter();
  iXDir = xdir;
  iYDir = ydir - 7;
  inherited(iXDir,iYDir,iDmg,a,b,c);
  if(Stuck()) Hit();
}

/* Treffer */

func Hit()
{
  HitObject();
}

protected func RejectEntrance(pNewContainer)
{
  return 1;
}

func HitObject(object pObj)
{
  if(pObj)
  {
    DoDmg(40, DMG_Energy, pObj, 0, 0, 0, iAttachment);

    //if(GetEffectData(EFSM_ExplosionEffects) > 0) CastSmoke("Smoke3",12, 10, 0, 0, 100, 200, RGBa(255,255,255,100), RGBa(255,255,255,130));
    if(GetOCF(pObj) & OCF_Living)
    {
      Sound("SharpnelImpact*.ogg");
    }
    else
    {
      Sound("BulletHitMetal*.ogg");
      Sparks(30,RGB(255,128));
    }
    RemoveObject();
    return;
  }
  else
  {
    Sound("GrenadeHit*.ogg");
    Sparks(30,RGB(255,128));
    //if(GetEffectData(EFSM_ExplosionEffects) > 0) CastSmoke("Smoke3",12, 10, 0, 0, 100, 200, RGBa(255,255,255,100), RGBa(255,255,255,130));
    RemoveObject();
    return;
  }

  if(Hostile(iLastAttacker, GetController()) && GetID() != ERND)
    //Punkte bei Belohnungssystem (Granatenabwehr)
    DoPlayerPoints(BonusPoints("Protection"), RWDS_TeamPoints, iLastAttacker, GetCursor(iLastAttacker), IC16);
}

/* Zerstörung */

func IsBulletTarget(id id)
{
  //Kann von anderen Geschossen getroffen werden
  return true;
}

func Damage()
{
  if(GetDamage() > 10)
  {
    //Effekte
    CastParticles("MetalSplinter",3,80,0,0,45,50,RGB(40,20,20));
    Sparks(8,RGB(255,128));
    Sound("BulletHitMetal*.ogg");
  }
}

/* Schaden */

public func OnHit(a, b, pFrom)
{
  iLastAttacker = GetController(pFrom);
  return;
}

public func OnDmg(int iDmg, int iType)
{
  if(iType == DMG_Fire)		return 100;	//Feuer
  if(iType == DMG_Bio)		return 100;	//Säure und biologische Schadstoffe
}
