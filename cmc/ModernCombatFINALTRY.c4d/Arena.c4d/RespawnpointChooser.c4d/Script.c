/*-- Respawnmenu --*/

#strict 2

local iClass;
local selection, respawns, oldvisrange, oldvisstate;

/* Initalisierung */

public func Initialize()
{
  SetVisibility(VIS_Owner);
  
  selection = 0;
  respawns = [];
  
  return;
}

global func CreateRelaunchMenu(object pCrew, int iChoosedClass)
{
  if(!pCrew) 
	pCrew = this;

  if(!pCrew) 
	return false;

  var spawner = CreateObject(RPCH);
  Enter(spawner, pCrew);

  LocalN("iClass", spawner) = iChoosedClass;

  return spawner;
}

protected func Collection2(object pObject)
{
  if(!(GetOCF(pObject) & OCF_CrewMember)) 
	return;

  SetOwner(GetController(pObject));

  //Bei Initialisierung die Werte speichern
  oldvisrange = GetObjPlrViewRange(pObject);
  oldvisstate = GetPlrFogOfWar(GetOwner(pObject));

  SetFoW(true, GetOwner(pObject));
  SetPlrViewRange(0, pObject);

  RelaunchMenu();
}

public func ContainedLeft()  { return 1; }
public func ContainedRight() { return 1; }
public func ContainedDown()  { return 1; }
public func ContainedUp()    { return 1; }
public func ContainedDig()   { return 1; }
public func ContainedThrow() { return 1; }


public func RelaunchMenu()
{
  var crew = Contents();
  if(!crew) 
    return;

  var team = GetPlayerTeam(GetOwner(crew));

  CloseMenu (crew);

  var tmp,point;  
  CreateMenu(OFLG,crew,0,C4MN_Extra_Info,"$RelaunchMenu$",0,C4MN_Style_Dialog);

  var i = 0;
  for(point in FindObjects(Find_Func("IsRespawnpoint", crew)))
  {
    var tmp = CreateObject(GetID(this));
	SetGraphics("", this, SM16);	
	point->~GetIcon(tmp, crew);

    AddMenuItem(point->~GetText(),"SelectRelaunchPoint",GetID(),crew,point->~GetNumber(crew),ObjectNumber(point),"",4,tmp);
    RemoveObject(tmp);
	
    //Cache, fuer die Sicht
    respawns[i] = ObjectNumber(point);
    i++;
  }
  if(i > 0 && selection != -1)
    SelectMenuItem(selection, crew);
}

public func SelectRelaunchPoint(id unused, int iObject)
{
  var pObject = Object(iObject);

  if(!pObject)
	return;
  
  var crew = Contents();
  if(!crew) 
	return;

  if(!pObject->~IsAvailable(crew))
  {
    SetPlrViewRange(0, crew);
    RelaunchMenu();
    return Sound("Error", false, crew, 100, GetOwner(crew)+1);
  }
  
  var respawnpoint = pObject->~GetRespawnpoints();

  if(!respawnpoint || !(GetType(respawnpoint) == C4V_Array))
	return;
  
  var iX, iY;
  GetBestSpawnpoint(respawnpoint, GetOwner(crew), iX, iY);  
  
  SetPosition(iX, iY);
  
  //Sichtdaten zurücksetzen
  SetFoW(oldvisstate, GetOwner(crew));
  SetPlrViewRange(oldvisrange, crew);
  
  if(!Contents()) 
	return RemoveObject();

  Exit(crew);
  
  //todo: eigene Spawnmoeglichkeit via Callback/Gamecall
  var tim = CreateObject(TIM2);
  Enter(tim, crew);
  tim->Spawn();
  RemoveObject();

  if(FindObject(MCSL))
    FindObject(MCSL)->SpawnEventInfo(Format("$SpawnAt$", GetName(pObject)), GetOwner(crew), iClass, FindObject(GOCC));

  CloseMenu(crew);
}

protected func Timer()
{
  var crew = Contents();
  if(!crew) 
	return;

  selection = GetMenuSelection(crew); 

  if(selection >= 0 && respawns[selection])
    ShowRespawnpoint(Object(respawns[selection]), Contents(), this, oldvisrange);

  RelaunchMenu();
}

//todo: eventuell die Methode verallgemeinern und outsourcen
global func ShowRespawnpoint(object pObject, object pCrew, object pContainer, int iMaxrange)
{
  if(!pCrew) 
	return;

  if(!pObject) 
	return;

  if(!iMaxrange) 
	iMaxrange = 200;

  if(!pObject->~IsViewable(pCrew))
  {
    SetPlrViewRange(0, pCrew);
    return;
  }

  SetPlrViewRange(iMaxrange, pCrew);

  if(!pContainer)
	pContainer = pCrew;

  SetPosition(GetX(pObject), GetY(pObject), pContainer);
  return true;
}
