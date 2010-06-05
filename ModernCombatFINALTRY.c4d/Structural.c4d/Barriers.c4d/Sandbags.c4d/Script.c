/*-- Sandsackbarriere --*/

#strict 2

local left,permanent;


/* Initalisierung */

func Initialize()
{
  Unstuck();
}

/* Schaden */

public func OnHit(int iDamage, int iType, object pFrom)
{
  if(!pFrom) return ;
  if(iType != DMG_Projectile) return ;

  if(GetEffectData(EFSM_BulletEffects) > 1) CreateParticle ("MaterialBlast",Min(GetX(pFrom)-GetX(),GetDefWidth()/2),Min(GetY(pFrom)-GetY(),GetDefHeight()/2),0,0,Min(iDamage*10,200),RGB(194,155,108),0,false);  
}

func Incineration()
{
  if(permanent)
  {
   Extinguish();
   SetCon(100);
  }
  else
   CastParticles("Sandbag", 10, 70, 0,0, 35, 45, RGBa(228,228,228,0), RGBa(250,250,250,50));
}

func Permanent()
{
  permanent = true;
}

/* Konstruktion */

func Construction(object pByObj)//Wird sofort beim Bauen aufgerufen.
{
  var dir = GetDir(pByObj);

  if(!dir)
   dir = GetDir(Contained(pByObj));

  if(dir)
  {
   Right();
  }
  else
  {
   Left();
  }
}

func Right()
{
  SetGraphics();
  left = 0;
  return this;
}

func Left()
{
  SetGraphics("LEFT");
  left = 1;
  return this;
}

func Unstuck()
{
  for(var obj in FindObjects(Find_Exclude(this),Find_InRect(-7,-15,15,32)))
  {
   if(Stuck(obj))
   {
    if(left)
     AutoUnstuck(obj,-1,0);
    else
     AutoUnstuck(obj,1,0);
   }
  }
}
