/*-- Fesselsymbol --*/

#strict 2

protected func Initialize() { SetAlive(0); }

func AttachTargetLost() { return RemoveObject(); }
