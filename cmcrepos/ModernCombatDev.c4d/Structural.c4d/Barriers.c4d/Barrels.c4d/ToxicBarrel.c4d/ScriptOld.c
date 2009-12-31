/*-- Fass --*/

#strict 2
#include XBRL

public func OnHit(int iDamage, int iType, object pFrom)
{
  if(iDamage >= 10)
    SplatterWaste(Min(iDamage,60),pFrom);
}

public func SplatterWaste(int iPower, object pFrom)
{
  if(!iPower) iPower = 10;
  var angle = 0;
  var spread = 360;
  
  if(pFrom)
  {
    angle = ObjectAngle(pFrom);
    spread = 60;
  }
  
  var glob;
  for(var i = 0 ; i < iPower/10 ; i++)
  {
    glob = CreateObject(SLST,0,0,GetOwner());
    glob->Launch(+Sin(angle+(Random(spread)-spread/2),iPower),
                 -Cos(angle+(Random(spread)-spread/2),iPower),
                 6);
  }
  glob->Sound("SlimeShot*");
}

func Damage(int iChange, int iPlr)
{
  if(GetDamage() < 40) return ;

  InstaExplode(iPlr);
}

func InstaExplode(int iPlr)
{
  if(damaged) return ;
  damaged = true;

  var glob;
  for(var i = 0 ; i < 8 ; i++)
  {
    glob = CreateObject(SLST,0,0,GetOwner());
    glob->Launch(RandomX(-20,+20),
                 RandomX(-20,+20),
                 6);
  }
  
  //Effektgehasche
  CastParticles("SlimeGrav", 10, 25, 0, 0, 20, 40, RGBa(0, 240, 0, 10), RGBa(20, 255, 20, 75));
  CastParticles("FrSprk", 30, 5, 0, 0, 70, 130, RGBa(0, 240, 0, 10), RGBa(20, 255, 20, 75));  
  Sound("SlimeHit");
  Sound("Poff");
  Sound("BarrelImpact*.ogg");
  
  SetAction("Wreck");
  FadeOut();
}

/* Aufschlag */ 
  
protected func Hit()
{
  Sound("BarrelImpact*.ogg");
  return 1;
}

protected func Hit3()
{
  DoDamage(40);
}
