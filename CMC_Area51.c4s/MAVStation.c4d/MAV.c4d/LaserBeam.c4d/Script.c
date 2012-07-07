/*-- Laserstrahl --*/

#strict 2


/* Initialisierung */

func Initialize()
{
	Timer();
}

func Timer()
{
  SetColorDw(SetRGBaValue(GetColorDw(), Random(150), 0));
}
