/*-- Clonk Respawnpunkt --*/

#strict 2

local respawnpoints, team;

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
  
  return _inherited(...);
}

public func AddRespawnpoint(int iX, int iY)
{
  respawnpoints[GetLength(respawnpoints)] = [iX, iY];
  return true;	
}

public func AddRespawnpoints(array points)
{ 
  for(var i = 0; i < GetLength(points); i++)
    AddRespawnpoint(points[i][0], points[i][1]);

  return true;
}

public func GetRespawnpoints() { return respawnpoints; }

public func GetObjectTeam() { return team; }
public func SetObjectTeam(int iTeam) { team = iTeam; }
