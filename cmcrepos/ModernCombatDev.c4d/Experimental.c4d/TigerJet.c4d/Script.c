/* Jet */

#strict 2
#include L_SE

public func Initialize()
{
  SetAction("Stand");
  SetComDir(COMD_None);
  SetEntrance(1);
  _inherited();
}

public func SeatVMenuItems(int iSeat)
{
  AddMenuItem(" ","Dummy",VI4K,aSeat[iSeat]);
  AddMenuItem(" ","DoRoll",FLNT,aSeat[iSeat]);
  AddMenuItem(" ","Dummy",VI4K,aSeat[iSeat]);
  AddMenuItem(" ","Dummy",VI4K,aSeat[iSeat]);
  AddMenuItem(" ","SwitchEngi!=",ROCK,aSeat[iSeat]);
  AddMenuItem(" ","Dummy",VI4K,aSeat[iSeat]);
  AddMenuItem(" ","Dummy",VI4K,aSeat[iSeat]);
  AddMenuItem(" ","Dummy",VI4K,aSeat[iSeat]);
  AddMenuItem(" ","Dummy",VI4K,aSeat[iSeat]);
  return 1;
}

public func SeatToolCount(int iSeat)
{
  return 1;
}

private func DoRoll()
{
  if(InFlight())
    SetAction("RollL");
}

private func SwitchEngine()
{
  var state = EngineState();
  if(state == ENGINE_ONLINE)
    StopEngine();
  else
  if(state == ENGINE_OFFLINE)
    StartEngine();
}

/* Aktionen */
private func OnWheelsOut()
{
  SetVertex(3,1,19,this,2);
  SetVertex(4,1,19,this,2);
}

private func OnWheelsIn()
{
  SetVertex(3,1,8,this,0);
  SetVertex(4,1,8,this,0);
}

private func OnRollEnd()
{
  if(GetDir() == DIR_Right)
    SetDir(DIR_Left);
  else
    SetDir(DIR_Right);
    
  SetR(GetR()+180);
}

private func OnTurnEnd()
{
  if(GetDir() == DIR_Right)
    SetDir(DIR_Left);
  else
    SetDir(DIR_Right);
}

private func OnFly()
{
  if(CheckOverGround())
    SetAction("WheelsOut");
}

private func CheckOverGround()
{
  return !PathFreeAngle4K(0,0,GetR()+(-(GetDir()*2-1)*135),100,6);
}

private func OnFlyWOut()
{
  if(!CheckOverGround())
    SetAction("WheelsIn");

  if(GetContact(this,-1,CNAT_Bottom) || (EngineState() == ENGINE_OFFLINE))
    SetAction("Stand");
}

public func Hit()
{
  if(GetAction() == "FlyWOut")
  {
    OnFlyWOut();
  }
}

private func Ejection(object pObj)
{
  if(!GetOCF(pObj)&OCF_Living)
    pObj->Sound("TIGJ_Drop.ogg");
}


/* Kontrolle */

public func UseTool(object pClonk, int iSeat, int iTool)
{
  if(GetEffect("Reload",this)) return;
  ScheduleCall(this,"DoBomb",7,5);
  AddEffect("Reload",this,1,35*5,this,GetID());
}

public func DoBomb()
{
  var bomb = CreateObject(ABMB,0,0,GetOwner());
  bomb->SetExplo(40);
  bomb->SetR(Angle(0,0,GetXDir(),GetYDir()));
  bomb->Launch(GetXDir(),GetYDir());
  bomb->Sound("TIGJ_Drop.ogg");
}

public func FxReloadTimer(){return -1;}

func Up()
{
  if(EngineState() != ENGINE_ONLINE) return 1;
  
  if(GetAction() == "Stand")
  {
    if(Distance(GetXDir(),GetYDir()) >= MinFlightSpeed())
    {
      SetPosition(GetX(),GetY()-1);
      var d = -(GetDir()*2-1);
      SetRAt(GetR() + 2*d,32*d,19);
      SetRDir(d);
      SetAction("FlyWOut");
    }
  }
  else
  if(InFlight())
  {
    SetRDir(BoundBy(GetRDir()-(GetDir()*2-1),-3,+3));
  }
}

func Down(object pClonk)
{
  if(EngineState() != ENGINE_ONLINE) return 1;
  
  if(GetAction() == "Stand")
  {
    if(!GetPlrCoreJumpAndRunControl(GetController(pClonk)))
      StopDrive();
  }
  else
  if(InFlight())
  {
    SetRDir(BoundBy(GetRDir()+(GetDir()*2-1),-3,+3));
  }
}

func Left()
{
  if(EngineState() != ENGINE_ONLINE) return 1;
  
  if(GetAction() == "Stand")
  {
    if(GetDir() == DIR_Right)
    {
      StopDrive();
      SetAction("TurnL");
    }
    else
    {
      StartDrive();
      DriveGear(-(GetDir()*2-1));
    }
  }
  else
  if(InFlight())
  {
    StartDrive();
    DriveGear(-(GetDir()*2-1));
  }

  return 1;
}

