/*-- Lobby --*/

#strict 2

#include TSYS

public func UsedRules() { return [NOFD, NOFF, NOAM]; } 
public func SectionName() { return "LobbyWin"; } 
public func HideInMenu() { return true; } 

static g_aPlayerScores, g_iMinigamePos, g_aClonkCoordinates;
static const g_iWinWayStart = 220;
static const g_iWinWayEnd = 360;

local firsttime;

public func Initialize()
{
	if(!g_aPlayerScores)
	{
		g_aPlayerScores = [];
		firsttime = true;
	}
	if(!g_aClonkCoordinates)
		g_aClonkCoordinates = [];
	
	Music("@CMC_Beach Walk.ogg");
  FakeScriptGo(true);
  
  InitializeScoreboard();
  
  SetPosition(190, 180, g_Gamemaster);
  
  return _inherited(...);
}

public func InitializeScoreboard()
{
	SetScoreboardData(SBRD_Caption, 0, "$Points$");
	for(var i = 0; i < GetPlayerCount(C4PT_User); i++)
	{
		var plr = GetPlayerByIndex(i, C4PT_User);
		SetScoreboardData(GetPlayerID(plr)-1, SBRD_Caption, GetTaggedPlayerName(plr));
		SetScoreboardData(GetPlayerID(plr)-1, 0, Format("%d/%d", g_aPlayerScores[plr], g_iWinScore), g_aPlayerScores[plr]);
	}
	
	SortScoreboard(0);
	DoScoreboardShow(true);
}

public func InitializePlayer(int iPlr)
{
	if(GetType(arRespawn[iPlr]) != C4V_Array)
		arRespawn[iPlr] = [];
	
	if(firsttime)
		SetRespawnPosition(RandomX(20, 70), 200, iPlr);
	
	return _inherited(iPlr, ...);
}

public func CreateInterior()
{
  
  return true;
}

public func OnClonkRelaunch(object pCrew, bool fInitPlr) 
{
	if(!firsttime && GetType(g_aClonkCoordinates[GetOwner(pCrew)]) == C4V_Array)
	{
		var pos = g_aClonkCoordinates[GetOwner(pCrew)];
		SetPosition(pos[0], pos[1], pCrew);
		SetComDir(COMD_Stop, pCrew);
		SetDir(DIR_Right, pCrew);
	}
	if(!fInitPlr && GetIndexOf(g_iWinScore, g_aPlayerScores) != -1)
		EliminatePlayer(GetOwner(pCrew));
	
	return true;
}

public func LoadingFinished()
{
	Wait(-1, -1); // Alle Spieler dürfen sich in dieser Sektion _nicht_ bewegen.
}

public func Script1()
{
	if(firsttime)
	{
		Message("$WelcomeMsg6$", g_Gamemaster);
		for(var i = 0; i < GetPlayerCount(); i++) 
		{
  	  var iPlr = GetPlayerByIndex(i);
  	  SetCommand(GetCrew(iPlr), "MoveTo", 0, g_iWinWayStart, 190, 0, 0, 10);
  	}
  }
  
  return true;
}

static g_iLastRoundResult; // Ergebnis der letzten Runde.
static g_aLastRoundWinners; // Gewinner der letzten Runde.

static const RESULT_OneWinner = 1;
static const RESULT_MoreWinners = 2;
static const RESULT_NoWinners = 3;

