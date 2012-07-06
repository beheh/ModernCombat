/*-- Laserpunkt --*/

#strict 2

local fActive;

public func Active() {return fActive;}

func Initialize() {
	Start();
}

func Start()
{
	fActive = true;
	CreateParticle("PSpark", 0, 0, 0, 0, 50, GetColor(this));
	AddEffect("Activity", this, 1, 1 ,this);
}

func Stop()
{
	fActive = false;
	if(GetEffect("Activity",this))
		RemoveEffect("Activity",this);
}

public func FxActivityTimer(object pTarget, int iEffectNumber, int iEffectTime) {
	CreateParticle("PSpark", 0, 0, 0, 0, 50, GetColor(this));
}
