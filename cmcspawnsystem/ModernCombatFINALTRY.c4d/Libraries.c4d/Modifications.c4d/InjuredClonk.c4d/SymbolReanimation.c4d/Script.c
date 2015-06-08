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
  SetVertex(0,1,GetVertex(0,1,target) + GetObjHeight(target)/2+10);
  SetAction("Attach",target);

  SetVisibility(VIS_Allies | VIS_Owner);
}

public func Update()
{
  var target = GetActionTarget();

  if(!obj || Hostile(GetOwner(), GetOwner(obj)) || time == 0 || !target || !GetAlive(target->GetClonk()))
    return RemoveObject();

  var percent = time*255/starttime;
  SetClrModulation(RGBa(255,255,255,BoundBy(InvertA1(percent,255),0,255)));
  time -= 3;

  //Symbol aktualisieren
  if(FindContents(CDBT, target))
  {
    if(target->~RejectReanimation())
      SetGraphics("NegativeArrow", this);
    else
      SetGraphics("Arrow", this);
  }
  else
  {
    if(target->~RejectReanimation())
      SetGraphics("Negative", this);
    else
      SetGraphics(0, this);
  }
}