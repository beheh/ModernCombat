/*-- Nebliger Wind --*/

#strict 2

local oldwind;


/* Initialisierung */

protected func Initialize()
{
  //Positionieren
  SetPosition();

  oldwind = GetFogWind();

  //Nebel auf Landschaft plazieren
  for(var i = LandscapeWidth()/10; i > 0; i--)
    DoFog(i*10);
}

/* Ermittlung der Windbewegung */

protected func GetFogWind()
{
  var wind = GetWind(0,0,true);

  if(!wind)
  {
    if(!oldwind)
      wind = (Random(2)*2-1)*10;
    else
      wind = Sgn(oldwind)*10;
  }

  if(wind > 0)
    BoundBy(wind, 10,40);
  else
    BoundBy(wind, -40,-10);

  return wind;
}

/* Nebelerstellung */

protected func Timer()
{
  //Effektmanager beachten
  if(GetEffectData(EFSM_Fog))
  {
    //Nebel erstellen
    if(GetWind(0,0,true)>0)
      DoFog(0);
    else
      DoFog(LandscapeWidth());

    //Partikel entsprechend dem Wind schieben
    var fogwind = GetFogWind();
    PushParticles("Fog", -(oldwind-fogwind), 0);
    oldwind = fogwind;
  }
}

protected func DoFog(x)
{
  //Neuen Nebel an Kartenrändern erstellen
  var grey = 100+Random(155);
  CreateParticle("Fog", x, Random(LandscapeHeight()), oldwind, 0, 300+Random(600), RGBa(grey,grey,grey,Random(100)), this);
}