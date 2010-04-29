/*-- Heilzeichen --*/

#strict 2

local obj;

/* Einstellung */

public func Set(object target)
{
  obj = target;

  SetVertex(0,0,GetVertex(0,0,target));
  SetVertex(0,1,GetVertex(0,1,target) - GetObjHeight(target)/2-10);
  SetAction("Attach",target);

  SetVisibility(VIS_Allies | VIS_Owner);

  Update();
}

public func Update()
{
  if(!(GetOCF(obj) & OCF_Alive) || Contained(obj) || Hostile(GetOwner(), GetOwner(obj)))
   RemoveObject();
  if(BoundBy(GetEnergy(obj)*100/(GetDefCoreVal("Energy","Physical",GetID(obj))/1000),0,100) > 30)
   RemoveObject();
  ScheduleCall(0,"Update",1);
}
