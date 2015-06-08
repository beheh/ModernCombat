/*-- Granatengürtel --*/

#strict 2

local pUser;


/* Initialisierung */

public func Initialize()
{
  SetPosition();
  SetVisibility(VIS_None);
  return 1;
}

public func Collection2(object pObj)
{
  pUser->~UpdateGrenadeCount();
}

public func Ejection(object pObj)
{
  pUser->~UpdateGrenadeCount();
}

public func SetUser(user)
{
  pUser = user;
}

public func GetUser()
{
  return pUser;
}