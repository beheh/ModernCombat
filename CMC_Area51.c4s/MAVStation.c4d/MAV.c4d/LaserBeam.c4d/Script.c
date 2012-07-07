/*-- Laserstrahl --*/

#strict 2


/* Initialisierung */

func Initialize()
{
  ScheduleCall(this,"Death",1);
}

func Death()
{
  RemoveObject(this);
}