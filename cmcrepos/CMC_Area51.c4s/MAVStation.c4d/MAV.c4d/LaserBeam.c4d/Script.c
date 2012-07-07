/*-- Laserstrahl --*/

#strict 2


/* Initialisierung */

func Initialize()
{
	SetColorDw(SetRGBaValue(GetColorDw(), Random(150), 0));
  ScheduleCall(this,"Death",1);
}

func Death()
{
  RemoveObject(this);
}
