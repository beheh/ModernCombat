/*-- Belohnungssystem --*/

#strict 2

static aAchievementProgress, aAchievementExtra, iAchievementCount, iRibbonCount, RWDS_aAchievementBlocked;
static aRanks, iRankCount;
local aData, fEvaluation, aStats, aLastPage, aRibbonAvenger;

public func IsChooseable()	{return true;}
public func RWDS_MenuAbort()	{return true;}


/* Initialisierung */

protected func Initialize()
{
  aData = CreateArray();
  fEvaluation = false;
  aAchievementProgress = CreateArray();
  aAchievementExtra = CreateArray();
  aRibbonAvenger = CreateArray();
  RWDS_aAchievementBlocked = CreateArray();
  aLastPage = CreateArray();
  iAchievementCount = 0;
  while(GetName(0, C4Id(Format("AC%02d", iAchievementCount+1))))
    iAchievementCount++;

  while(GetName(0, C4Id(Format("RB%02d", iRibbonCount+1))))
    iRibbonCount++;

  for(var i = 0; i < GetPlayerCount(C4PT_User); i++)
  {
    var iPlr = GetPlayerByIndex(i, C4PT_User);
    SetPlayerData(GetFullPlayerData(iPlr, RWDS_BattlePoints) + GetFullPlayerData(iPlr, RWDS_TeamPoints), RWDS_StartTotalPoints, iPlr);
  }

  for(var i = 0; i < GetPlayerCount(); i++)
    InitPlayerData(GetPlayerByIndex(i));
}

protected func Activate(iByPlayer)
{
  ShowLastPage(iByPlayer);
  return 1;
}

static const RWDS_Page_Points		= 0;
static const RWDS_Page_Statistics	= 1;
static const RWDS_Page_RibbonList	= 2;
static const RWDS_Page_ShowRibbon	= 3;
static const RWDS_Page_AchievementList	= 4;
static const RWDS_Page_ShowAchievement	= 5;

public func ShowLastPage(int iPlr)
{
  var lastPage = aLastPage[iPlr];
  if(!lastPage)
    lastPage = [];

  var page = lastPage[0];

  if(page == RWDS_Page_Points)
    StatsPoints(iPlr);
  else if(page == RWDS_Page_Statistics)
    StatsStatistics(iPlr);
  else if(page == RWDS_Page_AchievementList)
    StatsList(iPlr, lastPage[1], lastPage[2], lastPage[3]);
  else if(page == RWDS_Page_ShowAchievement)
    StatsAchievement(iPlr, lastPage[1], lastPage[2]);
  else if(page == RWDS_Page_RibbonList)
    StatsRibbonList(iPlr, lastPage[1], lastPage[2]);
  else if(page == RWDS_Page_ShowRibbon)
    StatsRibbon(iPlr, lastPage[1], lastPage[2]);
}

public func StatsPoints(int iPlr)
{
  var pClonk = GetCursor(iPlr);
  if(GetMenu(pClonk))
  {
    if(GetMenu(pClonk)->~RWDS_MenuAbort())
      CloseMenu(pClonk);
    else
      return;
  }
  if(!CreateMenu(GetID(),pClonk,this,0,0,0,C4MN_Style_Dialog)) return;

  aLastPage[iPlr] = [RWDS_Page_Points];

  AddMenuItem(" | ", "", RWDS, pClonk, 0, 0, "", 514, 0, 0);

  //Erst mal einsortieren
  var aList = CreateArray();
  var szString;
  for(var iPlayer = 0; iPlayer < GetLength(aData); ++iPlayer)
  {
    if(!aData[iPlayer])
      continue;

    var iTeam = GetPlayerData(RWDS_PlayerTeam, iPlayer);
    if(!aList[iTeam]) aList[iTeam] = CreateArray();

    szString = Format("%s:| %d $Points$", GetPlayerData(RWDS_CPlayerName, iPlayer), GetPlayerPoints(RWDS_TotalPoints, iPlayer));
    aList[iTeam][GetLength(aList[iTeam])] = szString;
  }
  //Nach Team ausgeben
  AddMenuItem("<c 33ccff>$PointTable$</c>", 0, NONE, pClonk);

  for(var aTeam in aList)
    if(aTeam)
      for(var szString in aTeam)
        if(szString)
          AddMenuItem(szString, 0, NONE, pClonk);

  //Leerzeile
  AddMenuItem(" ", 0, NONE, pClonk);

  //Eigene Errungenschaften anzeigen
  AddMenuItem("$Achievements$", Format("StatsList(%d, 0, 0, 2)", iPlr), NONE, pClonk, 0, 0, "", C4MN_Add_ForceNoDesc);
  //Statistik einblenden
  AddMenuItem("$Stats$", Format("StatsStatistics(%d)", iPlr), NONE, pClonk, 0, 0, "", C4MN_Add_ForceNoDesc);
}

