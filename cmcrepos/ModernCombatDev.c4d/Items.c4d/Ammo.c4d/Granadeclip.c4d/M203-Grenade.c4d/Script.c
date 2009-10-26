/*-- M203-Granate --*/

#strict 2
#include MCAM

local iDamage;
local shooter;

func Construction(object byObj)
{
  if(!byObj) return;
  shooter = GetShooter(byObj);
}

func Launch(int xdir, int ydir, int iDmg)
{
  SetCategory(C4D_Vehicle,this);
  AddEffect("HitCheck", this, 1, 1, 0, SHTX,shooter);
  AddEffect("Grenade", this, 1, 1, this);
  AddEffect("IntSecure", this, 1, SecureTime(), this);
  SetSpeed(xdir, ydir);
  iDamage = iDmg;
  if(!iDamage) iDamage = 30;
}

func FxIntSecureTimer() { return -1; }

func ExplodeDelay() { return 35*3; }
func SecureTime() { return 25; }

func IsBulletTarget(id id)
{
  return true;
}

public func NoDecoDamage() { return true; }

func FxGrenadeTimer(object target, int effect, int time)
{
  if(time > ExplodeDelay()) return HitObject();
  var vel=Abs(GetXDir())+Abs(GetYDir());
  var alpha=Max(0,100-vel);
  
  CreateParticle("Smoke2", -GetXDir()/6, -GetYDir()/6, RandomX(-15, 15), -5,
                       vel/2+RandomX(20, 60), RGBa(100,100,100,50+alpha)); 
}

func FxGrenadeDamage()
{
  HitObject();
}

func IsBouncy() { return true; }
public func IsAmmoPacket() { return true; }
public func AmmoID() { return GRAM; }
public func AmmoCount() { return 1; }
public func IsRifleGrenade() { return true; }

func FxGrenadeTimer(object target, int effect, int time)
{
  if(time > ExplodeDelay()) return HitObject();
  var vel=Abs(GetXDir())+Abs(GetYDir());
  var alpha=Max(0,100-vel);
  
  CreateParticle("Smoke2", -GetXDir()/6, -GetYDir()/6, RandomX(-15, 15), -5,
                       vel/6+RandomX(10, 30), RGBa(100,100,100,50+alpha));
                       
  SetR(Angle(0,0,GetXDir(),GetYDir()));
}

func Hit()
{
  HitObject();
}

func HitObject(object pObj)
{
  RemoveEffect("Grenade", this,0,1);
	if(!GetEffect("IntSecure",this))
    Trigger(pObj);
  else
	{
	  if(pObj)
		{
		  pObj->Fling(0,-1);
			Sound("BlockOff*.ogg");
			SetXDir(GetXDir()/6);
			SetYDir(GetYDir()/6);
			FadeOut4K(3,pObj);
		}
	}
}

func Trigger(object pObj)
{
  Explode(iDamage*2/3);
  DamageObjects(iDamage*3/2,iDamage*2,this);
  CreateParticle("Blast",0,0,0,0,10*iDamage,RGB(255,255,128));//FakeBlast :°
}