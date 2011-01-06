/*-- Money Run --*/

#strict 2
#include TEAM

local aMoney;
local iGoal;
local fFulfilled;


/* Initialisierung */

public func ChooserFinished()
{
  aMoney = [];
  //Teams
  var iStartMoney = 50;
  if (Teams())
    iStartMoney *= 2;
  iGoal = 150 + iStartMoney * GetPlayerCount() / 2;

  //Teams?
  if (Teams())
    for (var i = 0; i < GetTeamCount(); i++)
      aMoney[GetTeamByIndex(i)] = iStartMoney;
  else
    for (var i = 0; i < GetPlayerCount(); i++)
      aMoney[GetPlayerByIndex(i)] = iStartMoney;

  var i = AddEffect("IntGoal", this, 1, 150, this);
  ScheduleCall(this, "UpdateScoreboard", 1);

  //Geldsäcke verzögert füllen
  AddEffect("IntMoneyCollected", 0, 1, 1);

  //Spielzielhinweise erstellen
  for(var i = 0; i < GetPlayerCount(); i++)
  {
    DoScoreboardShow(1, GetPlayerByIndex(i) + 1);
    CreateObject(TK09, 0, 0, GetPlayerByIndex(i));
    Sound("RadioConfirm*.ogg", true, 0, 100, GetPlayerByIndex(i) + 1);
  }

  return _inherited(...);
}

protected func FxIntGoalTimer()
{
  UpdateScoreboard();
  if (IsFulfilled())
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
  if (FindObject(CHOS))
    return;
  //Geld geben, falls nötig
  if (Teams())
  {
    if (!aMoney[GetPlayerTeam(iPlr)] && GetTeamPlayerCount(GetPlayerTeam(iPlr)) == 1)
      aMoney[GetPlayerTeam(iPlr)] = 100;
  }
  else
    aMoney[iPlr] = 50;

  //Scoreboard
  UpdateScoreboard();
}

protected func RemovePlayer(int iPlr)
{
  //Scoreboard
  UpdateScoreboard();
}

/* Geldsäcke verteilen */

static const GMNR_MoneySpawnTime = 1575;	//45 Sekunden

global func AddMoneySpawn(int iX, int iY, MoneyCount)
{
  if (!FindObject(GMNR))
    return false;
  var obj = CreateObject(MNYS, iX, iY, NO_OWNER);
  obj->~Set(MoneyCount);
  return obj;
}

global func FxIntMoneyCollectedStart(object pTarget, int iEffect, int iTemp, int iAmount, object pClonk)
{
  if (iTemp || !pClonk)
    return;
  //Auszahlen
  var obj = FindObject(GMNR);
  if (obj)
  {
    obj->~GoalMoney(GetOwner(pClonk), iAmount);
    //Scoreboard
    obj->~UpdateScoreboard();
  }
  //Echtes Geld auszahlen, wenn Spielziel nicht vorhanden
  else
    DoWealth(GetOwner(pClonk), iAmount);
}

global func FxIntMoneyCollectedStop(object pTarget, int iEffect, int iReason, bool fTemp)
{
  if (fTemp)
    return;
  //Nur weiter, wenn das der letzte Effekt ist
  if (GetEffectCount("IntMoneyCollected"))
    return;
  //Alle Geldsäcke geleert? Nachspawnen
  var objs = FindObjects(Find_ID(MNYS));
  for (var obj in objs)
    if (obj->~GetCurrent())
      return;
  for (var obj in objs)
    obj->~Refill();
}

/* Scoreboard */

static const GMNR_Name = -1;
static const GMNR_Count = 0;

public func UpdateScoreboard()
{
  if (FindObject(CHOS))
    return;

  //Titelzeile
  SetScoreboardData(SBRD_Caption, GMNR_Name, Format("%d Punkte", iGoal));
  SetScoreboardData(SBRD_Caption, GMNR_Count, Format("{{%i}}", GetID()));

  //Teams durchgehen
  if (Teams())
    for (var i = 0; i < GetTeamCount(); i++)
    {
      var team = GetTeamByIndex(i);
      SetScoreboardData(team, GMNR_Name, GetTaggedTeamName(team));
      SetScoreboardData(team, GMNR_Count, Format("<c %x>%d</c>", GetTeamColor(team), aMoney[team]), aMoney[team]);
    }
  else
    for (var i = 0; i < GetPlayerCount(); i++)
    {
      var plr = GetPlayerByIndex(i);
      SetScoreboardData(plr, GMNR_Name, GetTaggedPlayerName(plr));
      SetScoreboardData(plr, GMNR_Count, Format("<c %x>%d</c>", GetPlrColorDw(plr), aMoney[plr]), aMoney[plr]);
    }

  //Sortieren noch
  SortScoreboard(GMNR_Count, true);
}

