/* MG-Animation */

#strict

local heli, vis;

protected func Initialize()
{
  vis = true;
  //am Anfang sitzt bestimmt noch keiner drin
  SetAction("Empty");
}

public func Set(object pObj)
{
  heli = pObj;
}

protected func TimerCall()
{
  //ohne Heli fühlen wir uns einsam...
  if (!heli) return(RemoveObject());
  //nur sichtbar, wenn nicht drehend
  if (GetAction(heli) eq "Turn")
  {
    if (vis)
    {
      SetVisibility(VIS_None);
      vis = false;
    }
  }
  else
    if (!vis)
    {
      SetVisibility(VIS_All);
      vis = true;
    }
  //schauen, ob MG überhaupt besetzt
  if (LocalN("Gunner", heli) && GetAction()ne"Occupied")
  {
    SetAction("Occupied");
    SetOwner(GetOwner(LocalN("Gunner", heli)), this());
  }
  if (!LocalN("Gunner", heli) && GetAction()ne"Empty")
  {
    SetAction("Empty");
    SetOwner(-1, this());
  }
  //Drehung des Heli abfragen
  var rot = GetR(heli)+(GetDir(heli)*2-1)*90+(GetDir(heli)*2-1)*30;
  //und in die Positionsbestimmung einfließen lassen
  SetPosition(GetX(heli) + Sin(rot, 24),
              GetY(heli) - Cos(rot, 24), this());
  SetXDir(GetXDir(heli));
  SetYDir(GetYDir(heli));
  SetR(GetR(heli));
  SetRDir(GetRDir(heli));
  //zu guter letzt: Phase setzen
  if (GetAction()eq"Occupied")
  {    
    SetDir(GetDir(heli));
    if (!GetDir(heli)) 
      SetPhase(BoundBy(60 - ((LocalN("MG_rot", heli) + 90) / 3), 0, 60));
    else
      SetPhase(BoundBy((LocalN("MG_rot", heli) + 90) / 3, 0, 60));
  }
  else
  {
    SetPhase(20+GetDir(heli)*20);
  }
  return(true);
}

