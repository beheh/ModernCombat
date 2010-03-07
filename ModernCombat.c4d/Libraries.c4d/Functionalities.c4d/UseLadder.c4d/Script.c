/* Use ladder functionality */

#strict 2


/* Dieses Script bietet die Grundfunktionalit�t des an Leitern kletterns. Damit
   der Clonk auch wirklich daran klettern kann, muss folgendes noch hinzugef�gt
   werden. In der ActMap die Aktion hinzuf�gen:

[Action]
Name=ScaleLadder
Procedure=NONE
Directions=2
FlipDir=1
Length=16
Facet=0,20,16,20,0,0
NextAction=ScaleLadder

   In allen Jump-Aktionen wie "Jump", "JumpArmed" oder "Dive"
   
StartCall=JumpStart

   hinzuf�gen. Sowie in allen Control*-Funktionen ei!= Zeile an der geeigneten
   Stelle hinzuf�gen. Im Beispiel von ControlUp:
   
if(ControlLadder("ControlUp")) return 1;
   
   Und zwar f�r Up, Down, Right, Left, Dig, Throw, Command (f�r "MoveTo")
*/

local lastladder;

/* Zum Klettern an Leitern und Co. */

// Funktionen:
// GetLadderSpeed -		Gibt Leiterklettergeschwindigkeit zur�ck
// FindLadder -			Findet kletterbares in der Umgebung
// GrabLadder - 		Clonk greift angegebene Leiter
// ReleaseLadder - 		Clonk l�sst Leiter los
// ReleaseLadderUp -	Clonk klettert nach oben und l�sst los
// ReleaseLadderDown - 	Clonk klettert nach unten und l�sst los
// ReleaseLadderStop - 	Clonk klettert nicht und l�sst los
// LadderThrow - 		Werfen beim Klettern
// LadderDig -			Graben beim Klettern
// 
// ClimbLadder -		Mauskontrolle
// ControlLadder -		Steuern des Leiterkletterns
// AdjustLadderOffset - L�sst den Clonk an der Leiter kleben
//
// FxScalingLadderStart-Effekt f�rs Klettern
// FxScalingLadderTimer-...
//
// JumpStart -			StartCall von Jump
// IsJumping -			Einfacher Callback ob der Clonk springt
// 
// FxJumpingTimer -		Effekt f�rs Springen (checkt ob Leiter da)


// Um wieviel der Clonk an Leitern schneller klettert
// Wert/10 = Wand/Leiter; 10 = 1.0, 15 = 1.5,...
// Abw�rtskompatibilit�t!
static const HZCK_LADDERSPEED = 12;
public func GetLadderSpeed() { return HZCK_LADDERSPEED; }

//Ladder wird hier nur so gebraucht, alle Objekte, bei denen IsClimbable true zur�ckgibt k�nnen bestiegen werden!
public func FindLadder()
{
  var iOffset = 6;
  if (GetDir() == DIR_Left)
    iOffset = -6;
  var tmp;
  while(tmp = FindObject(0, iOffset, 0,0,0,0,0,0,0,tmp))
    if(Abs(GetX() - GetX(tmp)) < 6)
      if(tmp->~IsClimbable())
      return tmp;
  return 0;
}

public func GrabLadder(object pLadder)
{
  if(lastladder == pLadder)
    return 0;
  // Kletteraktion
//  SetAction("ScaleLadder", pLadder);
  if(!GetPlrCoreJumpAndRunControl(GetController()))
    SetComDir(COMD_Stop);  

  //erstmal checken ob Backflip 4 great Specialeffects!
  if((GetAction() == "BackFlip" && !Stuck())) {
    //Sound, weil Geschwindigkeit oder so
    Sound("Kime*");
  }

  SetXDir();
  SetYDir();

  //Position anpassen (in der Luft klettern ist doch weird ;))
  AdjustLadderOffset(pLadder);

  //n�, dann eben normal anhalten
  SetAction("ScaleLadder", pLadder);
   
  // Klettereffekt
  AddEffect("ScalingLadder", this, 1, 1, this);
}