/* Relaunch */

public func OnDeathAnnounce(object pCrew, int iKiller, int iAssist)
{
  var iPlr = GetOwner(pCrew);

  if (!GetPlayerName(iPlr) || (!pCrew && !(pCrew = GetCrew(iPlr))))
    return false;

  var iChange;
  //Ungeklärte Ursache oder Selbstmord
  if (iKiller == NO_OWNER || iPlr == iKiller)
    GoalMoney(iPlr, iChange = -10);
  else
    //Teamkill
    if (GetPlayerTeam(iPlr) == GetPlayerTeam(iKiller))
      GoalMoney(iPlr, iChange = -20);
    //Gegner getötet
    else
    {
      //10%, mindestens aber 10 Clunker abziehen
      iChange = GoalMoney(iPlr, -Max(10, aMoney[GetPlayerTeam(iPlr)] / 10));

      //Killer bekommt 80%, mindestens aber 5 Clunker
      var iToKiller = Max(-iChange * 4 / 5, 4);
      //Assist bekommt 20%, mindestens aber 1 Clunker
      var iToAssist = Max(-iChange - iToKiller, 1);
      GoalMoney(iKiller, iToKiller);
      GoalMoney(iAssist, iToAssist);
    }

  //Credits verteilen
  Money(iPlr, pCrew, iKiller);

  //Scoreboard
  UpdateScoreboard();

  //Effekte
  Sound("GetCash*.ogg", false, pCrew);
  Sound("PaperFly*.ogg", false, pCrew);
  while ((iChange++) < 0)
    CreateParticle("Paper2", GetX(pCrew) - GetX() + Random(13) - 6, GetY(pCrew) - GetY() - Random(4), Random(7) - 3, -Random(6), 40 + Random(16), HSLa(Random(256), 192 + Random(64), 96 + Random(64)));
}

public func RelaunchPlayer(int iPlr, object pCrew, int iKiller)
{
  Money(iPlr, pCrew, iKiller);
}

private func GoalMoney(int iPlr, int iAmount)
{
  var index = iPlr;
  //Teams?
  if (Teams())
    index = GetPlayerTeam(iPlr);
  //Wieviel ist drauf
  var iMoney = aMoney[index];
  //Ändern
  aMoney[index] = Max(iMoney + iAmount);
  //Und Änderung zurückgeben
  return aMoney[index] - iMoney;
}

global func Teams()
{
  return GetTeamConfig(TEAM_Active) && (!GetTeamConfig(TEAM_AutoGenerateTeams) || GetTeamConfig(TEAM_Dist) != 2);
}

/* Sieg */

public func IsFulfilled()
{
  //Wird noch eingestellt
  if (FindObject(CHOS))
    return false;
  if (fFulfilled)
    return true;
  //Jemand hat genug Geld?
  if (Teams())
  {
    for (var i = 0; i < GetTeamCount(); i++)
      if (aMoney[GetTeamByIndex(i)] >= iGoal)
      {
        //Die anderen eliminieren
        for (var j = GetPlayerCount(); j >= 0; j--)
          if (GetPlayerTeam(GetPlayerByIndex(j)) != GetTeamByIndex(i))
            EliminatePlayer(GetPlayerByIndex(j));
        Message("<c %x>Team $TeamHasWon$</c>", 0, GetTeamColor(GetTeamByIndex(i)), GetTeamName(GetTeamByIndex(i)));
        return fFulfilled = true;
      }
    //Keiner hat gewonnen. Unentschieden?
    if (GetActiveTeamCount() == 1)
      return fFulfilled = true;
  }
  else
  {
    for (var i = 0; i < GetPlayerCount(); i++)
      if (aMoney[GetPlayerByIndex(i)] >= iGoal)
      {
        //Die anderen eliminieren
        for (var j = GetPlayerCount(); j >= 0; j--)
          if (GetPlayerByIndex(j) != GetPlayerByIndex(i))
            EliminatePlayer(GetPlayerByIndex(j));
        Message("<c %x>$TeamHasWon$</c>", 0, GetPlrColorDw(GetPlayerByIndex(i)), GetPlayerName(GetPlayerByIndex(i)));
        return fFulfilled = true;
      }
    //Keiner hat gewonnen. Unentschieden?
    if (GetPlayerCount() == 1)
      return fFulfilled = true;
  }

  return false;
}

/* Ungenutze Funktionen */

public func InitScoreboard()            {}
public func InitMultiplayerTeam()       {}
public func RemoveMultiplayerTeam()     {}
public func InitSingleplayerTeam()      {}
public func RemoveSingleplayerTeam()    {}
public func InitPlayer()                {}
public func RemoveScoreboardPlayer()	{}
public func SortTeamScoreboard()        {}