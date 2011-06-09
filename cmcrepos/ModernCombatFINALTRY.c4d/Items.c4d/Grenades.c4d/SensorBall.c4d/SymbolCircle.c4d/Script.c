/*-- Kreissymbol --*/

#strict 2

local obj,time,starttime;


/* Einstellung */

public func Set(object target)
{
  obj = target;

  SetVertex(0,0,GetVertex(0,0,target));
  SetVertex(0,1,GetVertex(0,0,target));
  SetAction("Attach",target);

  SetVisibility(VIS_Allies | VIS_Owner);

  //Ausfaden
  FadeOut();

  //Entfernung planen
  Schedule("RemoveObject()", 80);
}