public func StatsList(int iPlr, int iIndex, int iOffset, int iMenuEntry)
{
  var pClonk = GetCursor(iPlr);
  if(GetMenu(pClonk))
  {
    if(GetMenu(pClonk)->~RWDS_MenuAbort())
      CloseMenu(pClonk);
    else
      return;
  }
  if(!CreateMenu(GetID(), pClonk, this, 0, 0, 0, C4MN_Style_Dialog)) return;
  aLastPage[iPlr] = [RWDS_Page_AchievementList, iIndex, iOffset, iMenuEntry];

  AddMenuItem(" | ", 0, RWDS, pClonk, 0, 0, "", 514, 0, 0);

  //Überschrift
  AddMenuItem(Format("<c 33ccff>$Achievements$ ($Showing$)</c>", iOffset/10+1, (iAchievementCount-1)/10+2+((GetPlrAvailableRibbonCount(iPlr)-1)/10)), 0, NONE, pClonk);

  //Liste
  var i = 1+iOffset;
  var idAchievement;
  while(i <= iAchievementCount && i <= 10+iOffset)
  {
    idAchievement = C4Id(Format("AC%02d", i));
    if(GetPlayerAchievement(iPlr, idAchievement))
      AddMenuItem(Format("<c ffff33>%s</c>", GetName(0, idAchievement)), Format("StatsAchievement(%d, %d, %d)", iPlr, i, iOffset), NONE, pClonk, 0, 0, "", C4MN_Add_ForceNoDesc);
    else
      AddMenuItem(Format("<c 777777>%s</c>", GetName(0, idAchievement)), Format("StatsAchievement(%d, %d, %d)", iPlr, i, iOffset), NONE, pClonk, 0, 0, "", 0, 0, 0);
    i++;
  }

  //Leerzeile
  AddMenuItem(" ", 0, NONE, pClonk);

  //Weiter
  var szCmd = 0;
  if(iOffset+10 <= iAchievementCount-1)
    szCmd = Format("StatsList(%d, %d, %d, 0)", iPlr, 0, BoundBy(iOffset+10, 0, iAchievementCount));
  else
    szCmd = Format("StatsRibbonList(%d, 0, 0, 1)", iPlr);

  AddMenuItem("$NextPage$", szCmd, NONE, pClonk, 0, 0, "", C4MN_Add_ForceNoDesc);

  //Zurück
  if(iOffset > 0)
    AddMenuItem("$LastPage$", Format("StatsList(%d, %d, %d, 1)", iPlr, 0, BoundBy(iOffset-10, 0, iAchievementCount)), NONE, pClonk, 0, 0, "", C4MN_Add_ForceNoDesc);
  else
    iMenuEntry--;

  //Eigene Errungenschaften anzeigen
  AddMenuItem("$PointTable$", Format("StatsPoints(%d)", iPlr), NONE, pClonk, 0, 0, "", C4MN_Add_ForceNoDesc);
  //Statistik einblenden
  AddMenuItem("$Stats$", Format("StatsStatistics(%d)", iPlr), NONE, pClonk, 0, 0, "", C4MN_Add_ForceNoDesc);

  if(iIndex)
    SelectMenuItem(iIndex+1, pClonk);
  else
    SelectMenuItem(i-iOffset+2+iMenuEntry, pClonk);

  return true;
}

public func StatsAchievement(int iPlr, int iSelect, int iOffset)
{
  var pClonk = GetCursor(iPlr);
  if(GetMenu(pClonk))
  {
    if(GetMenu(pClonk)->~RWDS_MenuAbort())
      CloseMenu(pClonk);
    else
      return;
  }
  if(!CreateMenu(GetID(),pClonk,this,0,0,0,C4MN_Style_Dialog)) return;

  aLastPage[iPlr] = [RWDS_Page_ShowAchievement, iSelect, iOffset];

  AddMenuItem(" | ", 0, RWDS, pClonk, 0, 0, "", 514);

  var iIndex = iSelect;
  var idAchievement = C4Id(Format("AC%02d", iIndex));
  if(GetPlayerAchievement(iPlr, idAchievement))
  {
    AddMenuItem(Format("<c ffff33>%s</c>", GetName(0, idAchievement)), 0, NONE, pClonk, 0, 0, "", C4MN_Add_ForceNoDesc);
    AddMenuItem(Format("%s", GetDesc(0, idAchievement)), 0, NONE, pClonk, 0, 0, "", C4MN_Add_ForceNoDesc);
  }
  else
  {
    AddMenuItem(Format("<c 777777>%s</c>", GetName(0, idAchievement)), 0, NONE, pClonk, 0, 0, "", C4MN_Add_ForceNoDesc);
    AddMenuItem(Format("%s", GetDesc(0, idAchievement)), 0, NONE, pClonk, 0, 0, "", C4MN_Add_ForceNoDesc);
  }

  //Leerzeile
  AddMenuItem(" ", 0, NONE, pClonk);

  //Zurück
  AddMenuItem("$Back$", Format("StatsList(%d, %d, %d)", iPlr, iSelect-iOffset, iOffset), NONE, pClonk, 0, 0, "", C4MN_Add_ForceNoDesc);
  return true;
}

public func GetPlrAvailableRibbonCount(int iPlr)
{
  var id, i, count;
  while(GetName(0, id = C4Id(Format("RB%02d", ++i))))
  {
    if(id->~RibbonDisabled())
    {
      if(GetPlayerRibbon(iPlr, id))
        count++;
    }
    else
      count++;
  }

  return count;
}

