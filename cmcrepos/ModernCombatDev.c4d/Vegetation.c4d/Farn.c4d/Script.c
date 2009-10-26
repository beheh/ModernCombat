/*-- Farn --*/

#strict

func RejectEntrance(object pObj)
{
  if(GetAction() eq "Still")
  {
    if(GetXDir(pObj) || GetYDir(pObj))
    {
      if(!Random(2))
        SetAction("Motion1");
      else
        SetAction("Motion2");
      Sound("Rustle*");
    }
  }
  return(true);
}

func Still()
{
  if(!Random(40))
  {
    if(!Random(2))
      SetAction("Breeze1");
    else
      SetAction("Breeze2");
  }
}
