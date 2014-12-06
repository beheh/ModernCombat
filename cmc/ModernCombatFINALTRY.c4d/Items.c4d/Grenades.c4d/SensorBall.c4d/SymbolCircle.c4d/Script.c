/*-- Kreissymbol --*/

#strict 2

local obj,time,starttime;


/* Einstellung */

public func Set(object target)
{
  obj = target;

  //Positionieren
  SetVertex(0,0,GetVertex(0,0,target));
  SetVertex(0,1,GetVertex(0,0,target));
  SetAction("Attach",target);

  //Farbe
  SetColor(RGB(255,255,255), this);

  //Sichtbarkeit gegen�ber Verb�ndeten
  SetVisibility(VIS_Allies | VIS_Owner);

  //Ausfaden
  FadeOut();

  //Entfernung planen
  Schedule("RemoveObject()", 80);
}