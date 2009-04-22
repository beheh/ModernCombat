/* Stinger Rakete */

#strict
#include MISS

public func Acceleration() { return(3); }
public func MaxTime() { return(35*5); }
public func MaxSpeed() { return(100); }

public func Launch(int iAngle, int iDmg, object pFollow)
{
  iSpeed = 5;
  iDamage = iDmg;
  if(!iDamage) iDamage = 35;

  SetR(+iAngle);
  SetXDir(+Sin(iAngle,iSpeed));
  SetYDir(-Cos(iAngle,iSpeed));
  SetAction("Travel");
  
  Sound("RPGP_ThrustStart.ogg");
  AddEffect("ThrustSound",this(),1,11,this());

  AddLight(70,RGB(255,200,200),this(),GLOW);

  AddEffect("HitCheck", this(), 1,1, 0, SHT1,shooter);
  if(pFollow)
    AddEffect("Follow", this(), 1,1, 0, GetID(),pFollow);
}

public func FxThrustSoundTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
  Sound("RPGP_Thrust.ogg",0,0,0,0,+1);
  return(-1);
}

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
  if(obj->~Invalid())
    return(0);
  
  pTarget->SetR(Angle(GetX(pTarget),GetY(pTarget),GetX(obj),GetY(obj)));
  
  /*var aoff = AngleOffset4K(GetR(pTarget),Angle(GetX(pTarget),GetY(pTarget),GetX(obj),GetY(obj)));
  if(aoff != 0)
  {
    if(aoff < 0)
      pTarget->SetR(pTarget->GetR()+Min(-20,aoff));
    else
      pTarget->SetR(pTarget->GetR()+Min(+20,aoff));
  }*/
}

private func Traveling()
{
  if(GetActTime() >= MaxTime()) return(Hit());
  Accelerate();
  Smoking();
}

private func StopThrust()
{
  Sound("RPGP_Thrust.ogg",0,0,0,0,-1);
}

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

public func Damage()
{
  if(GetDamage() > 1 && !exploding)
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

private func HitObject()
{
  if(GetAction() eq "Idle")
    Explode(3,0,0,0,1);

  exploding = true;
  Sound("GrenadeExplosion*.ogg");
  DamageObjects(iDamage*8/6,iDamage,this());
  DamageObjects(iDamage*8/6,iDamage,this());
  DamageObjects(iDamage*8/6,iDamage,this());
  Explode(iDamage*4/5,0,0,0,1);
}

public func Destruction()
{
  StopThrust();
}