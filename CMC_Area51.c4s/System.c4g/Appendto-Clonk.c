/*-- Neues Script --*/

#strict 2

#appendto CLNK

/* Endgültiger Tod */

func Death(object pTarget)
{
  if(!pTarget) pTarget = this;
  if(!pTarget) return;

  //Achievements
  ResetAchievementExtra(AC08, GetOwner(pTarget));
  var data = GetAchievementExtra(AC08, GetKiller(pTarget));
  if(!data) data = CreateArray();
  data[GetOwner(pTarget)] = 0;
  if(data[GetOwner(pTarget)] >= AC08->GetAchievementScore())
  {
    //Achievement-Fortschritt (Epic Kill)
    AwardAchievement(AC08, GetKiller(pTarget));
  }
  SetAchievementExtra(data, AC08, GetKiller(pTarget));
  ResetAchievementProgress(AC12, GetOwner());
  if(!IsFakeDeath())
  {
  	ResetAchievementProgress(AC14, GetOwner());
		//Evtl. Statistiken speichern...
  	var rewards;
  	if((rewards = FindObject(RWDS)))
  	{
  		if(Hostile(GetKiller(pTarget), GetOwner(pTarget)))
  			rewards->SetPlayerData(rewards->GetPlayerData(RWDS_KillCount, GetKiller(pTarget))+1, RWDS_KillCount, GetKiller(pTarget));
  		
  		rewards->SetPlayerData(rewards->GetPlayerData(RWDS_DeathCount, GetOwner(pTarget))+1, RWDS_DeathCount, GetOwner(pTarget));
  	}
  }
  
  // Bei Soforttod Todesnachricht einblenden und Achievements bearbeiten
  if(FindObject(NOFD))
  {
    // Todesnachricht:
    pTarget->DeathAnnounce(GetOwner(pTarget), pTarget, GetKiller(pTarget));
  }

  if(IsFakeDeath())
  {
    pTarget->InstantDie();
  }
  else
  {
    ScheduleCall(pTarget, "DeathSound", 1, 0, pTarget);
  }

  //Verschwinden
  FadeOut(pTarget);
}
