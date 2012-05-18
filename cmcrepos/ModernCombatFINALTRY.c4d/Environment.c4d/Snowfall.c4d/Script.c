/*-- Schneefall --*/

#strict 2


/* Timer */

private func Snow()
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
      CreateParticle("Snowflake", iRand, -iY, 0, 12, 35, 0xFFFFFF);
  }

  return true;
}