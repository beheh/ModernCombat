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
  
  return(spawner);
}

public func FxIntSpawnCounterStart(object pTarget, int iEffectNumber, int iTemp)
{
  //Speichern
  EffectVar(0, pTarget, iEffectNumber) = GetCategory(pTarget);
  SetCategory(C4D_Foreground, pTarget);
  //Zählen
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
  //Und wieder laden
  SetCategory(EffectVar(0, pTarget, iEffectNumber), pTarget);
  //Normale Action
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

  if(!flagpoles) {
    flagpoles = GetGOCCFlags();
    if(!flagpoles) {
      Log("ERROR: Could not find any flags");
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

global func GetFlagDistance(object pFlag) {
  var flags = GetGOCCFlags();  
  var i = 0;
  while(i < GetLength(flags)-1) {
    if(pFlag == flags[i]) {
      break;
    } 
    i++;
  }
  var down = i;
  var up = i;
  var iTeam = pFlag->GetTeam();
  var dist = 0;
  while(down >= 0 || up <= GetLength(flags)-1) {
    if(flags[down] && down >= 0)
		  if(flags[down]->GetTeam() != iTeam) {
		    dist = i - down;
		    break;
		  }
    down--;
		if(flags[up] && up >= 0)
		  if(flags[up]->GetTeam() != iTeam) {
		    dist = up - i;
		    break;
		  }
    up++;
  }
  return(dist);
}

global func GetBestFlag(int iTeam)
{
  var capture;
  var best;
  var dist;
  
  var flags = GetGOCCFlags();  
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
  
  if((pObject->GetTeam() != GetPlayerTeam(GetOwner(crew))) || (!pObject->IsFullyCaptured()))
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
    ShowFlagpole(GetSelected(), Contents(), this, oldvisrange);//TODO: Wozu wird denn nochmal der Timer gebraucht? Es gibt doch einen Callback dafür?
  
  SpawnMenu();
}

global func ShowFlagpole(object pObject, object pCrew, object pContainer, int iMaxrange)
{
  if(!pCrew) return;
  if(!pObject) return;
  if(!iMaxrange) iMaxrange = 200;
  
  if(pObject->GetTeam() != GetPlayerTeam(GetOwner(pCrew)))
  {
    SetPlrViewRange(0,pCrew); //ZENSIERT! :I
    return();
  }
  
  SetPlrViewRange(Min(200,iMaxrange), pCrew); //blah
  
  pContainer->SetPosition(GetX(pObject), GetY(pObject));
  return(true);
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