func Right()
{
  if(EngineState() != ENGINE_ONLINE) return 1;
  
  if(GetAction() == "Stand")
  {
    if(GetDir() == DIR_Left)
    {
      StopDrive();
      SetAction("TurnL");
    }
    else
    {
      StartDrive();
      DriveGear(GetDir()*2-1);
    }
  }
  else
  if(InFlight())
  {
    StartDrive();
    DriveGear(GetDir()*2-1);
  }

  return 1;
}

public func Update(object pClonk, int iSeat, int iComDir)
{
  if(GetAction() == "Stand")
  {
    if(iComDir == COMD_None)
      StopDrive();
  }
}


/* Fahren und Fliegen */

public func InFlight()
{
  return GetProcedure() == "FLOAT";
}

public func MaxSpeed(){return 80;}
public func GearCount(){return 4;}
public func MinFlightSpeed(){return 40;}

private func CurMaxGear()
{
  if(InFlight())
    return GearCount();
  else
    return MinFlightSpeed()*GearCount()/MaxSpeed();
}

private func CurMinGear()
{
  if(InFlight())
    return MinFlightSpeed()*GearCount()/MaxSpeed();
  else
    return 0;
}

private func StartDrive()
{
  var effect = GetEffect("IntDrive",this);
  if(!effect)
    effect = AddEffect("IntDrive", this, 10, 2, this);
  
  return effect;
}

private func DriveGear(int iChange)
{
  var effect = GetEffect("IntDrive",this);
  if(!effect)
    return 0;
    
  var pClonk = aSeat[0];
  if(pClonk)
    if(GetPlrCoreJumpAndRunControl(GetController(pClonk)))
      iChange = 0;
      
  var val = EffectVar(0,this,effect) += iChange;
  val = BoundBy(val,CurMinGear(),CurMaxGear());
  
  if(iChange && pClonk)
  {
    Message("Gang %d",this,val);
  }
  
  return val;
}

private func StopDrive()
{
  return RemoveEffect("IntDrive",this);
}

public func FxIntDriveStart(object pTarget, int iEffectNumber, int iTemp)
{
  if(iTemp) return 0;

  var clonk = aSeat[0];
  if(!clonk) return -1;
  
  //Gang?
  if(GetPlrCoreJumpAndRunControl(GetController(clonk)))
    EffectVar(0,pTarget,iEffectNumber) = CurMaxGear();
  else
    EffectVar(0,pTarget,iEffectNumber) = CurMinGear();
    
  EffectVar(1,pTarget,iEffectNumber) = 0;
}

public func FxIntDriveTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
  //Variablen laden
  var cur_s = EffectVar(1,pTarget,iEffectNumber);
  var trg_s = EffectVar(0,pTarget,iEffectNumber)*MaxSpeed()/GearCount();
  var r = GetR() + 90*(GetDir()*2-1);

  //Geschwindigkeit setzen
  if(InFlight() || (GetContact(this,3) || GetContact(this,4)))
  {
    SetXDir(+Sin(r,cur_s));
    SetYDir(-Cos(r,cur_s));
  }
  else
  {
    EffectVar(1,pTarget,iEffectNumber) = 0;
    return 0;
  }
  
  //Beschleunigung
  if(cur_s < trg_s)
    EffectVar(1,pTarget,iEffectNumber) += 3;
  else
  if(cur_s > trg_s)
    EffectVar(1,pTarget,iEffectNumber) -= 3;

  return 0;
}

public func FxIntDriveStop(object pTarget, int iEffectNumber, int iReason, bool fTemp)
{
  //...
}



/* Motor */

//Callbacks
public func OnEngineStart(){}
public func OnEngineStop()
{
  StopDrive();
  if(InFlight())
    SetAction("Stand");//Doof, aber muss sein. :I
}
public func OnEnginePace(){}


//Konstanten
static const ENGINE_OFFLINE = 0;
static const ENGINE_STARTING = 1;
static const ENGINE_ONLINE = 2;
static const ENGINE_STOPPING = 3;

private func EnginePace() {return 35;}

private func EngineStartSoundLength() {return 114;}
private func EngineStartSoundFile() {return "TIGJ_EngineStart.ogg";}

private func EngineIdleSoundFile() {return "TIGJ_EngineIdle.ogg";}

private func EngineStopSoundLength() {return 101;}
private func EngineStopSoundFile() {return "TIGJ_EngineStop.ogg";}


//Kontrollfunktionen
public func StartEngine()
{
  var effect = GetEffect("IntEngi!=",this);
  if(!effect)
    effect = AddEffect("IntEngi!=", this, 10, EnginePace(), this);
  
  return effect;
}