public func StatsRibbonList(int iPlr, int iIndex, int iOffset, int iButton)
{
  var pClonk = GetCursor(iPlr);
  if(GetMenu(pClonk))
  {
    if(GetMenu(pClonk)->~RWDS_MenuAbort())
      CloseMenu(pClonk);
    else
      return;
  }
  if(!CreateMenu(GetID(), pClonk, this, 0, 0, 0, C4MN_Style_Dialog)) return;

  var ribbonCount = GetPlrAvailableRibbonCount(iPlr);
  aLastPage[iPlr] = [RWDS_Page_RibbonList, iIndex, iOffset];

  AddMenuItem(" | ", 0, RWDS, pClonk, 0, 0, "", 514, 0, 0);

  //Überschrift
  AddMenuItem(Format("<c 33ccff>$Ribbons$ ($Showing$)</c>", (iAchievementCount-1)/10+2+(iOffset/10), (iAchievementCount-1)/10+2+((ribbonCount-1)/10)), 0, NONE, pClonk);

  //Liste
  var idRibbon, i, count, iMenuEntryCount;
  while(GetName(0, (idRibbon = C4Id(Format("RB%02d", ++i)))))
  {
    if(idRibbon->~RibbonDisabled() && !GetPlayerRibbon(iPlr, idRibbon))
      continue;

    count++;

    if(count > iOffset)
    {
      if(GetPlayerRibbon(iPlr, idRibbon))
        AddMenuItem(Format("<c ffff33>%s</c>", GetName(0, idRibbon)), Format("StatsRibbon(%d, %d, %d)", iPlr, i, iOffset), NONE, pClonk, 0, 0, "", C4MN_Add_ForceNoDesc);
      else
        AddMenuItem(Format("<c 777777>%s</c>", GetName(0, idRibbon)), Format("StatsRibbon(%d, %d, %d)", iPlr, i, iOffset), NONE, pClonk, 0, 0, "", 0, 0, 0);
      iMenuEntryCount++;
    }

    if(count >= 10+iOffset)
      break;
  }

  //Leerzeile
  AddMenuItem(" ", 0, NONE, pClonk);

  //Weiter
  if(iOffset+10 <= ribbonCount-1)
    AddMenuItem("$NextPage$", Format("StatsRibbonList(%d, 0, %d, 1)", iPlr, BoundBy(iOffset+10, 0, ribbonCount)), NONE, pClonk, 0, 0, "", C4MN_Add_ForceNoDesc);

  //Zurück
  var szCmd = 0;
  if(iOffset > 0)
    szCmd = Format("StatsRibbonList(%d, 0, %d, 2)", iPlr, BoundBy(iOffset-10, 0), i);
  else
    szCmd = Format("StatsList(%d, 0, %d, 1)", iPlr, iAchievementCount-10);
  AddMenuItem("$LastPage$", szCmd, NONE, pClonk, 0, 0, "", C4MN_Add_ForceNoDesc);

  //Eigene Errungenschaften anzeigen
  AddMenuItem("$PointTable$", Format("StatsPoints(%d)", iPlr), NONE, pClonk, 0, 0, "", C4MN_Add_ForceNoDesc);
  //Statistik einblenden
  AddMenuItem("$Stats$", Format("StatsStatistics(%d)", iPlr), NONE, pClonk, 0, 0, "", C4MN_Add_ForceNoDesc);

  if(iButton)
    SelectMenuItem(2+iMenuEntryCount+iButton, pClonk);
  else if(iIndex)
    SelectMenuItem(iIndex+1, pClonk);
  else
    SelectMenuItem(i+2, pClonk);

  return true;
}

public func StatsRibbon(int iPlr, int iSelect, int iOffset)
{
  var pClonk = GetCursor(iPlr);
  if(GetMenu(pClonk))
  {
    if(GetMenu(pClonk)->~RWDS_MenuAbort())
      CloseMenu(pClonk);
    else
      return;
  }
  if(!CreateMenu(GetID(), pClonk, this, 0, 0, 0, C4MN_Style_Dialog)) return;

  aLastPage[iPlr] = [RWDS_Page_ShowRibbon, iSelect, iOffset];

  AddMenuItem(" | ", 0, RWDS, pClonk, 0, 0, "", 514);

  var idRibbon = C4Id(Format("RB%02d", iSelect));
  if(GetPlayerRibbon(iPlr, idRibbon))
  {
    AddMenuItem(Format("<c ffff33>%s</c>", GetName(0, idRibbon)), 0, NONE, pClonk, 0, 0, "", C4MN_Add_ForceNoDesc);
    AddMenuItem(Format("%s", GetDesc(0, idRibbon)), 0, NONE, pClonk, 0, 0, "", C4MN_Add_ForceNoDesc);
  }
  else
  {
    Sound("Error", true, 0, 100, iPlr + 1);
    return StatsRibbonList(iPlr, iSelect, iOffset);
  }

  //Leerzeile
  AddMenuItem(" ", 0, NONE, pClonk);

  //Zurück
  AddMenuItem("$Back$", Format("StatsRibbonList(%d, %d, %d)", iPlr, iSelect, iOffset), NONE, pClonk, 0, 0, "", C4MN_Add_ForceNoDesc);
  return true;
}

global func RewardsActive()
{
  return FindObject2(Find_ID(RWDS)) != false;
}

/* Auswerten */

global func RewardEvaluation()
{
  //Nur mit Belohnungen-Regel fortfahren
  var db = FindObject(RWDS);
  if(!db) return;

  //Siedlungspunkte ausblenden
  HideSettlementScoreInEvaluation(true);

  //Auswertung starten
  db->Evaluate();
}

public func Evaluate()
{
  //Einmalige Auswertung starten
  if(fEvaluation) return 0;
  fEvaluation = true;

  //Sortieren
  var aList = CreateArray();
  var iPlr, szFirstLine, szSecondLine, aTeam, aStrings;

  //Endpunktzahl aktualisieren und Statistiken speichern
  for(iPlr = 0; iPlr < GetPlayerCount(C4PT_User); iPlr++)
    SavePlrStatistics(GetPlayerByIndex(iPlr, C4PT_User));

  //Daten ermitteln
  for(iPlr = 0; iPlr < GetLength(GetData()); iPlr++)
  {
    if(!GetData()[iPlr])
      continue;

    if(!aList[GetPlayerTeam(iPlr)]) aList[GetPlayerTeam(iPlr)] = CreateArray();

    szFirstLine = Format("$FirstLine$",									//Erste Zeile
     GetPlayerPoints(RWDS_PlayerName, iPlr),								//Spielername
     GetPlayerData(RWDS_StartTotalPoints, iPlr),							//Gesamtpunktzahl am Anfang
     (GetPlayerPoints(RWDS_BattlePoints, iPlr) + GetPlayerPoints(RWDS_TeamPoints, iPlr)),		//Gesamtpunktzahl der Runde
     GetPlayerData(RWDS_SavedTotalPoints, iPlr)								//Gesamtpunktzahl
    );

    szSecondLine = Format("$SecondLine$",								//Dritte Zeile
     GetPlayerPoints(RWDS_BattlePoints, iPlr),								//Gefechtspunkte
     GetPlayerPoints(RWDS_TeamPoints, iPlr),								//Teampunkte
     GetPlayerPoints(RWDS_MinusPoints, iPlr)								//Minuspunkte
    );

    aList[GetPlayerTeam(iPlr)][GetLength(aList[GetPlayerTeam(iPlr)])] = [szFirstLine, szSecondLine];
    AddEvaluationData(Format("$PlayerLine$",
     GetPlayerData(RWDS_KillCount, iPlr),
     GetPlayerData(RWDS_DeathCount, iPlr),
     (GetPlayerPoints(RWDS_BattlePoints, iPlr) + GetPlayerPoints(RWDS_TeamPoints, iPlr))),
     GetPlayerData(RWDS_PlayerID, iPlr)
    );
  }

  //Teamweise Auflistung der Daten
  for(aTeam in aList)
  {
    if(!aTeam) continue;
    for(aStrings in aTeam)
    {
      AddEvaluationData(aStrings[0], 0);
      AddEvaluationData(aStrings[1], 0);
    }
  }
  return 1;
}

