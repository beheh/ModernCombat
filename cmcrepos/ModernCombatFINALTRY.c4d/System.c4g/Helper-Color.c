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