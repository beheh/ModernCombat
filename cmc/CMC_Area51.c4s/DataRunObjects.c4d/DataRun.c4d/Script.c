/*-- Data Run --*/

#strict 2
#include TEAM

local aData, aMessage;
static iGoal;
local fFulfilled;

public func IsConfigurable()	{return true;}
public func GoalExtraValue()	{return iGoal;}	//Spielzielinformationen an Scoreboard weitergeben


/* Initialisierung */

protected func Initialize()
{
  aMessage = [];

  //Standard-Daten-Anzahl ermitteln
  if(FrameCounter() < 100)
  {
    iGoal += 100;
    if(FindObject(GDAR))
      return RemoveObject(this);
  }

  return true;
}

public func ChooserFinished()
{
  aData = [];

  var i = AddEffect("IntGoal", this, 1, 150, this);
  ScheduleCall(this, "UpdateScoreboard", 1);

  //Spielzielhinweise erstellen
  for(var i = 0; i < GetPlayerCount(); i++)
  {
    DoScoreboardShow(1, GetPlayerByIndex(i) + 1);
    CreateObject(TK10, 0, 0, GetPlayerByIndex(i));
    Sound("Info_Round.ogg", true, 0, 100, GetPlayerByIndex(i) + 1);
  }

  return _inherited(...);
}

/* Konfigurationsmenü */

local iChoosedPlr;

public func ConfigMenu(object pCaller)
{
  iChoosedPlr = GetOwner(pCaller);
  OpenGoalMenu();
  return 1;
}

private func ConfigFinished()
{
  var chos = FindObject(CHOS);
  if(chos)
    chos->OpenMenu();
}

private func OpenGoalMenu(id dummy, int iSelection)
{
  var pClonk = GetCursor(iChoosedPlr);
  CreateMenu(GetID(),pClonk,0,0,0,0,1);

  AddMenuItem(" ", "OpenGoalMenu", GetID(), pClonk, iGoal, 0, " ");
  AddMenuItem("$MoreData$", "ChangeData", CHOS, pClonk, 0, +100, "$MoreData$",2,1);
  AddMenuItem("$LessData$", "ChangeData", CHOS, pClonk, 0, -100, "$LessData$",2,2);
  AddMenuItem("$Finished$", "ConfigFinished", CHOS, pClonk,0,0,"$Finished$",2,3);

  SelectMenuItem(iSelection, pClonk);
}

private func ChangeData(id dummy, int iChange)
{
  //Zu erreichenden Punktestand verändern (maximal 1000 (normal) oder 500 (Liga))
  if(!GetLeague())
    iGoal = BoundBy(iGoal + iChange,100,1000);
  else
    iGoal = BoundBy(iGoal + iChange,200,500);
  //Sound
  Sound("Grab", 1,0,0,1);
  //Menü wieder öffnen
  var iSel = 1;
  if(iChange == -100) iSel = 2;
  OpenGoalMenu(0, iSel);
}

/* Spielzielerfüllungscheck */

protected func FxIntGoalTimer()
{
  UpdateScoreboard();
  if(IsFulfilled())
  {
    Schedule("GameOver()", 150);
    RewardEvaluation();
    Sound("Cheer.ogg", true);
    return -1;
  }
}

/* Relaunch */

protected func InitializePlayer(int iPlr)
{
  //Wird noch eingestellt
  if(FindObject(CHOS))
    return;

  //Scoreboard
  UpdateScoreboard();
}

protected func RemovePlayer(int iPlr)
{
  //Scoreboard
  UpdateScoreboard();
}

/* Computer platzieren */

public func GetComputer()	{return FindObject(C4P2);}

local compSpawns;

global func SetupComputerSpawnpoint()
{
  if(FindObject(GDAR))
    return FindObject(GDAR)->~SetupComputerSpawnpoint(...);

  return false;
}

global func PlaceComputerSpawnpoint()
{
  if(FindObject(GDAR))
    return FindObject(GDAR)->~PlaceComputerSpawnpoint(...);

  return false;
}

/* Computer entfernen und neuen Computer planen */

static const GDAR_ComputerRespawnDelay = 350;

