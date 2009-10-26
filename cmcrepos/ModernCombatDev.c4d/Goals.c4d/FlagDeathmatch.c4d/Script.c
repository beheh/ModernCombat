/*-- Flag Deathmatch --*/

#strict
#include TEAM

local flag;
local flag_base;

global func CreateFDMBase(int iX, int iY)
{
  var base = CreateObject(FLBS,iX,iY,NO_OWNER);

  var goal = FindObject(GFDM);
  if(!goal)
  {
    Log("No FDM-Goal. (Call CreateFDMBase in ChooserFinished!)");
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
  
  AddMsgBoardCmd("FDMResetFlag","FDMResetFlag");
    
  flag_base = FindObject(FLBS);//Bwoah! *lol*
  
  return(_inherited());
}

protected func InitializePlayer()
{
 //ALLvsALL();
  return(_inherited(...));
}

protected func Destruction()
{
  if(flag) Log("Flag hat überlebt!");
  return(_inherited());
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
    
  if(flag_base)
    InitFlag();
    
  inherited();
}

public func CollectFlag(object pClonk, object pFlag)//Callback: Jemand hat die Flagge aufgesammelt.
{
  Log("%s hat die Flagge aufgesammelt.",GetTaggedPlayerName(GetController(pClonk)));
  UpdateHUDs();
}

public func DropFlag(object pClonk, object pFlag)//Callback: Fallengelassen. -.-
{
  if(pClonk)
    Log("%s hat die Flagge fallengelassen.",GetTaggedPlayerName(GetController(pClonk)));
  else
    Log("Die Flagge wurde fallengelassen.");
    
  RemoveEffect("Hold",this());
  UpdateHUDs();
}

global func FDMResetFlag()
{
  FindObject(GFDM)->ResetFlag();
}

public func ResetFlag()//Flagge wieder an Basis kleben.
{
  flag->SetPosition(GetX(flag_base),GetY(flag_base));
  flag->SetR();
  flag->SetRDir();
  flag->SetAction("Fly",flag_base);
}

public func InitFlag()//Flagge erstellen und für unsere Zwecke initialisieren.
{
  if(!flag)
    flag = FindObject(HTFF);
  
  if(!flag)
    flag = CreateObject(HTFF,0,0,NO_OWNER);
    
  flag->Activate(this());
  
  ResetFlag();
}

// Kann mittels des Spielzielauswählers konfiguriert werden
public func IsConfigurable() { return(true); }

// KI-Taktik
/*public func AITactic(object pAIPlayer)
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
}*/

public func RelaunchPlayer(int iPlr, object pClonk, int iMurdererPlr)
{
  inherited(iPlr, pClonk, iMurdererPlr);

  if(iMurdererPlr == -1)
    return();
  
  if(flag)
    if(flag->GetClonk())
      if(iMurdererPlr == GetOwner(flag->GetClonk()))
      {
        if(Hostile(iPlr,iMurdererPlr)) 
          DoTeamScore(GetPlayerTeam(iMurdererPlr),+1);
        else//Teamkiller!
          DoTeamScore(GetPlayerTeam(iMurdererPlr),-1);
      }
      
   UpdateHUDs();
}

//Ugh... wird garnicht mehr benutzt?!
private func ALLvsALL()
{
  for(var i = 0; i < GetPlayerCount(); i++)
  {
    for(var j = 0; j < GetPlayerCount(); j++)
    {
      if(j == i) continue;
      SetHostility(i,j,true,true,false); 
    }
  }
}

public func GetHUDInfo(int player)
{
  if(!flag) return(" ");
  var clonk =  flag->GetClonk();
  if(!clonk) return(" ");
  
  var team = GetPlayerTeam(player);
	var diff = Max(0,TeamGetScore(team))-BestTeamGetScore(team);
	
	var szdiff;
	if(diff == 0) szdiff = "";
	if(diff > 0) szdiff = Format(": +%d",diff);
	if(diff < 0) szdiff = Format(": %d",diff);
  
  return(Format("<c %x>%s</c>%s",GetTeamColor(team),GetPlayerName(player),szdiff));
}