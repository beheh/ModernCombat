/*-- Geschoss --*/

#strict 2

local sounded;

public func IsBulletTarget() {return true;}

func Initialize()
{
   sounded = false;
   return(1);
}

func ResetRotation()
{
  SetR(Angle(GetX(),GetY(),GetX()+GetXDir(),GetY()+GetYDir()),this());
    
  if(GetYDir() > 1 && !sounded)
  {
    sounded = true;
    Schedule("Sound(\"Artillery*.ogg\")",20);
  }
}

func Hit()
{
    Explode(40+RandomX(0,10));
}

public func Damage()
{
  if(GetDamage() > 20)
    Hit();
}