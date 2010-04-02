/*-- Flaggenwähler --*/

#strict


/* Initalisierung */

public func Initialize()
{
  SetVisibility(VIS_Owner);
}

global func CreateGOCCSpawner(object pCrew)
{
  if(!pCrew) pCrew = this();
  if(!pCrew) return(false);

  var spawner = CreateObject(OSPW);
  pCrew->Enter(spawner);
  
  return(true);
}

public func FxIntSpawnCounterStart(object pTarget, int iEffectNumber, int iTemp)
{
  SetAction("Counter");
  return(1);
}

public func FxIntSpawnCounterTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
  if(!pTarget)
    return(-1);
  
  var crew = pTarget->Contents();
  Sound("Select.ogg",false,crew,100,GetOwner(crew)+1);
  
  if(iEffectTime >= 35*5)
  {
    pTarget->Spawn();
    return(-1);
  }
  else
  {
    SetPhase(iEffectTime/35);
  }

  return(1);
}

public func FxIntSpawnCounterStop(object pTarget, int iEffectNumber, int iTemp,iFrames)
{
  SetAction("Idle");
  return(1);
}

local spawn,flagpoles,selection,oldvisrange,oldvisstate;

protected func Collection2(object pObject)
{
  if(!(GetOCF(pObject) & OCF_CrewMember)) return();
  SetOwner(GetController(pObject));

  //Bei der Initialisierung die Werte speichern.
  oldvisrange = GetObjPlrViewRange(pObject);
  oldvisstate = GetPlrFogOfWar(GetOwner(pObject));

  if(!flagpoles)
    flagpoles = FindObjects(Find_Func("IsFlagpole"));
	
  SpawnMenu();
  SelectBestFlag();
}

private func SelectBestFlag()
{
  var flag = GetBestFlag();
  if(flag)
    SetSelected(flag);
}

private func GetBestFlag()
{
  var capture;
  var best;
  for(var flag in flagpoles)
  {
    if(flag->GetTeam() == GetPlayerTeam(GetOwner(Contents())))
    {
      if(flag->GetProcess() > capture)//Wer bietet mehr?
      {
        capture = flag->GetProcess();
        best = flag;
      }
    }
  }
  
  return(best);
}

public func AutoSelect()//T0ll für Bots. ;)
{
  SelectBestFlag();
  SpawnOk();
}

func SpawnOk()
{
  if(!Contents())
    return(RemoveObject());
  if(Contents()->GetOwner() == NO_OWNER)
    return(RemoveObject());
    
  //Sichtdaten zurücksetzen.
  SetFoW(oldvisstate,GetOwner(Contents()));
  SetPlrViewRange(oldvisrange,Contents());

  spawn = 1;
  CloseMenu(Contents());
  AddEffect ("IntSpawnCounter",this(),200,35,this(),0);
}

// Bitte nicht so hibbelig sein!
public func ContainedLeft() {return(1);}
public func ContainedRight() {return(1);}
public func ContainedDown() {return(1);}
public func ContainedUp() {return(1);}
public func ContainedDig() {return(1);}
public func ContainedThrow() {return(1);}

public func Spawn()
{
  if(!Contents()) return(RemoveObject());

  if(!GameCall("OccupationSpawn",Contents()))//INFO: Eigene Spawnmöglichkeiten via GameCall. Z.B. Fallschirm-Sprung o.Ä.! ;D
    {
    // Igitt. Aber obj->ID::Call(); geht nicht. :(
    var tim = CreateObject(TIM2);
    var obj = Contents();
    Exit(obj);
    Enter(tim, obj);
    tim->Spawn();
    RemoveObject();
    }
  
  return(1);
}

public func SpawnMenu()
{
  var crew = Contents();
  if(!crew) return();
  
  var team = GetPlayerTeam(GetOwner(crew));
  if(!team) return();
  
  CloseMenu (crew);
  
  var tmp,point;
  CreateMenu(OFLG,crew,0,C4MN_Extra_Info,"$SpawnMenu$",0,C4MN_Style_Dialog);
  
  for(point in flagpoles)
  {
    tmp = GraphicsHelper(point);
    if(!tmp) continue;
    
    AddMenuItem (point->GetName(),"SelectFlagpole2",GetID(),crew,point->GetProcess(),ObjectNumber(point),"",4,tmp);
    tmp->RemoveObject();
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
  if(!crew) return();
  
  if((pObject->GetTeam() != GetPlayerTeam(GetOwner(crew))) || (pObject->GetProcess() < 1))
  {
    SetPlrViewRange(0,crew);
    return(Sound("Error",false,crew,100,GetOwner(crew)+1));
  }

  SetPlrViewRange(Min(200,oldvisrange),crew);//Nicht mehr als maximal 200px sehen.
  
  var X,Y;
  pObject->GetSpawnPoint(X,Y);
  
  SetPosition(GetX(pObject)+X,GetY(pObject)+Y);
    
  SpawnOk();
  CloseMenu(crew);
}

protected func Timer()
{
  if(spawn) return();

  var crew = Contents();
  if(!crew) return();

  if(!flagpoles) return();

  selection = GetMenuSelection (crew); 
  
  if(GetSelected())
    ShowFlagpole(GetSelected());//TODO: Wozu wird denn nochmal der Timer gebraucht? Es gibt doch einen Callback dafür?
  
  SpawnMenu();
}

public func ShowFlagpole(object pObject)
{
  var crew = Contents();
  if(!crew) return();
  
  if(pObject->GetTeam() != GetPlayerTeam(GetOwner(crew)))
  {
    SetPlrViewRange(0,crew);//ZENSIERT! :I
    return();
  }
  
  SetPlrViewRange(Min(200,oldvisrange),crew);//blah
  
  SetPosition(GetX(pObject),GetY(pObject));
}

protected func MenuQueryCancel()
{
  if(spawn) return(false);
  else      return(true);
}

//Helper
protected func GraphicsHelper(object pFlagpole)
{
  if(!pFlagpole) return();

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
  
  return(tmp);
}

protected func GetSelected()
{
  var crew = Contents();
  if(!crew) return();
  
  if(selection == -1)
    return();

  if(selection >= -1)
    return(flagpoles[selection]);
}

protected func SetSelected(object flag)
{
  var crew = Contents();
  if(!crew) return();
  
  if(!flag) return();
  
  var i = FindFlag(flag);
  if(i == -1) return();
  
  selection = i;
  SelectMenuItem(selection,crew);
}

protected func FindFlag(object flag)
{
  for(var i = GetLength(flagpoles)-1;i > 0;i--)
    if(flag == flagpoles[i])
      return(i);
      
  return(-1);
}
