/*---- Zombie ----*/

#strict
#include CLNK

/* Allgemeines */
public func IsPossessible() {return(1);}
public func IsHeadcrab() {return(1);}
public func IsThreat() {return(true);}

protected func Initialize()
{
  _inherited();

  aCmdData = [7];
  bCmd = false;
  bWait = false;
  SetCmd();
  
  SetName(GetName(0,GetID()));
  
  if(!UserControlled())
  {
    SetColorDw(HSL(Random(255),Random(128),70+Random(70)));
    SetPhysical("Walk", GetPhysical("Walk",1,0,GetID()) + RandomX(-3000,+3000), 1);
  }
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
  HurtSounds(iDamage,iType);
  Splatter(iDamage,iType,pFrom);

  if(GetAction() eq "FakeDead")
  {
    SetAction("FlatUp");
    return();
  }
}

func Hit2(int xDir, int yDir){}//Keine Hit-Sounds oder so...

/* Haupt-KI */
local pEnemy,aCmdData,bCmd,bCmdUntilEnemy,bWait;

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

public func AttackingSameEnemy(object pCompare)
{
  return((pEnemy == pCompare) && !bWait);
}

protected func Activity() 
{
  if(UserControlled()) return();
  if(GetAction() eq "Dead") return();
  
  if(GetAction() eq "FakeDead")
  {
    if(pEnemy)
    {
      if((ObjectDistance(pEnemy) > 200) || !GetAlive(pEnemy) || pEnemy->Contained())
      {
        if(GetCommand(this(),1,0) == pEnemy)
          FinishCommand(this(),false,0);
        pEnemy = 0;
        SetComDir(COMD_Stop);
        return();
      }
      
      if(ObjectDistance(pEnemy) < 100)
      {
        SetAction("FlatUp");
        Alert(pEnemy);
        return();
      }
    }
    else
    {
      pEnemy = FindPrey();
    }
    return();
  }
  
  if(pEnemy)
  {
    var dst = ObjectDistance(pEnemy);
    if((dst > 300) || !GetAlive(pEnemy) || pEnemy->Contained())
    {
      if(GetCommand(this(),1,0) == pEnemy)
        FinishCommand(this(),false,0);
      pEnemy = 0;
      SetComDir(COMD_Stop);
      return();
    }
    
    if(pEnemy->ObjectCount2(Find_Distance(40),Find_Exclude(this()),Find_Func("IsHeadcrab"),Find_Func("AttackingSameEnemy",pEnemy)) >= 3)
    {
      if(GetCommand(this(),1,0) == pEnemy)
        FinishCommand(this(),false,0);
      bWait = true;
      Idle();
      return();
    }
    else
      bWait = false;
    
    if(dst < 10)
    {
      SetAction("Fight",pEnemy);
      ObjectSetAction(pEnemy,"Fight",this()); 
      return();
    }
    
    if(ReadyForJumpAttack() && !Random(4))
    {
      DoJumpAttack();
    }
    
    if(ReadyToPunch())// || ((dst < 8) && (GetAction() eq "Walk")))
    {
      if(Random(5))
        DoBeatAttack();
      else
        DoBeat2Attack();
      return();
    }
    
    if(GetCommand(this()) ne "Follow")
    {
      bCmd = false;
      if(bCmdUntilEnemy) SetCmd();
      SetCommand(this(),"Follow",pEnemy); 
    }
      
    if((GetY(pEnemy) < GetY()-5) || (pEnemy && (dst < 40) && !Random(20)))
    {
      if(!Random(10) && (GetProcedure() eq "WALK"))
        Jump();
    }
    
    if(!Random(4) && (GetProcedure() eq "WALK"))
      Sound("ZOMB_Idle*.ogg");
  }
  else
  {
    pEnemy = FindPrey();
    if(pEnemy) return(Alert(pEnemy));
  }
  
  Idle();
}