/* Spieler updaten */

public func InitializePlayer(int iPlr)
{
  return InitPlayerData(iPlr);
}

public func InitPlayerData(int iPlr)
{
  if(!RewardsActive()) return;

  var iDataOld = GetPlrExtraData(iPlr, "CMC_Achievements");
  if(iDataOld)
  {
    iDataOld = iDataOld >> 1; //Konvertieren, Bit 0 wird jetzt mitgenutzt
    var iDataNew = GetPlrExtraData(iPlr, "CMC_Achievements1");
    SetPlrExtraData(iPlr, "CMC_Achievements1", iDataOld | iDataNew);
    SetPlrExtraData(iPlr, "CMC_Achievements", 0);
  }

  //Gegebenenfalls eigenes Ribbon verteilen
  if(IsDeveloper(GetPlayerID(iPlr)))
  {
    var iRibbon = GetPlrExtraData(iPlr, "CMC_Team_Ribbon");
    if(iRibbon && GetName(0, C4Id(Format("RB%02d", iRibbon))))
    {
      AttemptAwardRibbon(C4Id(Format("RB%02d", iRibbon)), iPlr, iPlr);
    }
  }

  if(GetPlayerRank(iPlr) == -1)
  {
    SetPlayerData(GetTaggedPlayerName(iPlr), RWDS_PlayerName, iPlr);
    SetPlayerData(GetTaggedPlayerName(iPlr), RWDS_CPlayerName, iPlr);
  }
  else
    UpdatePlayerNames(iPlr);

  SetPlayerData(GetPlayerTeam(iPlr), RWDS_PlayerTeam, iPlr);
  SetPlayerData(GetPlayerID(iPlr), RWDS_PlayerID, iPlr);
  if(!aData[iPlr]) aData[iPlr] = CreateArray();
  if(!aAchievementProgress[iPlr]) aAchievementProgress[iPlr] = CreateArray();
  if(!aAchievementExtra[iPlr]) aAchievementExtra[iPlr] = CreateArray();

  return true;
}

public func OnTeamSwitch(int iPlr, int idNewTeam)
{
  SetPlayerData(idNewTeam, RWDS_PlayerTeam, iPlr);
  return true;
}

/* Werte setzen und auslesen */

static const RWDS_PlayerName		= 0;
static const RWDS_PlayerTeam		= 1;
static const RWDS_TotalPoints		= 2;
static const RWDS_BattlePoints		= 3;
static const RWDS_TeamPoints		= 4;
static const RWDS_MinusPoints		= 5;
static const RWDS_KillCount		= 6;
static const RWDS_DeathCount		= 7;
static const RWDS_StartTotalPoints	= 8;
static const RWDS_SavedTotalPoints	= 9;
static const RWDS_PlayerID		= 10;
static const RWDS_CPlayerName		= 11;

global func DoPlayerPoints(int iPoints, int iType, int iPlr, object pClonk, id idIcon)
{
  var db = FindObject2(Find_ID(RWDS));
  if(!db) return;			//Keine Belohnungen-Regel?
  if(!GetPlayerName(iPlr)) return;	//Kein Spieler?
  if(!iPoints) return;			//Kein Wert wiedergegeben?
  if(iPoints > 50) return;		//Wert über 50?
  if(iType != RWDS_BattlePoints && iType != RWDS_TeamPoints && iType != RWDS_MinusPoints)
    return ErrorLog("Invalid points type %d for %d points at %v", iType, iPoints, pClonk);
  if(db->SetPlayerData(db->GetPlayerPoints(iType, iPlr)+iPoints, iType, iPlr))
  {
    //Achievement-Fortschritt (Point Hunter)
    DoAchievementProgress(iPoints, AC13, iPlr);

    //Achievement-Fortschritt (Speedster)
    DoAchievementProgress(iPoints, AC38, iPlr);

    //Neuen Rang berechnen
    if(iType == RWDS_TeamPoints || iType == RWDS_BattlePoints)
      RecalcPlayerRank(iPlr);
    if(pClonk)
    {
      if(!idIcon) idIcon = RWDS;
      var szMsg;
      if(iPoints < 0) szMsg = Format("<c ff0000>%d</c>", iPoints);
      if(iPoints > 0) szMsg = Format("<c 00ff00>+%d</c>", iPoints);
      if(iPoints == 0) szMsg = Format("<c ffff00>+%d</c>", iPoints);

      //Punkteanzeige entsprechend der Einstellung des Spielers
      if(pClonk->~ScoreDisplayType())
        pClonk->AddEffect("PointMessage", pClonk, 130, 1, pClonk, 0, Format("{{%i}} %s", idIcon, szMsg));
      else
        pClonk->ScoreInfo(iPlr+1, Format("%s %s", szMsg, GetName(false,idIcon)), idIcon);
      return true;
    }
  }
  return;
}

global func GetPlayerPoints(int iType, int iPlr)
{
  var db = FindObject2(Find_ID(RWDS));
  if(!db) return;
  return db->GetPlayerData(iType, iPlr);
}

public func GetData()
{
  return aData;
}

/* Ränge */

global func GetRankID(int iRank)	{return C4Id(Format("R%03d", iRank));}

/* Ränge-Cache laden */

global func LoadRanks2Cache()
{
  if(!iRankCount)
  {
    iRankCount = 0;
    while(GetName(0, GetRankID(iRankCount)))
    {
      iRankCount++;
    }
  }
  aRanks = [];

  for(var i = 0; i < GetPlayerCount(C4PT_User); i++)
  {
    var iPlr = GetPlayerByIndex(i, C4PT_User);
    aRanks[iPlr] = CalcRank(iPlr);
    //Namen updaten
    if(FindObject2(Find_ID(RWDS)))
      FindObject2(Find_ID(RWDS))->UpdatePlayerNames(iPlr);
  }

  return true;
}

