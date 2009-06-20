/*-- Motor --*/

#strict

public func Initialize()
{
  AddDamageEffect(this,35);
}

public func MaxDamage()		{ return(50); }

public func IsBulletTarget(){return(true);}
public func IgnoreFriendlyFire() {return(true);}

private func UpdateDmg()
{
  if(!GetActionTarget()) return;
  GetActionTarget()->DoDamage(GetDamage() - GetActionTarget()->GetDamage());
}

public func Damage()
{
	UpdateDmg();
}

public func SetBoat(object pBoat)
{
  SetAction("Be",pBoat);
  UpdateShape();
}

protected func AttachTargetLost()
{
  RemoveObject();
}

protected func UpdateTransferZone()
{
  UpdateShape();
}

public func UpdateShape()
{
  var boat = GetActionTarget();
  if(!boat) return(false);
  
  
  var iXOff;
  
  if(boat->GetAction() eq "Turn")
  {
    var phases = boat->GetActMapVal("Length", "Turn")-1;
    var phase = boat->GetPhase()-1;
  
    var alpha = (phase * 180 / phases) - 90;
    iXOff = Sin(alpha, -25);
    
    //Log("phase %d (alpha = %d / xoff = %d)",phase,alpha,iXOff);
  }
  else
    iXOff = -25;

  if(boat->GetDir() == DIR_Right)
    iXOff = -iXOff;

  SetVertex(0,0,iXOff,this,2);
  SetShape(-15,-15,30,30);
  
  //SetShape((-10)+iXOff,-15,30,30);
  return(true);
}

func SetUser(object pUser)
{
  var plr = GetOwner(pUser);
  SetOwner(plr);
  if(GetActionTarget())
    GetActionTarget()->SetOwner(plr);
}

func Grabbed(object byObj)
{
  SetUser(byObj);
}


//Steuerung

protected func ControlUpdate(object clonk, int comdir)
{
  SetUser(clonk);

  if(comdir == COMD_UpLeft || comdir == COMD_Left || comdir == COMD_DownLeft)
    GetActionTarget()->Left();
  else if(comdir == COMD_UpRight || comdir == COMD_Right || comdir == COMD_DownRight)
    GetActionTarget()->Right();
  else
    GetActionTarget()->Stop();
}

protected func ControlCommand(string szCommand, object pTarget, int iX, int iY)
{
  // Bewegungskommando (nur links/rechts auswerten)
  if(szCommand eq "MoveTo")
    return(Command2Control(iX,iY));
}

private func Command2Control(int iX, int iY)
{
  // nur X wird ausgewertet
  if(iX > GetActionTarget()->GetX()) GetActionTarget()->Right();
  if(iX < GetActionTarget()->GetX()) GetActionTarget()->Left();

  return(1);
}

public func ControlLeft(object pByObj)
{
  SetUser(pByObj);

  if(!GetPlrCoreJumpAndRunControl(pByObj->GetController()))
    GetActionTarget()->Left();
    
  return(1);
}

public func ControlRight(object pByObj)
{
  SetUser(pByObj);

  if(!GetPlrCoreJumpAndRunControl(pByObj->GetController()))
    GetActionTarget()->Right();
    
  return(1);
}

public func ControlDown(object pByObj)
{
  SetUser(pByObj);

  if(!GetPlrCoreJumpAndRunControl(pByObj->GetController()))
    GetActionTarget()->Stop();
    
  return(1);
}

public func ControlUpDouble(object pByObj)
{
  SetUser(pByObj);

  if(GetContact(GetActionTarget(),-1))
    GetActionTarget()->LandOn();
    
  return(1);
}