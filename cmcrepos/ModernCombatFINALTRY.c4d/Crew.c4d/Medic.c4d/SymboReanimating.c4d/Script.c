/*-- Reanimationszeichen --*/

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

  Update();
}

public func Update()
{
  var target = GetActionTarget();
  if(!target) return;

  if( !obj || Hostile(GetOwner(), GetOwner(obj)) || time == 0 || !target)
   RemoveObject();
  
  var percent = time*255/starttime;
  SetClrModulation(RGBa(255,255,255,BoundBy(InvertA1(percent,255),0,255)));
  time -= 3;
  
  var fPaddles = false;
  var i = target->ContentsCount();
  while(i--) {
		if(target->Contents(i)->GetID() == CDBT) {
			fPaddles = true;
			break;
		}
	}
	if(fPaddles) {
		SetGraphics("Arrow", this);
	}
	else {
		SetGraphics(0, this);
	}		     

  ScheduleCall(0,"Update",3);
}
