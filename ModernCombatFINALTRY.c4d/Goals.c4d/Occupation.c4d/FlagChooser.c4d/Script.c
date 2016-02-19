/*-- Flaggenwähler --*/

#strict 2

local szFunction, iClass;

public func IsSpawnObject()	{return true;}


/* Initalisierung */

public func Initialize()
{
  SetVisibility(VIS_Owner);
  szFunction = "";
}

global func CreateGOCCSpawner(object pCrew, int iChoosedClass)
{
  if(!pCrew) pCrew = this;
  if(!pCrew) return false;

  var spawner = CreateObject(OSPW);
  Enter(spawner, pCrew);

  LocalN("iClass", spawner) = iChoosedClass;

  return spawner;
}

local spawn,flagpoles,selection,oldvisrange,oldvisstate;

protected func Collection2(object pObject)
{
  if(!(GetOCF(pObject) & OCF_CrewMember)) return;
  SetOwner(GetController(pObject));

  //Bei Initialisierung Werte speichern
  oldvisrange = GetObjPlrViewRange(pObject);
  oldvisstate = GetPlrFogOfWar(GetOwner(pObject));

  SetFoW(true, GetOwner(pObject));
  SetPlrViewRange(0, pObject);

  if(!flagpoles)
  {
    flagpoles = FindObject(GOCC)->GetSortedFlags();
    if(!flagpoles) {
      ErrorLog("Couldn't find any flags");
      GameOver();
    }
  }

  SpawnMenu();
  SelectBestFlag();
}

private func SelectBestFlag()
{
  var flag = GetBestFlag(GetPlayerTeam(GetOwner(Contents())));
  if(flag)
    SetSelected(flag);
}

global func GetFlagDistance(object pFlag)
{
  var flags = [];
  var gocc = FindObject2(Find_ID(GOCC));
  if(gocc) flags = gocc->GetFlags();
  var i = 0;
  while(i < GetLength(flags)-1)
  {
    if(pFlag == flags[i])
    {
      break;
    } 
    i++;
  }

  var down = i;
  var up = i;
  var iTeam = pFlag->GetTeam();
  var dist = 0;
  while(down >= 0 || up <= GetLength(flags)-1)
  {
    if(flags[down] && down >= 0)
      if(flags[down]->GetTeam() != iTeam)
      {
        dist = i - down;
        break;
      }
    down--;
    if(flags[up] && up >= 0)
      if(flags[up]->GetTeam() != iTeam)
      {
        dist = up - i;
        break;
      }
    up++;
  }
  return dist;
}

global func GetBestFlag(int iTeam)
{
  var capture;
  var best;
  var dist;
  var flags = [];

  var gocc = FindObject2(Find_ID(GOCC));
  if(gocc) flags = gocc->GetFlags();
  for(var flag in flags)
  {
    if(flag->GetTeam() == iTeam)
    {
      if(flag->GetProcess() > capture || GetFlagDistance(flag) < dist) //Wer bietet mehr?
      {
        dist = GetFlagDistance(flag);
        capture = flag->GetProcess();
        best = flag;
      }
    }
  }

  return best;
}

/* KI-Funktion */

public func AutoSelect()
{
  SelectBestFlag();
  SpawnOk();
}

func SpawnOk()
{
  if(!Contents())
    return RemoveObject();
  if(GetOwner(Contents()) == NO_OWNER)
    return RemoveObject();

  //Sichtdaten zurücksetzen
  SetFoW(oldvisstate,GetOwner(Contents()));
  SetPlrViewRange(oldvisrange,Contents());

  spawn = 1;
  CloseMenu(Contents());
  Spawn();
}

public func ContainedLeft()	{return 1;}
public func ContainedRight()	{return 1;}
public func ContainedDown()	{return 1;}
public func ContainedUp()	{return 1;}
public func ContainedDig()	{return 1;}
public func ContainedThrow()	{return 1;}

public func Spawn()
{
  if(!Contents()) return RemoveObject();

  var obj = Contents();
  Exit(obj);
  //Hinweis: Eigene Spawnmöglichkeiten via GameCall
  if(!GameCall(szFunction,obj))
  {
    var tim = CreateObject(TIM2);
    Enter(tim, obj);
    tim->Spawn();
    RemoveObject();
  }

  return 1;
}

