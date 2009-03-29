/* Fahne */

#strict

protected func Initialize()
{
  SetOwner(NO_OWNER);
  SetAction("Fly");
  return(1);
}

/* TimerCall */

private func Wind()
{
    if(GetAction() S= "Fly") SetDir(BoundBy(GetWind() / 15 + 3, 0, 6));
}