public func UpdatePlayerNames(int iPlr)
{
  SetPlayerData(GetTaggedPlayerName(iPlr, true), RWDS_PlayerName, iPlr);
  SetPlayerData(GetTaggedPlayerName(iPlr, true, true), RWDS_CPlayerName, iPlr);
}

global func GetPlayerRank(int iPlr)
{
  if(GetType(aRanks) == C4V_Array)
    return aRanks[iPlr];
  else
    return -1;
}

/* Spielerrangwert errechnen und speichern */

global func RecalcPlayerRank(int iPlr)
{
  if(GetType(aRanks) == C4V_Array)
  {
    var nRank = CalcRank(iPlr);

    if(nRank != aRanks[iPlr])
    {
      aRanks[iPlr] = nRank;

      var info = CreateObject(GetRankID(nRank), 0, 0, iPlr);
      info->SetHighlightColor(RGB(0,153,255));
      info->SetCustomDesc(Format("$PlayerPromoted$", GetName(info)));
      //Eventnachricht: Beförderung
      EventInfo4K(0, Format("$YouHaveBeenPromoted$", GetTaggedPlayerName(iPlr), GetName(info)), GetID(info), 0, 0, 0, "Info_Event.ogg");

      //Namen updaten
      if(FindObject2(Find_ID(RWDS)))
        FindObject2(Find_ID(RWDS))->UpdatePlayerNames(iPlr);

      //Sound-Hinweis
      Sound("RWDS_Promotion.ogg", true, 0, 100, iPlr+1);

      GameCallEx("OnPlayerRankUp", iPlr, nRank);
    }
    return aRanks[iPlr];
  }
  else
    return -1;
}

/* Rang für iPlr berechnen */

global func CalcRank(int iPlr)
{
  var bpoints = BoundBy(GetPlrExtraData(iPlr, "CMC_BattlePoints"), 0, 0x7FFFFFFF);
  var tpoints = BoundBy(GetPlrExtraData(iPlr, "CMC_TeamPoints"), 0, 0x7FFFFFFF);

  var rewards;
  var rpoints = bpoints + tpoints;
  if(rpoints < 0)
    rpoints = 0x7FFFFFFF;

  if((rewards = FindObject2(Find_ID(RWDS))))
    if((rpoints += BoundBy(rewards->GetPlayerData(RWDS_BattlePoints, iPlr), 0, 0x7FFFFFFF) + BoundBy(rewards->GetPlayerData(RWDS_TeamPoints, iPlr), 0, 0x7FFFFFFF)) < 0)
      rpoints = 0x7FFFFFFF;

  var rank;

  for(var j = 0; j < iRankCount; j++)
  {
    var k = 500 * j ** 2;
    k -= (k % 1000) * (k > 1000); // benötigte Punktzahl (unter Grenze)
    var k2 = 500 * (j+1) ** 2;
    k2 -= (k2 % 1000) * (k2 > 1000); // benötigte Punktzahl (obere Grenze)

    if(j+1 == iRankCount)
      k2 = 0x80000000;

    if(Inside(rpoints, k, k2-1))
    {
      rank = j;
      break;
    }
  }

  return rank;
}

/* Statistiken */

public func SavePlrStatistics(int iPlr)
{
  var bpoints = GetFullPlayerData(iPlr, RWDS_BattlePoints);
  var tpoints = GetFullPlayerData(iPlr, RWDS_TeamPoints);
  var kcnt = GetFullPlayerData(iPlr, RWDS_KillCount);
  var dcnt = GetFullPlayerData(iPlr, RWDS_DeathCount);

  var total = tpoints + bpoints;
  if(total < 0)
    total = 0x7FFFFFFF;

  SetPlayerData(total, RWDS_SavedTotalPoints, iPlr);

  SetPlrExtraData(iPlr, "CMC_BattlePoints", bpoints);
  SetPlrExtraData(iPlr, "CMC_TeamPoints", tpoints);
  SetPlrExtraData(iPlr, "CMC_KillCount", kcnt);
  SetPlrExtraData(iPlr, "CMC_DeathCount", dcnt);

  return true;
}

global func EliminatePlayer(int iPlr)
{
  if(FindObject2(Find_ID(RWDS)))
    FindObject2(Find_ID(RWDS))->SavePlrStatistics(iPlr);

  return _inherited(iPlr, ...);
}

global func SurrenderPlayer(int iPlr)
{
  if(FindObject2(Find_ID(RWDS)))
    FindObject2(Find_ID(RWDS))->SavePlrStatistics(iPlr);

  return _inherited(iPlr, ...);
}

public func GetFullPlayerData(int iPlr, int iType)
{
  if(iType != RWDS_BattlePoints && iType != RWDS_TeamPoints && iType != RWDS_KillCount && iType != RWDS_DeathCount)
    return false;

  if(iType == RWDS_BattlePoints)
  {
    var bpoints = BoundBy(GetPlrExtraData(iPlr, "CMC_BattlePoints"), 0, 0x7FFFFFFF);

    if((bpoints += BoundBy(GetPlayerData(RWDS_BattlePoints, iPlr), 0, 0x7FFFFFFF)) < 0)
      bpoints = 0x7FFFFFFF;

    return bpoints;
  }
  if(iType == RWDS_TeamPoints)
  {
    var tpoints = BoundBy(GetPlrExtraData(iPlr, "CMC_TeamPoints"), 0, 0x7FFFFFFF);

    if((tpoints += BoundBy(GetPlayerData(RWDS_TeamPoints, iPlr), 0, 0x7FFFFFFF)) < 0)
      tpoints = 0x7FFFFFFF;

    return tpoints;
  }
  if(iType == RWDS_KillCount)
  {
    var kcnt = BoundBy(GetPlrExtraData(iPlr, "CMC_KillCount"), 0, 0x7FFFFFFF);

    if((kcnt += BoundBy(GetPlayerData(RWDS_KillCount, iPlr), 0, 0x7FFFFFFF)) < 0)
      kcnt = 0x7FFFFFFF;

    return kcnt;
  }
  if(iType == RWDS_DeathCount)
  {
    var dcnt = BoundBy(GetPlrExtraData(iPlr, "CMC_DeathCount"), 0, 0x7FFFFFFF);

    if((dcnt += BoundBy(GetPlayerData(RWDS_DeathCount, iPlr), 0, 0x7FFFFFFF)) < 0)
      dcnt = 0x7FFFFFFF;

    return dcnt;
  }
}

