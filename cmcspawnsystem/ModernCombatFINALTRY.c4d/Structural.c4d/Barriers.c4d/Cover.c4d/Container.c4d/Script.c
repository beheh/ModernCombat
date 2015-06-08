/*-- Container --*/

#strict 2

public func IsCraneGrabable()	{return(1);}


/* Initialisierung */

public func Initialize()
{ 
  SetAction("Be");
  SetPerspective(1);

  //Zufälliges Aussehen
  var rnd = Random(6)+2;
  SetGraphics(Format("%d", rnd));
}

public func SetPerspective(int pers)
{
  SetPhase(pers);
  SetSolidMask(pers*64,0,64,29);
  if(pers == 2)
  {
    SetVertexXY(0,+15,13);
    SetVertexXY(0,-15,13);
  }
  else
  {
    SetVertexXY(0,+28,13);
    SetVertexXY(1,-28,13);	
  }
}

/* Aufschlag */

public func Hit()
{
  Sound("ClonkHit*");
}

/* Serialisierung */

public func Serialize(array& extra)
{
  if (GetPhase() != 1)
  {
    extra[GetLength(extra)] = Format("SetPerspective(%d)", GetPhase());
  }
}