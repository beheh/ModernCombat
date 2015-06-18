/*-- Bildschirm --*/

#strict 2


/* Initialisierung */

func Initialize()
{
  //Standardanimation
  SetAction("Eurocorps");
  AddFrame();
}

/* Clips festlegen */

local clips, lastclip;

public func SetClips(array aClips) {
  clips = aClips;

  lastclip = -1;

  //Ersten Clip auswählen
  PickClip();

  return true;
}

public func PickClip() {
  if(!clips)
    return;

  var r = Random(GetLength(clips) - 1);
  if(lastclip == -1 || r >= lastclip)
    r += 1;

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
  if(GetAction() != "Eurocorps")
    extra[GetLength(extra)] = Format("SetAction(%s)",GetAction());
}