global func ResetPlayerStats(int iPlr)
{
  SetPlrExtraData(iPlr, "CMC_BattlePoints", 0);
  SetPlrExtraData(iPlr, "CMC_TeamPoints", 0);
  SetPlrExtraData(iPlr, "CMC_KillCount", 0);
  SetPlrExtraData(iPlr, "CMC_DeathCount", 0);

  RecalcPlayerRank(iPlr);

  return true;
}

/* Statistikenmenü */

public func StatsStatistics(int iPlr)
{
  var pClonk = GetCursor(iPlr);
  if(GetMenu(pClonk))
  {
    if(GetMenu(pClonk)->~RWDS_MenuAbort())
      CloseMenu(pClonk);
    else
      return;
  }
  if(!CreateMenu(GetID(), pClonk, this, 0, 0, 0, C4MN_Style_Dialog)) return;

  aLastPage[iPlr] = [RWDS_Page_Statistics];

  AddMenuItem(" | ", "", RWDS, pClonk, 0, 0, "", 514, 0, 0);

  var bpoints = GetFullPlayerData(iPlr, RWDS_BattlePoints);
  var tpoints = GetFullPlayerData(iPlr, RWDS_TeamPoints);

  AddMenuItem("<c 33ccff>$Stats$</c>", 0, 0, pClonk);
  AddMenuItem(Format("$BattlePoints$|$TeamPoints$", bpoints, tpoints), 0, 0, pClonk);
  AddMenuItem(Format("$KillCount$|$DeathCount$", GetFullPlayerData(iPlr, RWDS_KillCount), GetFullPlayerData(iPlr, RWDS_DeathCount)), 0, 0, pClonk);

  var iGAchievementCnt = 0;
  for(var i = 1; i <= iAchievementCount; i++)
  {
    var idAchievement = C4Id(Format("AC%02d", i));
    if(GetPlayerAchievement(iPlr, idAchievement))
      iGAchievementCnt++;
  }

  AddMenuItem(Format("$AchievedAchievements$", iGAchievementCnt, iAchievementCount), 0, 0, pClonk);

  AddMenuItem(" | ", 0, 0, pClonk);

  var rank = GetPlayerRank(iPlr);
  if(rank != iRankCount-1)
    AddMenuItem(Format("$NextRank$", GetName(0, GetRankID(rank)), rank, GetName(0, C4Id(Format("R%03d", rank+1))), rank+1), 0, 0, pClonk);
  else
    AddMenuItem(Format("$MaxRankReached$", GetName(0, GetRankID(rank)), rank), 0, 0, pClonk);

  //Balkenanzeige einblenden, sofern nicht der letzte Rang
  if(rank != iRankCount-1)
  {
    AddMenuItem(" ", 0, 0, pClonk);

    var str = "";

    var k = 500 * rank ** 2;
    k -= (k % 1000) * (k > 1000);
    var k2 = 500 * (rank+1) ** 2;
    k2 -= (k2 % 1000) * (k2 > 1000);

    var diff = k2-k;
    var percent = ((bpoints + tpoints) - k) * 100 / diff;
    percent /= 5; var j = 20;
    var colored;
    var offset = 0;
    while(j > 0)
    {
      offset = 1;
      if(j == 1)
        offset = 2;
      if(j == 20)
        offset = 0;

      if(j == 20 && percent)
      {
        str = Format("<c %x>%s", 0xFFCC00, str);
        colored = true;
      }
      else if(!percent)
      {
        if(colored)
          str = Format("%s</c>", str);

        str = Format("%s<c 444444>", str);
      }

      str = Format("%s{{PBAR:%d}}", str, offset);

      percent--; j--;
    }

    AddMenuItem(Format("{{%i}} %d - %d {{%i}}", GetRankID(rank), (bpoints + tpoints), k2, GetRankID(rank+1)), 0, 0, pClonk);
    AddMenuItem(Format("%d $UntilRankUp$", k2-(bpoints + tpoints)), 0, 0, pClonk);

    AddMenuItem(Format("%s</c>", str), 0, 0, pClonk);
  }
  else
    AddMenuItem(Format("{{%i}} %d", GetRankID(rank), (bpoints + tpoints)), 0, 0, pClonk);

  AddMenuItem(" | ", 0, 0, pClonk);

  AddMenuItem("$PointTable$", Format("StatsPoints(%d)", iPlr), NONE, pClonk, 0, 0, "", C4MN_Add_ForceNoDesc);
  AddMenuItem("$Achievements$", Format("StatsList(%d, 0, 0, 2)", iPlr), NONE, pClonk, 0, 0, "", C4MN_Add_ForceNoDesc);
}

/* Achievements */

global func ResetAllPlayerAchievements()
{
  for(var i = 0; i < GetPlayerCount(); i++)
    ResetPlayerAchievements(GetPlayerByIndex(i));
  return true;
}

global func ResetPlayerAchievements(int iPlr)
{
  SetPlrExtraData(iPlr, "CMC_Achievements", 0);
  for(var i = 1; i <= (iAchievementCount/32)+1; i++)
    SetPlrExtraData(iPlr, Format("CMC_Achievements%d", i), 0);

  return true;
}

