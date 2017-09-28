/*-- Reanimationssymbol --*/

#strict 2

local obj,time,starttime;


/* Einstellung */

public func Set(object target)
{
  obj = target;
  time = FKDT_SuicideTime*35;
  starttime = time;

  SetVertex(0,0,GetVertex(0,0,target));
  SetVertex(0,1,GetVertex(0,1,target) + GetObjHeight(target)/2+20);
  SetAction("Attach",target);

  SetVisibility(VIS_Allies | VIS_Owner | VIS_God);
}

public func Update()
{
  var target = GetActionTarget();

  //Konditionen prüfen
  if(!obj || Hostile(GetOwner(), GetOwner(obj)) || time <= 0 || !target || !GetAlive(target->GetClonk()))
    return RemoveObject();

  time -= 3;

  //Symbol aktualisieren
  //Zeitanzeige
  var percent = InvertA1(time*43/starttime,43);
  SetPhase(percent);

  //Transparenz
  percent = time*210/starttime;
  SetClrModulation(RGBa(255,255,255,BoundBy(InvertA1(percent,180),0,100)));

  //Status
  if(FindContents(CDBT, target))
  {
    if(target->~RejectReanimation())
      SetGraphics("NegativeDefib", this);
    else
      SetGraphics("Defib", this);
  }
  else
  {
    if(target->~RejectReanimation())
      SetGraphics("Negative", this);
    else
      SetGraphics(0, this);
  }
}