/*-- SimFlight --*/

//SimFlight2 berücksichtigt Solidmasks.

#strict 2

static const SIM_Precision = 1000;
static const SIM_StepPrecision = 2;

global func SimFlight2(int &iX, int &iY, int &iXDir, int &iYDir, int iPrecision)
{
  var iPX, iPY;
  //An Präzision anpassen
  iX *= SIM_Precision;
  iY *= SIM_Precision;
  if (!iPrecision)
    iPrecision = 10;
  iXDir = iXDir * SIM_Precision / iPrecision;
  iYDir = iYDir * SIM_Precision / iPrecision;

  //Noch im Bild?
  while (Inside(GetX() + iX / SIM_Precision, 0, LandscapeWidth()) && GetY() + iY / SIM_Precision < LandscapeHeight())
  {
    //Erste Runde nicht prüfen
    if (iPX || iPY)
    {
      //Winkel und Abstand berechnen
      var iDistance = Distance(iPX / SIM_Precision, iPY / SIM_Precision, iX / SIM_Precision, iY / SIM_Precision),
      iAngle = Angle(iPX, iPY, iX, iY);

      //Weg abtasten
      for (var i = 0; i < iDistance; i += SIM_StepPrecision)
        if (GBackSolid(iX / SIM_Precision + Sin(iAngle, i), iY / SIM_Precision - Cos(iAngle, i)))
        {
          iX = iX/SIM_Precision + Sin(iAngle, i);
          iY = iY/SIM_Precision - Cos(iAngle, i);
          iXDir = iXDir * iPrecision / SIM_Precision;
          iYDir = iYDir * iPrecision / SIM_Precision;
          return true;
        }

      //Schwerkraft
      iYDir += 2 * GetGravity() * SIM_Precision / 1000;
    }

    //Und weiter
    iPX = iX;
    iPY = iY;
    iX += iXDir;
    iY += iYDir;
  }

  //War wohl nix
  return false;
}