global func ResetPlayerRibbons(int iPlr)
{
  for(var i = 1; i <= (iRibbonCount/32)+1; i++)
    SetPlrExtraData(iPlr, Format("CMC_Ribbon%d", i), 0);

  //Gegebenenfalls eigenes Ribbon verteilen
  if(IsDeveloper(GetPlayerID(iPlr)))
  {
    var iRibbon = GetPlrExtraData(iPlr, "CMC_Team_Ribbon");
    if(iRibbon && GetName(0, C4Id(Format("RB%02d", iRibbon))))
    {
      AttemptAwardRibbon(C4Id(Format("RB%02d", iRibbon)), iPlr, iPlr);
    }
  }

  return true;
}

/* Abfrage */

public func GetPlayerAchievement(int iPlr, id idAchievement)
{
  if(!RewardsActive()) return;
  if(!idAchievement->IsAchievement()) return false;
  if(GetPlayerType(iPlr) != C4PT_User) return false;
  var iSlot = idAchievement->GetSavingSlot()-1;
  var iBlock = iSlot / 32 + 1;

  var iData = GetPlrExtraData(iPlr, Format("CMC_Achievements%d", iBlock));
  return iData >> (iSlot % 32) & 1;
}

public func SetPlayerAchievement(int iPlr, id idAchievement, bool fAwarded)
{
  if(!RewardsActive()) return;
  if(!idAchievement->IsAchievement()) return false;
  if(GetPlayerType(iPlr) != C4PT_User) return false;
  var iSlot = idAchievement->GetSavingSlot()-1;
  var iBlock = iSlot / 32 + 1;

  var iData = GetPlrExtraData(iPlr, Format("CMC_Achievements%d", iBlock));
  SetPlrExtraData(iPlr, Format("CMC_Achievements%d", iBlock), iData ^ fAwarded << (iSlot % 32));

  return true;
}

public func GetPlayerRibbon(int iPlr, id idRibbon)
{
  if(!RewardsActive()) return;
  if(!idRibbon->IsRibbon()) return false;
  if(GetPlayerType(iPlr) != C4PT_User) return false;
  var iSlot = idRibbon->GetSavingSlot()-1;
  var iBlock = iSlot / 32 + 1;

  var iData = GetPlrExtraData(iPlr, Format("CMC_Ribbon%d", iBlock));
  return iData >> (iSlot % 32) & 1;
}

public func SetPlayerRibbon(int iPlr, id idRibbon, bool fAwarded)
{
  if(!RewardsActive()) return;
  if(!idRibbon->IsRibbon()) return false;
  if(GetPlayerType(iPlr) != C4PT_User) return false;
  var iSlot = idRibbon->GetSavingSlot()-1;
  var iBlock = iSlot / 32 + 1;

  var iData = GetPlrExtraData(iPlr, Format("CMC_Ribbon%d", iBlock));
  SetPlrExtraData(iPlr, Format("CMC_Ribbon%d", iBlock), iData ^ fAwarded << (iSlot % 32));

  return true;
}

/* Achievements */

global func AwardAchievement(id idAchievement, int iPlr)
{
  //Vergabe berechtigt?
  if(!RewardsActive()) return;
  if(GetPlayerType(iPlr) != C4PT_User) return false;
  if(!idAchievement->IsAchievement()) return false;
  var db = FindObject2(Find_ID(RWDS));
  if(!db) return false;

  //Einmalige Achievements beachten
  if(IsAchievementBlocked(idAchievement)) return;
  RWDS_aAchievementBlocked[idAchievement->GetSavingSlot()] = idAchievement->~SingleAward();

  //Bereits vorhanden?
  if(db->GetPlayerAchievement(iPlr, idAchievement))
  {
    //Anzeige entsprechend der Einstellung des Spielers
    if(GetCursor(iPlr)->~AchievementDisplayType())
    {
      //Achievement-Anzeige mit goldenem Hintergrund
      var achievement = CreateObject(idAchievement, 0, 0, iPlr);
      achievement->SetHighlightColor(RGB(250,210,20));
      achievement->SetDurationTime(100);

      //Sound-Hinweis
      Sound("RWDS_Achievement.ogg", true, 0, 100, iPlr+1);
    }
    else
      return true;
  }
  else
  {
    //Vergabe
    db->SetPlayerAchievement(iPlr, idAchievement, true);

    //Achievement-Anzeige mit blauem Hintergrund
    var achievement = CreateObject(idAchievement, 0, 0, iPlr);
    achievement->SetHighlightColor(RGB(0,153,255));
    //Eventnachricht: Errungenschaft erhalten
    EventInfo4K(0, Format("$AchievementNewUnlocked$", GetPlrColorDw(iPlr), GetPlayerName(iPlr), GetName(0, idAchievement)), IC28, 0, 0, 0, "Info_Event.ogg");

    //Sound-Hinweis
    Sound("RWDS_FirstAchievement.ogg", true, 0, 100, iPlr+1);
  }

  return true;
}

global func ResetAchievementProgress(id idAchievement, int iPlr)
{
  if(!FindObject(RWDS)) return;
  var index = idAchievement->GetSavingSlot();
  if(aAchievementProgress[iPlr][index])
  {
    aAchievementProgress[iPlr][index] = 0;
  }
  return true;
}

global func DoAchievementProgress(int iProgress, id idAchievement, int iPlr)
{
  if(!RewardsActive()) return;
  var index = idAchievement->~GetSavingSlot();
  if(!index)
  {
    ErrorLog("Achievement %v without SavingSlot", idAchievement);
    return;
  }
  if(!aAchievementProgress[iPlr]) aAchievementProgress[iPlr] = CreateArray();
  aAchievementProgress[iPlr][index] += iProgress;
  if(aAchievementProgress[iPlr][index] >= idAchievement->~GetAchievementScore() && !IsAchievementBlocked(idAchievement))
  {
    ResetAchievementProgress(idAchievement, iPlr);
    return AwardAchievement(idAchievement, iPlr);
  }
  return true;
}

global func GetAchievementProgress(id idAchievement, int iPlr)
{
  if(!RewardsActive()) return;
  var index = idAchievement->GetSavingSlot();
  if(aAchievementProgress[iPlr][index])
  {
    return aAchievementProgress[iPlr][index];
  }
  return false;
}

