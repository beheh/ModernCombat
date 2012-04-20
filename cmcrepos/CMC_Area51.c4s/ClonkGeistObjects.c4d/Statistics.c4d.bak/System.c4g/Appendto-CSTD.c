/*-- Neues Script --*/

#strict 2

#appendto CSTD

public func InitializePlayer(int iPlr)
{
	LoadRanks2Cache();
	var db;
	if(db = FindObject(RWDS))
		db->SetPlayerData(db->GetFullPlayerData(iPlr, RWDS_BattlePoints) + db->GetFullPlayerData(iPlr, RWDS_TeamPoints), RWDS_StartTotalPoints, iPlr);
	
	return _inherited(iPlr, ...);
}
