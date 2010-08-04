/*-- Geschütz --*/

#strict 2


/* Initalisierung */

func Initialize()
{
  //Ausfahraktion
  SetAction("Extend");

  return 1;
}

/* Hülsenauswurf */

func EjectBullet()
{
  SABulletCasing(0, 0, RandomX(-20,-10), RandomX(-30,-20), 12, RGB(200,200,200));
}