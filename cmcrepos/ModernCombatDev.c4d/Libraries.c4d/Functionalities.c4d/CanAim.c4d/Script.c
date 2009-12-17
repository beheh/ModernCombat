#strict 2

/* Dieses Objekt stellt die Grundfunktionalit�t des Zielens f�r den Hazardclonk
     bereit, lso das Zielen mit dem Zielkreuz. SquatAiming (Dreimal-Stopp) ist
     hier nicht mit drin.
     Damit ein Clonk diese Funktionalit�t benutzen kann, muss er bei ReadyToFire
      am Ende return _inherited(); aufrufen sowie falls AimAngle() �berladen werden
     soll den Script hier ber�cksichtigen.
	 
     Ansonsten um Zielen zu steuern bei den Control*-Funktionen jeweils
     if(ControlAim("ControlUpDouble")) return 1; (im Beispiel von ControlUpDouble)
     aufrufen. Und zwar bei -Update, -Up, -UpDouble, -Down, -Left, -Right,
     -LeftDouble, -RightDouble, -Command, -Contents.

     Oder beim Hazard abgucken.
*/

local crosshair;

static const AIM_Step = 10;
static const AIM_Max = 150;

public func AimStep()	{ return AIM_Step; }
public func AimMax()	{ return AIM_Max; }

public func GetCrosshair()
{
  return crosshair;
}

// Muss Aktion "Aim" haben (und "AimLow")

protected func ControlAim(string command)
{
	if(!IsAiming()) return false;
	
	else if(command == "ControlUpdate")
		AimUpdate(this, Par(1), 1, "ControlConf");

	else if(command == "ControlUp" || command == "ControlUpDouble")
		AimUp(this, 1, "ControlConf");

	else if(command == "ControlDown")
		AimDown(this, 1, "ControlConf");
		
	else if(command == "ControlLeft")
  {
		if(GetDir() == DIR_Left) return false;
	    SetDir(DIR_Left);
	    UpdateAiming();
	}
	
	else if(command == "ControlRight")
  {
		if(GetDir() == DIR_Right) return false;
	    SetDir(DIR_Right);
	    UpdateAiming();
	}
	
	else if(command == "ControlLeftDouble" || command == "ControlRightDouble")
  {
		StopAiming();
	}
	
	else if(command == "ControlCommand")
  {
		return DoMouseAiming(Par(2),Par(3));
	}
	
	else if(command == "ControlContents")
  {
		var pTarget = FindContents(Par(1));
		ChangeWeapon(pTarget);
		return false;

	}

	return true;
}

/* Zielen */

public func IsAiming()
{
  if(Contained())
  {
    if(Contained()->~CanAim())
      return true;
  }
  
  if(GetProcedure() == "PUSH")
  {
    if(GetActionTarget())
      if(GetActionTarget()->~CanAim())
        return true;
  }

  return WildcardMatch(GetAction(),"Aim*");
}

public func ReadyToAim()
{
   // In Geb�ude
   if(Contained()) return Contained()->~ReadyToAim();
   // Nur beim Laufen
   if(GetAction() == "Walk") return true;
}

public func ControlConf(int conf)
{
  // Feineres Zielen mit JnR-Steuerung m�glich
  if(GetPlrCoreJumpAndRunControl(GetController()))
    DoAiming(conf * AimStep() / 5);
  else
    DoAiming(conf * AimStep());
}

public func DoAiming(int iChange)
{
  //zielen wir �berhaupt?
  if(!IsAiming())
    return;

  var angle = Abs(crosshair->GetAngle()) + iChange;

  // Winkel anpassen, wenn keine freie Auswahl (klassische Steuerung)
  if(!GetPlrCoreJumpAndRunControl(GetController()))
    angle = angle-angle%AimStep();

  // Winkel wird zu gro�?
  if(angle > AimMax() || angle < 0)
    return;

  if(GetDir() == DIR_Left)
    angle = 360-angle;
  crosshair->SetAngle(angle);

  UpdateAiming();
}

