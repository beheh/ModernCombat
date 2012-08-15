/*-- Base Assault --*/

#strict 2

#appendto GBAS

public func DoRelaunch(object pCrew, object pTarget)
{
  //Ziel weg? Neu öffnen
  if(!pTarget)
    return OpenRelaunchMenu(pCrew);

  var container = Contained(pCrew);
  var id = GetID(pTarget);

  //Fake?
  if(id == AHBS)
    id = pTarget->GetImitationID();

  //Relaunchposition
  var x, y, iTeam = pTarget->GetTeam(), array = aSpawn[iTeam];
  if(GetType(array) == C4V_Array)
    array = array[GetIndexOf(pTarget, aTargets[iTeam])];

  //Da ist kein Array? Dann am Objekt respawnen lassen
  if(GetType(array) != C4V_Array) 
  {
    x = GetX(pTarget);
    y = GetY(pTarget)+GetDefHeight(id)/2-10;
  }
  else
  {
  	GetBestSpawnpoint(array, GetOwner(pCrew), x, y);
  	y -= 10;
    /*var i = Random(GetLength(array));
    x = array[i][0];
    y = array[i][1]-10;*/
  }

  SetPosition(x, y, Contained(pCrew));
  container->Spawn();
  SetPlrViewRange(500, pCrew);
}
