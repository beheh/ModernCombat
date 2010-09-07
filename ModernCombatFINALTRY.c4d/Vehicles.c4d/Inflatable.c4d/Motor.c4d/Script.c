/*-- Motor --*/

#strict 2

public func MaxDamage()		 { return 60; }
public func IsBulletTarget()     {return true;}
public func IgnoreFriendlyFire() {return true;}

local motoridle;


/* Initalisierung */

public func Initialize()
{
  AddDamageEffect(this,35);
}

/* Schaden */

private func UpdateDmg()
{
  if(!GetActionTarget()) return;
  DoDamage(GetDamage() - GetDamage(GetActionTarget()), GetActionTarget());
}

public func Damage()
{
  UpdateDmg();
}

public func OnDmg(int iDmg, int iType)
{
  if(iType == DMG_Fire)		return 60;	//Feuer
  if(iType == DMG_Bio)		return 100;	//Säure und biologische Schadstoffe
}

/* Funktionen */

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

func SetUser(object pUser)
{
  var plr = GetOwner(pUser);
  //Für die FF-Erkennung
  SetOwner(plr);
  if(GetActionTarget())
    SetOwner(plr, GetActionTarget());
}

/* Darstellung */

public func UpdateShape()
{
  var boat = GetActionTarget();
  if(!boat) return false;  
  
  var iXOff;
  
  if(GetAction(boat) == "Turn")
  {
    var phases = GetActMapVal("Length", "Turn", GetID(boat))-1;
    var phase = GetPhase(boat)-1;
  
    var alpha = (phase * 180 / phases) - 90;
    iXOff = Sin(alpha, -25);
    
  }
  else
    iXOff = -25;

  if(boat->GetDir() == DIR_Right)
    iXOff = -iXOff;

  SetVertex(0,0,iXOff,this,2);
  SetShape(-15,-15,30,30);
  
  //SetShape((-10)+iXOff,-15,30,30);
  return true;
}

/* Soundeffekte */

protected func Grabbed(object pByObject, bool fGrab)
{
  var boat = GetActionTarget();
  if(fGrab)
  {
   Sound("MotorStart.ogg");
   boat -> Sound("MotorIdleLoop.ogg",false,motoridle,100,0,+1);
   SetOwner(GetOwner(pByObject), boat);
  }
  else
  {
   Sound("MotorEnd.ogg");
   boat -> Sound("MotorIdleLoop.ogg",false,motoridle,100,0,-1);
  }
}

/* Steuerung */

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
  if(szCommand == "MoveTo")
   return Command2Control(iX,iY);
}

private func Command2Control(int iX, int iY)
{
  // nur X wird ausgewertet
  if(iX > GetActionTarget()->GetX()+30) GetActionTarget()->Right();
  if(iX < GetActionTarget()->GetX()-30) GetActionTarget()->Left();
  if(iX < GetActionTarget()->GetX()+30 && iX > GetActionTarget()->GetX()-30) GetActionTarget()->Stop();
  return 1;
}

public func ControlLeft(object pByObj)
{
  SetUser(pByObj);

  if(!GetPlrCoreJumpAndRunControl(pByObj->GetController()))
   GetActionTarget()->Left();
  return 1;
}

public func ControlRight(object pByObj)
{
  SetUser(pByObj);
  if(!GetPlrCoreJumpAndRunControl(pByObj->GetController()))
   GetActionTarget()->Right();
  return 1;
}

public func ControlDown(object pByObj)
{
  SetUser(pByObj);
  if(!GetPlrCoreJumpAndRunControl(pByObj->GetController()))
   GetActionTarget()->Stop();
  return 1;
}

/*public func ControlUpDouble(object pByObj)
{
  SetUser(pByObj);
  if(GetContact(GetActionTarget(),-1))
   GetActionTarget()->LandOn();
  return 1;
}*/