public func DelayedComputerRespawn(object pComp, int iX, int iY)
{
  if(fFulfilled)
    return false;

  //Eventnachricht: Computer verloren
  EventInfo4K(0, "$CompLost$", COMP, 0, 0, 0, "Info_Objective.ogg");

  //Effekte
  Sound("StructureHeavyHit*.ogg", false, pComp);
  ShakeViewPort(400, pComp);

  //Computer explodieren lassen (WTF)
  Explode(50, pComp);

  //Neue Computerplatzierung planen
  ScheduleCall(this, "PlaceComputerSpawnpoint", GDAR_ComputerRespawnDelay, 0, iX, iY, true);
}

/* Computer platzieren */

public func SetupComputerSpawnpoint(array aSpawnCoordinates)
{
  compSpawns = aSpawnCoordinates;
  if(!compSpawns)
    compSpawns = [[]];

  EventInfo4K(0, "$CompSpawnDelay$", COMP, 0, 0, 0, "Info_Objective.ogg");
  ScheduleCall(this, "PlaceComputerSpawnpoint", GDAR_ComputerRespawnDelay, 0, 0, 0, true);
  return true;
}

public func PlaceComputerSpawnpoint(int iX, int iY, bool fNoDelay)
{
  if(!iX && !iY)
  {
    //Eventnachricht: Computer gesichtet
    EventInfo4K(0, "$ComputerSpawned$", COMP, 0, 0, 0, "Info_Objective.ogg");

    var sp = compSpawns[Random(GetLength(compSpawns))];
    iX = sp[0]; iY = sp[1];
  }

  var comp = CreateObject(COMP, AbsX(iX), AbsY(iY), NO_OWNER);
  if(!GBAS->SpawningConditions(comp))
  {
    if(!fNoDelay)
      DelayedComputerRespawn(comp, 0, 0);
    else
    {
      var sp = compSpawns[Random(GetLength(compSpawns))];
      iX = sp[0]; iY = sp[1];
      comp->SetPosition(iX, iY);
    }
  }

  return true;
}


/* Scoreboard */

static const GDAR_Icon = 0;
static const GDAR_Goal = 1;
static const GDAR_GoalCount = 2;
static const GDAR_Name = 1;
static const GDAR_Count = 2;

