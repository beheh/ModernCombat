/*-- Boje --*/

#strict 2

local sway,dir,max;


/* Initialisierung */

func Initialize()
{
  if(FrameCounter() <= 10)
    sway = RandomX(20,-20);
}

/* Bewegung */

func CalcMax()
{
  max = Abs(GetWind(0,-2))/6;
}

func Timer()
{
  if(GBackLiquid(0,+2) != GBackLiquid(0,-2))
  {
    if(sway == 0)
      CalcMax();

    if(dir == 0)
    {
      sway++;
      if(sway >= max)
        dir = 1;
    }
    else
    {
      sway--;
      if(sway <= -max)
        dir = 0;
    }

    var r2 = Sin(sway*90/max,max);
    SetDTRotation(r2,0,0,this);
  }
  else
    SetDTRotation(0,0,0,this);
}

protected func SetDTRotation(int r, int xoff, int yoff, object obj)
{
  var fsin = Sin(r,1000),fcos=Cos(r,1000);

  SetObjDrawTransform(+fcos, +fsin, (1000-fcos)*xoff - fsin*yoff, -fsin, +fcos, (1000-fcos)*yoff + fsin*xoff, obj);
}