public func ReleaseLadder(int iXDir, int iYDir)
{
  var dir;
  //nicht mitten in Material loslassen. W�re ja evil!
  /*
  if(Stuck()) {
    var x = GetX(),y, lasty = y = GetY(), dup, ddown;
    //rauf?
    while(Stuck() && SetPosition(x,++y))
      dup++;
    dup--;

    y=lasty;  
    SetPosition(x,lasty);
    //runter?
    while(Stuck() && SetPosition(x,--y))
      ddown++;

    SetPosition(x,lasty);
    //max. 10 Pixel anpassen
    if(Min(dup,ddown) > 10)
      return ;
    dir = BoundBy(ddown-dup,-1,1);
    if(!dir)
      dir = -1;
    SetPosition(x,lasty+(Min(dup,ddown)*dir));
  } */

  var diffx, pLadder = GetActionTarget(), xpos = GetX(pLadder);
  // von was loslassen..?
  if(!pLadder)
   return 0;

/*  if(!(diffx = pLadder->~LocalN("SizeX",pLadder)))
    diffx = GetDefCoreVal("Width",0,GetID(pLadder));
  diffx+=2;
  SetPosition((xpos+diffx*(GetDir()*-2+1)),GetY());
  */
   
  lastladder = pLadder;
    
  // Klettereffekt beenden
  RemoveEffect("ScalingLadder", this);

  //und runter von der Leiter
  if(dir < 0 || GetContact(0,1,CNAT_Bottom)) {
    SetAction("Walk");
  }
  if(dir > 0) {
    SetAction("Hangle");
  }
  else if(GetAction() != "Tumble")  
    SetAction("Jump");
    
  SetXDir(iXDir);
  SetYDir(iYDir);
  return 1;
}

public func ReleaseLadderUp() {
	//Normal runterspringen
	ReleaseLadder(-10*(GetDir()*2-1));
}

public func ReleaseLadderDown() {
	//Normal runterspringen
	ReleaseLadder(-10*(GetDir()*2-1));
}

public func ReleaseLadderStop() {
	//Normal runterspringen
	ReleaseLadder(-10*(GetDir()*2-1));
}

public func LadderThrow() {
	// Ablegen
	SetComDir(COMD_Stop);
	SetCommand(0, "Drop");
}

public func LadderDig() {

}

protected func ClimbLadder()
{
  if(!GetCommand()) return ;
  // Testen, ob wir noch klettern
  if(GetAction() != "ScaleLadder") return ;
  // Feststellen, ob nach oben oder unten
  var targetx = GetCommand(this, 2);
  var targety = GetCommand(this, 3);
  
  /*
  if(GetActTime() > 5) {
    if(targetx < GetX() && GetDir() != DIR_Right)
      ControlLadder("ControlLeft");
    if(targetx > GetX() && GetDir() != DIR_Left)
      ControlLadder("ControlRight");
  }
  */
  
  if(targety < GetY()) SetComDir(COMD_Up);
  if(targety > GetY()) SetComDir(COMD_Down);
  
  // 2do: Wir m�ssen irgendwie feststellen, ob die Leiter aus ist. :(
  
  
  // Wegpunkt oben? fr�her abspringen.
  if(Inside(GetY()-targety,5,15) && Abs(GetX()-targetx) < 30)
  {
    var comd = GetComDir();
    if(targetx < GetX())
    {
    	ControlLadder("ControlLeft");
    	if(GetDir() == DIR_Left)
    	  SetComDir(comd);
   	}
    if(targetx > GetX())
    {
    	ControlLadder("ControlRight");
    	if(GetDir() == DIR_Right)
    	  SetComDir(comd);
   	}
  }
    
  // Sind wir ungef�hr da?
  if(Inside(GetY(), targety-5, targety+5) && PathFree(GetX(), GetY(), targetx, targety))
  {
    // Wir versuchen mal, abzuspringen (das klappt sp�testens beim 2. Mal(theoretisch))
    
    
    //wenn Ziel weiter oben: immer nach oben abspringen
    if(GetY() > targety)
      SetComDir(COMD_Up);
    //Ziel weiter unten? Nach unten abspringen.
    else if(Abs(GetY()-targety) > 50)
      SetComDir(COMD_Down);
    //Ziel weiter weg? Backflip! :D
    else if(Abs(GetX()-targetx) > 80)
      SetComDir(COMD_Stop);
    
    var comd = GetComDir();
    if(targetx < GetX())
    {
    	ControlLadder("ControlLeft");
    	if(GetDir() == DIR_Left)
    	  SetComDir(comd);
   	}
    if(targetx > GetX())
    {
    	ControlLadder("ControlRight");
    	if(GetDir() == DIR_Right)
    	  SetComDir(comd);
   	}
  }
  // Weiter checken
  ScheduleCall(this, "ClimbLadder", 10);
}

