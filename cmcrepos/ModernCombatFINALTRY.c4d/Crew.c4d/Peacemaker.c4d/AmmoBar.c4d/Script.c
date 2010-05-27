/*-- Munitionsbalken --*/

#strict 2

local obj;
local iBarCount;
local fActive;

public func IsBar() {	return true; }
public func BarActive() {	return fActive; }

/* Initalisierung */

protected func Initialize()
{
	fActive = false;
	SetVisibility(VIS_None);
  SetGraphics("Row", this(), GetID(), 1, 1);
}

/* Einstellung */

public func Set(object target, int color)
{
  obj = target;

  SetVertex(0,0,GetVertex(0,0,target));
  //Erst mal von einem Balken ausgehen
  iBarCount = 1;
  SetVertex(0,1,GetVertex(0,1,target) - GetObjHeight(target)/2-(iBarCount*10));
  //Und dranmachen
  SetAction("Attach",target);

  SetClrModulation(color,this(),1);
  
  return true;
}

public func SetBarCount(int iCount)
{
	iBarCount = iCount;
}

public func Update()
{
	//Erst mal annehmen, dass wir verschwinden
	SetVisibility(VIS_None);
	fActive = false;
	//Schauen, ob wir überhaupt anzeigen brauchen
  if(!obj || !(GetOCF(obj) & OCF_Alive) || Contained(obj) || Hostile(GetOwner(), GetOwner(obj)))
   return;
  if(!obj->Contents() || !obj->Contents()->~IsWeapon() || !obj->AmmoStoring())
   return;
  //Alles ok? Waffe und Waffendaten holen...
  var weapon = obj->Contents();
  var ammobag = obj->AmmoStoring();
  //Die Munition die wir haben holen
  var ammocount = obj->GetAmmo(weapon->GetFMData(FM_AmmoID));
  //Und die Munition, die die Waffe aufnehmen kann
  var ammomax = weapon->GetFMData(FM_AmmoLoad);
  //Falls wir maximal 1 im Magazin haben wollen wir 10fach als 100%, ansonsten 3fach.
  if(ammomax == 1) {
  	ammomax *= 10;
  }
  else {
  	ammomax *= 3;
  }
  //Prozentsatz errechnen
  var percent = BoundBy((((100*1000)/ammomax)*ammocount)/1000,0,100);
  //Bei fast vollem Balken lohnt es sich nicht wirklich
  if(percent > 95) return;
	//Anpassen (runterrucken falls wir andere Balken haben)
  SetVertex(0,1,GetVertex(0,1,obj) - GetObjHeight(obj)/2-(iBarCount*10));
  //Prozentbalken anpassen
  SetObjDrawTransform(10*percent,0,-160*(100-percent),0,1000,0,0,1);
  //Jetzt können wir uns wieder sichtbar machen
  SetVisibility(VIS_Owner);
  fActive = true;
  return true;
}
