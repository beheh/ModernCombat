/*-- Neues Script --*/

#strict 2

#appendto RWDS

static aRanks, iRankCount;

static const RWDS_KillCount = 6;
static const RWDS_DeathCount = 7;

/** Rang- und Statistiksystem **/

// Ladt alle Ränge in den Ränge-Cache.
global func LoadRanks2Cache()
{
	if(!iRankCount)
	{
		iRankCount = 0;
  	while(GetName(0, C4Id(Format("RG%02d", iRankCount))))
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

global func DebugStatMenu() {  FindObject(RWDS)->StatsStatistics(0); }

public func StatsStatistics(int iPlr)
{
  var pClonk = GetCursor(iPlr);
  if(GetMenu(pClonk)) CloseMenu(pClonk);
  if(!CreateMenu(GetID(), pClonk, this, 0, 0, 0, C4MN_Style_Dialog)) return;
  AddMenuItem(" | ", "", RWDS, pClonk, 0, 0, "", 514, 0, 0);
  
  var bpoints = GetFullPlayerData(iPlr, RWDS_BattlePoints);
  var tpoints = GetFullPlayerData(iPlr, RWDS_TeamPoints);
  
  AddMenuItem("$PlayerStats$", 0, 0, pClonk);
  AddMenuItem(Format("$BattlePoints$", bpoints), 0, 0, pClonk);
  AddMenuItem(Format("$TeamPoints$", tpoints), 0, 0, pClonk);
  AddMenuItem(Format("$KillCount$", GetFullPlayerData(iPlr, RWDS_KillCount)), 0, 0, pClonk);
  AddMenuItem(Format("$DeathCount$", GetFullPlayerData(iPlr, RWDS_DeathCount)), 0, 0, pClonk);
  
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
  if(rank != iRankCount)
		AddMenuItem(Format("$NextRank$", GetName(0, C4Id(Format("RG%02d", rank))), rank, GetName(0, C4Id(Format("RG%02d", rank+1))), rank+1), 0, 0, pClonk);
	else
		AddMenuItem(Format("$YourRank$", GetName(0, C4Id(Format("RG%02d", rank))), rank), 0, 0, pClonk);
  
  if(rank != iRankCount)
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
			/*if(percent > 0)
				clr = GetPlrColorDw(iPlr);
			else
				clr = 0xFFFFFF;*/
			
			str = Format("%s{{%i}}", str, barID);
		
			percent--; j--;
		}
		
		AddMenuItem(Format("{{%i}} (%d) %s</c> (%d) {{%i}}", C4Id(Format("RG%02d", rank)), (bpoints + tpoints), str, k2, C4Id(Format("RG%02d", rank+1))), 0, 0, pClonk);
  }
  
  AddMenuItem(" | ", 0, 0, pClonk);
  
  AddMenuItem("$ShowAchievements$", Format("StatsList(%d, 0, 0, 2)", iPlr), NONE, pClonk, 0, 0, "", C4MN_Add_ForceNoDesc);
}






