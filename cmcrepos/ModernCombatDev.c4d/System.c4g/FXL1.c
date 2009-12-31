/*-- Bitte Lächeln! BLITZ! --*/

#strict 2
#appendto FXL1

/*-- Blitz --*/

#strict 2



/* Bewegung */

private func Advance()
{
  // Einschlag
  var iVtx = GetVertexNum()-1, iVtxX = GetVertex(iVtx, 0), iVtxY = GetVertex(iVtx, 1);
  if (GBackSemiSolid(iVtxX-GetX(), iVtxY-GetY() ))
  {
    var iVtx2 = GetVertexNum()-2, iVtxX2 = GetVertex(iVtx2, 0), iVtxY2 = GetVertex(iVtx2, 1);
  
    var iColor = RGB(255,255,255);
    var iSize = 40+Random(20);
    var iX = iVtxX-GetX();
    var iY = iVtxY-GetY();
    /*var iAngle = Angle(iX,iY,iVtxX2-GetX(),iVtxY2-GetY());//SurfaceNormal4K(iX,iY,2);

    CreateParticle("WhiteFlash",iX,iY,
                   +Sin(iAngle,500),
                   -Cos(iAngle,500),
                   iSize*5,iColor);*/
    if(IsDark())
    {
      if(!iColor)
        iColor = RGB(255,255,255);

      var r,g,b,a;
      SplitRGBaValue(iColor,r,g,b,a);
      iColor = RGBa(r,g,b,Min(a+65,255));

      AddLightFlash(iSize*15, iX, iY, iColor);
    }
  
    return Remove();
  }

  // Objektschaden
  if (iVtx>5) BlastObjects(iVtxX, iVtxY, 5);

  // Verzweigung
  if (!Random(35))
    LaunchLightning(iVtxX, iVtxY, iAdvX, iVarX, iAdvY, iVarY);

  // Neuen Eckpunkt
  if (!AddVertex( iVtxX+iAdvX+Random(iVarX), iVtxY+iAdvY+Random(iVarY)))
    return Remove();

  // Objektanziehung
  iVtx = GetVertexNum()-1; iVtxX = GetVertex(iVtx, 0); iVtxY = GetVertex(iVtx, 1);
  var obj;
  if (iVtx>7)
    if (obj = FindObject( 0, iVtxX-GetX()-50, iVtxY-GetY()-50, 100, 100, OCF_AttractLightning, 0,0, NoContainer()  ) )
      Attraction(obj);

  LightningEffect(4);

  // Weiter	
  return ExecLgt();
}
