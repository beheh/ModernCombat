/*-- Geschütz --*/

#strict

func Initialize()
{
  return(1);
}

func EjectBullet()
{
  SABulletCasing(0, 0, RandomX(-20,-10), RandomX(-30,-20), 8, RGB(130,130,100));
}
