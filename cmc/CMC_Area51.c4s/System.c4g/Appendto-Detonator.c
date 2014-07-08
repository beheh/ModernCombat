#strict 2
#appendto C4PA

/* Zündung: Zeit ist nun fest auf 36 Frames/1 Sekunde */

public func Activate(object pActivator)
{
  for (var c4 in GetC4())
  {
    SetController(GetOwner(pActivator), c4);
    ScheduleCall(c4, "Trigger", Max(ObjectDistance(c4) / 20, 1));
  }

  //Effekte
  CreateParticle("FlashLight", 0, 0, 0, 0, 60, RGBa(255), this);
  Sound("C4PA_Activate.ogg");

  //Bild ändern wenn C4 übrig
  if (GetPackPoints())
  {
    SetPicture(0, 4, 64, 64);
    Schedule("SetPicture(64, 4, 64, 64)", 25);
  }
}
