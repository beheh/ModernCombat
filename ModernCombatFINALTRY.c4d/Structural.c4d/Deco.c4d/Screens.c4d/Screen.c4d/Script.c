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

local clips;

public func SetClips(array aClips) {
	clips = aClips;
	
	//Ersten Clip auswählen
	PickClip();
	
	return true;
}

public func PickClip() {
	if(!clips)
		return;
	
	var clip = clips[Random(GetLength(clips))];
	if(GetType(clip) == C4V_Int)
		SetAction(Format("Clip%02d", clip));
	else
		SetAction(clip);
	
	return true;
}

/* Serialisierung */

public func Serialize(array& extra)
{
  if(GetAction() != "Eurocorps")
    extra[GetLength(extra)] = Format("SetAction(%s)",GetAction());
}
