/*-- Minenflagge --*/

#strict 2


public func Set(object target)
{
  SetAction("Attach",target);
  
  SetR(GetR(target));
  SetDir(Random(2));
  
  var plr = GetController(target);
  SetOwner(plr);
  SetNewOwner();
}

public func OnOwnerChanged()
{
  SetNewOwner();
}

private func SetNewOwner()
{
  SetClrModulation(ModulateColor(GetPlrColorDw(GetOwner()),RGBa(255,255,255,30)));

  if(GetOwner() == -1)
   SetVisibility(VIS_None);
  else
   SetVisibility(VIS_Allies | VIS_Owner);
}

public func AttachTargetLost()
{
  RemoveObject();
}