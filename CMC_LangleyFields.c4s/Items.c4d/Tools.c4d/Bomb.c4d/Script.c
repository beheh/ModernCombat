/*-- Sprengladung --*/

#strict 2

protected func Activate(object pClonk)
{
  for(var building in FindObjects(Find_AtPoint(), Find_Category(C4D_Structure), Find_Func("IsCMCBuilding")))
    if(building && !GetEffect("BuildingBomb"))
      if(HostileTeam(GetPlayerTeam(GetOwner()), GetPlayerTeam(GetOwner(building))))
        if(AddEffect("BuildingBomb", building, 100, 1, building, GetID(building),GetOwner()))
	      return RemoveObject();
	  
  return;
}

protected func Entrance(object pObject)
{
  SetOwner(GetOwner(pObject));
  return 1;
}
