/*-- Waffenständer --*/

#strict 2
#include L_WR


/* Allgemein */

private func IsMachine() { return true; }
private func IsBulletTarget() { return true; }

public func Initialize()
{
  SetController(GetOwner());
  SetAction("Be");
  SetGraphics(0,this,GetID(),3,5,0,0,this);
  return _inherited();
}

public func Damage()
{
  if(GetDamage() > 20)//TODO: Anpassen.
    Explode(20);
}

public func WeaponAt(&x, &y, &r)
{
  if(!crosshair) return 0;

  r = Abs(crosshair->GetAngle())-90;
  x = 0;
  y = -5000;
  return 1;
}

/* Steuerung */

public func FxSelectWeaponStart(object pClonk, int iEffectNumber, int iTemp, wpn)
{
  if(!wpn) return -1;
  EffectVar(0,pClonk,iEffectNumber) = wpn;
}

public func FxSelectWeaponTimer(object pClonk, int iEffectNumber)
{
  pClonk->ShiftContents(0,0,GetID(EffectVar(0,pClonk,iEffectNumber)));
  EffectCall(pClonk,LocalN("wpneffect",pClonk),"Timer");
  pClonk->~CheckArmed();
  return -1;
}

public func GrabLost()
{
	Grabbed(0,true);
}

public func Grabbed(object byObj, bool grabbed)
{
  var clonk = GetUser();

  if(!grabbed && (user == byObj))
  {
    var wp = GetWeapon();
    Disarm();
    if(wp && byObj)
    {
      byObj->Collect(wp);
      AddEffect("SelectWeapon",byObj,0,1,0,GetID(),wp);
    }
  	SetUser(0);
    
    return 0;
  }

	if(user && (user != byObj))
	{
		return AddCommand(byObj,"UnGrab");
	}
	else
	{
		SetUser(byObj);
	}

  if(byObj && !GetWeapon())
  {
    var wpn = byObj->Contents();//potentielle Waffe
    if(wpn)
    {
      if(wpn->~IsWeapon())
      {
        while(Contents())
          Exit(Contents(),0,0);
        Arm(wpn);
        StartAiming();
      }
    }
     
    if(!GetWeapon())
    {
      return AddCommand(byObj,"UnGrab");//Scher dich zum Teufel du Lausbub!
    }
  }
}


/* Zielen */

private func AimMax() { return(90); }

public func CorrectAimAngle(int iAngle)
{
  //iAngle = BoundBy(iAngle, 0,AimMax());
  
  if(Direction() == DIR_Left)
    iAngle = 360-iAngle;
  
  return iAngle;
}

public func OnTurnLeft()
{
  SetDir(DIR_Left);
  UpdateAiming();
  
  var clonk = GetUser();
  if(!clonk) return;
  
  if((clonk->GetDir() != DIR_Left) || (Direction() != DIR_Left))
  {
    SetDir(DIR_Left);
    clonk->SetDir(DIR_Left);
    var xoff = clonk->GetX() - GetX();
    clonk->SetPosition(GetX() - xoff, clonk->GetY());
  }
}

public func OnTurnRight()
{
  SetDir(DIR_Right);
  UpdateAiming();
  
  var clonk = GetUser();
  if(!clonk) return;
  
  if((clonk->GetDir() != DIR_Right) || (Direction() != DIR_Right))
  {
    SetDir(DIR_Right);
    clonk->SetDir(DIR_Right);
    var xoff = clonk->GetX() - GetX();
    clonk->SetPosition(GetX() - xoff, clonk->GetY());
  }
}

public func Direction()
{
  return GetDir();
}


/* Steuerung */

public func Control2Grab(string command, object clonk)
{
  Log("Stand: %s",command);

	if(!IsAiming()) return false;
	
	else if(command == "ControlUpdate")
		AimUpdate(this(), Par(2), 1, "ControlConf");

	else if(command == "ControlUp" || command == "ControlUpDouble")
		AimUp(this(), 1, "ControlConf");

	else if(command == "ControlDown" || command == "ControlDownDouble")
		AimDown(this(), 1, "ControlConf");
    
	else if(command == "ControlThrow")
		Fire();
    
	else if(command == "ControlDigDouble")
		WeaponMenu();
		
	else if(command == "ControlLeft")
	  TurnLeft();
	
	else if(command == "ControlRight")
		TurnRight();
	
	else if(command == "ControlLeftDouble" || command == "ControlRightDouble")
		AddCommand(GetUser(),"UnGrab");
	
	else if(command == "ControlCommand")
		DoMouseAiming(Par(3),Par(4));
  
	/*else if(command == "ControlContents")
  {
		var pTarget = clonk->FindContents(Par(2));
    var old = GetWeapon();
		Disarm();
    old->Enter(clonk);
    Arm(pTarget);
		return false;
	}*/

	return true;
}