public func UpdateScoreboard()
{
  //Wird noch eingestellt
  if(FindObject(CHOS)) return;

  //Titelzeile
  SetScoreboardData(SBRD_Caption, SBRD_Caption, GetName());

  //Zu erreichende Datenanzahl
  SetScoreboardData(SBRD_Caption, GDAR_Icon, Format("{{%i}}", GetID()));
  SetScoreboardData(SBRD_Caption, GDAR_Goal, Format("$Goal$"));
  SetScoreboardData(SBRD_Caption, GDAR_GoalCount, Format("%d", iGoal));

  //Leerzeile
  SetScoreboardData(0, GDAR_Count, " ", iGoal*2);
  
  //Computerinformation
  var state = "", icon = COMP;
  var comp = GetComputer();
  if(!fFulfilled)
  {
    //Anfangs von Idle ausgehen
    if(comp)
      state = "$ComputerIdle$";
    else
    {
      var comp_carrier, e;
      for(var obj in FindObjects(Find_OCF(OCF_CrewMember)))
      {
        if(e = GetEffect("DataRunComputer", obj))
        {
          comp_carrier = obj;
          break;
        }
      }

      //Kein Träger verfügbar: Als gesucht melden
      if(!comp_carrier)
        state = "$SearchingComp$";
      //Ansonsten Träger anzeigen
      else
      {
        state = Format("$CompInPossession$", GetTaggedPlayerName(GetOwner(comp_carrier)));
        var t =  EffectVar(5, comp_carrier, e);

        //Bei Transmitter
        if(t)
        {
          if(t->IsDownloading())
            icon = SM27;
        }
      }
    }

    SetScoreboardData(1, GDAR_Icon, Format("{{%i}}", icon));
    SetScoreboardData(1, GDAR_Name, "Computer:");
    SetScoreboardData(1, GDAR_Count, state, iGoal*2);
  }
  SetScoreboardData(2, GDAR_Count, " ", iGoal*2);

  var iRow = 0;

  //Teams auflisten
  if(Teams())
  {
    var engine_teams = GetTeamConfig(TEAM_AutoGenerateTeams);
    for(var i = 0; i < GetTeamCount(); i++)
    {
      var team = GetTeamByIndex(i);
      var name = GetTaggedTeamName(team);
      var clr = GetTeamColor(team);
      var plr;

      //Kein Spieler im Team? Dann nicht anzeigen
      if(GetTeamMemberByIndex(team, 0) == -1)
        continue;

      //Bei Engine-Teams statt "Team X" und Teamfarbe, Spielername und Spielerfarbe (falls nur ein Spieler im Team)
      if(engine_teams && GetTeamPlayerCount(team) <= 1 && (plr = GetTeamMemberByIndex(team, 0)) > -1)
      {
        name = GetTaggedPlayerName(plr, true);
        clr = GetPlrColorDw(plr);
      }

      SetScoreboardData(team+2, GDAR_Name, name);
      SetScoreboardData(team+2, GDAR_Count, Format("<c %x>%d</c>", clr, aData[team]), aData[team]);
      iRow = team+2;
    }
  }
  else
    for(var i = 0; i < GetPlayerCount(); i++)
    {
      var plr = GetPlayerByIndex(i);
      SetScoreboardData(plr+2, GDAR_Name, GetTaggedPlayerName(plr, true));
      SetScoreboardData(plr+2, GDAR_Count, Format("<c %x>%d</c>", GetPlrColorDw(plr), aData[plr]), aData[plr]);
      iRow = plr+2;
    }

  SetScoreboardData(iRow+1, GDAR_Count, " ", -1);

  var i = iRow+2;
  for(var obj in FindObjects(Find_Func("IsDataTransmitter", true)))
  {
    SetScoreboardData(i, GDAR_Icon, Format("{{%i}}", obj->GetIcon()));
    SetScoreboardData(i, GDAR_Name, GetName(obj));
    SetScoreboardData(i++, GDAR_Count, Format("%d%", obj->GetPoints()*100/obj->MaxPoints()), -2);
  }

  //Sortieren noch
  SortScoreboard(GDAR_Count, true);
  
  return true;
}

/* Relaunch */

public func OnDeathAnnounce(object pCrew, int iKiller, int iAssist)
{
  var iPlr = GetOwner(pCrew);

  if(!GetPlayerName(iPlr) || (!pCrew && !(pCrew = GetCrew(iPlr))))
    return false;

  var iChange;
  //Datenträger fallenlasen bei Gegnerkill
  if(!(iKiller == NO_OWNER || iPlr == iKiller || GetPlayerTeam(iPlr) == GetPlayerTeam(iKiller)))
  {
    var index = iPlr;
    if(Teams())
      index = GetPlayerTeam(iPlr);

    var data_dead = aData[index];
    if(data_dead*5/100)
      CreateObject(DASE, GetX(pCrew), GetY(pCrew), GetOwner(pCrew))->Set(data_dead*5/100);

    GiveData(iPlr, -data_dead*5/100);
    GiveData(iKiller, 10);
  }

  //Credits verteilen
  Money(iPlr, pCrew, iKiller);

  //Scoreboard
  UpdateScoreboard();
}

public func RelaunchPlayer(int iPlr, object pCrew, int iKiller)
{
  Money(iPlr, pCrew, iKiller);
}

