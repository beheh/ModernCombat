/*-- Radio --*/

#strict 2

local fPlaying;
local iTrack;

static const RDIO_TrackCount = 6;


/* Initialisierung */

func Initialize()
{
  //Zufälligen Song anwählen
  iTrack = Random(RDIO_TrackCount)+1;
}

/* Bedienung */

protected func ControlDig(pClonk)
{
  if(!IsPlaying())
    TurnOn();
  else
    TurnOff();
  Sound("ArrowHit");
}

protected func ControlRight(pClonk)
{
  if(IsPlaying())
  {
    NextTrack();
  }
  Sound("ArrowHit");
}

protected func ControlLeft(pClonk)
{
  if(IsPlaying())
  {
    LastTrack();
  }
  Sound("ArrowHit");
}

/* Radiologik */

public func IsPlaying()
{
  return fPlaying;
}

public func TurnOn()
{
  ScheduleCall(this, "StartSong", 8);
  Static();
  CreateParticle("NoGravSpark", 3, 3, 0, 0, 50, RGBa(10, 150, 250, 50));
  fPlaying = true;
}

public func TurnOff()
{
  StopSong();
  fPlaying = false;
}

public func NextTrack()
{
  var fOn = IsPlaying();
  if(fOn) TurnOff();
  if(iTrack == RDIO_TrackCount)
    iTrack = 1;
  else
    iTrack++;
  if(fOn) TurnOn();
}

public func LastTrack()
{
  var fOn = IsPlaying();
  if(fOn) TurnOff();
  if(iTrack == 1)
    iTrack = RDIO_TrackCount;
  else
    iTrack--;
  if(fOn) TurnOn();
}

protected func StartSong()
{
  SoundLevel(Format("RadioMusicTitle_O0%d.ogg", iTrack), 100, this);
}

protected func StopSong()
{
  ClearScheduleCall(this, "StartSong");
  Sound("RadioMusicTitle_O0*.ogg", false, this, 0, 0, -1);
}

protected func Static()
{
  Sound("RadioStatic.ogg", 0, this);
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
    TurnOff();

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

protected func Hit3()
{
  DoDmg(20);
  if(Random(3)) Static();
}