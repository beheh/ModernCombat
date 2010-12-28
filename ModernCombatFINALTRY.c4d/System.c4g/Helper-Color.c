/*-- Farbhilfsfunktionen --*/

//Hellt Farbe color um value auf oder dunkelt um den negativen Wert ab.

#strict 2


global func DoColorBrightness(int color, int value)
{
  if(value > 0)
  {
    color = DoRGBaValue(color, Min(255-GetRGBaValue(color, 1), value), 1);
    color = DoRGBaValue(color, Min(255-GetRGBaValue(color, 2), value), 2);
    color = DoRGBaValue(color, Min(255-GetRGBaValue(color, 3), value), 3);
  }
  if(value < 0)
  {
    color = DoRGBaValue(color, -Min(GetRGBaValue(color, 1), -value), 1);
    color = DoRGBaValue(color, -Min(GetRGBaValue(color, 2), -value), 2);
    color = DoRGBaValue(color, -Min(GetRGBaValue(color, 3), -value), 3);  
  }
  return color;
}

global func LightenColor(int dwClr, int iMinLight)
{
  if (!iMinLight)
    iMinLight = 127;
  var r, g, b, a;
  SplitRGBaValue(dwClr, r, g, b, a);
  var iLight = 50 * r + 87 * g + 27 * b;
  if (iLight < iMinLight * 164)
  {
    var iIncrease = (iMinLight * 164 - iLight) / 164;
    dwClr = RGB(Min(r + iIncrease, 255), Min(g + iIncrease, 255), Min(b + iIncrease, 255));
  }
  return dwClr;
}