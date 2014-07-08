/*-- Flagge --*/

#strict 2


/* Initialisierung */

protected func Initialize()
{
  SetOwner(NO_OWNER);
  SetAction("Fly");
  return 1;
}

/* TimerCall */

private func Wind()
{
  //Sound
  if(Random(6))
    Sound("FlagWave*.ogg");

  //Animation an Wind ausrichten
  if(GetAction() == "Fly")
    SetDir(BoundBy(GetWind() / 15 + 3, 0, 6));
}