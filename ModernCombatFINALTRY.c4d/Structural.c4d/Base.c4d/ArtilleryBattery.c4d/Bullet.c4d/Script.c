/*-- Geschoss --*/

#strict 2

local sounded;

public func IsBulletTarget()	{return true;}


/* Initalisierung */

func Initialize()
{
   sounded = false;
   return(1);
}

/* Rotation */

func ResetRotation()
{
  SetR(Angle(GetX(),GetY(),GetX()+GetXDir(),GetY()+GetYDir()),this());
    
  if(GetYDir() > 2 && !sounded)
  {
    sounded = true;
    Sound("Artillery*.ogg");
  }
}

/* Schaden */

func Hit()
{
  //Explosion
  DamageObjects(50,30,this());
  Explode(30+Random(10),0,0,0,1);

  //Effekte
  Sound("C4EX_Detonation*.ogg");
  CastParticles("Smoke3",15,10,0,0,300,700);
}

public func Damage()
{
  if(GetDamage() > 20)
    Hit();
}