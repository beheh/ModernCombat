/*--- Headcrab---*/

#strict

/* Allgemein */
public func IsPossessible() { return(1); }
public func Faction() { return(FACTION_Xen); }
public func IsThreat() { return(true); }

protected func Initialize()
{
  aCmdData = [7];
  SetCmd();
  SetAction("Walk");
  SetDir(Random(2));
  AddEffect("SelfHeal",this(),20,30,this());
  
  SetName(GetName(0,GetID()));
}

public func FxSelfHealTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
  if(GetEnergy(pTarget) < GetPhysical("Energy",0,pTarget)/1000)
    DoEnergy(1,pTarget);
}

protected func Death(int iKilledBy)
{
  DieSound();
  SetAction("Dead");
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

public func IsBulletTarget(id idBullet, object pBullet, object pShooter)
{
  if(idBullet == GLOB) return(false);
  if(idBullet == SLST) return(false);
  if(idBullet == SGRN) return(false);
  return(GetAlive());
}

public func OnDmg(int iDamage, int iType)
{
  if(iType == DMG_Bio)
    return(100);
}

public func OnHit(int iDamage, int iType, object pFrom)
{
  Hurt();
  Splatter(iDamage,iType,pFrom);
  
  if(pFrom)
  {
    if(pFrom->~IsBullet())
    {
      SetXDir(GetXDir()/2+Sin(GetR(pFrom),iDamage));
      SetYDir(GetYDir()/2-Cos(GetR(pFrom),iDamage));
    }
  }
}

public func Hurt()
{
  HurtSound();
  return(1);
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
  if(GetAction() eq "Dead") return();
  
  if(pEnemy)
  {
    if((Abs(GetX(pEnemy)-GetX()) > 300) || (Abs(GetY(pEnemy)-GetY()) > 100) || !GetAlive(pEnemy) || pEnemy->Contained())
    {
      pEnemy = 0;
      FinishCommand(this(),false,0);
      SetComDir(COMD_Stop);
      return();
    }
    
    if((ObjectDistance(pEnemy) < 100) && (GetAction() eq "Walk"))
    {
      DoAttackJump(pEnemy);
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
      
      if(GetAction() eq "Walk")
      {
        if(!Random(8))
        {
          if(GetDir() == DIR_Left)
          {
            if(GBackSolid(-10,0))
            {
              if(!GBackSolid(-10,-15))
              {
                SetComDir(COMD_Stop);
                TurnLeft();
                DoJump();
                return();
              }
            }
          }
        }
        else
        {
          if(GBackSolid(+10,0))
          {
            if(!GBackSolid(+10,-15))
            {
              SetComDir(COMD_Stop);
              TurnRight();
              DoJump();
              return();
            }
          }
        }
          
        if(Random(2)) TurnRight();
        else TurnLeft();
      }
    }
      
    if(!Random(2))
      SetComDir(COMD_Stop);
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

  AlertSound();
  //Message("×",this());
  for(var headcrab in FindObjects(Find_InRect(-200,-200,+400,+400),Find_Exclude(this()),Find_OCF(OCF_Alive),Find_Faction(Faction())))
    headcrab->~OnAlert(pTarget);
}

public func OnAlert(object pTarget)
{
  if(UserControlled()) return();
  //Message("•",this());
  
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

private func FindPrey()
{
  var angle = 90;
  if(GetDir() == DIR_Left) angle = 270;
  var targets = FindTargets(this(),100,45,angle);
  
  if(GetLength(targets))
    return(targets[0]);
  return();
  //for(var prey in FindObjects(Find_InRect(/*((GetDir()-1)*2)*180*/-100,-100,+200,+150),Find_OCF(OCF_Prey|OCF_Alive),Find_Not(Find_Faction(Faction())),Find_NoContainer(),Sort_Distance()))
    //if(PathFreeObject4K(this(),prey,5))
      //return(prey);
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
public func DoJump()
{
  if(((GetAction() ne "Walk") && (GetAction() ne "Stand")) || GetEffect("IntAttackDelay", this())) return(0);
  return(SetAction("PrepareJump"));
}

public func DoAttackJump(object pTarget)
{
  if(((GetAction() ne "Walk") && (GetAction() ne "Stand")) || GetEffect("IntAttackDelay", this())) return(0);
  AttackSound();
  return(SetAction("PrepareAttackJump",pTarget));
}

private func ExecJump()
{
  Jump();
  var jump = GetPhysical("Jump", 0) * 100 / GetPhysical("Jump", 0,0, GetID());
  SetXDir(22 * (GetDir()*2-1) * jump / 100);
  SetYDir(-28 * jump / 100);
  if(!GetEffect("IntAttackDelay", this())) AddEffect("IntAttackDelay", this(), 1, 70);
  JumpSound();
}

private func ExecAttackJump()
{
  Jump();
  var target = GetActionTarget();
  if(target)
  {
    SetXDir(BoundBy(GetX(target) - GetX(), -40, +40));
    SetYDir(BoundBy(GetY(target) - GetY(), -40, -20));
    SetActionTargets();
  }
  else
  {
    var jump = GetPhysical("Jump", 0) * 100 / GetPhysical("Jump", 0,0, GetID());
    SetXDir(22 * (GetDir()*2-1) * jump / 100);
    SetYDir(-28 * jump / 100);
    if(!GetEffect("IntAttackDelay", this())) AddEffect("IntAttackDelay", this(), 1, 90);
  }
  SetAction("AttackJump");
  JumpSound();
}

private func AttackStart()
{
  if(!GetEffect("Attacking",this()))
    AddEffect("Attacking",this(),20,2,this());
}

public func FxAttackingTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
  var victim = FindObject2(Find_AtPoint(),Find_OCF(OCF_Prey|OCF_Alive),Find_NoContainer(),Find_Not(Find_Faction(Faction())),Find_Exclude(this()),Sort_Distance());
  if(victim)
  {
    BiteSound();
    Fling(victim, BoundBy(GetXDir(),-2,+2), BoundBy(GetYDir(),-2,+2));
    DoDmg(15,DMG_Melee,victim);
    SetYDir(0);
    if(!GetEffect("IntAttackDelay", this())) AddEffect("IntAttackDelay", this(), 1, 35);
    return(-1);
  }
}

private func AttackEnd()
{
  Message("AttackEnd",this());
  RemoveEffect("Attacking",this());
}

private func TurnRight()
{
  if (Stuck() || (GetAction() ne "Walk" && GetAction() ne "Stand")) return();
  SetDir(DIR_Right);
  SetComDir(COMD_Right);
  return(1);
}

private func TurnLeft()
{
  if (Stuck() || (GetAction() ne "Walk" && GetAction() ne "Stand")) return();
  SetDir(DIR_Left);
  SetComDir(COMD_Left);
  return(1);
}

/* Steuerung durch Besessenheit */

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
  return(ControlUp());
}
protected func ControlUp()
{
  [$TxtMovement$]
  if(DoJump())
    return(1);
  return(0);
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

public func ContainedUpdate(object self, int comdir){return(ControlUpdate(self,comdir));}
public func ControlUpdate(object self, int comdir)
{
  SetComDir(comdir);
  ClearScheduleCall(this(), "ClearDir");
  if(comdir == COMD_Down || comdir == COMD_Up) ScheduleCall(this(), "ClearDir", 1, (Abs(GetXDir())+1)/2, true);
  if(comdir == COMD_Left || comdir == COMD_Right) ScheduleCall(this(), "ClearDir", 1, (Abs(GetYDir())+1)/2, false);

  return(1);
}

protected func ContainedThrow()
{
  [$TxtAttack$]
  return(ControlThrow());
}
protected func ControlThrow()
{
  [$TxtAttack$]
  DoAttackJump(FindPrey());
  return(1);
}

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
  if(GetProcedure() eq "WALK")
    SetAction("Alert");
  return(1);
}

protected func ContainedDigDouble()
{
  [$TxtLeave$]
  RemoveEffect("PossessionSpell", this());
  return(1);
}


/* Geräusche */

public func JumpSound(){Sound("HCRB_Jump.ogg");}
public func AttackSound(){Sound("HCRB_Attack*.ogg");}
public func AlertSound(){Sound("HCRB_Alert.ogg");}
public func HurtSound(){Sound("HCRB_Hurt*.ogg");}
public func DieSound(){Sound("HCRB_Die*.ogg");}
public func BiteSound(){Sound("HCRB_Bite.ogg");}