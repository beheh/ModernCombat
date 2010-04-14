/*-- Rakete --*/

#strict
#include MISS

public func Acceleration()	{return(3);}
public func MaxTime()		{return(35*5);}
public func MaxSpeed()		{return(100);}
protected func SecureDistance()	{return(100);} //Mindestabstand

local sx,sy;

/* Start */

public func Launch(int iAngle, int iDmg, object pFollow)
{
  //Werte setzen
  iSpeed = 5;
  iDamage = iDmg;
  if(!iDamage) iDamage = 35;

  SetR(+iAngle);
  SetXDir(+Sin(iAngle,iSpeed));
  SetYDir(-Cos(iAngle,iSpeed));
  SetAction("Travel");
  
  SetPlrViewRange(120);
  
  sx = GetX();
  sy = GetY();

  //Effekte
  //Sound("RPGP_ThrustStart.ogg");
  AddEffect("ThrustSound",this(),1,11,this());
  AddLight(70,RGB(255,200,200),this(),GLOW);

  //Treffer- und Steuereffekt einsetzen
  AddEffect("HitCheck", this(), 1,1, 0, SHT1,shooter);
  if(pFollow)
   AddEffect("Follow", this(), 1,1, 0, GetID(),pFollow);
}

protected func Secure()
{
  if(Distance(GetX(),GetY(),sx,sy) <= SecureDistance())
   return(true);

  return(false);
}

/* Soundeffekt */

public func FxThrustSoundTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
  //Sound("RPGP_Thrust.ogg",0,0,0,0,+1);
  return(-1);
}

/* Optischer Steuerungseffekt */

public func FxFollowStart(object pTarget, int iEffectNumber, int iTemp, obj)
{
  if(!obj) return(-1);
  EffectVar(0,pTarget,iEffectNumber) = obj;
}

public func FxFollowTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
  var obj = EffectVar(0,pTarget,iEffectNumber);
  if(!obj)
   return(-1);
  //Sch�tze nicht mehr am Zielen?
  if(!obj->~IsAiming())
   return(-1);

   var iDAngle = obj->AimAngle();
   var iAngle = GetR(pTarget);

   var iDiff = Normalize(iDAngle - iAngle,-180);
   var iTurn = Min(Abs(iDiff),6);

   pTarget->SetR(iAngle+iTurn*((iDiff > 0)*2-1));
}

private func Traveling()
{
  if(GetActTime() >= MaxTime()) return(Hit());
  //Geschwindigkeit erh�hen
  Accelerate();
  //Rauchspur
  Smoking();
}

private func StopThrust()
{
  Sound("RPGP_Thrust.ogg",0,0,0,0,-1);
}

/* Rauch */

private func Smoking()
{
  var dist = Distance(0,0,GetXDir(),GetYDir());
  var maxx = +Sin(GetR(),dist/10);
  var maxy = -Cos(GetR(),dist/10);
  var ptrdist = 50;

  for(var i=0; i<dist; i+=ptrdist)
  {
   var x = -maxx*i/dist;
   var y = -maxy*i/dist;

   var rand = RandomX(-10,10);
   var xdir = +Sin(GetR()+rand,8);
   var ydir = -Cos(GetR()+rand,8);

   CreateParticle("Smoke3",x,y,xdir,ydir,RandomX(50,70),RGBa(255,255,255,85),0,0);
  }

  CreateParticle("MuzzleFlash2",-maxx,-maxy,+Sin(GetR()+180,500),-Cos(GetR()+180,500),RandomX(80,140),RGBa(255,200),this());
}

/* Schaden */

public func Damage()
{
  //Rakete abschie�bar
  if(GetDamage() > 5 && !exploding)
  {
   SetAction("Idle");
   FadeOut4K(4);
   AddEffect("Damaged",this(),1,1,this());
   Sound("RPGP_ShotDown.ogg");
  }
}

public func FxDamagedTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
  pTarget->CreateParticle("Thrust",0,0,0,0,70+Random(30),RGBa(255,255,255,0),0,0);
}

public func Hit()
{
  if(GetAction() eq "Idle")
   Explode(3,0,0,0,1);
  else
   HitObject();
}

private func HitObject(pObj)
{
  if(GetAction() eq "Idle")
   Explode(3,0,0,0,1);
  
  //Von Granate kopiert
  if(Secure())
  {
   if(pObj)
   {
    DoDmg(Distance(GetXDir(),GetYDir())/5,DMG_Projectile,pObj); 
    CastParticles("Smoke3",12,10,0,0,100,200,RGBa(255,255,255,100),RGBa(255,255,255,130));
    if(GetOCF(pObj) & OCF_Living)
    {
     Sound("SharpnelImpact*.ogg");
    }
    else
    {
     Sound("BlockOff*.ogg");
     Sparks(30,RGB(255,128));
     CastParticles("Smoke3",12,10,0,0,100,200,RGBa(255,255,255,100),RGBa(255,255,255,130));
    }
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
  }

  exploding = true;
  Sound("GrenadeExplosion*.ogg");

  //Schaden verursachen
  DamageObjects(iDamage,iDamage,this());
  Explode(iDamage,0,0,0,1);
}

public func Destruction()
{
  StopThrust();
}
