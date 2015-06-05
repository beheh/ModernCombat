/*-- Clonk Respawnpunkt --*/

#strict 2

local respawnpoints, respawnobjects, team;

public func IsRespawnpoint(object pClonk) { return (GetObjectTeam() == GetPlayerTeam(GetOwner(pClonk))); }
public func IsTeamRespawnpoint(int iTeam) { return (GetObjectTeam() == iTeam); }
public func IsAvailable(object pClonk) { return true; }
public func IsViewable(object pClonk) { return true; }

//fuer die kleine Nummer, die unten im Menu steht
public func GetNumber(object pClonk) { return 0; }
public func GetIconID(object pClonk) { return SM16; }

public func GetIcon(object pIcon, object pClonk)
{
  SetColorDw(RGBa(255, 255, 255, 0), pIcon);
  SetGraphics("", pIcon, GetIconID(pClonk));
  
  return;
}

public func GetText(object pClonk)
{
  return GetName();
}

protected func Initialize()
{
  respawnpoints = [];
  respawnobjects = [];
  
  return _inherited(...);
}

public func AddRespawnpoint(int iX, int iY)
{
  respawnpoints[GetLength(respawnpoints)] = [iX, iY];
  return true;	
}

public func AddRespawnobject(object pObject)
{
  respawnobjects[GetLength(respawnobjects)] = pObject;
  return true;	
}

public func AddRespawnpoints(array points)
{ 
  for(var i = 0; i < GetLength(points); i++)
    AddRespawnpoint(points[i][0], points[i][1]);

  return true;
}

public func AddRespawnobjects(array objects)
{
  for(var obj in objects)
    AddRespawnobject(obj);

  return true;
}

public func GetRespawnpoints() 
{
  var arr = [];
  for(var arritem in respawnpoints)
	arr[GetLength(arr)] = arritem;

  for(var obj in respawnobjects)
  {
	arr[GetLength(arr)][0] = GetX(obj);
	arr[GetLength(arr)][1] = GetY(obj);
  }

  return arr; 
}

public func GetObjectTeam() { return team; }
public func SetObjectTeam(int iTeam) { team = iTeam; }
