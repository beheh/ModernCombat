/*-- Bildschirm --*/

#strict 2


/* Initialisierung */

func Initialize()
{
  //Standardanimation
  SetAction("Eurocorps");
  //Rahmen erstellen
  AddFrame();
}

/* Clips festlegen */

local clips, lastclip;

public func SetClips(array aClips)
{
  clips = aClips;

  lastclip = -1;

  //Ersten Clip auswählen
  PickClip();

  return true;
}

/* Clip auswählen */

public func PickClip()
{
  //Keine Clips: Nichts unternehmen
  if(!clips)
    return;

  var r = 0;

  //Zufällig auswählen wenn mehr als ein Clip
  if(GetLength(clips) > 1)
  {
    r = Random(GetLength(clips) - 1);
    if(r >= lastclip)
      r += 1;
  }

  //Clip starten
  var clip = clips[r];
  if(GetType(clip) == C4V_Int)
    SetAction(Format("Clip%02d", clip));
  else
    SetAction(clip);

  lastclip = r;

  return true;
}

/* Serialisierung */

public func Serialize(array& extra)
{
  extra[GetLength(extra)] = Format("SetAction(%s)",GetAction());
}