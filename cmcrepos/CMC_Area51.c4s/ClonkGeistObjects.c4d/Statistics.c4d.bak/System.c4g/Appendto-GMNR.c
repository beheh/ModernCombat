/*-- Neues Script --*/

#strict 2

#appendto GMNR

public func UpdateScoreboard()
{
  //Wird noch eingestellt
  if (FindObject(CHOS)) return;

  //Titelzeile
  SetScoreboardData(SBRD_Caption, SBRD_Caption, GetName());

  //Zu erreichende Credits
  SetScoreboardData(SBRD_Caption, GMNR_Icon, Format("{{%i}}", GetID()));
  SetScoreboardData(SBRD_Caption, GMNR_Goal, Format("$Goal$"));
  SetScoreboardData(SBRD_Caption, GMNR_GoalCount, Format("%d", iGoal));

  //Leerzeile
  SetScoreboardData(0, GMNR_Count, " ", iGoal*2);

  //Teams auflisten
  if (Teams())
    for (var i = 0; i < GetPlayerCount(); i++)
    {
      var team = GetPlayerTeam(GetPlayerByIndex(i));
      SetScoreboardData(team, GMNR_Name, GetTaggedTeamName(team));
      SetScoreboardData(team, GMNR_Count, Format("<c %x>%d</c>", GetTeamColor(team), aMoney[team]), aMoney[team]);
    }
  else
    for (var i = 0; i < GetPlayerCount(); i++)
    {
      var plr = GetPlayerByIndex(i);
      SetScoreboardData(plr, GMNR_Name, GetTaggedPlayerName(plr, true));
      SetScoreboardData(plr, GMNR_Count, Format("<c %x>%d</c>", GetPlrColorDw(plr), aMoney[plr]), aMoney[plr]);
    }

  //Sortieren noch
  SortScoreboard(GMNR_Count, true);
}
