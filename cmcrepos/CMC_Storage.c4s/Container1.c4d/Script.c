/*-- Container --*/

#strict

public func Initialize() { 
	SetAction("Be");
	SetPerspective(1);
	SetColorDw(HSL(Random(40), Random(100)+150, 150));
}

public func IsCraneGrabable() { return(1); }

public func Hit() { Sound("ClonkHit*"); }

public func SetPerspective(int pers) {
	SetPhase(pers);
	SetSolidMask(pers*64,0,64,29);
	if(pers == 2) {
		SetVertexXY(0,+15,13);
		SetVertexXY(0,-15,13);
	}
	else {
		SetVertexXY(0,+28,13);
		SetVertexXY(1,-28,13);	
	}
}

protected func Damage(iChange)
{
  if(GetDamage() > 100 && !OnFire())
  {
    Incinerate();
    Schedule("Explode(60)",120);
  }
}

/* Serialisierung */

public func Serialize(array& extra)
{
	// TODO: Farbe?
	if (GetPhase() != 1) {
		extra[GetLength(extra)] = Format("SetPerspective(%d)", GetPhase());
	}
}
