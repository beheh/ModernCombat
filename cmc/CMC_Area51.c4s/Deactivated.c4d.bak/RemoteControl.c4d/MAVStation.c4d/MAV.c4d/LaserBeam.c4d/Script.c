/*-- Laserstrahl --*/

#strict 2


/* Initialisierung */

func Initialize()
{
  //Flackern
  Timer();
}

func Timer()
{
  SetColorDw(SetRGBaValue(GetColorDw(), Random(150), 0));
}