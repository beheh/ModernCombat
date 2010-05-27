/*-- Energiebalken --*/

#strict 2

local obj;
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
  SetVertex(0,1,GetVertex(0,1,target) - GetObjHeight(target)/2-10);
  SetAction("Attach",target);

  SetClrModulation(color,this(),1);
  
  return true;
}

public func Update()
{
	SetVisibility(VIS_None);
	fActive = false;
  if(!obj || !(GetOCF(obj) & OCF_Alive) || Contained(obj) || Hostile(GetOwner(), GetOwner(obj)))
   return;
  var percent = BoundBy(GetEnergy(obj)*100/(GetDefCoreVal("Energy","Physical",GetID(obj))/1000),0,100);
  if(percent > 95)
   return;
  SetVisibility(VIS_Owner);
  fActive = true;
  SetObjDrawTransform(10*percent,0,-160*(100-percent),0,1000,0,0,1);
  return true;
}
