/*-- Neues Objekt --*/

#strict 2

local skin;

public func SetSkin(string szSkin)
{
	skin = szSkin;
	SetGraphics(skin);
}

public func ActivateEntrance(object pObj) { return Enter(this, pObj); }
