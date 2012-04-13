/*-- Neues Script --*/

#strict 2

#appendto RWDS

static aRanks, iRankCount;

static const RWDS_KillCount = 6;
static const RWDS_DeathCount = 7;

/** Rang- und Statistiksystem **/

global func GetRankID(int iRank) { return C4Id(Format("RG%02d", iRank)); }

// Ladt alle Ränge in den Ränge-Cache.
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

global func RecalcPlayerRank(int iPlr) // Berechnet den neuen Spielerrangwert und speichert diesen.
{
	if(GetType(aRanks) == C4V_Array)
	{
		var nRank = CalcRank(iPlr);
		
		if(nRank != aRanks[iPlr])
		{
			aRanks[iPlr] = nRank;
			
    	//var info = CreateObject(GetRankID(nRank), 0, 0, iPlr);
    	//info->SetHighlightColor(RGB(0,153,255));
    	EventInfo4K(0, Format("$YouHaveB33nPromoted$", GetTaggedPlayerName(iPlr), GetName(0, GetRankID(nRank))), GetRankID(nRank), 0, 0, 0, "PriorityInfo.ogg");
    	//EventInfo4K(0, Format("$YouHaveB33nPromoted$", GetTaggedPlayerName(iPlr), GetName(info)), GetID(info), 0, 0, 0, "PriorityInfo.ogg");
 		}
		return aRanks[iPlr];
	}
	else
		return -1;
}

// Berechnet den Rang für iPlr.
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

public func SavePlrStatistics(int iPlr)
{
	var bpoints = GetFullPlayerData(iPlr, RWDS_BattlePoints);
	var tpoints = GetFullPlayerData(iPlr, RWDS_TeamPoints);
	var kcnt = GetFullPlayerData(iPlr, RWDS_KillCount);
	var dcnt = GetFullPlayerData(iPlr, RWDS_DeathCount);
	
	SetPlrExtraData(iPlr, "CMC_BattlePoints", bpoints);
	SetPlrExtraData(iPlr, "CMC_TeamPoints", tpoints);
	SetPlrExtraData(iPlr, "CMC_KillCount", kcnt);
	SetPlrExtraData(iPlr, "CMC_DeathCount", dcnt);
	
	return true;
}

public func OnGameOver()
{
	for(var i = 0; i < GetPlayerCount(C4PT_User); i++)
		SavePlrStatistics(GetPlayerByIndex(i, C4PT_User));
		
	return true;
}

