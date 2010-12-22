/*-- Munitionsbalken --*/

#strict 2

local obj;
local iBarCount;
local fActive;

public func IsBar()
{
	return true;
}
public func BarActive()
{
	return fActive;
}
public func RejectEntrance()
{
	return true;
}


/* Initialisierung */

protected func Initialize()
{
	fActive = false;
	SetVisibility(VIS_None);

	SetGraphics("Row", this, GetID(), 1, 1);
}

/* Einstellung */

public func Set(object target, int color, int barcount)
{
	obj = target;
	iBarCount = barcount;

	SetVertex(0, 0, GetVertex(0, 0, target));

	//Balken setzen
	SetVertex(0, 1, GetVertex(0, 1, target) - GetObjHeight(target) / 2 - (iBarCount * 10));

	//Und festsetzen
	SetAction("Attach", target);

	//Färben
	SetClrModulation(color, this, 1);

	return true;
}

public func SetBarCount(int iCount)
{
	iBarCount = iCount;
}

public func Update()
{
	//Verschwinden annehmen
	SetVisibility(VIS_None);
	fActive = false;

	//Anzeige benötigt?
	if (!obj || !(GetOCF(obj) & OCF_Alive) || Contained(obj) || Hostile(GetOwner(), GetOwner(obj)))
		return;
	if (!Contents(0, obj) || !Contents(0, obj)->~IsWeapon() || !obj->AmmoStoring())
		return;

	//Waffendaten einholen
	var weapon = Contents(0, obj);

	//Munitionsdaten einholen
	var ammocount = obj->GetAmmo(weapon->GetFMData(FM_AmmoID));
	var ammomax = weapon->GetFMData(FM_AmmoLoad);

	//Falls maximal 1 im Magazin, 10fach als 100%, ansonsten 3fach
	if (ammomax == 1)
	{
		ammomax *= 10;
	}
	else 
	{
		ammomax *= 3;
	}

	//Prozentsatz errechnen
	var percent = BoundBy((((100 * 1000) / ammomax) * ammocount) / 1000, 0, 100);

	//Anzeige bei fast voller Anzeige unnötig
	if (percent > 95)
		return;

	//Anpassen (falls andere Anzeigen vorhanden)
	SetVertex(0, 1, GetVertex(0, 1, obj) - GetObjHeight(obj) / 2 - (iBarCount * 10));

	//Prozentbalken anpassen
	SetObjDrawTransform(10 * percent, 0, -160 * (100 - percent), 0, 1000, 0, 0, 1);

	//Sichtbarkeit setzen
	SetVisibility(VIS_Owner);
	fActive = true;
	return true;
}