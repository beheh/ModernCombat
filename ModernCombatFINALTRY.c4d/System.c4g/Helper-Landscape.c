/*-- Mapping-Tools --*/

//Erweitert Mapping-Werkzeug um weitere Scriptbefehle.

#strict 2


/* Material zeichnen */

global func DrawRamp(string szMat, int iX, int iY, int iW, int iH)
{
  DrawMaterialQuad(szMat, iX   , iY   ,
                          iX   , iY+iH,
                          iX+iW, iY   ,
                          iX   , iY   ,
                          true);
}

global func DrawCircle(string szMat, int iX, int iY, int iW, int iH, int iStartAngle, int iEndAngle, int iWInner, int iHInner)
{
  if(!iEndAngle) iEndAngle = 360;
  var r = iStartAngle;
  var step = 10;
  iW -= iWInner;
  iH -= iHInner;

  for(; r < iEndAngle; r += step)
  {
    step = Min(r+step, iEndAngle) - r;
  
    DrawMaterialQuad(szMat,iX + Sin(r, iWInner+iW),
                           iY - Cos(r, iHInner+iH),
                           
                           iX + Sin(r+step, iWInner+iW),
                           iY - Cos(r+step, iHInner+iH),
                           
                           iX + Sin(r+10, iWInner),
                           iY - Cos(r+10, iHInner),
                           
                           iX + Sin(r, iWInner),
                           iY - Cos(r, iHInner),
                           true);                                                       
  }
}
//DrawCircle("Rock",100,100,50,50,180,270,20,20);


global func DrawStripe(string szMat, int iX1, int iY1, int iX2, int iY2, int iWidth, int iAngle)
{
  var xoff = +Sin(iAngle,iWidth);
  var yoff = -Cos(iAngle,iWidth);

  DrawMaterialQuad(szMat, iX1     , iY1     ,
                          iX1+xoff, iY1+yoff,
                          iX2+xoff, iY2+yoff,
                          iX2     , iY2     ,
                          true);
}
//DrawStripe("Tunnel",100,100,200,200,10,45);

/* Bodendekoration plazieren */

global func PlaceGroundDeco(anyDecorationType, int iX, int iY, int iWidth, int iHeight, int iRandom, string szGraphic, bool fAllSurfaces)
{
  //Objekte platzieren
  if(GetType(anyDecorationType) == C4V_Array)
  {
    for(var deco in anyDecorationType)
      PlaceGroundDeco(deco[0], iX, iY, iWidth, iHeight, iRandom, deco[1], fAllSurfaces);

    return 1;
  }

  var x_dens = 12;

  for(var x = iX; x < iX+iWidth; x += x_dens)
  {
    var y = iY;
    while(y < iY+iHeight && !GBackSemiSolid(x, y))
      y += 10;

    if((fAllSurfaces || GetMaterial(x, y + 1) == Material("Earth")) && !Random(iRandom))
    {
      var decoration = CreateObject(anyDecorationType, x, y, -1);
      if(szGraphic)
        SetGraphics(szGraphic, decoration, anyDecorationType);
    }
  }

  return 1;
}