/*-- Neues Script --*/

#strict 2

#appendto GBAS

public func UpdateScoreboard()
{
  if(!aScoreboardTeams)
    aScoreboardTeams = [];

  //Titelzeile
  SetScoreboardData(SBRD_Caption, SBRD_Caption, GetName());

  //Teams auflisten
  for(var i, iTeam; i < GetTeamCount(); i++)
  {
    //Team eliminiert
	  if(!GetTeamName(iTeam = GetTeamByIndex(i)) || !GetTeamPlayerCount(iTeam))
	  { 
	    if(aScoreboardTeams[iTeam])
	      RemoveScoreboardTeam(iTeam);
	    
	    continue;
	  }
	  RemoveScoreboardTeam(iTeam);
	  aScoreboardTeams[iTeam] = true;
  	//Team hat noch Ziele
	  if(ObjectCount2(Find_InArray(aTargets[iTeam])))
	  {
	    for(var k = 0, row = 0; k < GetLength(aTargets[iTeam]); k++)
	    {
	      var target = aTargets[iTeam][k];
	      //Ziel noch da?
	      if(target)
	      {
	        var dmg = Interpolate2(100, 0, GetDamage(target), EffectVar(0, target, GetEffect("IntAssaultTarget", target))),
	        clr = InterpolateRGBa3(GetTeamColor(iTeam), RGB(255, 255, 255), 100 - dmg, 100);
          SetScoreboardData(row + iTeam * GBAS_MaxTargetCount, GBAS_Icon, Format("{{%i}}", target->~GetImitationID() || GetID(target)));
	        SetScoreboardData(row + iTeam * GBAS_MaxTargetCount, GBAS_Name, Format("<c %x>%s</c>", clr, GetName(target)), iTeam);
	        SetScoreboardData(row + iTeam * GBAS_MaxTargetCount, GBAS_Status, Format("<c %x>%d%</c>", clr, dmg), dmg);
	        ++row;
	      }
	    }
	  }
	  //Keine Ziele mehr. Clonks anzeigen
	  else
	  {
	    //Spieler abklappern
	    for(var l = 0, iPlr; l < GetTeamPlayerCount(iTeam); l++)
	    {
	      iPlr = GetTeamPlayerByIndex(l, iTeam);
	      var clonk = GetCrew(iPlr);
	      //Kein Clonk?
	      if(!clonk || !GetPlayerName(iPlr))
	        continue;
	      
	      //Tot?
	      var symbol = GetID(clonk),
  	    dmg = 1;
	      if(GetID(Contained(clonk)) == FKDT || !GetAlive(clonk) || GetID(Contained(clonk)) == TIM1 || GetID(Contained(clonk)) == TIM2)
	      {
	        symbol = CDBT;
	        dmg = 0;
	      }
        SetScoreboardData(l + iTeam * GBAS_MaxTargetCount, GBAS_Icon, Format("{{%i}}", symbol));
	      SetScoreboardData(l + iTeam * GBAS_MaxTargetCount, GBAS_Name, GetTaggedPlayerName(iPlr, true), iTeam);
        SetScoreboardData(l + iTeam * GBAS_MaxTargetCount, GBAS_Status, "", dmg);
	    }
	  }
  }
  SortScoreboard(GBAS_Status);
  SortScoreboard(GBAS_Name);
}
