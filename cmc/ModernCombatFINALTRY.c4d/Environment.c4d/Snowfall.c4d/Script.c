/*-- Schneefall --*/

#strict 2


/* Initialisierung */

protected func Initialize()
{
  //Positionieren
  SetPosition();
}

/* Schneeerstellung */

private func Timer()
{
  //Effektmanager beachten
  if(!GetEffectData(EFSM_Fog)) return;

  var i, iX, iY;

  iX = GetX();
  iY = GetY();

  //Partikelschnee erzeugen
  for(i = 0; i < 20; i++)
  {
    var iRand = Random(LandscapeWidth()) - iX;
    if(GBackSky(iRand, 5))
      CreateParticle("Snowflake", iRand, -iY, 0, 12, Random(35)+1, RGBa(255,255,255,RandomX(255,80)));
  }

  return true;
}