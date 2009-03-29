/*-- Hold the Flag --*/

#strict
#include TEAM

local flag;
local flag_base;
local htime;

private func UpdateHUDs()
{
  Log("UpdateHUDs()");
  inherited();
}

global func CreateHTFBase(int iX, int iY)
{
  var base = CreateObject(FLBS,iX,iY,NO_OWNER);

  var goal = FindObject(GHTF);
  if(!goal)
  {
    Log("No HTF-Goal. (Call CreateHTFBase() in ChooserFinished!)");
    return(false);
  }
  
  goal->SetFlagBase(base);
  return(true);
}

protected func Initialize()
{
  //Anzahl Gewinne wird durch Anzahl Aktivierungen eingestellt
  var pGoal;
  if (pGoal = FindObject(GetID()))
  { 
    LocalN("iWinScore", pGoal)++;
    return(RemoveObject()); 
  }
  iWinScore = 1;
  
  AddMsgBoardCmd("HTFResetFlag","HTFResetFlag");
  
  htime = GameCall("GetHTFHoldTime");
  if(!htime)
    htime =  60*35;
    
  flag_base = FindObject(FLBS);//Bwoah! *lol*
  
  ALLvsONE(0);
  
  return(_inherited());
}

protected func InitializePlayer()
{
  if(flag)
    ALLvsONE(GetPlayerTeam(GetOwner(flag->GetClonk())));
  else
    ALLvsONE(0);
      
  return(_inherited(...));
}

protected func Destruction()
{
  if(flag) Log("Flag hat überlebt!");
  return(_inherited());
}

public func GetHoldTime()//Wie lange muss man denn die Flagge halten?
{
  if(!htime)
    htime = 60*35;
  //TODO: Spielerzahl einbeziehen. :P
  return(htime);
}

public func SetFlagBase(object pFlagBase)//Das ist meine Flaggenbasis.
{
  if(flag_base)
    if(pFlagBase != flag_base)
      RemoveObject(flag_base);

  flag_base = pFlagBase;
  
  InitFlag();
}

public func SetFlag(object pFlag)
{
  flag = pFlag;
}

public func ChooserFinished()
{
  if(!flag_base)
    flag_base = FindObject(FLBS);

  if(!flag)    
    InitFlag();
    
  inherited();
}

public func CollectFlag(object pClonk, object pFlag)//Callback: Jemand hat die Flagge aufgesammelt.
{
  Log("%s hat die Flagge aufgesammelt.",GetTaggedPlayerName(GetController(pClonk)));
  AddEffect("Hold", this(), 101, 1, this());
  
  ALLvsONE(GetPlayerTeam(GetOwner(pClonk)));
  UpdateHUDs();
  Sound("GHTF_Start.ogg",true);
}

public func DropFlag(object pClonk, object pFlag)//Callback: Fallengelassen. -.-
{
  if(pClonk)
    Log("%s hat die Flagge fallengelassen.",GetTaggedPlayerName(GetController(pClonk)));
  else
    Log("Die Flagge wurde fallengelassen.");
    
  ALLvsONE(0);
    
  RemoveEffect("Hold",this());
  UpdateHUDs();
}

public func HoldTheFlag(object pClonk)
{
  Log("%s hat die Flagge %d Sekunden gehalten!",GetTaggedPlayerName(GetController(pClonk)),GetHoldTime()/35);
  DoTeamScore(GetPlayerTeam(GetController(pClonk)),+1);
  Sound("Applause",true);//Huh... jaaahhh... ganz toll. *gelangweiltes-klatschen*
  
  ResetFlag();
  UpdateHUDs();
}

global func HTFResetFlag()
{
  FindObject(GHTF)->ResetFlag();
}

public func ResetFlag()//Flagge wieder an Basis kleben.
{
  flag->SetPosition(GetX(flag_base),GetY(flag_base));
  flag->SetR();
  flag->SetRDir();
  flag->SetAction("Fly",flag_base);
  
  ALLvsONE(0);
  UpdateHUDs();
}

public func InitFlag()//Flagge erstellen und für unsere Zwecke initialisieren.
{
  if(!flag)
    flag = FindObject(HTFF);
  
  if(!flag)
    flag = CreateObject(HTFF,0,0,NO_OWNER);
    
  flag->Activate(this());
  
  ResetFlag();
  UpdateHUDs();
}

// Kann mittels des Spielzielauswählers konfiguriert werden
public func IsConfigurable() { return(true); }

// KI-Taktik
public func AITactic(object pAIPlayer)
{
  var owner = pAIPlayer->GetOwner();

  for(var i=0, pCrew ; pCrew = GetCrew(owner, i) ; i++)
  {
    if(pCrew->Contained()) continue;
    // Es ist der Anführer?
    if(!i)
      pAIPlayer->TacticDefault(pCrew);
    // Nicht? Dann folge doch bitte dem Anführer
    else
    {
      pCrew->CheckInventory();
      if(!(pCrew->GetCommand() || pCrew->GetMacroCommand()))
        pCrew->SetMacroCommand(this(), "Follow", GetCrew(owner, 0),0,0,0,Aggro_Follow);
    }
  }
  return(true);
}

private func ALLvsONE(int iTeam)//*möp*
{
  if(!iTeam)
  {
    for(var i = 0; i < GetPlayerCount(); i++)
    {
      for(var j = 0; j < GetPlayerCount(); j++)
      {
        if(j == i) continue;
        SetHostility(i,j,false,true,false); 
      }
    }
  }
  else
  {
    for(var i = 0; i < GetPlayerCount(); i++)
    {
      if(GetPlayerTeam(i) == iTeam) continue;
      
      for(var j = 0; j < GetPlayerCount(); j++)
      {
        if(i == j) continue;
        
        if(GetPlayerTeam(j) == iTeam)
          SetHostility(i,j,true,true,false);
        else
          SetHostility(i,j,false,true,false); 
      }
      
      for(var j = 0; j < GetPlayerCount(); j++)
      {
        if(iTeam == GetPlayerTeam(j)) continue;
        
        SetHostility(0,j,true,true,false);
      }
    }
  }
}

/* Timer für die Flagge */
public func FxHoldTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
  if(!flag)
    return(-1);
  if(!flag->GetClonk())
    return(-1);

  if(!(iEffectTime%35))
  {
    if((GetHoldTime()-iEffectTime)/35 < 5)
      Sound("GHTF_Warn.ogg",true);
    UpdateHUDs();
  }

  if(iEffectTime >= GetHoldTime())
  {
    var c = flag->GetClonk();
    if(c)
      HoldTheFlag(c);
    return(-1);
  }
}

public func GetHUDInfo(int player)
{
  if(!flag) return(" ");
  var clonk =  flag->GetClonk();
  if(!clonk) return(" ");
  
  var team = GetPlayerTeam(player);
  var time = GetHoldTime() - GetEffect("Hold",this(),0,6);
  
  return(Format("<c %x>%s</c> : %.2d",GetTeamColor(team),GetPlayerName(player),time/35));
}