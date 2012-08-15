/*-- Flaggenwähler --*/

#strict 2

#appendto OSPW

public func SelectFlagpole(object pObject)
{
  var crew = Contents();
  if(!crew) return;
  
  if((pObject->GetTeam() != GetPlayerTeam(GetOwner(crew))) || (!pObject->IsFullyCaptured()))
  {
    SetPlrViewRange(0, crew);
    SpawnMenu();
    return Sound("Error", false, crew, 100, GetOwner(crew)+1);
  }

  SetPlrViewRange(Min(200, oldvisrange), crew);//Nicht mehr als maximal 200px sehen.
  
  var X, Y, szFunc;
  pObject->GetSpawnPoint(X, Y, szFunc, GetOwner(crew));
  
  SetPosition(GetX(pObject) + X, GetY(pObject) + Y);
  
  if(szFunc) szFunction = szFunc;
    
  SpawnOk();
  CloseMenu(crew);
}
