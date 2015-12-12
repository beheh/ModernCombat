/*-- Phosphorgranate --*/

#strict 2
#include GREN

local active,sx,sy, start;
local iLastAttacker;
local iAttachment;

public func Color()		{return RGB(255,0,0);}	//Kennfarbe
public func BlastRadius()	{return 5;}		//Explosionsradius
protected func SecureTime()	{return 2;}		//Sicherungszeit
func ExplodeDelay()		{return 2;}		//Verzögerung bis zu automatischer Zündung
public func IgnoreTracer()	{return true;}		//Nicht markierbar
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

func Launch(int xdir, int ydir, int iDmg,a,b,c, int attachment, object pUser)
{
  iAttachment = attachment;
  SetCategory(C4D_Vehicle);
  active = true;
  sx = GetX();
  sy = GetY();
  start = FrameCounter();
  var ffx,ffy;
  pUser->~WeaponEnd(ffx,ffy);
  ffx += GetX(pUser);
  ffy += GetY(pUser);
  AddEffect("HitCheck", this, 1, 1, 0, SHT1,pUser,0,ffx,ffy);
  AddEffect("Grenade", this, 1, 1, this);
  SetSpeed(xdir, ydir);  
  if(!iDmg)
    //Standardexplosion
    iDmg = 30;
  if(Stuck()) Hit();
}

protected func Secure()
{
  if(!active)
    return true;

  if(FrameCounter() <= start+SecureTime())
    return true;

  return false;
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

func HitObject(object pObj, fNoSecure)
{
  if(!fNoSecure && Secure())
  {
  	CreateObject(PSPR, 0,0, GetController(this));
  	CreateObject(PSPR, 0,0, GetController(this));
  
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
  for(var i = 0; i < 10; i++)
  { var rand = Random(30) - 15;
  	var flame = CreateObject(PSPR, 0,0, GetController(this));
  	var xdir = GetXDir(this)/2 + rand + i*4 - i*2;
  	var ydir = GetYDir(this)/2 - rand + i*4 - i*2 - 18;


		SetXDir(xdir, flame);
		SetYDir(ydir, flame);
	}
  Sound("ShellExplosion*.ogg");
  Explode(BlastRadius(), this);
}

/* Timer */

func FxGrenadeTimer(object target, int effect, int time)
{
  //Bei aufgebrauchter Flugzeit explodieren
  if(time > ExplodeDelay()) return HitObject(0, true);
  var vel=Abs(GetXDir())+Abs(GetYDir());
  var alpha=Max(0,100-vel);

  //Raucheffekt
  var vel=Abs(GetXDir())+Abs(GetYDir());
  var alpha=Max(0,60-vel);
  var rgb = Color();
  if(!rgb) rgb = RGB(100,100,100);

  if(!GBackLiquid())
    CreateParticle("Smoke2", -GetXDir()/6, -GetYDir()/6, RandomX(-10, 10), -5, vel/3+RandomX(10, 20), SetRGBaValue(rgb,alpha)); 
  else
    CastObjects(FXU1,2,6);

  SetR(Angle (0,0,GetXDir(),GetYDir()));
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

    Trigger();
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
