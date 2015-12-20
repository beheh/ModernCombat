/*-- Phosphorgranate --*/

#strict 2
#include ESHL

public func BlastRadius()	{return 5;}		//Explosionsradius
protected func SecureTime()	{return 4;}		//Sicherungszeit
func ExplodeDelay()		{return 4;}		//Verzögerung bis zu automatischer Zündung

protected func Secure()
{
  if(!active)
    return true;

  if(FrameCounter() <= start+SecureTime())
    return true;

  return false;
}

func HitObject(object pObj, fNoSecure)
{
  if(!fNoSecure && Secure())
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
        Sound("BulletHitMetal*.ogg");
        Sparks(30,RGB(255,128));
      }
      Trigger();
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
	//Rauch erzeugen
  //CastObjects(BGSE,3,10);

  for(var i = 0; i < 3; i++)
  { var rand = Random(10) - 5;
  	var flame = CreateObject(BGSE, 0,0, GetController(this));
  	var xdir = GetXDir(this)/10 + rand + i*4 - i*2;
  	var ydir = GetYDir(this)/10 - rand + i*4 - i*2;


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