public func DoMouseAiming(int iTx, int iTy)
{
	if(!Contained())
  {
		var iAngle = Normalize(Angle(GetX(),GetY(),iTx,iTy),-180);

		if(iAngle > 0)
			SetDir(DIR_Right);
		else
			SetDir(DIR_Left);

		//Winkel wird zu gro�?
		iAngle = BoundBy(iAngle,-this->~AimMax(),+this->~AimMax());
    
    //Henry empfiehlt das nicht, aber gewisse Leute wollen das ja unbedingt. -,-
		/*
    if(!(Contents()->IsRecharging()) && !(Contents()->IsShooting()))
    {
      var old = crosshair->GetAngle();
      if((Abs(AngleOffset4K(old,iAngle)) < 5))
        this->~FireAimWeapon();
    }
		*/
    
    AddEffect("IntMouseAiming",this,10,1,this,0,iAngle);
    
		return true;
	}
	return false;
}

public func FxIntMouseAimingStart(object pTarget, int iEffectNumber, int iTemp, iAngle)
{
  EffectVar(0,pTarget,iEffectNumber) = iAngle;
  return 0;
}

public func FxIntMouseAimingEffect(string szNewEffect, object pTarget, int iEffectNumber)
{
  if(szNewEffect == "IntMouseAiming") return -3;
  return 0;
}

public func FxIntMouseAimingAdd(object pTarget, int iEffectNumber, string szNewEffectName, int iNewEffectTimer, iAngle)
{
  EffectVar(0,pTarget,iEffectNumber) = iAngle;
  return 1;
}

public func FxIntMouseAimingTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
  if(!pTarget->IsAiming()) return -1;

  var end = EffectVar(0,pTarget,iEffectNumber);
  var cur = crosshair->GetAngle();
  
  var dir = +1;
  if(end < cur)
    dir = -1;
  
  var change = dir * Min(1 * this->~AimStep() / 5, Abs(end - cur));

  crosshair->SetAngle(cur+change);
  this->~UpdateAiming();
  EffectCall(this,LocalN("wpneffect",this),"Timer");

  if(cur+change == end)
    return -1;
}

public func UpdateAiming()
{
  if(!IsAiming())
    return;

  var a = crosshair->GetAngle();

  //Position des Attachvertex so verschieben, dass das Kreuz von der Waffe aus ist
  UpdateVertices();

  if((GetDir() == DIR_Left && a > 0) ||
     (GetDir() == DIR_Right && a < 0))
    crosshair->ChangeDir();
  
  SetPhase(AimAngle2Phase(a));
}

public func StopAiming()
{
  if(crosshair)
  {
    ResetCrosshair();
    if(GetDir() == DIR_Left)
      crosshair->SetAngle(-90);
    else
      crosshair->SetAngle(+90);
  }

  SetComDir(COMD_Stop);
  SetAction("Walk");
  this->~CheckArmed();
  Sound("Grab");
 
  //Vertices zur�cksetzen
  ResetVertices();
  
  // Callback
  if(Contents(0)) Contents(0)->~AimStop();
  return 1;
}


public func StopAiming()
{
  if(crosshair)
  {
    ResetCrosshair();
    if(GetDir() == DIR_Left)
      crosshair->SetAngle(-90);
    else
      crosshair->SetAngle(+90);
  }
  return(_inherited());
}

public func StartAiming()
{
  if(Contained()) return Contained()->~StartAiming();

  SetXDir(0);
  
  SetAction("Aim");
  
  // Aim low if possble
  if(Contents(0)->~GetFMData(FM_Aim) == 2 || Contents(0)->~GetFMData(FM_Aim) == -1)
  {
    if(GetActMapVal("Name","AimLow",GetID()))
      SetAction("AimLow");
  }
  else
  if(Contents(0)->~GetFMData(FM_Aim) == 3)
  {
    if(GetActMapVal("Name","AimSquat",GetID()))
      SetAction("AimSquat");
  }

  SetPhase(AimAngle2Phase(90));
  SetComDir(COMD_Stop);

  InitCrosshair();
  
  ScheduleCall(this,"UpdateAiming",1);

  // Callback
  if(Contents(0)) Contents(0)->~AimStart();
}

/* Interne Funktionen */

public func InitCrosshair()
{
	if(!crosshair)
	{
		crosshair = CreateObject(HCRH,0,0,GetOwner());
		crosshair->Init(this);
		crosshair->SetAngle(90);
	}
  
  ResetCrosshair();
}

