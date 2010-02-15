/*-- Explosivgranate --*/

#strict
#include GREN

local active,sx,sy;

public func IsRifleGrenade()	{return(true);}
public func BlastRadius()	{return(20);}


/* Start */

func Launch(int xdir, int ydir, int iDmg,a,b,c)
{
  SetCategory(C4D_Vehicle(),this());
  active = true;
  sx = GetX();
  sy = GetY();
  inherited(xdir,ydir,iDmg,a,b,c);
}

/* Mindestabstand */

protected func SecureDistance(){return(100);}

protected func Secure()
{
  if(!active)
   return(true);
    
  if(Distance(GetX(),GetY(),sx,sy) <= SecureDistance())
   return(true);
    
  return(false);
}

/* Treffer */

func Hit()
{
  HitObject();
}

func HitObject(object pObj)
{
  if(Secure())
  {
   if(pObj)
   {
    DoDmg(Distance(GetXDir(),GetYDir())/5,DMG_Projectile,pObj); 
    CastParticles("Smoke3",12,10,0,0,100,200,RGBa(255,255,255,100),RGBa(255,255,255,130));
    if(GetOCF(pObj) & OCF_Living)
     Sound("SharpnelImpact*.ogg");
    else
    Sound("BlockOff*.ogg");
    RemoveObject();
    return();
   }
   else
   {
    Sound("GrenadeHit*.ogg");
    Sparks(30,RGB(255,128));
    CastParticles("Smoke3",12,10,0,0,100,200,RGBa(255,255,255,100),RGBa(255,255,255,130));
    RemoveObject();
    return();
   }
   Sound("GrenadeHit*.ogg");
   Sparks(30,RGB(255,128));
   CastParticles("Smoke3",12,10,0,0,100,200,RGBa(255,255,255,100),RGBa(255,255,255,130));
   RemoveObject();
   return();
  }
  Trigger();
}

func Trigger(object pObj)
{
  Explode(BlastRadius()*2/3);
  DamageObjects(BlastRadius()*3/2,BlastRadius()*2,this());
  CreateParticle("Blast",0,0,0,0,10*BlastRadius(),RGB(255,255,128));
  Sound("ShellExplosion*.ogg");
}

/* Timer */

func ExplodeDelay() { return(35*3); }

func FxGrenadeTimer(object target, int effect, int time)
{
  if(time > ExplodeDelay()) return(HitObject());
  var vel=Abs(GetXDir())+Abs(GetYDir());
  var alpha=Max(0,100-vel);

  CreateParticle("Smoke2", -GetXDir()/6, -GetYDir()/6, RandomX(-15, 15), -5,
                       vel/6+RandomX(10, 30), RGBa(100,100,100,50+alpha));
  SetR(Angle (0,0,GetXDir(),GetYDir()));
}

/* Zerstörung */

func IsBulletTarget(id id)
{
  // Kann von anderen Geschossen getroffen werden
  return(true);
}

func Damage()
{
  if(GetDamage() > 15)
  {
   //Effekte
   CastParticles("MetalSplinter",3,80,0,0,45,50,RGB(40,20,20));
   Sparks(8,RGB(255,128));
   Sound("ProjectileHit*.ogg");
   Trigger();
  }
}