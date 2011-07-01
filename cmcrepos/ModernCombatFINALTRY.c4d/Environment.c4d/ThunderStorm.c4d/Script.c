/*-- Gewitter --*/

#strict 2

local skyfade;


/* Initialisierung */

protected func Initialize()
{
  //Materialregen starten
  LaunchRain(0, Material("Rain"), LandscapeWidth(), 100)->SetCategory(1);

  //Wolkendecke erstellen
  DoClouds();

  //Globaler Regensound als Loop
  Sound("Rain.ogg",true,0,0,0,+1);

  return true;
}

/* Wolkenerstellung */

global func DoClouds()
{
  if(!EFSM_Level == 3) return;

  ClearParticles("Cloud");
  for(var i = LandscapeWidth()/8; i > 0; i--)
  {
    var grey = Random(60);
    CreateParticle ("Cloud", i*8, Random(20), 0, 0, 300+Random(600), RGBa(grey,grey,grey+1),0);
  }
}

/* Timer */

protected func Timer()
{
  //Hintergrund beleuchten
  if(skyfade) skyfade-=20;

  if(skyfade <= 255) SetSkyAdjust(RGBa(255,255,255,220), RGB(skyfade,skyfade,skyfade));

  if(!Random(350) && !skyfade)
  {
    skyfade = 300;
    Sound("Thunder*.ogg",1);
    return true;
  }

  //Zufällig Blitz erstellen
  if(!Random(550))
  {
    var x = Random(LandscapeWidth());
    var lightning = CreateObject(FXL1,x,1,NO_OWNER);
    lightning->Activate(x, 1, -20, 41, +5, 15);
    lightning->Sound("ThunderStrike*.ogg",1);
    lightning->AddLightFlash(1500+Random(1500),0,0,RGB(200,255,255), lightning);
    return;
  }
}

protected func Destruction()
{
  ClearParticles("Cloud");
  SetSkyAdjust(RGBa(255,255,255,220), RGB(0,0,0));
  return true;
}