/*-- Stahlbrücke --*/

#strict 2

public func IsBridge() { return 1; }
public func IsLocked() { return true; }


/* Initalisierung */

protected func Initialize()
{
  return 1;
}

/* Zerstörung */

func Damage()
{
  if(GetDamage() > 150)
  {
   //Effekte
   CastParticles("MetalSplinter",4,150,0,0,40,150);
   CastParticles("MetalSplinter",8,230,0,0,40,15,RGB(40,20,20));
   CastParticles("Smoke3",12,10,0,0,100,200,RGBa(255,255,255,100),RGBa(255,255,255,130));
   Sound("StructuralDamage*.ogg");
   //Zum brennenden Wrack werden
   ChangeDef(_HBB); 
   Incinerate();
  }
}