private func GiveData(int iPlr, int iAmount)
{
  var index = iPlr;
  //Teams?
  if (Teams())
    index = GetPlayerTeam(iPlr);
  //Wieviel ist drauf
  var iData = aData[index];
  //Ändern
  aData[index] = Max(iData + iAmount);
  //Eine kurze Nachricht
  var pClonk = GetCursor(iPlr);
  if (pClonk && pClonk->~GetRealCursor())
    pClonk = pClonk->~GetRealCursor();
  var string;
  if (iAmount > 0)
    string = Format("{{IC20}} <c 00ff00>+%d</c>", iAmount);
  else
    string = Format("{{IC20}} <c ff0000>%d</c>", iAmount);
  if (pClonk)
    AddEffect("PointMessage", pClonk, 130, 1, pClonk, 0, string);

  var iWarningData = iGoal - Min(32 * (iGoal/2) / 100, 100);
  if(aData[index] >= iWarningData - (iWarningData % 10))
  {
    if(!aMessage[index])
    {
      aMessage[index] = true;
      var message = Format("$EnemyTeamIsWinning$", GetTaggedTeamName(GetPlayerTeam(iPlr)));
      if(!Teams() || GetTeamPlayerCount(GetPlayerTeam(iPlr)) == 1)
        message = Format("$EnemyPlayerIsWinning$", GetTaggedPlayerName(iPlr));
        //Eventnachricht: Gegnerischer Spieler gewinnt bald
        EnemyEventInfo(iPlr, message, GDAR, 0, 0, 0, "Info_Alarm.ogg");
    }
  }
  else
    aMessage[index] = false;

  //Und Änderung zurückgeben
  return aData[index] - iData;
}

global func Teams()
{
  return GetTeamConfig(TEAM_Active) && (!GetTeamConfig(TEAM_AutoGenerateTeams) || GetTeamConfig(TEAM_Dist) != 2);
}

/* Sieg */

public func IsFulfilled()
{
  //Wird noch eingestellt
  if (FindObject(CHOS)) return;

  if (fFulfilled) return true;

  //Siegerermittlung je nach Teameinstellung
  if (Teams())
  {
    for (var i = 0; i < GetTeamCount(); i++)
      if (aData[GetTeamByIndex(i)] >= iGoal)
      {
        //Verlierer eliminieren
        for (var j = GetPlayerCount(); j >= 0; j--)
          if (GetPlayerTeam(GetPlayerByIndex(j)) != GetTeamByIndex(i))
            EliminatePlayer(GetPlayerByIndex(j));

        //Spielende planen
        Schedule("GameOver()", 150);

        //Auswertung
        RewardEvaluation();

        //Nachricht über Gewinner
        Message("$TeamHasWon$", 0, GetTeamColor(GetTeamByIndex(i)), GetTeamName(GetTeamByIndex(i)));

        //Sound
        Sound("Cheer.ogg", true);

        return fFulfilled = true;
      }
  }
  else
  {
    for (var i = 0; i < GetPlayerCount(); i++)
      if (aData[GetPlayerByIndex(i)] >= iGoal)
      {
        //Verlierer eliminieren
        for (var j = GetPlayerCount(); j >= 0; j--)
          if (GetPlayerByIndex(j) != GetPlayerByIndex(i))
            EliminatePlayer(GetPlayerByIndex(j));

        //Spielende planen
        Schedule("GameOver()", 150);

        //Auswertung
        RewardEvaluation();

        //Nachricht über Gewinner
        Message("$TeamHasWon$", 0, GetPlrColorDw(GetPlayerByIndex(i)), GetPlayerName(GetPlayerByIndex(i)));

        //Sound
        Sound("Cheer.ogg", true);

        return fFulfilled = true;
      }
  }

  //Nur noch eins übrig
  if (GetActiveTeamCount() <= 1)
  {
    var i = GetPlayerTeam(GetPlayerByIndex());

    //Spielende planen
    Schedule("GameOver()", 150);

    //Auswertung
    RewardEvaluation();

    //Nachricht über Gewinner
    Message("$TeamHasWon$", 0, GetTeamColor(i), GetTeamName(i));

    //Sound
    Sound("Cheer.ogg", true);

    return fFulfilled = true;
  }

  return false;
}

protected func Activate(int iPlr)
{
  if(FindObject(CHOS))
    return MessageWindow(GetDesc(this), iPlr);

  var iIndex = iPlr;
  if (Teams())
    iIndex = GetPlayerTeam(iPlr);
  return MessageWindow(Format("$MsgGoalUnfulfilled$", aData[iIndex], iGoal), iPlr);
}

/* Ungenutze Funktionen */

public func InitScoreboard()		{}
public func InitMultiplayerTeam()	{}
public func RemoveMultiplayerTeam()	{}
public func InitSingleplayerTeam()	{}
public func RemoveSingleplayerTeam()	{}
public func InitPlayer()		{}
public func RemoveScoreboardPlayer()	{}
public func SortTeamScoreboard()	{}
