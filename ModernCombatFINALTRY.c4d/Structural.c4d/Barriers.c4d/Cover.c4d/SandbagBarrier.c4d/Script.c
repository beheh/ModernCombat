/*-- Sandsackbarriere --*/

#strict 2

local left;


/* Initialisierung */

func Initialize()
{
  //Eventuell feststeckende Objekte freimachen
  Unstuck();
}

/* Schaden */

func Damage(int iChange)
{
  if(GetDamage() < 30) return;
  Destruct();
}

public func OnDmg(int iDamage, int iType)
{
  if (iType == DMG_Explosion) return 50;
  return 100;
}

func Destruct()
{
  //Effekte
  Sound("FenceDestruct.ogg");
  if(GetEffectData(EFSM_ExplosionEffects) > 0) CastSmoke("Smoke3",8,15,0,0,250,200,RGBa(255,255,255,100),RGBa(255,255,255,130));
  CastParticles("Sandbag", 15, 70, 0,0, 35, 45, RGBa(228,228,228,0), RGBa(250,250,250,50));

  RemoveObject();
}

/* Konstruktion */

func Construction(object pByObj)
{
  var dir = GetDir(pByObj);

  if(!dir)
    dir = GetDir(Contained(pByObj));

  if(dir)
    Right();
  else
    Left();
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