global func IsAchievementBlocked(id idAchievement)
{
  if(!RewardsActive()) return;
  return RWDS_aAchievementBlocked[idAchievement->~GetSavingSlot()];
}

global func ResetAchievementExtra(id idAchievement, int iPlr)
{
  if(!RewardsActive()) return;
  var index = idAchievement->GetSavingSlot();
  aAchievementExtra[iPlr][index] = 0;
  return true;
}

global func SetAchievementExtra(data, id idAchievement, int iPlr)
{
  if(!RewardsActive()) return;
  var index = idAchievement->GetSavingSlot();
  aAchievementExtra[iPlr][index] = data;
  return true;
}

global func GetAchievementExtra(id idAchievement, int iPlr)
{
  if(!RewardsActive()) return;
  var index = idAchievement->GetSavingSlot();
  if(!aAchievementExtra[iPlr]) aAchievementExtra[iPlr] = CreateArray();
  if(aAchievementExtra[iPlr][index])
  {
    return aAchievementExtra[iPlr][index];
  }
  return false;
}

/* Ehrenbänder */

global func AttemptAwardRibbon(id idRibbon, int iPlr, int iPlrFrom)
{
  //Vergabe berechtigt?
  if(!RewardsActive()) return;
  if(GetPlayerType(iPlr) != C4PT_User || GetPlayerType(iPlrFrom) != C4PT_User) return false;
  if(iPlr != iPlrFrom && !Hostile(iPlr, iPlrFrom)) return false;
  if(!IsDeveloper(GetPlayerID(iPlrFrom))) return false;
  if(!idRibbon->IsRibbon()) return false;
  if(GetPlrExtraData(iPlrFrom, "CMC_Team_Ribbon") != idRibbon->GetSavingSlot()) return false;
  var db = FindObject2(Find_ID(RWDS));
  if(!db) return false;

  //Abbrechen, falls bereits erhalten
  if(db->GetPlayerRibbon(iPlr, idRibbon)) return;

  //Vergabe
  db->SetPlayerRibbon(iPlr, idRibbon, true);

  //Ehrenbandanzeige mit weißem Hintergrund
  var ribbon = CreateObject(idRibbon, 0, 0, iPlr);
  ribbon->SetHighlightColor(RGB(255,255,255));
  if(iPlr == iPlrFrom)
  {
    ribbon->SetCustomDesc(Format("You are %s.", GetName(ribbon)));
  }
  else
  {
    //Eventnachricht: Ehrenband erhalten
    EventInfo4K(0, Format("$RibbonAwarded$", GetPlrColorDw(iPlr), GetPlayerName(iPlr), GetName(0, idRibbon), GetPlrColorDw(iPlrFrom), GetPlayerName(iPlrFrom)), IC28, 0, 0, 0, "Info_Event.ogg");
  }

  //Sound-Hinweis
  Sound("RWDS_Ribbon.ogg", true, 0, 100, iPlr+1);

  return true;
}

/* Punkteanzeige */

global func FxPointMessageStart(pTarget, iNo, iTemp, szString)
{
  if(iTemp)
    return -1;

  EffectVar(0,pTarget,iNo) = szString;	//Nachricht
  EffectVar(2,pTarget,iNo) = 0;		//Zeit
}

global func FxPointMessageTimer(object pTarget, int iEffectNumber)
{
  var pObject, iIndex, pContainer, i, j, iPlr;
  if(!EffectVar(2,pTarget,iEffectNumber))
  {
    if(pContainer = Contained(pTarget))
    {
      for(i = 0; pObject = Contents(i, pContainer); i++)
        for(j = 0; iIndex = GetEffect("PointMessage", pObject, j); j++)
          if(EffectVar(2,pObject,iIndex) > 0) return(FX_OK);
    }
    else
    {
      pObject = pTarget;
      for(i = 0; iIndex = GetEffect("PointMessage", pObject, i); i++)
        if(EffectVar(2,pObject,iIndex) > 0) return(FX_OK);
    }
    //Sound
    Sound("RWDS_Points*.ogg", 0, pContainer, 100, GetOwner(pTarget)+1);

    var iPlr = GetOwner(pTarget);
    AddEffect("PointPlayerMessage", CreateObject(ARHL,0,0,-1), 130, 1, 0, 0, iPlr, EffectVar(0,pTarget,iEffectNumber));
  }
  EffectVar(2, pTarget, iEffectNumber)++;
  if(-50+EffectVar(2,pTarget,iEffectNumber)*5 > 255)
    return(-1);

  return(FX_OK);
}

global func FxPointPlayerMessageStart(object target, int nr, int temp, int plr, string szMsg)
{
  EffectVar(0, target, nr) = plr;
  EffectVar(1, target, nr) = szMsg;
  EffectVar(2, target, nr) = 0;
}

global func FxPointPlayerMessageTimer(object target, int nr) 
{ 
  var iTime = EffectVar(2, target, nr); 
  CustomMessage(EffectVar(1,target,nr),target,EffectVar(0,target,nr),0,-iTime/2,RGBa(255,255,255,BoundBy(-50+iTime*5,0,255)));

  EffectVar(2, target, nr)++;
  if(-50+iTime*5 > 255)
  {
    RemoveObject(target);
    return -1;
  }

  return FX_OK;
}

/* Internes Handling */

public func SetPlayerData(xData, int iType, int iPlr)
{
  if(!aData[iPlr]) aData[iPlr] = CreateArray();
  if(iType == RWDS_TotalPoints) return;
  aData[iPlr][iType] = xData;
  return true;
}

public func GetPlayerData(int iType, int iPlr)
{
  if(!aData[iPlr]) aData[iPlr] = CreateArray();
  if(iType == RWDS_TotalPoints)
    {return aData[iPlr][RWDS_BattlePoints] + aData[iPlr][RWDS_TeamPoints] + aData[iPlr][RWDS_MinusPoints];}
  return aData[iPlr][iType];
}