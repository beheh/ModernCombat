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
}

public func ChooserFinished()
{
  ScheduleCall(this(),"InitScoreboard",1);
}

private func UpdateHUDs()
{
  if(FindObject(GOAL)) return;
  return inherited();
}