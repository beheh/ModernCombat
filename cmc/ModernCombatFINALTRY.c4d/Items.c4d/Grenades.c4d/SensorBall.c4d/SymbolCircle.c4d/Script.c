/*-- Kreissymbol --*/

#strict 2

local obj,time,starttime;


/* Einstellung */

public func Set(object target, bool nofadeout)
{
  obj = target;

  //Positionieren
  SetVertex(0,0,GetVertex(0,0,target));
  SetVertex(0,1,GetVertex(0,0,target));
  SetAction("Attach",target);

  //Standardfarbe: Weiß
  SetColorDw(RGB(255,255,255), this);

  //Sichtbarkeit gegenüber Verbündeten
  SetVisibility(VIS_Allies | VIS_Owner);

  //Bei Anfrage ausblenden
  if(nofadeout)
    return this;

  //Ausfaden
  FadeOut();

  //Entfernung planen
  Schedule("RemoveObject()", 80);

  return;
}