public func SpawnMenu()
{
  var crew = Contents();
  if(!crew) return;

  var team = GetPlayerTeam(GetOwner(crew));
  if(!team) return;

  CloseMenu (crew);

  var tmp,point;
  CreateMenu(OFLG,crew,0,C4MN_Extra_Info,"$SpawnMenu$",0,C4MN_Style_Dialog);

  for(point in flagpoles)
  {
    tmp = GraphicsHelper(point);
    if(!tmp) continue;

    AddMenuItem (GetName(point),"SelectFlagpole2",GetID(),crew,point->GetProcess(),ObjectNumber(point),"",4,tmp);
    RemoveObject(tmp);
  }

  SelectMenuItem(selection,crew);
}

protected func SelectFlagpole2(id unused,int iObject)
{
  SelectFlagpole(Object(iObject));
}

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

  if(FindObject(MCSL))
    FindObject(MCSL)->SpawnEventInfo(Format("$SpawnAt$", GetName(pObject)), GetOwner(crew), iClass, FindObject(GOCC));

  CloseMenu(crew);
}

protected func Timer()
{
  if(spawn) return;

  var crew = Contents();
  if(!crew) return;

  if(!flagpoles) return;

  selection = GetMenuSelection (crew); 

  if(GetSelected())
    ShowFlagpole(GetSelected(), Contents(), this, oldvisrange);//TODO: Wozu wird denn nochmal der Timer gebraucht? Es gibt doch einen Callback dafür?

  SpawnMenu();
}

protected func OnMenuSelection(int iSelection)
{
  var crew;
  if(spawn || !(crew = Contents()) || !flagpoles) return;
  selection = iSelection;
}

global func ShowFlagpole(object pObject, object pCrew, object pContainer, int iMaxrange)
{
  if(!pCrew) return;
  if(!pObject) return;
  if(!iMaxrange) iMaxrange = 200;

  if(pObject->GetTeam() != GetPlayerTeam(GetOwner(pCrew)))
  {
    SetPlrViewRange(0,pCrew);
    return;
  }

  SetPlrViewRange(Min(200,iMaxrange), pCrew);

  if(!pContainer) pContainer = pCrew;
  if(!pContainer) return false;
  SetPosition(GetX(pObject), GetY(pObject), pContainer);
  return true;
}

protected func MenuQueryCancel()
{
  if(spawn)
    return false;
  else
    return true;
}

protected func GraphicsHelper(object pFlagpole)
{
  if(!pFlagpole) return;

  var tmp = CreateObject(GetID());

  if(!pFlagpole->GetTeam())
    SetColorDw(RGB(255,255,255),tmp);
  else
  {
    if(pFlagpole->GetProcess() >= 100)
      SetColorDw(GetTeamColor(pFlagpole->GetTeam()),tmp);
    else
      SetColorDw(SetRGBaValue(GetTeamColor(pFlagpole->GetTeam()), 255/2, 0),tmp);
  }

  if(pFlagpole->IsAttacked())
  {
    SetGraphics ("WARN",tmp,GetID(tmp),1,GFXOV_MODE_Picture);
  }

  if(pFlagpole->GetTrend())
  {
    if(pFlagpole->GetTrend() < 0)
      SetGraphics ("DOWN",tmp,GetID(tmp),2,GFXOV_MODE_Picture);
    else
      SetGraphics ("UP",tmp,GetID(tmp),2,GFXOV_MODE_Picture);
  }

  return tmp;
}

protected func GetSelected()
{
  var crew = Contents();
  if(!crew) return;

  if(selection == -1)
    return;

  if(selection >= -1)
    return flagpoles[selection];
}

protected func SetSelected(object flag)
{
  var crew = Contents();
  if(!crew) return;

  if(!flag) return;

  var i = FindFlag(flag);
  if(i == -1) return;

  selection = i;
  SelectMenuItem(selection,crew);
}

protected func FindFlag(object flag)
{
  for(var i = GetLength(flagpoles)-1;i > 0;i--)
    if(flag == flagpoles[i])
      return i;

  return -1;
}