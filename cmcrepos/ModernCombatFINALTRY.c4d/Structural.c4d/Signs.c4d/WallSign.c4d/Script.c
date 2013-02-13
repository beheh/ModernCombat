/*-- Wandschild --*/

#strict 2


/* Intialisierung */

func Initialize()
{
  SetAction("Exist");
}

/* Icon setzen */

func Set(string szIcon)
{
  //Grafik aufdrucken
  SetGraphics(0,0,GetID(this),2,GFXOV_MODE_Action, szIcon);
}