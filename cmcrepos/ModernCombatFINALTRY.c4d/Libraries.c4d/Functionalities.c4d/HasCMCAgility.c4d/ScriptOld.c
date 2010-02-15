/*-- Has CMC Agility --*/

#strict

/* Allgemeines */

private func StopWallJump()
{
  var i = GetEffectCount(0,this());
  var j;
  while(i--)
    if(j = GetEffect("IntWallJump",this(),i))
      RemoveEffect(0,this(),j);
}

public func FxIntWallJumpTimer(){return(-1);}


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

private func MayJump()//Aus L_AG ("Has Agility") genommen. -> Komplatibel.
{
	if(this()->IsJumping() || GetAction() eq "JetpackFlight")
	  if(!GetEffect("ExtraJump", this))
	    return(true);
}


/* Overloads */

public func JumpStart(bool bBackflip)
{
  if(MayJump())
  {
    Log("JumpStart -> %d/%d",GetComDir(),GetDir());
    if(GetComDir() == COMD_Left)
      if(GetDir() == DIR_Right)
        BackFlipBoost();

    if(GetComDir() == COMD_Right)
      if(GetDir() == DIR_Left)
        BackFlipBoost();
  }

	_inherited(bBackflip);
}

public func ControlLeftDouble()
{
  if(GetDir() == DIR_Right)
  {
    if(MayJump())
    {
      BackFlipBoost();
      return(1);
    }
 
    if(GetEffect("IntWallJump",this()) && !GetContact(this()))
    {
      Log("WallJump");
      
      SetAction("Jump");
      SetDir(DIR_Left);
      SetComDir(COMD_Left);
      WallJumpBoost();
      Sound("Kime*");
      StopWallJump();
      return(1);
    }
  }
    
  return(_inherited(...));
}

public func ControlRightDouble()
{
  if(GetDir() == DIR_Left)
  {
    if(MayJump())
    {
      BackFlipBoost();
      return(1);
    }
 
    if(GetEffect("IntWallJump",this()) && !GetContact(this()))
    {
      Log("WallJump");
      
      SetAction("Jump");
      SetDir(DIR_Right);
      SetComDir(COMD_Right);
      WallJumpBoost();
      Sound("Kime*");
      StopWallJump();
      return(1);
    }
  }
    
  return(_inherited(...));
}

public func ControlLeft()
{
  if((GetDir() == DIR_Right) && (GetProcedure() eq "SCALE"))
  {
    Log("AddEffect");
    AddEffect("IntWallJump",this(),10,20,this());
  }

  return(_inherited(...));
}

public func ControlRight()
{
  if((GetDir() == DIR_Left) && (GetProcedure() eq "SCALE"))
  {
    Log("AddEffect");
    AddEffect("IntWallJump",this(),10,20,this());
  }

  return(_inherited(...));
}