/*-- Selbstschussanlage-Appendto --*/

#strict 2
#appendto SEGU

local fAAMode;
local curPrio;
local powerMode;

public func SearchLength()  {return 350+fAAMode*150;}	//Suchlänge
public func MaxRotLeft()			{return 80+GetR();}			//Maximaler Winkel links
public func MaxRotRight()			{return 280+GetR();}			//Maximaler Winkel rechts

func Set(bool fMode)
{
  fAAMode = fMode;
}

public func Activity()
{
  //Wuah, haben wir eine Waffe?
  if(!GetAttWeapon()) return;
  //Sind wir im Eimer?
  if(EMPShocked()) return;
  if(IsDestroyed()) return;
  //Sind wir aktiv?
  if(!fActive) return;
  if(IsRepairing()) return;
  //Wenn nicht schon gesetzt: Turn-Action
  if(GetAction() != "Turn")
    SetAction("Turn");
    
  //Owner updaten
  cur_Attachment->SetTeam(GetTeam());

  // alle 30 Frames
  if(!(GetActTime()%30))
  {
    if(GetTeam())
      var rgb = GetTeamColor(GetTeam());
    else if(GetOwner())
      var rgb = GetPlrColorDw(GetOwner());
    else
      var rgb = RGB(255,255,255);
    CreateParticle("FlashLight",0,4,0,0,3*15,rgb,this);
  }
  
  /* Patroullie fahren */
  if(!fAAMode)  
  {
    // alle 5 Frames
    if(!(GetActTime()%5))
    {
      //Zu weit links?
      if(AimAngle() <= MaxRotLeft())
      {
        //Wir fahren zurück
        iPat_Dir = 1;
        target_angle = MaxRotRight();
        GotTarget = 0;
      }
      //Oder zu weit rechts?
      else if(AimAngle() >= MaxRotRight())
      {
        //Hinfahren
        iPat_Dir = -1;
        target_angle = MaxRotLeft();
        GotTarget = 0;
      }
      if(!GotTarget)
      {
        aim_angle += iPat_Dir*3;
      }
    }
    if(GotTarget)
      aim_angle += BoundBy(target_angle-AimAngle(),-1,1);

    /* Feinde suchen */
  
    if(!GotTarget)
    {
      if(Shooting)
      {
        Shooting = false;
          GetAttWeapon()->StopAutoFire();
      }
      //Nur alle 3 Frames
      if(!(GetActTime()%3))
        GotTarget = Search();
    }
    else
    {
      target_angle = Angle(GetX(), GetY() + 7, GetX(GotTarget), GetY(GotTarget));
  
      if((MaxRotRight() >= 360) && (target_angle < MaxRotRight()-360))
        target_angle += 360;
  
      if(Abs(AimAngle() - target_angle) < 15)
      {
        Shooting = true;
        if(!GetAttWeapon()->IsShooting())
          GetAttWeapon()->Fire(this);
        if(GetAmmo(GetAttWeapon()->GetFMData(FM_AmmoID), GetAttWeapon()) < GetAttWeapon()->GetFMData(FM_AmmoUsage))
          Reload();
      }
      else
      {
        Shooting = false;
        GetAttWeapon()->StopAutoFire();
      }

      if(!CheckTarget(GotTarget,this))
        GotTarget = 0;
      else if(!PathFree(GetX(),GetY()+7,GetX(GotTarget), GetY(GotTarget)))
        GotTarget = 0;
      else if(ObjectDistance(GotTarget,this) > SearchLength())
        GotTarget = 0;
    }
  }
  else
  {
    powerMode--;
    if(!ValidTarget(GotTarget))
    {
      curPrio = 0;
      GotTarget = 0;
    }

    if(curPrio < 3 && (powerMode > 0 || !(GetActTime()%2)))
        GotTarget = SearchAA();
    
    if(GotTarget)
    {
      powerMode = 100;
      var target_angle = Angle(GetX(), GetY(), GetX(GotTarget), GetY(GotTarget));
    
      if((MaxRotRight() >= 360) && (target_angle < MaxRotRight()-360))
        target_angle += 360;
    
      aim_angle += BoundBy(target_angle-AimAngle(),-5,5);
      
      if(Abs(AimAngle() - target_angle) < 15)
      {
        Shooting = true;
        if(!GetAttWeapon()->IsShooting())
          GetAttWeapon()->Fire(this);
        if(GetAmmo(GetAttWeapon()->GetFMData(FM_AmmoID), GetAttWeapon()) < GetAttWeapon()->GetFMData(FM_AmmoUsage))
          Reload();
      }
      else
      {
        Shooting = false;
        GetAttWeapon()->StopAutoFire();
      }
    }
    else
    {
      aim_angle += BoundBy(180-aim_angle,-5,5);
      if(Shooting)
      {
        Shooting = false;
          GetAttWeapon()->StopAutoFire();
      }
      curPrio = 0;
    }
  }
}

public func SearchAA()
{
  var pAim;
  var Targets = FindAATargets(SearchLength()*3/2);
  var pTarget = GotTarget;
  
  for(pAim in Targets)
  {
    var priority = 0;
    
    if(pAim->~IsHelicopter())
      priority = 1;
    if(GetOCF(pAim) & OCF_Alive)
      priority = 2;
    if(pAim->~IsRifleGrenade() || pAim->~IsRocket())
      priority = 3;

    if(priority > curPrio)
    {
      pTarget = pAim;
      curPrio = priority;
    }
  }
  
  return pTarget;
}

func FindAATargets(int maxDist)
{
  var pT;
  var targets = CreateArray();
  
  var x = GetX();
  var y = GetY();

  var preTargets = FindObjects(
		Find_Distance(maxDist, AbsX(x), AbsY(y)),
		Find_NoContainer(),
        Find_Or(
                Find_Func("IsRifleGrenade"),
                Find_Func("IsRocket"),
                Find_Func("IsHelicopter"),
                Find_And(Find_OCF(OCF_Alive))),
    Find_Hostile(GetOwner()),
		Sort_Distance()
        );

  var gravity = GetGravity();
  SetGravity(0);
  
  for(pT in preTargets)
  {
    if(ValidTarget(pT))
		  // dann rein in Ergebnismenge...
		  targets[GetLength(targets)] = pT;
	}
	
	SetGravity(gravity);
	return targets;
}

func ValidTarget(object pT)
{
  if(!pT) return;

  if((GetOCF(pT) & OCF_Alive) && GetProcedure(pT) != "FLIGHT") return;
    
  var ox = GetX(pT);
	var oy = GetY(pT);
	  
	//Winkel zum Ziel
  target_angle = Angle(GetX(), GetY() + 7, ox, oy);
  target_angle = Normalize(target_angle, 0);
  if(MaxRotRight() < 360 && (target_angle < MaxRotLeft() || target_angle > MaxRotRight()))
    return;
  else if(MaxRotRight() >= 360 && (target_angle < MaxRotLeft() && target_angle > MaxRotRight()-360))
    return;
    
	// Pfad frei
  var simX = GetX();
	var simY = GetY();
	var xdir = Sin(target_angle, 10);
	var ydir = -Cos(target_angle, 10);

	if(SimFlight(simX, simY, xdir, ydir) && (Distance(GetX(), GetY(), simX, simY) < Distance(GetX(), GetY(), ox, oy)))
    return;

	// unsichtbare Ziele
	if(!CheckVisibility(pT, this))
		return;

	//Alles in Ordnung
	return true;
}
