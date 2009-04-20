/*--- Manhack ---*/

#strict

/* Allgemein */
public func IsPossessible() { return(1); }
public func Faction() {return(FACTION_Combine);}
public func IsThreat() { return(true); }
public func IsMachine() { return(true); }

protected func Initialize()
{
  aCmdData = [7];
  SetCmd();
  SetAction("Fly");
  BladingStart();
  SetDir(Random(2));
  SetColorDw(RGB(255));
  
  SetName(GetName(0,GetID()));
}

protected func Death(int iKilledBy)
{
  Sound("MHCK_Explode.ogg");
  SetAction("Idle");
  SetColorDw(RGBa(0,0,0,255));
  AddEffect("Frazling",this(),20,16,this());
  FadeOut4K(2);
  
  GameCallEx("MonsterKilled",GetID(),this(),iKilledBy);
  
  var gotcrew, plr = GetOwner();
  for(var i; i < GetCrewCount(plr); i++)
    if(GetOCF(GetCrew(plr,i)) & OCF_Alive)
      gotcrew = true;

  if(!gotcrew)
    GameCallEx("RelaunchPlayer",GetOwner(),this(), GetKiller());
  
  return(1);
}

public func FxFrazlingTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
  CreateParticle("Frazzle",0,0,RandomX(-10,+10),RandomX(-10,+10),RandomX(25,40), RGBa(0,200,255,150));
}

public func OnDmg(int iDamage, int iType)
{
  if(iType == DMG_Bio)
    return(100);

  if(iType == DMG_Melee)
    return(-25);
}

public func OnHit(int iDmg, int iType, object pFrom)
{
  var xdir,ydir,rnddir = iDmg*2;
  
  if(pFrom)
  {
    if((iType == DMG_Explosion)||
       (pFrom->~IsBullet()))
    {
      var angle = GetR(pFrom);
      if(!angle) angle = Angle(GetX(),GetY(),GetX(pFrom),GetY(pFrom));

      xdir += Sin(angle,iDmg*2);
      ydir -= Cos(angle,iDmg*2);
      rnddir = iDmg;
    }
  }
  
  for(var i = iDmg*2/5; i > 0; i--)
  {
    CreateParticle("Frazzle", 0, 0, xdir+RandomX(-rnddir,+rnddir), ydir+RandomX(-rnddir,+rnddir), RandomX(40,50), RGBa(0,200,255,100));
  }
  
  if(pFrom)
  {
    if(pFrom->~IsBullet())
    {
      SetXDir(GetXDir()/2+Sin(GetR(pFrom),iDmg));
      SetYDir(GetYDir()/2-Cos(GetR(pFrom),iDmg));
    }
  }

  if(!GetEffect("Damaged",this()))
  {
    SetColorDw(RGB(255,128));
    SetComDir(COMD_None);
    SetCommand(this(),"None");
    AddEffect("Damaged",this(),20,20,this());
  }
  if(!GetEffect("Smoking",this()))
    AddEffect("Smoking",this(),20,4,this());
}

public func FxSmokingTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
  var grey = 128*GetEnergy()/(GetPhysical("Energy")/1000); 
  CreateParticle("Smoke3",0,0,0,0,40+Random(20),RGBa(128,128,128,100),0,0);
}

public func FxDamagedStart(object pTarget, int iEffectNumber)
{
  EffectVar(0,pTarget,iEffectNumber) = GetX(); 
  EffectVar(1,pTarget,iEffectNumber) = GetY(); 
}

public func FxDamagedTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
  if(iEffectTime > 35*2)
  {
    SetColorDw(RGB(255));
    SetCommand(pTarget,"MoveTo",0,EffectVar(0,pTarget,iEffectNumber),EffectVar(1,pTarget,iEffectNumber)); 
    return(-1);
  }
}

/* Haupt-KI */
local pEnemy,aCmdData,bCmd,bCmdUntilEnemy;