public func StopEngine()
{
  var effect = GetEffect("IntEngi!=",this);
  if(!effect)
    return 0;
  
  return EffectCall(this,effect,"ShutDown");
}

public func EngineState()
{
  if(GetEffect("IntStartEngi!=",this))
    return ENGINE_STARTING;
  if(GetEffect("IntStopEngi!=",this))
    return ENGINE_STOPPING;
  if(GetEffect("IntEngi!=",this))
    return ENGINE_ONLINE;
  return ENGINE_OFFLINE;
}


//Haupt-Effekt
public func FxIntEngineStart(object pTarget, int iEffectNumber, int iTemp)
{
  if(iTemp) return;
  EffectVar(0,pTarget,iEffectNumber) = AddEffect("IntStartEngi!=", this, 10, EngineStartSoundLength(), this, 0, iEffectNumber);
  pTarget->OnEngineStart();
}

public func FxIntEngineStarted(object pTarget, int iEffectNumber)
{
  EffectVar(0,pTarget,iEffectNumber) = 0;
  Sound(EngineIdleSoundFile(),0,pTarget,0,0,+1);
}

public func FxIntEngineTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
  pTarget->OnEnginePace();
}

public func FxIntEngineShutDown(object pTarget, int iEffectNumber)
{
  Sound(EngineIdleSoundFile(),0,pTarget,0,0,-1);
  EffectVar(0,pTarget,iEffectNumber) = AddEffect("IntStopEngi!=", this, 10, EngineStopSoundLength(), this, 0, iEffectNumber);
  return 1;
}

public func FxIntEngineStopped(object pTarget, int iEffectNumber)
{
  EffectVar(0,pTarget,iEffectNumber) = 0;
  RemoveEffect(0,pTarget,iEffectNumber);
}

public func FxIntEngineStop(object pTarget, int iEffectNumber, int iReason, bool fTemp)
{
  if(fTemp) return;
  Sound(EngineIdleSoundFile(),0,pTarget,0,0,-1);//Zur Sicherheit.
  pTarget->OnEngineStop();
}


//Start-Effekt
public func FxIntStartEngineStart(object pTarget, int iEffectNumber, int iTemp, iMainEffect)
{
  EffectVar(0,pTarget,iEffectNumber) = iMainEffect;
  Sound(EngineStartSoundFile(),0,pTarget);
}

public func FxIntStartEngineTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
  EffectCall(pTarget,EffectVar(0,pTarget,iEffectNumber),"Started");
  return -1;
}

public func FxIntStartEngineStop(object pTarget, int iEffectNumber, int iReason, bool fTemp)
{
  //...
}


//Stop-Effekt
public func FxIntStopEngineStart(object pTarget, int iEffectNumber, int iTemp, iMainEffect)
{
  EffectVar(0,pTarget,iEffectNumber) = iMainEffect;
  Sound(EngineStopSoundFile(),0,pTarget);
}

public func FxIntStopEngineTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
  EffectCall(pTarget,EffectVar(0,pTarget,iEffectNumber),"Stopped");
  return -1;
}

public func FxIntStopEngineStop(object pTarget, int iEffectNumber, int iReason, bool fTemp)
{
  //...
}


/* Helper */

/*global func RotateAtPoint(int iR, int iXOff, int iYOff, int iPrec, object pTarget)
{
  //UNOPTIMIERT! OMGOMG!
  if(!pTarget) pTarget = this;
  if(!pTarget) return false;
  
  CreateParticle("PSpark",0,0,0,0,25,RGB(255));
  CreateParticle("PSpark",iXOff,iYOff,0,0,25,RGB(0,255));
  
  //Virtuell um Punkt drehen.
  var d = Distance(iXOff,iYOff);
  var a = Angle(iXOff,iYOff,0,0);
  var x = iXOff+Sin(a+iR,d,iPrec);
  var y = iYOff-Cos(a+iR,d,iPrec);
  
  CreateParticle("PSpark",x,y,0,0,25,RGB(0,0,255));
  
  //Position anpassen.
  pTarget->SetPosition(GetX()+x,GetY()+y);
  
  //Winkel anpassen.
  pTarget->SetR(90 + pTarget->GetR() - Angle(iXOff,iYOff,x,y));
  
  Log("D%d X%d Y%d",d,x,y);
  PauseGame();
  
  return true;
}*/

//Von Hazard. :>
public func SetRAt(int angle, int xoffs, int offs)
{
	var r = GetR();

	var ox = Sin(r,offs)-Cos(r,xoffs);
	var oy = Cos(r,offs)+Sin(r,xoffs);
	SetR(angle);
	var nx = Sin(GetR(),offs)-Cos(GetR(),xoffs);
	var ny = Cos(GetR(),offs)+Sin(GetR(),xoffs);
	
	var xpos = nx-ox;
	var ypos = -ny+oy;
	
	SetPosition(GetX()+xpos,GetY()+ypos);
}
