/*-- Lichtkegel --*/

//Modifiziert den hazard-eigenen Lichtkegel für CMC.

#strict 2
#appendto LGHC

local fAlpha, iAlpha;


/* Initialisierung */

protected func Initialize()
{
  if(GetEffectLevel() < 3)
    SetVisibility(VIS_None);
}

public func SetAlpha(int iValue)
{
  iAlpha = iValue;
  fAlpha = true;
  Draw();
  return true;
}

public func Draw()
{
  var dir;
  if(!bDir) dir = 1;
  else dir = 1-GetDir(GetActionTarget())*2;

  var alphamod, sizemod;
  CalcLight(alphamod, sizemod);
  if(fAlpha)
  {
    alphamod = -60;
    var r,g,b,a;
    SplitRGBaValue(GetClrModulation(), r, g, b, a);
    SetClrModulation(RGBa(r, g, b, iAlpha));
  }

  if(this->~IsLocked()) //Nur bei CMC Lichtkegel
    sizemod = 100;

  var fsin, fcos, xoff, yoff;
  fsin = Sin(iR, 1000);
  fcos = Cos(iR, 1000);
  xoff = 0;
  yoff = +64;

  var width = iSizeX*sizemod/100, height = iSizeY*sizemod/100;

  SetObjDrawTransform(
    +fcos*width/1000, +fsin*height/1000, (((1000-fcos)*xoff - fsin*yoff)*height)/1000+iOffX*1000*dir,
    -fsin*width/1000, +fcos*height/1000, (((1000-fcos)*yoff + fsin*xoff - 64000)*height)/1000+iOffY*1000
  );
}