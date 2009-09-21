/*-- Fensterscheibe --*/

#strict

public func Damage()
{
  if(GetDamage() > 100)
  {
   Shatter();
   return(0);
  }
  return(1);
}

public func OnHit(int iDamage, int iType, object pFrom)
{
  Shatter(BoundBy(iDamage,10,20),pFrom);
}

public func Shatter(int iPower, object pFrom)
{
  if(!iPower) iPower = 10;
  var angle = 0;
  var spread = 360;
  
  if(pFrom)
  {
    angle = ObjectAngle(pFrom);
    spread = 60;
  }
  
  // Zersplittern
  Sound("Glass");
  for(var i = 0 ; i < GetDefHeight()/3 ; i++)
    CreateParticle ("SplinterGlass", 0, i*3-GetDefHeight()/2, +Sin(angle+(Random(spread)-spread/2),iPower), -Cos(angle+(Random(spread)-spread/2),iPower), 20);
    
  RemoveObject();
}

public func IsBulletTarget(id def)
{
  if(def->~NoDecoDamage()) return(false);
  return(true);
}