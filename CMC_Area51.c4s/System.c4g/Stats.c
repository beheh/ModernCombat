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
	if(GetType(aRanks) == C4V_Array && GetType(aRanks[iPlr]) == C4V_Array)
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
	var bpoints = BoundBy(GetPlrExtraData(iPlr, "CMC_BattlePoints"), 0, 0x7FFFFFFF);
	var tpoints = BoundBy(GetPlrExtraData(iPlr, "CMC_TeamPoints"), 0, 0x7FFFFFFF);;
	var kcnt = BoundBy(GetPlrExtraData(iPlr, "CMC_KillCount"), 0, 0x7FFFFFFF);
	var dcnt = BoundBy(GetPlrExtraData(iPlr, "CMC_DeathCount"), 0, 0x7FFFFFFF);
	
	if((bpoints += BoundBy(GetPlayerData(RWDS_BattlePoints, iPlr), 0, 0x7FFFFFFF)) < 0)
		bpoints = 0x7FFFFFFF;
	if((tpoints += BoundBy(GetPlayerData(RWDS_TeamPoints, iPlr), 0, 0x7FFFFFFF)) < 0)
		tpoints = 0x7FFFFFFF;
	if((kcnt += BoundBy(GetPlayerData(RWDS_KillCount, iPlr), 0, 0x7FFFFFFF)) < 0)
		kcnt = 0x7FFFFFFF;
	if((dcnt += BoundBy(GetPlayerData(RWDS_DeathCount, iPlr), 0, 0x7FFFFFFF)) < 0)
		dcnt = 0x7FFFFFFF;
	
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
