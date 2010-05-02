/*-- Ziel --*/

#strict 2

func Initialize()
{
  SetPlrViewRange(200);
}

func Check()
{
  if(GetPlrView(GetOwner()) != this)
    RemoveObject();
}