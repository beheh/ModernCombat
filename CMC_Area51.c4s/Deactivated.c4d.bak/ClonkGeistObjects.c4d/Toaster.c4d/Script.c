/*-- Toaster --*/

#strict 2

public func IsToaster() { return true; }
public func IsAprilFool() { return true; }

protected func RejectCollect(id def, object obj)
{
	if(def == TOST && !LocalN("toasted", obj)) return false;
	else
		return true;
}

public func ControlDigDouble()
{
	if(FindContents(TOST) && !GetEffect("Toasting", this))
	{
		Sound("ToasterDown.ogg", 0, this, 100);
		AddEffect("Toasting", this, 101, 36*15, this);
	}
		
	return true;
}

public func FxToastingStop(object target, int nr)
{
	for(var toast in FindObjects(Find_ID(TOST), Find_Container(this)))
	{
		toast->Exit();
		toast->SetToasted();
		toast->SetYDir(RandomX(-25, -35));
		toast->SetXDir(RandomX(-15, 15));
		toast->SetRDir(RandomX(-25, 25));
	}
	Sound("ToasterUp.ogg", 0, this, 100);
}

