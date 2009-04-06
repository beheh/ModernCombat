/*-- Schlauchboot (Motor) --*/

#strict

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
  if(!GetActionTarget()) return(false);
  
  var iXOff = 30;
  if(GetActionTarget()->GetDir() == DIR_Right)
    iXOff = -iXOff;
    
  SetShape((-10)+iXOff,-15,30,30);
  return(true);
}


//Steuerung

protected func ControlUpdate(object clonk, int comdir)
{
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
  if(!GetPlrCoreJumpAndRunControl(pByObj->GetController()))
    GetActionTarget()->Left();
    
  return(1);
}

public func ControlRight(object pByObj)
{
  if(!GetPlrCoreJumpAndRunControl(pByObj->GetController()))
    GetActionTarget()->Right();
    
  return(1);
}

public func ControlDown(object pByObj)
{
  if(!GetPlrCoreJumpAndRunControl(pByObj->GetController()))
    GetActionTarget()->Stop();
    
  return(1);
}

public func ControlUpDouble(object pByObj)
{
  if(GetContact(GetActionTarget(),-1))
    GetActionTarget()->LandOn();
    
  return(1);
}