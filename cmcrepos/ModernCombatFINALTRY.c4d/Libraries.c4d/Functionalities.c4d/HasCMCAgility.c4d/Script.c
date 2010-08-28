/*-- CMC Agilit�t --*/

#strict 2

/* Check */

private func MayFlip()
{
  return (this->IsJumping() || GetAction() == "JetpackFlight") && !GetEffect("ExtraJump", this);
}

/* Geschwindigkeitsschub */

protected func WallJumpBoost()
{
  var jump = GetPhysical("Jump", 0) * 100 / GetPhysical("Jump", 0,0, GetID());
  SetXDir(25*(GetDir()*2-1) * jump / 100);
  SetYDir(-30 * jump / 100);
}

protected func BackFlipBoost()
{
  var jump = GetPhysical("Jump", 0) * 100 / GetPhysical("Jump", 0,0, GetID());
  SetXDir(42*((GetXDir()>0)*2-1) * jump / 100);
  SetYDir(-22 * jump / 100);
}

/* Wandsprung stoppen */

private func StopWallJump()
{
  var i = GetEffectCount(0,this);
  var j;
  while(i--)
   if(j = GetEffect("IntWallJump",this,i))
    RemoveEffect(0,this,j);
}

/* Backflip */

protected func BackFlip()
{
  Sound("ClonkAction*.ogg");
  SetComDir(!GetComDir());
  ScheduleCall(0, "BackFlipBoost", 1, 1);
}

/* Agilit�t */

private func ControlAgility(string strControl)
{
  //In Fl�ssigkeiten geht das Ganze nicht.
  if (InLiquid()) return;

  //Links
  if(strControl == "ControlLeft")
  {
   if(this->IsJumping())
   {
    SetDir(DIR_Left);
    SetComDir(COMD_Left); 
   }
   else
   if(GetProcedure() == "SCALE" && GetDir() != DIR_Left)
     AddEffect("IntWallJump",this,10,20,this);
   var iEff = AddEffect("ControlStack", this, 110, 5, this);
   EffectVar(0, this, iEff) = COMD_Left;
   return;
  }

  //Rechts
  if(strControl == "ControlRight")
  {
   if(this->IsJumping())
   {
    SetDir(DIR_Right);
    SetComDir(COMD_Right); 
   }
   else
   if(GetProcedure() == "SCALE" && GetDir() != DIR_Right)
    AddEffect("IntWallJump",this,10,20,this);

   var iEff = AddEffect("ControlStack", this, 110, 5, this);
   EffectVar(0, this, iEff) = COMD_Right;
   return;
  }

  //Doppellinks
  if(strControl == "ControlLeftDouble")
  {
   if(GetEffect("IntWallJump",this) && !GetContact(this))
   {
    SetAction("Jump");
    SetDir(DIR_Left);
    SetComDir(COMD_Left);
    WallJumpBoost();
    Sound("ClonkAction*.ogg");
    StopWallJump();
    return 1;
   }
  }
  //Doppelrechts
  if(strControl == "ControlRightDouble")
  {
   if(GetEffect("IntWallJump",this) && !GetContact(this))
   {
    SetAction("Jump");
    SetDir(DIR_Right);
    SetComDir(COMD_Right);
    WallJumpBoost();
    Sound("ClonkAction*.ogg");
    StopWallJump();
    return 1;
   }
  }

  //Einmal links
  if(strControl == "ControlLeftSingle")
  {
   if (this->IsJumping())
   {
    SetDir(DIR_Left);
    SetComDir(COMD_Left);
    return 1;
   }
  }

  //Einmal rechts
  if(strControl == "ControlLeftSingle")
  {
   if(this->IsJumping())
   {
    SetDir(DIR_Right);
    SetComDir(COMD_Right);
    return 1;
   }
  }

  //Springen
  if(strControl == "ControlUp")
  {
   if(WildcardMatch(GetAction(), "Scale*") && GetActTime() < 5)
           EffectVar(2, this, AddEffect("ControlStack", this, 110, 5, this)) = true;

   if(GetEffect("ControlStack", this))
           EffectVar(1, this, GetEffect("ControlStack", this)) = COMD_Up;

   return;
  }
}


/* Backflip starten */

public func JumpStart(bool bBackflip)
{
  var iEff, iComd;
  if((iEff = GetEffect("ControlStack", this)))
  {
   if(EffectVar(1, this, iEff) != COMD_Up && !bBackflip) return;
   iComd = EffectVar(0, this, iEff);

   RemoveEffect(0, this, iEff);
   if(iComd == COMD_Left)
   {
    if(GetDir() == DIR_Right)
     BackFlip();
   }
   if(iComd == COMD_Right)
   {
    if(GetDir() == DIR_Left)
     BackFlip();
   }
  }
  //F�r Leiter
  _inherited(bBackflip);
}

public func ReleaseLadderStop()
{
  if(this->ReleaseLadder(-20*(GetDir()*2-1)))
  {
   Sound("ClonkAction*.ogg");
   ScheduleCall(0, "BackFlipBoost", 1, 1);
  }
}

public func FxControlStackEffect(string newEffect, object pTarget, int iNo)
{
  if(newEffect != "ControlStack") return;
  ChangeEffect(0, pTarget, iNo, 0, -1);
  return -2;
}