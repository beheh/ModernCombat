/*-- Ungelenkte Raketen --*/

#strict 2

#appendto MISL

local fGuided;

public func TracerCompatible()	{return fGuided;}
public func Guideable()		{return fGuided;}

public func MaxSpeed()
{
  if(!fGuided) return 200;
  return _inherited();
}

public func Initialize()
{
  fGuided = false;
  return _inherited();
}

public func Launch(int iAngle, object pFollow, bool isGuided)
{
  fGuided = isGuided;
  var val = _inherited(iAngle, pFollow);
  SetPlrView();
  SetPlrViewRange(0);
  return val;
}
