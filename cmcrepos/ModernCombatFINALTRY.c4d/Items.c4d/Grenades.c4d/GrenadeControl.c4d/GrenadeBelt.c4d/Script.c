/*-- Granatengürtel --*/

#strict 2

local pUser;


/* Initialisierung */

func Initialize()
{
  SetPosition();
  SetVisibility(VIS_None);
  return 1;
}

func SetUser(user)
{
  pUser = user;
}

func GetUser()
{
  return pUser;
}