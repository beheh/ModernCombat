/*-- Neues Script --*/

#strict 2
#appendto MCSL

/* (Re-)Spawn */

func InitializePlayer(int iPlayer)
{
  var pClonk = GetCrew(iPlayer);
  if(!pClonk)
  {
    ScheduleCall(0,"InitializePlayer",1,0,iPlayer);
    return;
  }

  if(FindObject(CHOS))
    return;
  
  if(GetID(GetCrew(iPlayer,0)) != BUCR)
    ScheduleCall(0,"InitClassMenu",1,0,pClonk);
}

public func RelaunchPlayer(int iPlr, object pClonk)
{
  if(pClonk)
    if(GetID(pClonk) == BUCR)
      return;
  if(!pClonk)
    if(!(pClonk = GetCrew(iPlr)))
      return ScheduleCall(this,"RelaunchPlayer",1,0,iPlr,pClonk);
  if(!GetAlive(pClonk))
    return ScheduleCall(this,"RelaunchPlayer",1,0,iPlr);

  //Menü zeitverzögert erstellen
  ScheduleCall(0,"InitClassMenu",10,0,pClonk);
  
  return;
}
