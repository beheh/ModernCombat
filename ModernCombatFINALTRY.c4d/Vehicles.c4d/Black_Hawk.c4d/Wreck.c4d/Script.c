/* Wrack */

#strict


protected func Initialize()
{
  SetAction("Destroyed");
  Schedule("Smoke(0, 0, 30)", 5, 40, this());
}

protected func Hit3()
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
  var x, y, clonk;
  for (var i; i<10;i++)
  {
   clonk = 0;
   x = RandomX(-50,+50);
   y = RandomX(-30,+30);
   CreateParticle("Blast", x, y, 0, 0, 100+Random(150), RGB(255,255,255));
   while (clonk = FindObject(0, x, y, -1, -1, OCF_Living, 0, 0, NoContainer(), clonk))
   {
    if (ObjectDistance(clonk, this()) < 40)
     BlastUp(clonk);
   }
  }
  return(RemoveObject());
}

private func BlastUp(obj)
{
  BlastObjects(GetX(obj),GetY(obj),10);
  if (Distance(GetX(obj),GetY(obj),GetX(),GetY()) < 15)
  {
   DoEnergy(-50,obj);
   Fling(obj, 25-(GetX(obj)-GetX()), 25-(GetY(obj)-GetY()));
   return(true);
  }
  if (Distance(GetX(obj),GetY(obj),GetX(),GetY()) < 25)
  {
   DoEnergy(-30,obj);
   Fling(obj, 50-(GetX(obj)-GetX()), 50-(GetY(obj)-GetY()));
   return(true);
  }
  DoEnergy(-10,obj);
  return(true);
}