public func Script10()
{
	if(firsttime)
		Message("$WelcomeMsg7$", g_Gamemaster, g_iWinScore);
	else
	{
		// Auswertung ~
		if(!GetLength(g_aLastRoundWinners))
			g_iLastRoundResult = RESULT_NoWinners;
		else if(GetLength(g_aLastRoundWinners) == 1)
			g_iLastRoundResult = RESULT_OneWinner;
		else if(GetLength(g_aLastRoundWinners) > 1)
			g_iLastRoundResult = RESULT_MoreWinners;
		
		var result = g_iLastRoundResult;
		for(var plr in g_aLastRoundWinners)
		{
			g_aPlayerScores[plr]++;
			var pClonk = GetCrew(plr);
      if(GetCommand(pClonk)) 
      	SetCommand(pClonk, "");
      
      var iStep = ((g_iWinWayEnd - g_iWinWayStart) / g_iWinScore);
      SetCommand(pClonk, "MoveTo", 0, g_iWinWayStart + g_aPlayerScores[plr] * iStep, 190 - g_aPlayerScores[plr] * 10, 0, 0, 10);
		}
		
		InitializeScoreboard();
		
		var plr;
		if((plr = GetIndexOf(g_iWinScore, g_aPlayerScores)) != -1)
			return EvalGame(plr); 
		
		var msg;
		if(result == RESULT_OneWinner)
		{
			msg = $GameWinner$;
			Message(msg[Random(GetLength(msg))], g_Gamemaster, GetTaggedPlayerName(g_aLastRoundWinners[0])); 
		}
		else if(result == RESULT_MoreWinners)
		{
			msg = $GameMultipleWinners$;
			var plrstr = "";
			for(var i = 0; i < GetLength(g_aLastRoundWinners); i++)
			{
				var char = ", ";
				if(!i)
					char = "";
				else if(i+1 == GetLength(g_aLastRoundWinners))
					char = " $And$ ";
					
				plrstr = Format("%s%s%s", plrstr, char, GetTaggedPlayerName(g_aLastRoundWinners[i]));
			}
			Message(msg[Random(GetLength(msg))], g_Gamemaster, plrstr);
		}
		else if(result == RESULT_NoWinners)
		{	
			msg = $GameDraw$;
			Message(msg[Random(GetLength(msg))], g_Gamemaster);
		}
	}
	
	return true;
}

local iWonPlr;

public func EvalGame(int iPlr)
{
	FakeScriptGo(false);
	// Hier kommt noch Auswertung + Endsequenz hin :)
	
	var tank = CreateObject(TANK, 375, -20, iPlr);
	SetCommand(GetCrew(iPlr), "Enter", tank);
	iWonPlr = iPlr;
	SetDir(DIR_Left, tank);
	Indestructible(tank);
	BindTrigger(tank, TRIGGER_Collection, [], "EnteredTank", this, false, true, 1);
	SetPlayList("@CMC_PartyEnd.ogg");
	Music("@CMC_PartyEnd.ogg");
	/*FakeGoto(30);
	FakeScriptGo(true);*/
}

public func Script20()
{
	if(firsttime)
		Message("$WelcomeMsg8$", g_Gamemaster);
	else
	{
		var msg = $GoOn$;
		Message(msg[Random(GetLength(msg))], g_Gamemaster);
	}
}

public func Script23()
{
	Message("", g_Gamemaster);
	//Nächstes Minigame laden.
	var nMinigame = g_aMinigames[g_iMinigamePos++];
	if(!nMinigame)
	{
		g_iMinigamePos = 0;
		nMinigame = g_aMinigames[g_iMinigamePos++];
	}
	
	for(var clonk in FindObjects(Find_ID(PCMK), Find_NoContainer(), Find_Not(Find_Owner(-1))))
		g_aClonkCoordinates[GetOwner(clonk)] = [GetX(clonk), GetY(clonk)]; 
	
	FakeScriptGo(0);
	LoadMinigame(nMinigame); 
	
	return true;
}

/** Schlusssequenz **/
public func EnteredTank(object pObj)
{
	var tank = Contained(pObj);
	tank->~ContainedLeft(pObj);
	var tim1;
	BindTrigger((tim1 = CreateObject(TIM1, 0, 180, -1)), TRIGGER_Find, [[tim1->Find_InRect(0, 0, 50, 50), Find_ID(TANK)]], "ArrivedLeft", this, false, true, 1);

	return true;
}

public func FxMoreDarknessTimer(object target, int nr, int iTime)
{
	if(!FindObject(TIME))
		CreateObject(TIME);
	
  FindObject(TIME)->~SetTime(BoundBy(iTime / 10, 0, 50));
  
  var iGrade = BoundBy(iTime / 50, 0, 10);
  var g = BoundBy(128 - iGrade * 10, 0, 128);
  SetGamma(RGB(0, 0, 0), RGB(g, g, g), RGB(255, 255, 255), 2);
  SetSkyAdjust(RGB(127 + g, 127 + g, 127 + g), RGB(100 + g, 100 + g, 100 + g));
}

public func ArrivedLeft()
{
	AddEffect("MoreDarkness", this, 101, 1, this);
	for(var i = 0; i < GetPlayerCount(); i++)
	{
		var iPlr = GetPlayerByIndex(i);
		if(iPlr == iWonPlr)
			continue;
		
		var rnd = Random(2);
		rnd = rnd - !rnd;
		CreateObject(BKHK, GetX(GetCrew(iPlr)), -2000, -1)->SetR(90 * rnd); // Ouch.
		DoEnergy(-149, GetCrew(iPlr));
	}
	
	return Schedule("GameOver()", 35*15);
}