global func EliminatePlayer(int iPlr)
{
	if(FindObject(RWDS))
		FindObject(RWDS)->SavePlrStatistics(iPlr);
		
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

public func StatsStatistics(int iPlr)
{
  var pClonk = GetCursor(iPlr);
  if(GetMenu(pClonk)) CloseMenu(pClonk);
  if(!CreateMenu(GetID(), pClonk, this, 0, 0, 0, C4MN_Style_Dialog)) return;
  AddMenuItem(" | ", "", RWDS, pClonk, 0, 0, "", 514, 0, 0);
  
  var bpoints = GetFullPlayerData(iPlr, RWDS_BattlePoints);
  var tpoints = GetFullPlayerData(iPlr, RWDS_TeamPoints);
  
  AddMenuItem("$PlayerStats$", 0, 0, pClonk);
  AddMenuItem(Format("$BattlePoints$|$TeamPoints$", bpoints, tpoints), 0, 0, pClonk);
  //AddMenuItem(Format("$TeamPoints$", tpoints), 0, 0, pClonk);
  AddMenuItem(Format("$KillCount$|$DeathCount$", GetFullPlayerData(iPlr, RWDS_KillCount), GetFullPlayerData(iPlr, RWDS_DeathCount)), 0, 0, pClonk);
 // AddMenuItem(Format("$DeathCount$", GetFullPlayerData(iPlr, RWDS_DeathCount)), 0, 0, pClonk);
  
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
		while(j > 0)
		{
			var barID = PBP1;
			if(j == 1)
				barID = PBP2;
			if(j == 20)
				barID = PBP0;
		
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
			
			str = Format("%s{{%i}}", str, barID);
		
			percent--; j--;
		}
		
		AddMenuItem(Format("{{%i}} (%d) - %d - (%d) {{%i}}", GetRankID(rank), (bpoints + tpoints), k2-(bpoints + tpoints), k2, GetRankID(rank+1)), 0, 0, pClonk);
  	AddMenuItem(Format("%s</c>", str), 0, 0, pClonk);
  }
  
  AddMenuItem(" | ", 0, 0, pClonk);
  
  AddMenuItem("$ShowPoints$", Format("StatsPoints(%d)", iPlr), NONE, pClonk, 0, 0, "", C4MN_Add_ForceNoDesc);
  AddMenuItem("$ShowAchievements$", Format("StatsList(%d, 0, 0, 2)", iPlr), NONE, pClonk, 0, 0, "", C4MN_Add_ForceNoDesc);
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
    szString = Format("%s: %d $Points$", GetPlayerData(RWDS_PlayerName, iPlayer), GetPlayerPoints(RWDS_TotalPoints, iPlayer));
               aList[iTeam][GetLength(aList[iTeam])] = szString;
               iPlayer++;
  }
  //Nach Team ausgeben
  AddMenuItem("<c ffff33>$CurrentPoints$</c>", 0, NONE, pClonk);

  for(var aTeam in aList)
    if(aTeam)
      for(var szString in aTeam)
        if(szString)
          AddMenuItem(szString, 0, NONE, pClonk);

  //Leerzeile
  AddMenuItem(" ", 0, NONE, pClonk);

  //Eigene Errungenschaften anzeigen
  AddMenuItem("$ShowAchievements$", Format("StatsList(%d, 0, 0, 2)", iPlr), NONE, pClonk, 0, 0, "", C4MN_Add_ForceNoDesc);
  AddMenuItem("$ShowStats$", Format("StatsStatistics(%d)", iPlr), NONE, pClonk, 0, 0, "", C4MN_Add_ForceNoDesc);
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
	AddMenuItem(Format("<c ffff33>$MyAchievements$ ($showing$ %d/%d)</c>", BoundBy(iOffset+10, 0, iAchievementCount), iAchievementCount), 0, NONE, pClonk);

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

  //Zurück zur Punkteübersicht
  AddMenuItem("$ShowPoints$", Format("StatsPoints(%d)", iPlr), NONE, pClonk, 0, 0, "", C4MN_Add_ForceNoDesc);
	AddMenuItem("$ShowStats$", Format("StatsStatistics(%d)", iPlr), NONE, pClonk, 0, 0, "", C4MN_Add_ForceNoDesc);
	
  if(iIndex)
    SelectMenuItem(iIndex+1, pClonk);
  else
    SelectMenuItem(i-iOffset+2+iMenuEntry, pClonk);  	
    
  return true;
}

///////////////////////////////////////////////////////////////////////////////////

global func DoPlayerPoints(int iPoints, int iType, int iPlr, object pClonk, id idIcon)
{
  var db = FindObject2(Find_ID(RWDS));
  if(!db) return;
  if(!iPoints) return;
  if(iType != RWDS_BattlePoints && iType != RWDS_TeamPoints && iType != RWDS_MinusPoints)
    return ErrorLog("Invalid points type %d for %d points at %v", iType, iPoints, pClonk);
  if(db->SetPlayerData(db->GetPlayerPoints(iType, iPlr)+iPoints, iType, iPlr))
  {
    //Achievement-Fortschritt (Point Hunter)
    DoAchievementProgress(iPoints, AC13, iPlr);
    
    //Den neuen Rang berechnen...
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

global func GetTaggedPlayerName(int iPlr, bool fRank)
{
	var rank = GetPlayerRank(iPlr);
	if(fRank)
		return Format("{{%i}} %s <c %x>%s</c>", rank, GetName(0, rank), GetPlrColorDw(iPlr), GetPlayerName(iPlr));
		
	return _inherited(iPlr);
}



