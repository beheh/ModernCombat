/*-- Busch --*/

#strict 2

func RejectEntrance(object pObj)
{
  if(GetAction() == "Still")
  {
    if(GetXDir(pObj) || GetYDir(pObj))
    {
      SetAction("Motion");
      Sound("Rustle*");
    }
  }
  return true;
}

func Still()
{
  if(!Random(40))
  {
    SetAction("Breeze");
  }
}