private func ResetCrosshair()
{
  if(!crosshair) return;
  crosshair->Reset(this);

  //crosshair->SetSpread(spread);
  
  /*if(GetDir() == DIR_Left)
    crosshair->SetAngle(-90);
  else
    crosshair->SetAngle(+90);*/
  
  ScheduleCall(this,"UpdateAiming",1);
}

private func ShowCrosshair()
{
  if(!crosshair)
  {
    InitCrosshair();
    if(!crosshair) return;
  }
	
  if(crosshair->Contained())
    crosshair->Exit();
    
  if(crosshair->GetActionTarget() != this)
    crosshair->Reset(this);
    
  if(!this->~IsAiming())
  {   
    if(GetDir() == DIR_Left)
      crosshair->SetAngle(-90);
    else
      crosshair->SetAngle(+90);
  }
}

private func HideCrosshair()
{
  if(!crosshair) return;

  crosshair->UnSet();
}

private func DeleteCrosshair()
{
  if(crosshair) crosshair->RemoveObject();
}

protected func Entrance(object pContainer)
{
  HideCrosshair();
  return _inherited(pContainer,...);
}

protected func Departure(pContainer)
{
  ShowCrosshair();
  return _inherited(pContainer,...);
}

protected func Destruction()
{
  DeleteCrosshair();
  return _inherited(...);
}

private func UpdateVertices()
{
  var x,y,r;
  this->~WeaponAt(x,y,r);
  SetVertex(0,0,x/1000 * (GetDir()*2-1),0,2);
  SetVertex(0,1,y/1000,0,2);
}

private func ResetVertices()
{
  SetVertex(0,0,0);
  SetVertex(0,1,0);
}

private func AimAngle2Phase(int angle)
{
  angle = Normalize(angle,-180);
  var phases = GetActMapVal("Length",GetAction());
  var newphase = Min( phases-1, phases*Abs(angle)/AimMax() );
  return newphase;
}

private func FireAimWeapon()
{
	this->~Control2Contents("ControlThrow");
}

private func ChangeWeapon(object pTarget)
{
	// Zielaktion abbrechen (Spezial: au�er wenn mit anw�hlbarem 
	// Objekt auch gezielt werden kann...)
	// Zielaktion anpassen
	if(pTarget->~IsWeapon())
  {
		var phase = GetPhase();
		if(pTarget->~GetFMData(FM_Aim) > 0)
			SetAction("AimLow");
		else
			SetAction("Aim");
			SetPhase(phase);
	}
	else
  {
		StopAiming();
	}
}

/* Overloads */

public func ReadyToFire()
{
 
	if(WildcardMatch(GetAction(), "Aim*")) return true;
 
	return _inherited();
}

public func AimAngle(int iMaxAngle, int iRange, bool bSpread)
{
   var angle;
   var x,y,r;
   this->~WeaponAt(x,y,r);
   
   if(!this()->~IsAiming())
     angle = (90+r)*(GetDir()*2-1);
   else
     angle = crosshair->GetAngle();
  
   if(iRange)
   {
     var target = this()->~GetTarget(angle,iMaxAngle,iRange);
     if(target)
       angle = Angle(GetX(),GetY(),GetX(target),GetY(target));
   }
     
   if(bSpread)
     angle += GetSpreadAOff();

   return angle;
}

public func AimAngle2(int iMaxAngle, int iRange, bool bSpread)//Pr�zision 100. Also von 100 bis 36000.
{
   var angle;
   var x,y,r;
   this()->~WeaponAt(x,y,r);
   
   if(!this()->~IsAiming())
     angle = (90+r)*(GetDir()*2-1);
   else
     angle = crosshair->GetAngle();
  
   if(iRange)
   {
     var target = this()->~GetTarget(angle,iMaxAngle,iRange);
     if(target)
       angle = Angle(GetX(),GetY(),GetX(target),GetY(target));
   }
   
   if(bSpread)
     angle += GetSpreadAOff2();

   return(angle);
}

global func AimAngleEx(int iMaxAngle, int iRange, bool bSpread)
{
  if(!this()) return(0);

  var angle = this()->~AimAngle2(iMaxAngle,iRange,bSpread);
  if(!angle)
    angle = this()->AimAngle(iMaxAngle,iRange,bSpread)*100;
    
  return(angle);
}

global func GetSpreadAOff()
{
//...
}

global func GetSpreadAOff2()
{
//...
}