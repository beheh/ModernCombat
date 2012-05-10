/*-- Belohnungssystem --*/

#strict 2

static aAchievementProgress, aAchievementExtra, iAchievementCount;
static aRanks, iRankCount;
local aData, fEvaluation, aStats;

public func IsChooseable()	{return true;}


/* Initialisierung */

protected func Initialize()
{
  aData = CreateArray();
  fEvaluation = false;
  aAchievementProgress = CreateArray();
  aAchievementExtra = CreateArray();
  iAchievementCount = 0;
  while(GetName(0, C4Id(Format("AC%02d", iAchievementCount+1))))
  {
    iAchievementCount++;
  }

  for(var i = 0; i < GetPlayerCount(C4PT_User); i++)
  {
    var iPlr = GetPlayerByIndex(i, C4PT_User);
    SetPlayerData(GetFullPlayerData(iPlr, RWDS_BattlePoints) + GetFullPlayerData(iPlr, RWDS_TeamPoints), RWDS_StartTotalPoints, iPlr);
  }

  UpdatePlayers();
}

protected func Activate(iByPlayer)
{
  StatsPoints(iByPlayer);
  return 1;
}

public func StatsPoints(int iPlr)
{
  var pClonk = GetCursor(iPlr);
  if(GetMenu(pClonk)) CloseMenu(pClonk);
  if(!CreateMenu(GetID(),pClonk,this,0,0,0,C4MN_Style_Dialog)) return;
  AddMenuItem(" | ", "", RWDS, pClonk, 0, 0, "", 514, 0, 0);
  
  //Erst mal einsortieren
  var aList = CreateArray();
  var szString;
  var iPlayer = 0;
  while(aData[iPlayer] != 0)
  {
    var iTeam = GetPlayerData(RWDS_PlayerTeam, iPlayer);
    if(!aList[iTeam]) aList[iTeam] = CreateArray();
    szString = Format("%s:| %d $Points$", GetPlayerData(RWDS_CPlayerName, iPlayer), GetPlayerPoints(RWDS_TotalPoints, iPlayer));
               aList[iTeam][GetLength(aList[iTeam])] = szString;
               iPlayer++;
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
    CloseMenu(pClonk);
  if(!CreateMenu(GetID(), pClonk, this, 0, 0, 0, C4MN_Style_Dialog)) return;
  AddMenuItem(" | ", 0, RWDS, pClonk, 0, 0, "", 514, 0, 0);

  var iData = GetPlrExtraData(iPlr, "CMC_Achievements");

  //Überschrift
  AddMenuItem(Format("<c 33ccff>$Achievements$ ($Showing$)</c>", iOffset+1, BoundBy(iOffset+10, 1, iAchievementCount), iAchievementCount), 0, NONE, pClonk);

  //Liste
  var i = 1+iOffset;
  var idAchievement;
  while(i <= iAchievementCount && i <= 10+iOffset)
  {
    idAchievement = C4Id(Format("AC%02d", i));
    if(iData >> i & 1)
      AddMenuItem(Format("<c ffff33>%s</c>", GetName(0, idAchievement)), Format("StatsAchievement(%d, %d, %d)", iPlr, i, iOffset), NONE, pClonk, 0, 0, "", C4MN_Add_ForceNoDesc);
    else
      AddMenuItem(Format("<c 777777>%s</c>", GetName(0, idAchievement)), Format("StatsAchievement(%d, %d, %d)", iPlr, i, iOffset), NONE, pClonk, 0, 0, "", 0, 0, 0);
    i++;
  }

  //Leerzeile
  AddMenuItem(" ", 0, NONE, pClonk);

  //Weiter
  if(iOffset+10 <= iAchievementCount)
    AddMenuItem("$NextPage$", Format("StatsList(%d, %d, %d, 0)", iPlr, 0, BoundBy(iOffset+10, 0, iAchievementCount)), NONE, pClonk, 0, 0, "", C4MN_Add_ForceNoDesc);
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
  if(GetMenu(pClonk)) CloseMenu(pClonk);
  if(!CreateMenu(GetID(),pClonk,this,0,0,0,C4MN_Style_Dialog)) return;
  AddMenuItem(" | ", 0, RWDS, pClonk, 0, 0, "", 514);

  var iData = GetPlrExtraData(iPlr, "CMC_Achievements");
  var iIndex = iSelect;
  var idAchievement = C4Id(Format("AC%02d", iIndex));
  if(iData >> iSelect & 1)
  {
    AddMenuItem(Format("<c ffff33>%s</c>", GetName(0, idAchievement)), 0, NONE, pClonk, 0, 0, "", C4MN_Add_ForceNoDesc);
    AddMenuItem(Format("%s", GetDesc(0, idAchievement)), 0, NONE, pClonk, 0, 0, "", C4MN_Add_ForceNoDesc);
  }
  else
  {
    Sound("Error", true, 0, 100, iPlr + 1);
    return StatsList(iPlr, iSelect-iOffset, iOffset);

  }

  //Leerzeile
  AddMenuItem(" ", 0, NONE, pClonk);

  //Zurück
  AddMenuItem("$Back$", Format("StatsList(%d, %d, %d)", iPlr, iSelect-iOffset, iOffset), NONE, pClonk, 0, 0, "", C4MN_Add_ForceNoDesc);
  return true;
}

global func RewardsActive()
{
  return FindObject2(Find_ID(RWDS)) != false;
}

/* Auswerten */

public func RemovePlayer(int iPlr)
{
  if(iPlr == -1) return false;

  //Auswertungsdialog aufrufen
  SavePlrStatistics(iPlr);
}

global func RewardEvaluation()
{
  //Nur mit Belohnungen-Regel fortfahren
  var db = FindObject(RWDS);
  if(!db) return;

  //Auswertung starten
  db->Evaluate();
}

public func Evaluate()
{
  //Einmalige Auswertung starten
  if(fEvaluation) return 0;
  fEvaluation = true;
  var db = this;

  //Sortieren
  var aList = CreateArray();
  var iPlr, szFirstLine, szSecondLine;
  var iPlr = 0;

  //Endpunktzahl aktualisieren und Statistiken speichern
  for(var i = 0; i < GetPlayerCount(C4PT_User); i++)
    SavePlrStatistics(GetPlayerByIndex(i, C4PT_User));

  //Kopfzeilen erstellen
  //while(db->GetData()[iPlr] != 0)

  for(var iPlr = 0; iPlr < GetLength(db->GetData()); iPlr++)
  {
    if(!db->GetData()[iPlr])
      continue;

    if(!aList[GetPlayerTeam(iPlr)]) aList[GetPlayerTeam(iPlr)] = CreateArray();

    szFirstLine = Format("$FirstLine$",									//Erste Zeile
    db->GetPlayerPoints(RWDS_PlayerName, iPlr),								//Spielername
    db->GetPlayerData(RWDS_StartTotalPoints, iPlr),							//Gesamtpunktzahl am Anfang
    (db->GetPlayerPoints(RWDS_BattlePoints, iPlr) + db->GetPlayerPoints(RWDS_TeamPoints, iPlr)),	//Gesamtpunktzahl der Runde
    db->GetPlayerData(RWDS_SavedTotalPoints, iPlr));							//Gesamtpunktzahl

    szSecondLine = Format("$SecondLine$",								//Dritte Zeile
    db->GetPlayerPoints(RWDS_BattlePoints, iPlr),							//Gefechtspunkte
    db->GetPlayerPoints(RWDS_TeamPoints, iPlr),								//Teampunkte
    db->GetPlayerPoints(RWDS_MinusPoints, iPlr));							//Minuspunkte

    aList[GetPlayerTeam(iPlr)][GetLength(aList[GetPlayerTeam(iPlr)])] = [szFirstLine, szSecondLine];
    AddEvaluationData(Format("$PlayerLine$", db->GetPlayerData(RWDS_KillCount, iPlr), db->GetPlayerData(RWDS_DeathCount, iPlr), db->GetPlayerData(RWDS_SavedTotalPoints, iPlr)), db->GetPlayerData(RWDS_PlayerID, iPlr));
  }

  //Teamweise Auflistung der Daten
  for(var aTeam in aList)
  {
    if(!aTeam) continue;
    for(var aStrings in aTeam)
    {
      AddEvaluationData(aStrings[0], 0);
      AddEvaluationData(aStrings[1], 0);
    }
  }
  return 1;
}

/* Spieler updaten */

public func UpdatePlayers()
{
  if(!RewardsActive()) return;
  var iOffset = 0;
  for(var i = 0; i < GetPlayerCount()+iOffset; i++)
  {
    var iPlr = GetPlayerByIndex(i);
    if(!GetPlayerName(iPlr) && GetPlayerData(RWDS_PlayerName, iPlr))
    {
      iOffset++;
      continue;
    }
    
    SetPlayerData(GetTaggedPlayerName(iPlr, true), RWDS_PlayerName, iPlr);
    SetPlayerData(GetTaggedPlayerName(iPlr, true, true), RWDS_CPlayerName, iPlr);
    SetPlayerData(GetPlayerTeam(iPlr), RWDS_PlayerTeam, iPlr);
    SetPlayerData(GetPlayerID(iPlr), RWDS_PlayerID, iPlr);
    if(!aData[iPlr]) aData[iPlr] = CreateArray();
    if(!aAchievementProgress[iPlr]) aAchievementProgress[iPlr] = CreateArray();
    if(!aAchievementExtra[iPlr]) aAchievementExtra[iPlr] = CreateArray();
  }
  return 1;
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
  if(!db) return;		//Keine Belohnungen-Regel?
  if(!iPoints) return;		//Kein Wert wiedergegeben?
  if(iPoints > 50) return;	//Wert über 50?
  if(iType != RWDS_BattlePoints && iType != RWDS_TeamPoints && iType != RWDS_MinusPoints)
    return ErrorLog("Invalid points type %d for %d points at %v", iType, iPoints, pClonk);
  if(db->SetPlayerData(db->GetPlayerPoints(iType, iPlr)+iPoints, iType, iPlr))
  {
    //Achievement-Fortschritt (Point Hunter)
    DoAchievementProgress(iPoints, AC13, iPlr);
    
    //Neuen Rang berechnen
    if(iType == RWDS_TeamPoints || iType == RWDS_BattlePoints)
      RecalcPlayerRank(iPlr);
    if(pClonk)
    {
      if(!idIcon) idIcon = RWDS;
      var szMsg;
      if(iPoints < 0) szMsg = Format("{{%i}} <c ff0000>%d</c>", idIcon, iPoints);
      if(iPoints > 0) szMsg = Format("{{%i}} <c 00ff00>+%d</c>", idIcon, iPoints);
      if(iPoints == 0) szMsg = Format("{{%i}} <c ffff00>+%d</c>", idIcon, iPoints);
      pClonk->AddEffect("PointMessage", pClonk, 130, 1, pClonk, 0, szMsg);
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

global func GetRankID(int iRank)	{return C4Id(Format("RG%02d", iRank));}

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
    aRanks[GetPlayerByIndex(i, C4PT_User)] = CalcRank(GetPlayerByIndex(i, C4PT_User)); 

  return true;
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
      EventInfo4K(0, Format("$YouHaveBeenPromoted$", GetTaggedPlayerName(iPlr), GetName(info)), GetID(info), 0, 0, 0, "PriorityInfo.ogg");

      //Sound-Hinweis
      Sound("RankUp.ogg", true, 0, 100, iPlr+1);
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

  if((rewards = FindObject(RWDS)))
    if((rpoints += BoundBy(rewards->GetPlayerData(RWDS_BattlePoints, iPlr), 0, 0x7FFFFFFF) + BoundBy(rewards->GetPlayerData(RWDS_TeamPoints, iPlr), 0, 0x7FFFFFFF)) < 0)
      rpoints = 0x7FFFFFFF;

  var rank;

  for(var j = 0; j < iRankCount; j++)
  {
    var k = 225 * j ** 2;
    k -= (k % 1000) * (k > 1000); // benötigte Punktzahl (unter Grenze)
    var k2 = 225 * (j+1) ** 2;
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

global func GetTaggedPlayerName(int iPlr, bool fRank, bool fCompact)
{
  var rank = GetRankID(GetPlayerRank(iPlr));
  if(fRank && FindObject2(Find_Or(Find_ID(RWDS), Find_ID(CHOS))) && GetPlayerTeam(iPlr) != -1)
  {
    if(fCompact)
      return Format("{{%i}} <c %x>%s</c>", rank, GetPlrColorDw(iPlr), GetPlayerName(iPlr));
    
    return Format("{{%i}} %s <c %x>%s</c>", rank, GetName(0, rank), GetPlrColorDw(iPlr), GetPlayerName(iPlr));
  }

  return _inherited(iPlr);
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

/*global func EliminatePlayer(int iPlr)
{
  if(FindObject(RWDS))
    FindObject(RWDS)->SavePlrStatistics(iPlr);

  return _inherited(iPlr, ...);
}*/

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
  if(GetMenu(pClonk)) CloseMenu(pClonk);
  if(!CreateMenu(GetID(), pClonk, this, 0, 0, 0, C4MN_Style_Dialog)) return;
  AddMenuItem(" | ", "", RWDS, pClonk, 0, 0, "", 514, 0, 0);

  var bpoints = GetFullPlayerData(iPlr, RWDS_BattlePoints);
  var tpoints = GetFullPlayerData(iPlr, RWDS_TeamPoints);

  AddMenuItem("<c 33ccff>$Stats$</c>", 0, 0, pClonk);
  AddMenuItem(Format("$BattlePoints$|$TeamPoints$", bpoints, tpoints), 0, 0, pClonk);
  AddMenuItem(Format("$KillCount$|$DeathCount$", GetFullPlayerData(iPlr, RWDS_KillCount), GetFullPlayerData(iPlr, RWDS_DeathCount)), 0, 0, pClonk);

  var iGAchievementCnt = 0;
  var iData = GetPlrExtraData(iPlr, "CMC_Achievements");
  for(var i = 1; i <= iAchievementCount; i++)
  {
    if(iData >> i & 1)
      iGAchievementCnt++;
  }

  AddMenuItem(Format("$AchievedAchievements$", iGAchievementCnt, iAchievementCount), 0, 0, pClonk);

  AddMenuItem(" | ", 0, 0, pClonk);

  var rank = GetPlayerRank(iPlr);
  if(rank != iRankCount-1)
    AddMenuItem(Format("$NextRank$", GetName(0, GetRankID(rank)), rank, GetName(0, C4Id(Format("RG%02d", rank+1))), rank+1), 0, 0, pClonk);
  else
    AddMenuItem(Format("$YourRank$", GetName(0, GetRankID(rank)), rank), 0, 0, pClonk);

  //Balkenanzeige einblenden, sofern nicht der letzte Rang
  if(rank != iRankCount-1)
  {
    AddMenuItem(" ", 0, 0, pClonk);

    var str = "";

     var k = 225 * rank ** 2;
    k -= (k % 1000) * (k > 1000);
    var k2 = 225 * (rank+1) ** 2;
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
        str = Format("<c %x>%s", GetPlrColorDw(iPlr), str);
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

    AddMenuItem(Format("{{%i}} (%d) - %d - (%d) {{%i}}", GetRankID(rank), (bpoints + tpoints), k2-(bpoints + tpoints), k2, GetRankID(rank+1)), 0, 0, pClonk);
    AddMenuItem(Format("%s</c>", str), 0, 0, pClonk);
  }

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
  return true;
}

/* Vergabe */

global func AwardAchievement(id idAchievement, int iPlr)
{
  //Vergabe berechtigt?
  if(!RewardsActive()) return;
  if(GetPlayerType(iPlr) != C4PT_User) return false;
  if(!idAchievement->IsAchievement()) return false;

  //Achievement in Spielerdatei schreiben sofern nicht vorhanden
  var iData = GetPlrExtraData(iPlr, "CMC_Achievements");
  if(!(iData >> idAchievement->GetSavingSlot() & 1))
  {
    SetPlrExtraData(iPlr, "CMC_Achievements", iData ^ 1 << idAchievement->GetSavingSlot());

    //Achievementanzeige mit blauem Hintergrund
    var achievement = CreateObject(idAchievement, 0, 0, iPlr);
    achievement->SetHighlightColor(RGB(0,153,255));
    EventInfo4K(0, Format("$AchievementNewUnlocked$", GetPlrColorDw(iPlr), GetPlayerName(iPlr), GetName(0, idAchievement)), RWDS, 0, 0, 0, "PriorityInfo.ogg");
  }
  else
  {
    //Achievementanzeige mit Standardhintergrund
    CreateObject(idAchievement, 0, 0, iPlr);
    EventInfo4K(0, Format("$AchievementUnlocked$", GetPlrColorDw(iPlr), GetPlayerName(iPlr), GetName(0, idAchievement)), RWDS, 0, 0, 0, "PriorityInfo.ogg");
  }

  //Sound-Hinweis
  Sound("AchievementGet.ogg", true, 0, 100, iPlr+1);

  //Achievement-Fortschritt zurücksetzen
  ResetAchievementExtra(idAchievement, iPlr);
  ResetAchievementProgress(idAchievement, iPlr);

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
  if(aAchievementProgress[iPlr][index] >= idAchievement->~GetAchievementScore())
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

/* Punkteanzeige */

global func FxPointMessageStart(pTarget, iNo, iTemp, szString)
{
  if(iTemp)
    return -1;

  EffectVar(0,pTarget,iNo) = szString; //Nachricht
  EffectVar(2,pTarget,iNo) = 0; //Zeit
}

global func FxPointMessageTimer(pTarget, iNo)
{
  var iTime = EffectVar(2,pTarget,iNo);
  if(!iTime)
  {
    var pObject, index;
    if(Contained(pTarget))
    {
      var pContainer = Contained(pTarget);
      var j = 0;
      while((pObject = Contents(j, pContainer)) != false)
      {
        var i = 0;
        while((index = GetEffect("PointMessage", pObject, i)) != 0)
        {
          if(EffectVar(2,pObject,index) > 0) return FX_OK;
          i++;
        }
        j++;
      }
    }
    else
    {
      pObject = pTarget;
      var i = 0;
      while((index = GetEffect("PointMessage", pObject, i)) != 0)
      {
        if(EffectVar(2,pObject,index) > 0) return FX_OK;
        i++;
      }
    }
    //Sound
    Sound("PointsGet.ogg", 0, pContainer, 100);

    var plr = GetOwner(pTarget);
    for(var i = 0; i < GetPlayerCount(); i++)
    {
      if(Hostile(plr, i)) continue;
      AddEffect("PointPlayerMessage", CreateObject(ARHL,0,0,-1), 130, 1, 0, 0, i, EffectVar(0,pTarget,iNo));
    }
  }
  EffectVar(2, pTarget, iNo)++;
  if(-50+iTime*5 > 255)
  {
    return -1;
  }
  return FX_OK;
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
