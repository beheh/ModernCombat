/*-- Capture the Flag --*/

//Capture the Flag erstellt bei Spielstart Spielzielhinweise.

#strict 2

#appendto GCTF

public func GoalExtraValue()	{return iWinScore;}	//Spielzielinformationen an Scoreboard weitergeben


public func ChooserFinished()
{
  //Spielzielhinweise erstellen
  for(var i = 0; i < GetPlayerCount(); i++)
  {
    DoScoreboardShow(1, GetPlayerByIndex(i) + 1);
    CreateObject(TK06, 0, 0, GetPlayerByIndex(i));
    Sound("ObjectiveReceipt.ogg", true, 0, 100, GetPlayerByIndex(i) + 1);
  }
  return _inherited(...);
}

private func SetFlagMarker(int iPlr, int iCTeam, bool fRemove)
{
  var iCol = TEAM_PlayerColumn, iData;
//  var iPlr = GetOwner(pPlr);
  if(GetTeamPlayerCount(GetPlayerTeam(iPlr)) == 1)
  {
    iCol = TEAM_TeamColumn;
    iData = GetPlayerTeam(iPlr);
  }
  if(!fRemove)
    SetScoreboardData(iPlr, iCol, Format("<c %x>{{FLBS}}</c> %s", GetTeamColor(iCTeam), GetTaggedPlayerName(iPlr, true)), iData);
  else
    SetScoreboardData(iPlr, iCol, GetTaggedPlayerName(iPlr, true), iData);
}
