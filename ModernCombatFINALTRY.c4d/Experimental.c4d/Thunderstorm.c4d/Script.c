/*-- Gewitter --*/

#strict


/* Initalisierung */

protected func Initialize()
{
  SetPosition();

  DoClouds();
  Sound("Rain.ogg",true,0,0,0,+1);
}

/* Wolkenerstellung */

protected func DoClouds()
{
  //Partikel entfernen
  ClearParticles("Fog");
  //und neu setzen
  for(var i = LandscapeWidth()/6; i > 0; i--)
  {
    var grey = Random(60);
    CreateParticle ("Fog", i*6, Random(40), 0, 0, 300+Random(600), RGBa(grey,grey,grey+1),0);
  }
}

/* Timer */

protected func Timer()
{
  var plr = 0;
  var range = 700;

  var borderleft = GetX(GetCursor(GetPlayerByIndex(plr)))-range;
  var borderright = GetX(GetCursor(GetPlayerByIndex(plr)))+range;

  for(; plr < GetPlayerCount(); plr++)
  {
    borderleft = Min(borderleft, GetX(GetCursor(GetPlayerByIndex(plr)))-range);
    borderright = Max(borderright, GetX(GetCursor(GetPlayerByIndex(plr)))+range);
  }

  CreateParticle("Raindrop", RandomX(borderleft, borderright), 0, RandomX(GetWind(0,0,1)*3, GetWind(0,0,1)*5), RandomX(200, 300), 5*64 + Random(32));

  //Blitze
  if(Random(100) - (6 * 10)) return();

  var x = Random(LandscapeWidth());
  var lightning = CreateObject(FXL1,x,1,NO_OWNER);
  lightning->Activate(x, 1, -20, 41, +5, 15);
  lightning->Sound("Thunder*.ogg",1);
  lightning->AddLightFlash(1500+Random(1500),0,0,RGB(200,255,255), lightning); 
}