protected func Idle()
{
  if(!GetCommand() || !bCmd)
  {
    if(!pEnemy)
    {
      if(aCmdData[0])
      {
        bCmd = true;
        SetCommand(this(),aCmdData[0],aCmdData[1],aCmdData[2],aCmdData[3],aCmdData[4],aCmdData[5],aCmdData[6]);
        return();
      }
      
      if(GetAction() ne "FakeDead")
      {
        if(!Random(20))
          SetAction("FakeDead");
      }
      else
      {
        if(!Random(20) && (GetProcedure() eq "WALK"))
        {
          SetAction("Throw");
          Sound("ZOMB_Idle*.ogg");
        }
        //if(!Random(10))
          //SetAction("FlatUp");
      }
    }
  
    if(!Random(6))
    {      
      if(Random(2)) TurnRight();
      else TurnLeft();
    }
    
    if(!Random(4))
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

  if(pEnemy != pTarget)
    Sound("ZOMB_Alert*.ogg");
  //Message("×",this());
  for(var headcrab in FindObjects(Find_InRect(-200,-200,+400,+400),Find_Exclude(this()),Find_OCF(OCF_Alive),Find_Func("IsHeadcrab")))
    headcrab->OnAlert(pTarget);
}

public func OnAlert(object pTarget)
{
  if(UserControlled()) return();
  //Message("•",this());
  
  if(pTarget->~IsHeadcrab())
  {
    if(!pEnemy)
      SetCmd(false,this(),"Follow",pTarget);
  }
  else
    if(!pEnemy)
      pEnemy = pTarget;
}

private func ReadyToPunch()
{
  if(GetAction() eq "Fight") return(true);
  if(pEnemy)
    return((ObjectDistance(pEnemy) < 8) && (GetAction() eq "Walk"));
  return(false);
}

private func ReadyForJumpAttack()
{
  if(GetProcedure() ne "WALK") return(false);
  if(pEnemy)
    return((ObjectDistance(pEnemy) < 40) && (ObjectDistance(pEnemy) >= 12) && (GetY(pEnemy) > GetY()-5));
  return(false);
}

private func PunchTarget()
{
  if((GetAction() eq "Fight") && GetActionTarget())
    return(GetActionTarget());
  return(pEnemy);
}

private func FindPrey()
{
  var angle = 270;
  if(GetDir() == DIR_Left) angle = 90;
  var targets = FindTargets(this(),150,60,angle);
  
  if(GetLength(targets))
  {
    if(targets[0]->ObjectCount2(Find_Distance(20),Find_Func("IsHeadcrab")) < 3)
      return(targets[0]);
  }
  return();
}

protected func RejectCollect(id idObj, object pObj)
{
  return(!UserControlled());//Erstmal nur Spieler was aufnehmen lassen.
}

protected func ContactLeft()
{
  if(UserControlled()) return();
  if(Random(3))
    return(TurnRight());
}
  
protected func ContactRight()
{
  if(UserControlled()) return();
  if(Random(3))
    return(TurnLeft());
}

/* Geräusche */
public func Death(int iKilledBy)
{
  // Sound und Meldung
  Sound("ZOMB_Die*.ogg");

  SetAction("Dead");
  FadeOut4K(1);
  
  GameCallEx("MonsterKilled",GetID(),this(),iKilledBy);
  
  return(1);
}

public func Hurt()
{
  Sound("ZOMB_Hurt*.ogg");
  return(1);
}

public func HurtSounds(int iDmg, int iType)
{
  iDmg = -iDmg;
  if(iDmg <= 0) return();
  
  var val = BoundBy(iDmg,1,30);
  if(Random(val)*100/val < 50) return();

  Sound("ZOMB_Hurt*.ogg");
}


/* Aktionen */
public func DoJumpAttack()
{
  if(GetEffect("IntJumpAttackDelay", this()) || GetEffect("IntAttackDelay", this())) return();
  SetAction("JumpAttack");
  Sound("ZOMB_Hurt*.ogg");

  var jump = GetPhysical("Jump", 0) * 100 / GetPhysical("Jump", 0,0, GetID());
  SetXDir(25 * (GetDir()*2-1) * jump / 100);
  SetYDir(-22 * jump / 100);
}

public func DoBeatAttack()
{
  if(!ReadyToPunch() || GetEffect("IntAttackDelay", this())) return();
  SetAction("Beat");
}

public func DoBeat2Attack()
{
  if(!ReadyToPunch() || GetEffect("IntAttackDelay", this())) return();
  SetAction("Beat2");
}

private func JumpAttacking()
{
  CheckStuck();
  var victim = FindObject2(Find_AtPoint((GetDir()*2-1)*7),Find_Func("IsThreat"),Find_Exclude(this()),Find_NoContainer(),Find_Not(Find_Func("IsHeadcrab")));
  if(!victim) return();
  
  Sound("ZOMB_Beat*.ogg");
  if(!ObjectSetAction(victim,"FlatUp"))
    Fling(victim,GetDir()*2-1,-1);
  else
  {
    victim->SetXDir();
    victim->SetYDir();
  }
  DoDmg(Distance(GetXDir(),GetYDir())/2,DMG_Melee,victim);
}

private func JumpAttackStop()
{
  SetXDir();
  SetYDir();
  SetAction("FlatUp");
  AddEffect("IntJumpAttackDelay", this(), 1, 35*3);
}

private func ExecBeat()
{  
  var victim = PunchTarget();//FindObject2(Find_AtPoint((GetDir()*2-1)*6,0),Find_OCF(OCF_Prey|OCF_Alive),Find_NoContainer(),Find_Not(Find_Func("IsHeadcrab")),Find_Exclude(this()),Sort_Distance());
  if(victim)
  {
    Sound("ZOMB_Beat*.ogg");
    if(!ObjectSetAction(victim,"GetPunched") || !Random(2))
      Fling(victim,GetDir()*2-1, -1);
    DoDmg(10,DMG_Melee,victim);
    AddEffect("IntAttackDelay", this(), 1, 30);
    return();
  }
  
  Sound("ZOMB_BeatMiss*.ogg");
  SetActionTargets();
}

private func ExecBeat2()
{  
  var victim = PunchTarget();//FindObject2(Find_AtPoint((GetDir()*2-1)*6,0),Find_OCF(OCF_Prey|OCF_Alive),Find_NoContainer(),Find_Not(Find_Func("IsHeadcrab")),Find_Exclude(this()),Sort_Distance());
  if(victim)
  {
    Sound("ZOMB_Beat*.ogg");
    Fling(victim,(GetDir()*2-1)*2, -2);
    DoDmg(10,DMG_Melee,victim);
    AddEffect("IntAttackDelay", this(), 1, 45);
    return();
  }
  
  Sound("ZOMB_BeatMiss*.ogg");
  SetActionTargets();
}

private func Punching()
{
  Sound("Punch*");
  Punch(GetActionTarget());
  SetComDir(COMD_Stop);
}

private func Fighting()
{
  if(Random(3)) return();

  if(!Random(2))
    ExecBeat();
  else
    SetAction("Punch");
}


private func TurnRight()
{
  if (Stuck() || (GetAction() ne "Walk")) return();
  SetDir(DIR_Right);
  SetComDir(COMD_Right);
  return(1);
}

private func TurnLeft()
{
  if (Stuck() || (GetAction() ne "Walk")) return();
  SetDir(DIR_Left);
  SetComDir(COMD_Left);
  return(1);
}


/* Steuerung durch Besessenheit */

protected func ControlCommand(szCommand, pTarget, iTx, iTy)
{
  if(GetAction() eq "FakeDeath")
    SetAction("FlatUp");

  if(szCommand eq "MoveTo")
  {
    return(SetCommand(this(),szCommand, pTarget, iTx, iTy));
  }
  return(0);
}

protected func ControlCommandFinished(string strCommand, object pTarget, int iTx, int iTy, object pTarget2, Data)
{
  if(!aCmdData[0]) return();
  if(strCommand == aCmdData[0])
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
  if(!GetPlrCoreJumpAndRunControl(caller->GetController()))
    Jump();
  return(1);
}

protected func ContainedUpDouble(object caller)
{
  [$TxtMovement$]
  return(ControlUpDouble());
}

protected func ControlUpDouble()
{
  [$TxtMovement$]
  if(GetAction() ne "FakeDead") return(0);
  SetAction("FlatUp");
  return(1);
}

protected func ContainedDown(object caller)
{
  [$TxtMovement$]
  SetCommand(this(),"None");
  if(Contained()) return SetCommand(this, "Exit");
  
  if(!GetPlrCoreJumpAndRunControl(caller->GetController()))
    SetComDir(COMD_Down);
  ControlDown();
  return(1);
}

protected func ControlDown()
{
  [$TxtMovement$]
  if((GetProcedure() eq "WALK") && GetPlrDownDouble(GetController()))
  {
    SetAction("FakeDead");
    return(1);
  }
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
  pEnemy = FindPrey();
  if((GetAction() ne "Fight") && !ReadyToPunch())
  {
    if(GetProcedure() eq "WALK")
      DoJumpAttack();
  }
  else
  {
    if(Random(5))
      DoBeatAttack();
    else
      DoBeat2Attack();
  }

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
    SetAction("Throw");
  return(1);
}

protected func ContainedDigDouble()
{
  [$TxtLeave$]
  RemoveEffect("PossessionSpell", this());
  return(1);
}