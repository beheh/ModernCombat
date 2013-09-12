/*-- Ladder-Appendto, um Leitern runterzusliden --*/

#strict //Das Ladderscript ist leider auch nur #strict
#appendto L_LA

private func ControlLadder(string strControl, par1) // par1 is comdir for ControlUpdate
{

  // Wenn man schnell genug ist, kann man sich so sogar wieder an der Leiter anhalten!
  if(lastladder)
    lastladder = 0;
    
  // Nicht an einem kletterbaren Objekt.
  if (GetAction() ne "ScaleLadder") return(0); 

  // Klettert an der Leiter
  if(strControl S= "ControlCommand") {
    ClimbLadder();
    return(1);
  }
  
  var effect;
  if((effect = GetEffect("ScalingLadder", this())) && strControl ne "ControlDown")
  	EffectVar(1, this, effect) = false;

  //Log("%s: %s",GetName(),strControl); //DEBUGZ!

  // JnR
  if (strControl S= "ControlUpdate")
  {
    var comdir = par1;
    // Strip away COMD_Left/COMD_Right, this is handled via ControlLeft/ControlRight
    if(ComDirLike(comdir, COMD_Left)) comdir = ComDirTransform(comdir, COMD_Right);
    if(ComDirLike(comdir, COMD_Right)) comdir = ComDirTransform(comdir, COMD_Left);
    SetComDir(comdir);
  }
  
  // Rauf
  if (strControl S= "ControlUp" && !GetPlrCoreJumpAndRunControl(GetController()))
    SetComDir(COMD_Up);
  // Runter
  if (strControl S= "ControlDown" && !GetPlrCoreJumpAndRunControl(GetController()))
    SetComDir(COMD_Down);
  // Rechts
  if (strControl S= "ControlRight")
  {
    // Richtung wechseln
    if (GetDir() == DIR_Right) {
      SetDir(DIR_Left);
      SetComDir(COMD_Stop);
      if (!AdjustLadderOffset(GetActionTarget())) //können wir überhaupt?
        SetDir(DIR_Right());
    }
    // Abspringen
    else {
      if(GetComDir() == COMD_Up) ReleaseLadderUp();
      else if(GetComDir() == COMD_Stop) ReleaseLadderStop();
      else ReleaseLadderDown();
    }
  }
  // Links
  if (strControl S= "ControlLeft")
  {
    // Richtung wechseln
    if (GetDir() == DIR_Left) {
      SetDir(DIR_Right);
      SetComDir(COMD_Stop);
      if (!AdjustLadderOffset(GetActionTarget()))
        SetDir(DIR_Left);
    }
    // Loslassen
    else {
      if(GetComDir() == COMD_Up) ReleaseLadderUp();
      else if(GetComDir() == COMD_Stop) ReleaseLadderStop();
      else ReleaseLadderDown();
    }
  }
  
  //Runterrutschen
  if (strControl S= "ControlDownDouble" && effect)
  		EffectVar(1, this, effect) = true;
  
  // Werfen
  if (strControl S= "ControlThrow") LadderThrow();
  
  //Graben
  if (strControl S= "ControlDig") LadderDig();
  
  return(1);  
}  

protected func FxScalingLadderTimer(object pTarget, int iEffectNumber)
{
  var pLadder;
  // Clonk hat aus irgendeinem Grund seine Kletteraktion verloren
  if (!(GetAction() S= "ScaleLadder"))
    // Klettern beenden
    return(ReleaseLadder(0));

  // Keine Leiter mehr: klettern beenden
  if (!(pLadder = FindLadder()))
  {
    // Oberes Ende: versuchen, die Wand zu erreichen
    if (GetComDir() == COMD_Up()) {
      return(ReleaseLadder(-15 + GetDir() * 30,-20));
    }
    // Unteres Ende: einfach fallen lassen
    else
      return(ReleaseLadder(0));
  }

  // Ggf. neues Objekt zum dran klettern speichern
  SetActionTargets(pLadder);
  // Abstand zum Objekt
  AdjustLadderOffset(pLadder);

  // Klettergeschwindigkeit 
  //var iSpeed = BoundBy(GetPhysical("Scale") / 5000, 0, 3), iStep = 0;
  //var iStep = Max(1,(GetPhysical("Scale")/100*4*HZCK_LADDERSPEED)/200000);
  var iStep = Max(1,(GetPhysical("Scale")*4*GetLadderSpeed())/20000);
  
  // Kletterbewegung
  var iPosY = EffectVar(0, pTarget, iEffectNumber);
  var iLastY = GetY();
  var iPhase = GetPhase();
  var fSliding = EffectVar(1, pTarget, iEffectNumber);
  if (GetComDir() == COMD_Up()) { iPosY -= iStep; iPhase += iStep*14/1000; }
  if (GetComDir() == COMD_Down())
  {
  	var iPosX = GetX();
  	var iNewPosY = iPosY + (iStep*5)/2;
  	var iYAdd = GetObjectVal("Height", 0, this) + GetObjectVal("Offset", 0, this, 1);
  	if(fSliding && PathFree(iPosX, iLastY + iYAdd, iPosX, iNewPosY/100 + iYAdd))
  	{
  		iPosY = iNewPosY;
  		if(GetEffectData(EFSM_ExplosionEffects) > 0) CastSmoke("Smoke3",2,2,0,2,4,100);
  		Sound("AHBS_Progress*.ogg", 0, this);
  	}
  		
  	else
  	{
  		iPosY += iStep;
  		iPhase -= iStep*14/1000;
  	}
  }
  
  // stecken wir fest?
  if (Stuck())
  {
    // Nicht in festes Material klettern
		if(GetComDir() == COMD_Up)
		{
			ReleaseLadder(0,-2);
			SetAction("Hangle");
		}
		if(GetComDir() == COMD_Down)
		{
			ReleaseLadder(0,2);
			SetAction("Walk");
		}
		else
		{
	  	SetPosition(GetX(), iLastY);
	  	SetComDir(COMD_Stop());
	  }
  	
  	return();
  }
  
  if (iPhase < 0) iPhase = 15; if (iPhase > 15) iPhase = 0;
  SetPhase(iPhase);
  SetPosition(GetX(), iPosY/100);
  SetXDir(0); SetYDir(0);

  EffectVar(0, pTarget, iEffectNumber) = iPosY;
}
