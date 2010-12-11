/*-- Langstreckenrakete --*/

#strict 2
#include MISL


public func MaxTime()		{return 400;}			//Maximale Flugzeigt

public func StartSpeed()	{return 20;}			//Startgeschwindigkeit
public func Acceleration()	{return 4;}			//Beschleunigung
public func MaxSpeed()		{return 150;}			//Maximale Geschwindigkeit

public func SecureTime()	{return 50;}			//Mindestflugzeit
public func SecureDistance()	{return 50;}			//Mindestabstand

public func ExplosionDamage()	{return 18;}			//Explosionsschaden

public func MaxTurn()		{return 7;}			//max. Drehung
public func MaxTracerTurn()	{return 9;}			//max. Drehung bei Zielverfolgung

//Weniger direkter Schaden, größerer Flächenschaden
private func HitObject(pObj)
{

  if(Secure() || GetAction() == "Idle")
  {
    if(pObj)
    {
      DoDmg(Distance(GetXDir(),GetYDir())/5,DMG_Projectile,pObj);
      if(GetEffectData(EFSM_ExplosionEffects) > 0) CastParticles("Smoke3",12,10,0,0,100,200,RGBa(255,255,255,100),RGBa(255,255,255,130));
      if(GetOCF(pObj) & OCF_Living)
      {
        Sound("SharpnelImpact*.ogg");
        Fling(pObj,GetXDir()/15,GetYDir()/15-1);
      }
      else
      {
        Sound("BlockOff*.ogg");
        Sparks(30,RGB(255,128));
        if(GetEffectData(EFSM_ExplosionEffects) > 0) CastParticles("Smoke3",12,10,0,0,100,200,RGBa(255,255,255,100),RGBa(255,255,255,130));
      }
      RemoveObject();
      return;
    }
    else
    {
     Sound("GrenadeHit*.ogg");
     Sparks(30,RGB(255,128));
     if(GetEffectData(EFSM_ExplosionEffects) > 0) CastParticles("Smoke3",12,10,0,0,100,200,RGBa(255,255,255,100),RGBa(255,255,255,130));
     RemoveObject();
     return;
    }
   }

  exploding = true;
  Sound("GrenadeExplosion*.ogg");

  //Schaden verursachen
  Explode(ExplosionDamage());
}
