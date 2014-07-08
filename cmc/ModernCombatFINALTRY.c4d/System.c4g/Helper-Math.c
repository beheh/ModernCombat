/*-- Mathefunktionen --*/

//Erweitert um einige Rechnungsfunktionen.

#strict 2


/* Interpolierung eines Wertes */

global func Interpolate4K(y1,y2,x1,x2,x3)
{
  return(y1 + y2 + ((180 - y1)/(x2 - x1)) * (x3 - x1));
}

global func Interpolate(y1,y2,x1,x2,x3)
{
  return(Interpolate4K(y1,y2,x1,x2,x3));
}

global func Interpolate2(val1, val2, step, max)
{
  return val1+(val2-val1)*step/max;
}

/* Interpolierung zweier Farben */

global func InterpolateRGBa(RGBa_1,RGBa_2,x1,x2,x3)
{
  var r = Interpolate(GetRGBaValue(RGBa_1,1),GetRGBaValue (RGBa_2,1),x1,x2,x3);
  var g = Interpolate(GetRGBaValue(RGBa_1,2),GetRGBaValue (RGBa_2,2),x1,x2,x3);
  var b = Interpolate(GetRGBaValue(RGBa_1,3),GetRGBaValue (RGBa_2,3),x1,x2,x3);
  var a = Interpolate(GetRGBaValue(RGBa_1,0),GetRGBaValue (RGBa_2,0),x1,x2,x3);

  return(RGBa(r,g,b,a));
}

/* Interpolierung dreier Farben */

global func InterpolateRGBa2(RGBa_1,RGBa_2,RGBa_3,x1,x2,x3)
{
  if(x3 <= x2/2)
    return(InterpolateRGBa(RGBa_1,RGBa_2,x1,Average4K(x1,x2),x3));
  else
    return(InterpolateRGBa(RGBa_2,RGBa_3,Average4K(x1,x2),x2,x3));
}

global func InterpolateRGBa3(int dwRGBa1, int dwRGBa2)
{
  return RGBa(
  Interpolate2(GetRGBaValue(dwRGBa1, 1), GetRGBaValue(dwRGBa2, 1), ...),
  Interpolate2(GetRGBaValue(dwRGBa1, 2), GetRGBaValue(dwRGBa2, 2), ...),
  Interpolate2(GetRGBaValue(dwRGBa1, 3), GetRGBaValue(dwRGBa2, 3), ...),
  Interpolate2(GetRGBaValue(dwRGBa1), GetRGBaValue(dwRGBa2), ...)  );
}

/* Durchschnitt zurückgeben */

global func Average4K(int A, int B)
{
  return((B-A)/2+A);
}

/* Normalisierung angegebener Werte */

global func Normalize4K(int &a,int &b)
{
  if(!a || !b) return;
  if(a > b)
  {
    a = a/b;
    b = 1;
  }
  else
  {
    b = b/a;
    a = 1;
  }
}

/* Operatoren mit Nachkommastellen */

global func Mul4K(int iVal1, int iVal2, int iPrec)
{
  return(( iVal1 * iVal2 ) / iPrec);
}

global func Div4K(int iVal1, int iVal2, int iPrec)
{
  return(( iVal1 / iVal2 ) / iPrec);
}

global func Add4K(int iVal1, int iVal2, int iPrec)
{
  return(( iVal1 + iVal2 ) / iPrec);
}

global func Sub4K(int iVal1, int iVal2, int iPrec)
{
  return(( iVal1 - iVal2 ) / iPrec);
}

/*global func Wrap4K(int iValue, int iMin, int iMax)
{
  iValue %= iMax-iMin;

  if(iValue < iMin)
    iValue = iMax - (iMin - iValue);

  if(iValue > iMax)
    iValue = iMin + (iValue - iMax);

  return(iValue);
}*/

global func Wrap4K(int iValue, int iStart, int iEnd)
{
  if(!iEnd-iStart) return iStart;
  while(iValue<iStart) iValue += iEnd-iStart;
  while(iValue>=iEnd) iValue -= iEnd-iStart;

  return iValue;
}

/* Abstand von zwei Winkelwerten */

global func AngleOffset4K(int iBase, int iSecond)
{
  iBase = Wrap4K(iBase,0,360);
  iSecond = Wrap4K(iSecond,0,360);

  var offset = iSecond-iBase;

  if(Abs(offset) > 180)
  {
    if(iBase > iSecond)
      iBase -= 360;
    else
      iSecond -= 360;

    offset = iSecond-iBase;
  }

  return(offset);
}

/* Vorzeichen eines Wertes */

global func Sgn(int x)
{
  if (x < 0) return -1;
  if (x > 0) return 1;
  return 0;
}