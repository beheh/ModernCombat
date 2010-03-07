/*-- Has CMC Agility --*/

#strict 2

/* Allgemeines */

private func StopWallJump()
{
  var i = GetEffectCount(0,this);
  var j;
  while(i--)
    if(j = GetEffect("IntWallJump",this,i))
      RemoveEffect(0,this,j);
}

public func FxIntWallJumpTimer(){return -1;}

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

protected func BackFlip()
{
  if(GetComDir() == COMD_Left)
    SetComDir(COMD_Right);
  else
    SetComDir(COMD_Left);
	ScheduleCall(0, "BackFlipBoost", 1, 1);
}


/* aus L_AG ("HasAgility") */


/* Control from the clonk */

private func ControlAgility(string strControl)
{
	// return 0 if the rest of the clonks script should be executed

	// left
	if(strControl == "ControlLeft")
  {
		if(this->IsJumping())
    {
			SetDir(DIR_Left);
			SetComDir(COMD_Left); 
		}
    else if(GetProcedure() == "SCALE")
    {
      AddEffect("IntWallJump",this,10,20,this);
    }
		
		var iEff = AddEffect("ControlStack", this, 110, 5, this);
		EffectVar(0, this, iEff) = COMD_Left;
		return 0;
	}
  
	// right
	if(strControl == "ControlRight")
  {
		if(this->IsJumping())
    {
			SetDir(DIR_Right);
			SetComDir(COMD_Right); 
		}
    else if(GetProcedure() == "SCALE")
    {
      AddEffect("IntWallJump",this,10,20,this);
    }
		
		var iEff = AddEffect("ControlStack", this, 110, 5, this);
		EffectVar(0, this, iEff) = COMD_Right;
		return 0;
	}
  
	// left double
	if(strControl == "ControlLeftDouble")
  {
    if(GetEffect("IntWallJump",this) && !GetContact(this))
    {
      SetAction("Jump");
      SetDir(DIR_Left);
      SetComDir(COMD_Left);
      WallJumpBoost();
      Sound("Kime*");
      StopWallJump();
      return 1;
    }
	}

	// right double
	if(strControl == "ControlRightDouble")
  {
    if(GetEffect("IntWallJump",this) && !GetContact(this))
    {
      SetAction("Jump");
      SetDir(DIR_Right);
      SetComDir(COMD_Right);
      WallJumpBoost();
      Sound("Kime*");
      StopWallJump();
      return 1;
    }
	}
	
	// left single
	if(strControl == "ControlLeftSingle")
  {
		if (this->IsJumping())
    {
		    SetDir(DIR_Left);
		    SetComDir(COMD_Left);
		    return 1;
		}
	}
	
	// right single
	if(strControl == "ControlLeftSingle")
  {
		if (this->IsJumping())
    {
		    SetDir(DIR_Right);
		    SetComDir(COMD_Right);
		    return 1;
		}
	}
	
	// up
	if(strControl == "ControlUp")
  {
		if(WildcardMatch(GetAction(), "Scale*") && GetActTime() < 5)
			EffectVar(2, this, AddEffect("ControlStack", this, 110, 5, this)) = true;

		if(GetEffect("ControlStack", this))
			EffectVar(1, this, GetEffect("ControlStack", this)) = COMD_Up;
			
		return 0;
	}
}

private func MayFlip()
{
	if(this->IsJumping() || GetAction() == "JetpackFlight")
	  if(!GetEffect("ExtraJump", this))
	    return true;
}

/* JumpStart */

// Jump-StartCall: Spezialsprung?
// overloaded by Clonk
public func JumpStart(bool bBackflip)
{
	var iEff, iComd;
	if(iEff = GetEffect("ControlStack", this))
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
	// für ladder
	_inherited(bBackflip);
}

public func ReleaseLadderStop()
{
	if(this->ReleaseLadder(-20*(GetDir()*2-1)))
  {
	  //SetAction("BackFlip");
	  ScheduleCall(0, "BackFlipBoost", 1, 1);
	}
}

public func FxControlStackEffect(string newEffect, object pTarget, int iNo)
{
	if(newEffect != "ControlStack") return 0;
	ChangeEffect(0, pTarget, iNo, 0, -1);
	return -2;
}
