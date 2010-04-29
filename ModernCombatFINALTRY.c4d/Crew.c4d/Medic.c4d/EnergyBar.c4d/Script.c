/*-- Energiebalken --*/

#strict

local obj;


/* Initalisierung */

protected func Initialize()
{
  SetGraphics("Row", this(), GetID(), 1, 1);
}

/* Einstellung */

public func Set(object target, int color)
{
  obj = target;

  SetVertex(0,0,GetVertex(0,0,target));
  SetVertex(0,1,GetVertex(0,1,target) - GetObjHeight(target)/2-10);
  SetAction("Attach",target);

  SetVisibility(VIS_Owner);

  SetClrModulation(color,this(),1);

  Update();
}

public func Update()
{
  if(!(GetOCF(obj) & OCF_Alive) || Contained(obj) || Hostile(GetOwner(), GetOwner(obj)))
   RemoveObject();
  var percent = BoundBy(GetEnergy(obj)*100/(GetDefCoreVal("Energy","Physical",GetID(obj))/1000),0,100);
  if(percent > 95)
   RemoveObject();
  SetObjDrawTransform(10*percent,0,-160*(100-percent),0,1000,0,0,1);
  ScheduleCall(0,"Update",1);
}
