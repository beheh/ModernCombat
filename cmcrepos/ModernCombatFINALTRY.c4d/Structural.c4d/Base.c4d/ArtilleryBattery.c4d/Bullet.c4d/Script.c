/*-- Geschoss --*/

#strict 2

local fSounded;
local fHit;

public func IsBulletTarget()	{return true;}


/* Initalisierung */

func Initialize()
{
   fSounded = false;
   fHit = false;
   return(1);
}

/* Rotation */

func ResetRotation()
{
  SetR(Angle(GetX(),GetY(),GetX()+GetXDir(),GetY()+GetYDir()),this());
    
  if(GetYDir() > 2 && !fSounded)
  {
    fSounded = true;
    Sound("Artillery*.ogg");
  }
  
  SearchObjects();
}

/* Objekte treffen */

func SearchObjects()
{
	if(!fHit) {
		if(FindObject2(Find_AtPoint(), Find_Exclude(this), Find_Func("IsBulletTarget", GetID(), this, 0, GetX(), GetY()))) {
			Hit();
		}
	}
}

/* Schaden */

func Hit()
{
	if(fHit) return;
	
	//Schon getroffen
	fHit = true;

  //Explosion
  DamageObjects(50,30,this());
  Explode(30+Random(10),0,0,0,1);

  //Effekte
  Sound("C4EX_Detonation*.ogg");
  CastParticles("Smoke3",15,10,0,0,300,700);
  
  return;
}

public func Damage()
{
  if(GetDamage() > 20)
    Hit();
}
