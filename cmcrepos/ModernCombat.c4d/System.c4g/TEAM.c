/*-- TEAM-Bugfix --*/

//Damit man auf jeden Fall mehrere Spielziele zur Auswahl haben kann.

#strict 2
#appendto TEAM

protected func Initialize()
{
  aPoints = CreateArray();
  aKill = CreateArray();
  aDeath = CreateArray();
  if(!FindObject(CHOS))
    ChooserFinished();

  //return this->GOAL::Initialize();
  //Das andere geht nicht. D=
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