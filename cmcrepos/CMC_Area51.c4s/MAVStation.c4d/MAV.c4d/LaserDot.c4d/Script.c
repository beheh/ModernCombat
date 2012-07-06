/*-- Laserpunkt --*/

#strict

func Initialize() {
	CreateParticle("PSpark", 0, 0, 0, 0, 50, GetColor(this));
  AddEffect("Activity", this, 1, 1 ,this);
}

public func FxActivityTimer(object pTarget, int iEffectNumber, int iEffectTime) {
	CreateParticle("PSpark", 0, 0, 0, 0, 50, GetColor(this));
}
