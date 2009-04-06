/*-- Lavablase --*/

#strict


protected func Initialize()
{
  SetComDir(COMD_Up());
  SetAction("Grow");
  SetDir(Random(4));
}

private func Floating()
{
  // an der Oberfläche zerplatzen
  if (!GBackLiquid(0,-3) || (GetActTime() > 1500))
  {
    CastParticles ("LavaGlow2", 10,25, 0, -2, 60, 90);
    Sound("blub",0,0,50);
    return(RemoveObject());
  }
  // sich den Weg bahnen
  /*if (GBackSolid(0, -10)) SetYDir(+15);
  if (GBackSolid(-8,-8)) SetXDir(+5);
  if (GBackSolid(+8,-8)) SetXDir(-10);*/
}