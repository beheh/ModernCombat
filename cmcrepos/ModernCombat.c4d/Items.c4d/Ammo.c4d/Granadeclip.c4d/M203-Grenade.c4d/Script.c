/*-- M203-Granate --*/

#strict
#include GREN
#include STAP

local active,sx,sy;


public func IsAmmoPacket() { return(true); }
public func AmmoID()       { return(GRAM); }
public func AmmoCount()    { return(1); }
public func IsRifleGrenade()    { return(true); }

/*func IsBulletTarget(id id)
{
  return(true);
}*/

func Launch(int xdir, int ydir, int iDmg,a,b,c)
{
  SetCategory(C4D_Vehicle(),this());
  active = true;
  sx = GetX();
  sy = GetY();
  inherited(xdir,ydir,iDmg,a,b,c);
}

func Hit()
{
  HitObject();
}

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

func HitObject(object pObj)
{
  RemoveEffect("Grenade", this(),0,1);
  Trigger(pObj);
}

func Trigger(object pObj)
{
  Explode(iDamage*2/3);
  DamageObjects(iDamage*3/2,iDamage*2,this());
  CreateParticle("Blast",0,0,0,0,10*iDamage,RGB(255,255,128));//FakeBlast :�
}