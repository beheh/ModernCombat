/* Wrack */

#strict


protected func Initialize()
{
  SetAction("Destroyed");
  Schedule("Smoke(0, 0, 30)", 5, 40, this());
}

protected func Hit2()
{
  DestroyWreck();
}

protected func Hit()
{
  Sound("Collision*");
}

func BlastPar(int X, int Y, int Size, int Color)
{
  CreateParticle("Blast", X, Y, 0, 0, Size, Color, this());
}

func DestroyWreck()
{
  Sound("MainHelicopterExplosion", false, this());
  Explode(60);
  return(RemoveObject());
}