//Kommando das im "Idle"-Modus ausgeführt wird.
public func SetCmd(bool bUntilEnemy, string szCommand, object pTarget, int iX, int iY, object pTarget2, Data, int iRetries)
{
  bCmdUntilEnemy = bUntilEnemy;
  aCmdData[0] = szCommand;
  aCmdData[1] = pTarget;
  aCmdData[2] = iX;
  aCmdData[3] = iY;
  aCmdData[4] = pTarget2;
  aCmdData[5] = Data;
  aCmdData[6] = iRetries;
}

protected func Activity() 
{
  if(UserControlled()) return();
  if(GetAction() eq "Idle") return();
  if(GetEffect("Damaged",this())) return();
    
  if(InLiquid() || GBackLiquid(0,-20))
  {
    SetCommand(this(),"None");
    SetComDir(COMD_Up);
  }
    
  if((GetCommand(this()) eq "Follow") || (GetCommand(this()) eq "MoveTo"))
  {
    var x;
    if(GetCommand(this(),1))
      x = GetX(GetCommand(this(),1));
    else
      x = GetCommand(this(),2);
    
    if(x-GetX() < 0)
      SetDir(DIR_Left);
    else
      SetDir(DIR_Right);
  }
  
  if(pEnemy)
  {
    if((Abs(GetX(pEnemy)-GetX()) > 300) || (Abs(GetY(pEnemy)-GetY()) > 100) || !GetAlive(pEnemy) || pEnemy->Contained())
    {
      pEnemy = 0;
      FinishCommand(this(),false,0);
      SetComDir(COMD_Stop);
      return();
    }
    
    if(GetCommand(this()) ne "Follow")
    {
      bCmd = false;
      if(bCmdUntilEnemy) SetCmd();
      SetCommand(this(),"Follow",pEnemy); 
    }
  }
  else
  {
    pEnemy = FindPrey();
    if(pEnemy) return(Alert(pEnemy));
  
    if(!GetCommand(this()) || !bCmd)
    {
      if(aCmdData[0])
      {
        bCmd = true;
        SetCommand(this(),aCmdData[0],aCmdData[1],aCmdData[2],aCmdData[3],aCmdData[4],aCmdData[5],aCmdData[6]);
        return();
      }
      
      if(GetAction() eq "Fly")
      {            
        if(!Random(10))
          Sound("MHCK_Blade.ogg",false,0,0,0,0,0,150);
      }
    }
  }
}

public func UserControlled()
{
  if((GetController() != NO_OWNER) && (GetPlayerType(GetController()) == C4PT_User)) return(true);
  if(GetEffect("PossessionSpell", this())) return(true);
  if(FindObject2(Find_Container(this()),Find_OCF(OCF_CrewMember|OCF_Alive))) return(true);
  return(false);
}

private func Alert(object pTarget)
{
  if(!pTarget) return();

  SetColorDw(RGB(0,255));
  AddEffect("Alerting",this(),20,5,this());
  //Message("×",this());
  for(var combine in FindObjects(Find_InRect(-200,-200,+400,+400),Find_Exclude(this()),Find_OCF(OCF_Alive),Find_Faction(Faction())))
    combine->~OnAlert(pTarget);
}

public func OnAlert(object pTarget)
{
  if(UserControlled()) return();
  
  if(!GetEffect("Alerted",this()))
  {
    SetColorDw(RGB(0,255));
    AddEffect("Alerted",this(),20,35,this());
  }
  
  if(pTarget->~Faction() == Faction())
  {
    if(!pEnemy)
    {
      bCmd = false;
      SetCmd(false,"Follow",pTarget);
    }
  }
  else
    if(!pEnemy)
      pEnemy = pTarget;
}

public func FxAlertingTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
  if(iEffectTime < 30)
  {
    if(iEffectTime % 10)
      SetColorDw(RGB(0,255));
    else
      SetColorDw(RGB(255));
  }
  else
  {
    SetColorDw(RGB(255));
    return(-1);
  }
}