private func ControlLadder(string strControl, par1) // par1 is comdir for ControlUpdate
{

  // Wenn man schnell genug ist, kann man sich so sogar wieder an der Leiter anhalten!
  if(lastladder)
    lastladder = 0;
    
  // Nicht an einem kletterbaren Objekt.
  if (GetAction() != "ScaleLadder") return 0; 

  // Klettert an der Leiter
  if(strControl == "ControlCommand") {
    ClimbLadder();
    return 1;
  }

  //Log("%s: %s",GetName(),strControl); //DEBUGZ!

  // JnR
  if (strControl == "ControlUpdate")
  {
    var comdir = par1;
    // Strip away COMD_Left/COMD_Right, this is handled via ControlLeft/ControlRight
    if(ComDirLike(comdir, COMD_Left)) comdir = ComDirTransform(comdir, COMD_Right);
    if(ComDirLike(comdir, COMD_Right)) comdir = ComDirTransform(comdir, COMD_Left);
    SetComDir(comdir);
  }
  
  // Rauf
  if (strControl == "ControlUp" && !GetPlrCoreJumpAndRunControl(GetController()))
    SetComDir(COMD_Up);
  // Runter
  if (strControl == "ControlDown" && !GetPlrCoreJumpAndRunControl(GetController()))
    SetComDir(COMD_Down);
  // Rechts
  if (strControl == "ControlRight")
  {
    // Richtung wechseln
    if (GetDir() == DIR_Right) {
      SetDir(DIR_Left);
      SetComDir(COMD_Stop);
      if (!AdjustLadderOffset(GetActionTarget())) //k�nnen wir �berhaupt?
        SetDir(DIR_Right);
    }
    // Abspringen
    else {
      if(GetComDir() == COMD_Up) ReleaseLadderUp();
      else if(GetComDir() == COMD_Stop) ReleaseLadderStop();
      else ReleaseLadderDown();
    }
  }
  // Links
  if (strControl == "ControlLeft")
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
  
  // Werfen
  if (strControl == "ControlThrow") LadderThrow();
  
  //Graben
  if (strControl == "ControlDig") LadderDig();
  
  return 1;  
}  

// L�sst den Clonk an der Leiter kleben
private func AdjustLadderOffset(object pLadder)
{
  //keine Leiter �bergeben? An wtf soll dann adjustiert werden..?
  if(!pLadder)
    return 0;
  // Abstand nach Kletterrichtung
  var iLastX = GetX();
  var iChangeX;
  if(!(iChangeX = pLadder->~LocalN("SizeX",pLadder)/2))
    iChangeX = GetDefCoreVal("Width",0,GetID(pLadder))/4;
    
  if(GetDir() == DIR_Left)
    SetPosition(GetX(pLadder) + iChangeX, GetY());
  else
    SetPosition(GetX(pLadder) - iChangeX, GetY());  
  // Nicht in festes Material klettern
  if(Stuck()) 
  {
    SetPosition(iLastX, GetY());
    return 0;
  }   
  // Ausrichtung erfolgreich
  return 1;
}


//protected func FxLadderReleasedTimer() { Log("blub"); return(-1); }

protected func FxScalingLadderStart(object pTarget, int iEffectNumber, int iTemp) {
  if(iTemp)
    return ;
  ClimbLadder(true);
  EffectVar(0, pTarget, iEffectNumber) = GetY()*100;
}

//Oke, das hier ist wohl gro�teils aus dem Urwaldclonk *hidez*
protected func FxScalingLadderTimer(object pTarget, int iEffectNumber)
{
  var pLadder;
  // Clonk hat aus irgendeinem Grund seine Kletteraktion verloren
  if (!(GetAction() == "ScaleLadder"))
    // Klettern beenden
    return ReleaseLadder(0);

  // Keine Leiter mehr: klettern beenden
  if (!(pLadder = FindLadder()))
  {
    // Oberes Ende: versuchen, die Wand zu erreichen
    if (GetComDir() == COMD_Up) {
      return ReleaseLadder(-15 + GetDir() * 30,-20);
    }
    // Unteres Ende: einfach fallen lassen
    else
      return ReleaseLadder(0);
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
  if (GetComDir() == COMD_Up) { iPosY -= iStep; iPhase += iStep*14/1000; }
  if (GetComDir() == COMD_Down) { iPosY += iStep; iPhase -= iStep*14/1000; }
  
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
	  	SetComDir(COMD_Stop);
	  }
  	
  	return ;
  }
  
  if (iPhase < 0) iPhase = 15; if (iPhase > 15) iPhase = 0;
  SetPhase(iPhase);
  SetPosition(GetX(), iPosY/100);
  SetXDir(0); SetYDir(0);

  EffectVar(0, pTarget, iEffectNumber) = iPosY;
}

public func JumpStart(bool bBackflip)
{
  _inherited(bBackflip);

  // Spezielle Funktionen w�hrend des Sprungs (An Kletterzeugs festhalten)
  AddEffect("Jumping", this, 1, 2, this);
}

private func IsJumping() {
  if(GetAction() == "Dive")
    return true;
  return WildcardMatch(GetAction(),"Jump*");
}

protected func FxJumpingTimer()
{
  // Springt er?
  if (IsJumping() || WildcardMatch(GetAction(), "*Flip"))
  {
    // Schaun ob ein kletterbares Objekt da ist (Leiter z.b.)
    //var pVine; ...�h, falsch
    var pLadder; //Leiter nat�rlich!
    if (pLadder = FindLadder())
      GrabLadder(pLadder);
  }
  // ke, springt er halt nicht. *Effekt wegtun*
  else 
  {
    return -1;
  } 
}


