/*-- Laserstrahl --*/

#strict

func Initialize() {
  
  ScheduleCall(this,"Death",1);
}

func Death()
{
	RemoveObject(this);
}