public func FxAlertedTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
  SetColorDw(RGB(255));
  return(-1);
}

private func FindPrey()
{
  var targets = FindTargets(this(),100);
  
  if(GetLength(targets))
    return(targets[0]);
  return();
  //for(var prey in FindObjects(Find_InRect(/*((GetDir()-1)*2)*180*/-100,-100,+200,+150),Find_OCF(OCF_Prey|OCF_Alive),Find_Not(Find_Faction(Faction())),Find_NoContainer(),Sort_Distance()))
    //if(PathFreeObject4K(this(),prey,5))
      //return(prey);
}

func InWater()
{
  DoDmg(4,DMG_Melee,this());
}

/* Kontakte */
protected func ContactLeft()
{
  if(UserControlled()) return();
  return(TurnRight());
}
  
protected func ContactRight()
{
  if(UserControlled()) return();
  return(TurnLeft());
}

/* Aktionen */

func BladingStart()
{
  if(!GetEffect("Blading",this()))
    AddEffect("Blading",this(),20,4,this());
  Sound("MHCK_Engine.ogg",false,0,0,0,+1,0,200);
}

public func FxBladingTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
  var victim = FindObject2(Find_AtPoint(),Find_OCF(OCF_Prey|OCF_Alive),Find_NoContainer(),Find_Not(Find_Faction(Faction())),Find_Exclude(this()),Sort_Distance());
  if(victim)
  {
    Sound("MHCK_Blading*.ogg");
    DoDmg(7,DMG_Projectile,victim);
  }
}

func BladingEnd()
{
  RemoveEffect("Blading",this());
  Sound("MHCK_Engine.ogg",false,0,0,0,-1);
}

private func TurnRight()
{
  if (Stuck() || (GetAction() ne "Fly")) return();
  SetDir(DIR_Right);
  SetComDir(COMD_Right);
  return(1);
}

private func TurnLeft()
{
  if(Stuck() || (GetAction() ne "Fly")) return();
  SetDir(DIR_Left);
  SetComDir(COMD_Left);
  return(1);
}

/* Steuerung durch Besessenheit */

public func ContactLeft()
{
  CreateParticle("Frazzle", -6, 0, +25, RandomX(-6,+6), RandomX(40,50), RGBa(0,200,255,100));
  CreateParticle("Frazzle", -6, 0, +25, RandomX(-6,+6), RandomX(40,50), RGBa(0,200,255,100));
  SetXDir(+25);
  Sound("MHCK_Grind*.ogg");
}

public func ContactRight()
{
  CreateParticle("Frazzle", +6, 0, -25, RandomX(-6,+6), RandomX(40,50), RGBa(0,200,255,100));
  CreateParticle("Frazzle", +6, 0, -25, RandomX(-6,+6), RandomX(40,50), RGBa(0,200,255,100));
  SetXDir(-25);
  Sound("MHCK_Grind*.ogg");
}

protected func ControlCommand(szCommand, pTarget, iTx, iTy)
{
  if(szCommand eq "MoveTo")
  {
    return(SetCommand(this(),szCommand, pTarget, iTx, iTy));
  }
  return(0);
}

protected func ControlCommandFinished(string strCommand, object pTarget, int iTx, int iTy, object pTarget2, Data)
{
  if(!aCmdData[0]) return();
  if(strCommand eq aCmdData[0])
  {
    if(pTarget != aCmdData[1]) return();
    if(iTx != aCmdData[2]) return();
    if(iTy != aCmdData[3]) return();
    if(pTarget2 != aCmdData[4]) return();
    if(Data != aCmdData[5]) return();
    
    SetCmd();//Abgeschlossen... dann löschen!
  }
}

protected func ContainedLeft(object caller)
{
  [$TxtMovement$]
  SetCommand(this(),"None");
  if(!GetPlrCoreJumpAndRunControl(caller->GetController()))
    TurnLeft();
  return(1);
}

