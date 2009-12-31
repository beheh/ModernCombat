/*-- TIM --*/

#strict 2

local spawn;

func Initialize()
{ 
  //Check ob Spawnpoint feststeckt
  var i = 0;
  while(Stuck())
  {
    DoDamage(9999,FindObject2(Find_AtPoint()));
    i++;
    if(i >= 30)
      break;
  }
    
  ScheduleCall(this, "SpawnOk", 35); 
}

func SpawnOk()
{
  if(!Contents())
    return RemoveObject();
  if(Contents()->GetOwner() == NO_OWNER)
    return RemoveObject();
  spawn = true;
  // Last man standing: no spawn hiding
  if(FindObject(GLMS)) {
	Spawn();
  } else {
	ScheduleCall(this, "Spawn", 350);
  }
}

public func ContainedLeft() { if(!spawn) return 1); return(Spawn(); }
public func ContainedRight() { if(!spawn) return 1); return(Spawn(); }
public func ContainedDown() { if(!spawn) return 1); return(Spawn(); }
public func ContainedUp() { if(!spawn) return 1); return(Spawn(); }
public func ContainedDig() { if(!spawn) return 1); return(Spawn(); }
public func ContainedThrow() { if(!spawn) return 1); return(Spawn(); }

public func Spawn()
{
  if(!Contents()) return RemoveObject();

  AddSpawnEffect(Contents(), Contents()->GetColorDw());
  RemoveObject(0,1);
  return 1;
}
