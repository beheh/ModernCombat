/*-- Reanimationssymbol --*/

#strict 2

local obj;


/* Einstellung */

public func Set(object target)
{
  obj = target;

  SetVertex(0, 0, GetVertex(0, 0, target)-5);
  SetVertex(0, 1, GetVertex(0, 1, target)+GetObjHeight(target)/2+10);
  SetAction("Attach", target);

  SetVisibility(VIS_Owner);
}
