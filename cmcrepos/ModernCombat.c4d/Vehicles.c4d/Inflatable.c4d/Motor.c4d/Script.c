/*-- Motor --*/

#strict

local boat;

protected func Initialize()
{
  UpdateTransferZone();
  SetComDir(COMD_None());
  return(1);
}

public func SetBoat(object pBoat)
{
  if(pBoat)
    if(GetID(pBoat) == INFL)
      boat = pBoat;
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
  if(!boat) return(false);
  
  var iXOff = 30;
  if(boat->GetComDir() == 7)//Lol. Sinn?
    iXOff = -iXOff;
    
  SetShape((-10)+iXOff,-15,30,30);
  return(true);
}

protected func ControlUpdate(object clonk, int comdir)
{
  if(comdir == COMD_UpLeft || comdir == COMD_Left || comdir == COMD_DownLeft)
    boat->Left();
  else if(comdir == COMD_UpRight || comdir == COMD_Right || comdir == COMD_DownRight)
    boat->Right();
  else
    boat->Stop();
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
  if(iX>GetX()) ControlRight();
  if(iX<GetX()) ControlLeft();
  return(1);
}

public func ControlLeft(object pByObj)
{
  if(!GetPlrCoreJumpAndRunControl(pByObj->GetController()))
    boat->Left();
}

public func ControlRight(object pByObj)
{
  if(!GetPlrCoreJumpAndRunControl(pByObj->GetController()))
    boat->Right();
}

public func ControlDown(object pByObj)
{
  if(!GetPlrCoreJumpAndRunControl(pByObj->GetController()))
    boat->Stop();
}