protected func ControlUp()
{
  [$TxtMovement$]
  if(!GetPlrCoreJumpAndRunControl(GetController()))
    SetComDir(COMD_Up);
  return(1);
}

protected func ControlDown()
{
  [$TxtMovement$]
  if(!GetPlrCoreJumpAndRunControl(GetController()))
    SetComDir(COMD_Down);
  return(1);
}

protected func ControlLeft()
{
  [$TxtMovement$]
  if(!GetPlrCoreJumpAndRunControl(GetController()))
    TurnLeft();
  return(1);
}

protected func ControlRight()
{
  [$TxtMovement$]
  if(!GetPlrCoreJumpAndRunControl(GetController()))
    TurnRight();
  return(1);
}

protected func ContainedRight(object caller)
{
  [$TxtMovement$]
  SetCommand(this(),"None");
  if(!GetPlrCoreJumpAndRunControl(caller->GetController()))
    TurnRight();
  return(1);
}

protected func ContainedUp(object caller)
{
  [$TxtMovement$]
  SetCommand(this(),"None");
  return(SetComDir(COMD_Up));
}

protected func ContainedDown(object caller)
{
  [$TxtMovement$]
  SetCommand(this(),"None");
  if(Contained()) return SetCommand(this, "Exit");
  if(!GetPlrCoreJumpAndRunControl(caller->GetController()))
    SetComDir(COMD_Down());
  return(1);
}

/* JumpAndRun-Steuerung */

protected func ControlCommand(szCommand, pTarget, iTx, iTy)
{
 // Bewegungskommando
 if (szCommand eq "MoveTo")
  return(SetCommand(this(),szCommand, pTarget, iTx, iTy));
 return(0);
}

private func ClearDir(bool fX)
{
  if(fX && GetXDir())
  {
    if(GetXDir() > 0) SetXDir(Max(GetXDir() - 2, 0));
    else SetXDir(Min(GetXDir() + 2, 0));
  }
  if(!fX && GetYDir())
  {
    if(GetYDir() > 0) SetYDir(Max(GetYDir() - 2, 0));
    else SetYDir(Min(GetYDir() + 2, 0));
  }
}

public func ControlUpdate(object controller, int comdir)
{
  SetComDir(comdir);
  ClearScheduleCall(this(), "ClearDir");
  if(comdir == COMD_Up || comdir == COMD_Down || comdir == COMD_Stop)
    ScheduleCall(this(), "ClearDir", 1, (Abs(GetXDir())+1)/2, true);
  if(comdir == COMD_Left || comdir == COMD_Right || comdir == COMD_Stop)
    ScheduleCall(this(), "ClearDir", 1, (Abs(GetYDir())+1)/2, false);

  if(comdir == COMD_UpRight || comdir == COMD_Right || comdir == COMD_DownRight)
    SetDir(DIR_Right);
  if(comdir == COMD_UpLeft || comdir == COMD_Left || comdir == COMD_DownLeft)
    SetDir(DIR_Left);

  return(1);
}
public func ContainedUpdate(object self, int comdir){return(ControlUpdate(self,comdir));}

protected func ControlThrow()
{
  SetXDir();
  SetYDir();
  SetComDir(COMD_None);
  return(1);
}
protected func ControlThrowSingle(){return(1);}
protected func ControlThrowDouble(){return(1);}

protected func ContainedDig()
{
  [$TxtFollow$]
  return(ControlDig());
}
protected func ControlDigSingle(){return(1);}
protected func ControlDigDouble(){return(1);}
protected func ControlDig()
{
  [$TxtFollow$]
  Alert(this());
  SetAction("Alert");
  return(1);
}

protected func ContainedDigDouble()
{
  [$TxtLeave$]
  RemoveEffect("PossessionSpell", this());
  return(1);
}