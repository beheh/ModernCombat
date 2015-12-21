/*--- Raketenbatterie ---*/

#strict 2
#include CSTR
#include FDSE

local launches;

public func BuildCost() {return 25;}
public func AutoRepairDuration()	{return 0;}
public func MaxDamage()			{return 30;}
public func IsRepairable()	{return !fDestroyed;}

public func TracerRadius()	{return 350;}


public func Initialize() {return _inherited(...);}

public func CheckForTargets()
{
	if(fDestroyed || GetCon() < 100 || GetEffect("LaunchRockets", this))
		return;
		
	for(var pEnemy in FindObjects(Find_Distance(TracerRadius()), Sort_Distance()))
  {
    var iEffectTracer = GetEffect("TracerDart", pEnemy);
    if(!iEffectTracer) continue;
    var iTeam = EffectVar(2, pEnemy, iEffectTracer);
    if(iTeam != GetPlayerTeam(GetController())) continue;
    if(!PathFree(GetX(), GetY(), GetX(pEnemy), GetY(pEnemy))) continue;
    
    AddEffect("LaunchRockets", this, 1, 5, this);
    break;
  }
}

private func SingleLaunch()
{
	//Rakete abfeuern
  var rocket = CreateObject(SRML,0,-10,GetOwner());
  if(!rocket) return;
  rocket->Launch(0, this);
  rocket->Sound("RLSA_Fire*.ogg");
  SetController(GetController(), rocket);
}

public func FxLaunchRocketsStart()
{
	SingleLaunch();
	launches++;
}

public func FxLaunchRocketsTimer()
{
	if(fDestroyed)
		return -1;
	
	SingleLaunch();
	launches++;
	
	if(launches > 1)
	{
		Replaced();
		return -1;
	}
}

public func Replaced()
{
  fDestroyed = true;

  //Verschwinden
  FadeOut(this);
}
