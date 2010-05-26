/*-- Teams --*/

//Damit in jedem Falle mehrere Spielziele zur Auswahl stehen können.

#strict 2

#appendto TEAM


protected func Initialize()
{
  aPoints = CreateArray();
  aKill = CreateArray();
  aDeath = CreateArray();
  if(!FindObject(CHOS))
   ChooserFinished();

  if(!FindObject(GOAL)) CreateObject(GOAL,0,0,-1);
  
  ScheduleCall(this(),"InitScoreboard",1);
  return inherited(...);
}

private func UpdateHUD()
{
  if(FindObject(GOAL)) return;
  return inherited(...);
}
