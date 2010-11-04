/*-- Geschütz --*/

#strict 2

local arty;

public func Attach(object pObj, int iVertex)
{
  SetAction("Extend", arty = pObj);
  //Am dritten Vertex festmachen
  SetActionData((3 << 8) + iVertex);
}

/* Hülsenauswurf */

func EjectBullet()
{
  SABulletCasing(0, 0, RandomX(-20,-10), RandomX(-30,-20), 12, RGB(200,200,200));
}