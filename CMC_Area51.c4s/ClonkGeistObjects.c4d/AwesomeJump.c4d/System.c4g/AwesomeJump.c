/*-- Neues Script --*/

#strict 2

#appendto HZCK

public func ControlUp()
{
  if(IsCrawling() && !IsAiming())
  {
   	ScheduleCall(this, "StopCrawling", 10);
   	return 1;
  }
  return _inherited(...);
}

public func ControlUpDouble()
{
  if (GetAction() == "Push" || IsRiding())
  {
    if(GetActionTarget()->~ControlUpDouble(this))
      return true;
  }
  if(GetAction() == "StartCrawl" && Inside(GetPhase(), 3, 6))
  {
  	ClearScheduleCall(this, "StopCrawling");
  	var jump = GetPhysical("Jump", 0) * 100 / GetPhysical("Jump", 0,0, GetID());
  	SetXDir(5 *-(GetDir()*2-1) * jump / 100);
  	SetYDir(-42 * jump / 100);
  	SetAction("BackFlip");
  	return true;
  }
  
  return _inherited(...);
}
