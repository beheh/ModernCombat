/*-- Clonk Respawnpunkt --*/

#strict 2

local respawnmethod, respawnplaces, respawnobjects, team;

//Bestimmt, ob der Respawnpunkt im Menu angezeigt wird, oder nicht
public func IsRespawnplace(object pClonk) { return (GetObjectTeam() == GetPlayerTeam(GetOwner(pClonk))); }
//Bestimmt, ob der Respawnpunkt beim ersten Respawn (in der Klassenwahl) angezeigt werden soll, oder nicht
public func IsTeamRespawnplace(int iTeam) { return (GetObjectTeam() == iTeam); }
//Bestimmt, ob man bei dem Spawnpunkt spawnen darf
public func IsAvailable(object pClonk) { return true; }
//Bestimmt, ob man die Vorschau des Spawnpunkts im Menu sehen darf
public func IsViewable(object pClonk) { return true; }

public func GetRespawnTime(object pClonk) { return 0; }
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
	
  var clr;

  if(IsAvailable(pClonk))
    clr = RGB(255, 255, 255);
  else
  {
    if(GetObjectTeam() != GetPlayerTeam(GetOwner(pClonk)))
      clr = RGB(255, 0, 0);
    else
      clr = RGB(119, 119, 119);
  }

  return Format("<c %x>%s</c>", clr, GetName());
}

protected func Initialize()
{
  respawnplaces = [];
  respawnobjects = [];
  respawnmethod = "Default";

  return _inherited(...);
}

public func AddRespawnplace(int iX, int iY)
{
  respawnplaces[GetLength(respawnplaces)] = [iX, iY];
  return true;	
}

public func AddRespawnobject(object pObject)
{
  respawnobjects[GetLength(respawnobjects)] = pObject;
  return true;	
}

public func AddRespawnplaces(array aPoints)
{ 
  for(var i = 0; i < GetLength(aPoints); i++)
    AddRespawnplace(aPoints[i][0], aPoints[i][1]);

  return true;
}

public func AddRespawnobjects(array aObjects)
{
  for(var obj in aObjects)
    AddRespawnobject(obj);

  return true;
}

public func GetRespawnplaces() 
{
  var arr = [];
  for(var arritem in respawnplaces)
	arr[GetLength(arr)] = arritem;

  for(var obj in respawnobjects)
  {
	arr[GetLength(arr)][0] = GetX(obj);
	arr[GetLength(arr)][1] = GetY(obj);
  }

  if(!GetLength(arr))
    return [[GetX(), GetY()]];

  return arr; 
}

public func CustomRespawn(object pClonk, string sName)
{
  if(!GetRespawnMethod() || GetRespawnMethod() == "")
    SetRespawnMethod("Default");  

  return ObjectCall(this, Format("%sRespawnMethod", GetRespawnMethod()), pClonk);
} 

public func DefaultRespawnMethod(object pClonk)
{
  AddSpawnEffect(pClonk, pClonk->GetColorDw());

  return 1;
}

public func ParachuteRespawnMethod(object pClonk)
{
  DefaultRespawnMethod(pClonk);

  AddEffect("IntPara", pClonk, 1, 1);
  AddEffect("Flying", pClonk, 101, 5);
  Sound("Airstrike2", 0, pClonk);

  return 1;
}

global func FxIntParaTimer(object pTarget)
{
  if(!Contained(pTarget))
   CreateObject(PARA,0,0,GetOwner(pTarget))->Set(pTarget);
  return -1;
}

public func GetRespawnMethod() { return respawnmethod; }
public func SetRespawnMethod(string sMethod) { respawnmethod = sMethod; }

public func GetObjectTeam() { return team; }
public func SetObjectTeam(int iTeam) { team = iTeam; }
