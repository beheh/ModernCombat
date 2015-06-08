/*-- Gewitter --*/

#strict 2

local darken,beginfade,skyfade,lighten;


/* Initialisierung */

protected func Initialize()
{
  //Positionieren
  SetPosition();

  //Materialregen starten
  LaunchRain(0, Material("Rain"), LandscapeWidth(), 100)->SetCategory(1);

  //Globale Regengeräusche starten
  Sound("Rain.ogg",true,0,50,0,+1);

  //Wolkendecke erstellen
  DoClouds();

  //Hintergrundfärbung justieren
  beginfade = 250;
  skyfade = 240;
  SetSkyAdjust(RGBa(255,255,255,beginfade), RGB(skyfade,skyfade,skyfade));
  Sound("STRM_ThunderStrike*.ogg",true);

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
    CreateParticle("Cloud", i*8, Random(20)+1, 0, 0, 300+Random(600), RGBa(grey,grey,grey+1));
  }
}

/* Sturmverhalten */

protected func Timer()
{
  //Verdunkelung bei Sturmbeginn
  if(!darken)
  {
    beginfade-=5;
    skyfade-=8;

    if(beginfade == 100)
      darken = 1;
    else
      SetSkyAdjust(RGBa(255,255,255,beginfade), RGB(skyfade,skyfade,skyfade));

    return 1;
  }

  //Aufhellung bei Sturmende
  if(lighten)
  {
    beginfade-=2;

    SetSkyAdjust(RGBa(255,255,255,beginfade), RGB(skyfade,skyfade,skyfade));

    if(beginfade == 0)
    {
      //Wolkendecke stoppen und entfernen
      PushParticles("Cloud", 0, 0);
      ClearParticles("Cloud");

      //Verschwinden
      RemoveObject();
    }

    return 1;
  }

  //Hintergrundbeleuchtung normalisieren falls nötig
  if(skyfade > 0) skyfade-=5;
  if(skyfade <= 100) SetSkyAdjust(RGBa(255,255,255,100), RGB(skyfade,skyfade,skyfade));

  //Zufällig den Hintergrund erhellen
  if(!Random(350) && skyfade <= 0)
  {
    skyfade = 250;
    Sound("STRM_Thunder*.ogg",true,0,50);
    return true;
  }
  else
  if(!Random(250))
    ThunderSound(Format("STRM_DistantThunder%d.ogg", Random(4)+1), Random(LandscapeWidth()));

  //Zufällig Blitz erstellen
  if(!Random(550))
  {
    var x = Random(LandscapeWidth());
    var lightning = CreateObject(FXL1,x,1,NO_OWNER);
    lightning->Activate(x, 1, -20, 41, +5, 15);
    ThunderSound(Format("STRM_ThunderStrike%d.ogg", Random(5)+1), x);
    lightning->AddLightFlash(1500+Random(1500),0,0,RGB(200,255,255), lightning);
    return;
  }
}

/* Soundausgabe */

protected func ThunderSound(string szSound, int iX)
{
  for(var i = 0; i < GetPlayerCount(); i++)
  {
    if(!GetPlayerName(i)) continue;

    var clonk = GetCursor(i);
    AddEffect("Thunder", clonk, 101, 1, this, 0, iX, szSound);
  }
}

protected func FxThunderStart(object target, int nr, int temp, int iX, string szSound)
{
  EffectVar(0, target, nr) = 100;						//Lautstärke
  EffectVar(1, target, nr) = Distance(iX, 1, GetX(target), GetY(target));	//Distanz zwischen Clonk und Blitz
  EffectVar(2, target, nr) = iX;						//x-Position des Blitzes
  EffectVar(3, target, nr) = EffectVar(1, target, nr)/20;			//Zeit bis Soundausgabe
  EffectVar(4, target, nr) = szSound;						//Name des Sounds

  return true;
}

protected func FxThunderTimer(object target, int nr, int iTime)
{
  var x = EffectVar(2, target, nr);
  EffectVar(1, target, nr) = Distance(x, 0, GetX(target), GetY(target));

  //Zeit
  EffectVar(3, target, nr) = EffectVar(1, target, nr)/20-iTime;
  if(EffectVar(3, target, nr) <= 0)
  {
    //Lautstärke
    EffectVar(0, target, nr) = 100-EffectVar(1, target, nr)/50;
    EffectVar(0, target, nr) = BoundBy(EffectVar(0, target, nr), 1, 100);
    if(!PathFree(x, 1, GetX(target), GetY(target))) EffectVar(0, target, nr) -= 20;
    EffectVar(0, target, nr) = BoundBy(EffectVar(0, target, nr), 1, 100);

    Sound(EffectVar(4, target, nr), true, 0, EffectVar(0, target, nr), GetOwner(target)+1);
    return -1;
  }
}

/* Entfernung */

protected func ClearEffects()
{
  //Aufhellung starten
  lighten = 1;

  //Regen entfernen
  RemoveAll(FXP1);

  //Regengeräusche stoppen
  Sound("Rain.ogg",true,0,50,0,-1);

  //Wolkendecke wegbewegen
  PushParticles("Cloud", 0, -10);

  Sound("STRM_DistantThunder*.ogg",true,0,50);

  return true;
}