/*-- Radio --*/

#strict 2

local isPlaying;


/* Initialisierung */

func Initialize()
{
  isPlaying = -1;
}

/* Bedienung */

protected func ControlThrow(pClonk)
{
  //Eingeschaltet?
  if(isPlaying == 1)
  {
    //Zufälligen neuen Track abspielen
    Sound("Radio_*.ogg", false, this(), 0, 0, -1);
    Sound("Radio_*.ogg", false, this(), 0, 0, 1);
    Schedule("Sound(\"Radio_*.ogg\", false, this(), 0, 0, 1)", 5);

    //Effekt
    CreateParticle("NoGravSpark", 3, 3, 0, 0, 50, RGBa(10, 150, 250, 50));
  }
  Sound("ArrowHit.WAV");
}

protected func ControlDig(pClonk)
{
  //Ein- oder ausschalten
  if (isPlaying == 1) isPlaying -= 2;
  else isPlaying += 2; 
  Sound("Radio_*.ogg", false, this(), 0, 0, isPlaying);

  //Effekte
  CreateParticle("NoGravSpark", 3, 1, 0, 0, 50, RGBa(250, 10, 10, 50));
  Sound("ArrowHit.WAV");
}

/* Schaden */

protected func Damage()
{
  if(GetDamage() > 40)
  {
    //Effekte
    CastObjects(SPRK, 7, 50);
    Sparks(15, RGB(210, 210, 0));
    CreateSmokeTrail(RandomX(15,20), Random(360), 0,0, this());
    CreateSmokeTrail(RandomX(15,20), Random(360), 0,0, this());
    Sound("CrystalHit*");
    Sound("Blast1");
    Sound("RadioStatic.ogg");

    //Eventuelle Musik abstellen
    Sound("Radio_*.ogg", false, this(), 0, 0, -1);

    //Verschwinden
    RemoveObject();
  }
  else
  {
    //Umherfliegen
    SetSpeed(RandomX(-25, 25), RandomX(-45, -35));
    SetRDir(GetXDir()*2);
  }
}

/* Aufschlag */ 

protected func Hit()
{
  Sound("MetalHit*.WAV");
  return 1;
}