/*-- Der Clonk --*/

#strict
#appendto CLNK



/* Initialisierung */

protected func Initialize()
{
  // Clonks mit Magiephysikal aus fehlerhaften Szenarien korrigieren
  if (GetID () == CLNK)
    if (GetPhysical ("Magic", 1))
      SetPhysical ("Magic", 0, 1);
  SetAction("Walk");
  SetDir(Random(2));
  // Broadcast für Spielregeln
  GameCallEx("OnClonkCreation", this());
  return(1);
}

/* Bei Hinzufügen zu der Crew eines Spielers */

protected func Recruitment(int iPlr) {
  // Alchemieregel: Jeder Clonk kriegt einen angelegten Beutel spendiert
  if(ObjectCount(ALCO)) CreateObject(ALC_,0,0,-1)->~BelongTo(this());
  // Broadcast für Crew
  GameCallEx("OnClonkRecruitment", this(), iPlr);
}

/* Kontext */

public func HasKnowledge() { return(GetPlrKnowledge(GetOwner(),0,0,C4D_Structure())); }
public func HasBase()      { return(FindBase(GetOwner()) && GetBase(Contained()) != GetOwner()); }
public func ReleaseAllowed() { return(ObjectCount(REAC)); }

/* Steuerung */

protected func ControlLeft()
{
  // Steuerung an Pferd weiterleiten
  if (IsRiding()) return(GetActionTarget()->~ControlLeft(this()));
  // Keine überladene Steuerung
  return(0);
}

protected func ControlLeftReleased()
{
  // Steuerung an Pferd weiterleiten
  if (IsRiding()) return(GetActionTarget()->~ControlLeftReleased(this()));
  // Keine überladene Steuerung
  return(0);
}

protected func ControlRight()
{
  // Steuerung an Pferd weiterleiten
  if (IsRiding())  return(GetActionTarget()->~ControlRight(this()));
  // Keine überladene Steuerung
  return(0);
}

protected func ControlRightReleased()
{
  // Steuerung an Pferd weiterleiten
  if (IsRiding()) return(GetActionTarget()->~ControlRightReleased(this()));
  // Keine überladene Steuerung
  return(0);
}

protected func ControlUp()
{
  // Steuerung an Pferd weiterleiten
  if (IsRiding())  return(GetActionTarget()->~ControlUp(this()));
  // Keine überladene Steuerung
  return(0);
}

protected func ControlUpReleased()
{
  // Steuerung an Pferd weiterleiten
  if (IsRiding()) return(GetActionTarget()->~ControlUpReleased(this()));
  // Keine überladene Steuerung
  return(0);
}

protected func ControlUpDouble() 
{
  // nur wenn an Meeresoberfläche
  if(!InLiquid()) return(0);
  if(GBackSemiSolid(0,-1)) return(0);
  // herausspringen
  SetPosition(GetX(),GetY()-1);
  SetAction("Jump");
  SetSpeed(GetXDir(),-BoundBy(GetPhysical("Swim")/2500,24,38));
  var iX=GetX(),iY=GetY(),iXDir=GetXDir(),iYDir=GetYDir();
  // Wenn Sprung im Wasser endet und das Wasser tief genug ist, Kopfsprung machen
  if(SimFlight(iX,iY,iXDir,iYDir,25,50))
    if(GBackLiquid(iX-GetX(),iY-GetY()) && GBackLiquid(iX-GetX(),iY+9-GetY()))
      SetAction("Dive");
}

protected func ControlDown()
{
  // Steuerung an Pferd weiterleiten
  if (IsRiding())  return(GetActionTarget()->~ControlDown(this()));
  // Keine überladene Steuerung
  return(0);
}

protected func ControlDownReleased()
{
  // Steuerung an Pferd weiterleiten
  if (IsRiding()) return(GetActionTarget()->~ControlDownReleased(this()));
  // Keine überladene Steuerung
  return(0);
}

protected func ControlDownSingle()
{
  // Steuerung an Pferd weiterleiten
  if (IsRiding())  return(GetActionTarget()->~ControlDownSingle(this()));
  // Keine überladene Steuerung
  return(0);
}

protected func ControlDownDouble()
{
  // Steuerung an Pferd weiterleiten
  if (IsRiding())  return(GetActionTarget()->~ControlDownDouble(this()));
  // Keine überladene Steuerung
  return(0);
}

protected func ControlDig()
{
  // Steuerung an Pferd weiterleiten
  if (IsRiding())  return(GetActionTarget()->~ControlDig(this()));
  // Keine überladene Steuerung
  return(0);
}

protected func ControlDigReleased()
{
  // Steuerung an Pferd weiterleiten
  if (IsRiding()) return(GetActionTarget()->~ControlDigReleased(this()));
  // Keine überladene Steuerung
  return(0);
}

protected func ControlDigSingle()
{
  // Steuerung an Pferd weiterleiten
  if (IsRiding())  return(GetActionTarget()->~ControlDigSingle(this()));
  // Keine überladene Steuerung
  return(0);
}

protected func ControlDigDouble()
{
  // Steuerung an Pferd weiterleiten
  if (IsRiding())  return(GetActionTarget()->~ControlDigDouble(this()));
  // Keine überladene Steuerung
  return(0);
}

protected func ControlThrow()
{
  // Reiten und Werfen
  if (IsRiding())
    if (Contents(0))
    {
      SetAction("RideThrow");
      return(1);
    }
  // Keine überladene Steuerung
  return(0);
}

protected func ControlUpdate(object self, int comdir, bool dig, bool throw)
{
  // Steuerung an Pferd weiterleiten
  if(IsRiding()) return(GetActionTarget()->~ControlUpdate(self, comdir, dig, throw));
  // Keine überladene Steuerung
  return(0);
}

protected func ControlCommand(szCommand, pTarget, iTx, iTy, pTarget2, Data)
{
  // Kommando MoveTo an Pferd weiterleiten
  if (szCommand eq "MoveTo")
    if (IsRiding())
      return(GetActionTarget()->~ControlCommand(szCommand, pTarget, iTx, iTy));
  // Anderes Kommando beim Reiten: absteigen (Ausnahme: Context)
  if (IsRiding() && szCommand ne "Context")
  {
    SetComDir(COMD_Stop(),GetActionTarget());
    GetActionTarget()->~ControlDownDouble(this());
  }
  // RejectConstruction Callback beim Bauen durch Drag'n'Drop aus einem Gebaeude-Menu
  if(szCommand S= "Construct")
  {
    // Data ist eigentlich keine ID, sondern ein C4Value* - Damit ein DirectCall
    // möglich ist, muss sie aber zu einer C4ID gecastet werden.
    if(CastC4ID(Data)->~RejectConstruction(iTx - GetX(), iTy - GetY(), this()) )
    {
      return(1);
    }
  }
  // Kein überladenes Kommando
  return(0);
}

/* Verwandlung */

private func RedefinePhysical(szPhys, idTo)
{
  // Physical-Werte ermitteln
  var physDefFrom = GetPhysical(szPhys, 0, 0, GetID()),
      physDefTo   = GetPhysical(szPhys, 0, 0, idTo),
      physCurr    = GetPhysical(szPhys);
  // Neuen Wert berechnen
  var physNew; if (physDefTo) physNew=BoundBy(physDefTo-physDefFrom+physCurr, 0, 100000);
  // Neuen Wert für den Reset immer temporär setzen, selbst wenn keine Änderung besteht, damit der Reset richtig funktioniert
  SetPhysical(szPhys, physNew, PHYS_StackTemporary);
  // Fertig
  return(1);
}

protected func FxIntRedefineStart(object trg, int num, int tmp, id idTo)
  {
  // Ziel-ID in Effektvariable
  if (tmp)
    idTo = EffectVar(0, trg, num);
  else
    {
    EffectVar(0, trg, num) = idTo;
    EffectVar(1, trg, num) = GetID();
    }
  // Physicals anpassen
  RedefinePhysical("Energy", idTo);
  RedefinePhysical("Breath", idTo);
  RedefinePhysical("Walk", idTo);
  RedefinePhysical("Jump", idTo);
  RedefinePhysical("Scale", idTo);
  RedefinePhysical("Hangle", idTo);
  RedefinePhysical("Dig", idTo);
  RedefinePhysical("Swim", idTo);
  RedefinePhysical("Throw", idTo);
  RedefinePhysical("Push", idTo);
  RedefinePhysical("Fight", idTo);
  RedefinePhysical("Magic", idTo);
  RedefinePhysical("Float", idTo);
  /*if (GetRank()<4) RedefinePhysical("CanScale", idTo);
  if (GetRank()<6) RedefinePhysical("CanHangle", idTo);*/ // z.Z. können es alle
  RedefinePhysical("CanDig", idTo);
  RedefinePhysical("CanConstruct", idTo);
  RedefinePhysical("CanChop", idTo);
  RedefinePhysical("CanSwimDig", idTo);
  RedefinePhysical("CorrosionResist", idTo);
  RedefinePhysical("BreatheWater", idTo);
  // Damit Aufwertungen zu nicht-Magiern keine Zauberenergie übrig lassen
  if (GetPhysical("Magic")/1000 < GetMagicEnergy()) DoMagicEnergy(GetPhysical("Magic")/1000-GetMagicEnergy());
  // Echtes Redefine nur bei echten Aufrufen (hat zu viele Nebenwirkungen)
  if (tmp) return(FX_OK);
  Redefine(idTo);
  // Fertig
  return(FX_OK);
  }
  
protected func FxIntRedefineStop(object trg, int num, int iReason, bool tmp)
  {
  // Physicals wiederherstellen
  ResetPhysical(0, "BreatheWater");
  ResetPhysical(0, "CorrosionResist");
  ResetPhysical(0, "CanSwimDig");
  ResetPhysical(0, "CanChop");
  ResetPhysical(0, "CanConstruct");
  ResetPhysical(0, "CanDig");
  ResetPhysical(0, "Float");
  ResetPhysical(0, "Magic");
  ResetPhysical(0, "Fight");
  ResetPhysical(0, "Push");
  ResetPhysical(0, "Throw");
  ResetPhysical(0, "Swim");
  ResetPhysical(0, "Dig");
  ResetPhysical(0, "Hangle");
  ResetPhysical(0, "Scale");
  ResetPhysical(0, "Jump");
  ResetPhysical(0, "Walk");
  ResetPhysical(0, "Breath");
  ResetPhysical(0, "Energy");
  // Keine Rückänderung bei temporären Aufrufen oder beim Tod/Löschen
  if (tmp || iReason) return();
  // Damit Aufwertungen von nicht-Magiern keine Zauberenergie übrig lassen
  if (GetPhysical("Magic")/1000 < GetMagicEnergy()) DoMagicEnergy(GetPhysical("Magic")/1000-GetMagicEnergy());
  // OK; alte Definition wiederherstellen
  Redefine(EffectVar(1, trg, num));
  }

public func Redefine2(idTo)
{
  if (GetID() == idTo) return(true);
  RemoveEffect("IntRedefine", this());
  if (GetID() == idTo) return(true);
  return(!!AddEffect("IntRedefine", this(), 10, 0, this(), 0, idTo));
}

public func Redefine(idTo)
{
  // Aktivitätsdaten sichern
  var phs=GetPhase(),act=GetAction();
  // Umwandeln
  ChangeDef(idTo);
  // Aktivität wiederherstellen
  var chg=SetAction(act);
  if (!chg) SetAction("Walk");
  if (chg) SetPhase(phs);
  // Fertig
  return(1);
}

/* Essen */  

public func Feed(iLevel)
{
  DoEnergy(iLevel);
  Sound("ClonkMunch");
    return(1);
}

/* Aktionen */

private func Riding()
{
  // Richtung an die des Pferdes anpassen
  SetDir(GetDir(GetActionTarget()));
  // Pferd steht still: Clonk soll auch still sitzen
  if (GetActionTarget()->~IsStill())
  {
    if (GetAction() ne "RideStill")
      SetAction("RideStill");
  }
  // Pferd steht nicht still: Clonk soll auch nicht still sitzen
  else
    if (GetAction() ne "Ride")
      SetAction("Ride");
  return(1);
}

private func Throwing()
{
  // Erstes Inhaltsobjekt werfen
  var pObj = Contents(0);
  // Wurfparameter berechnen
  var iX, iY, iR, iXDir, iYDir, iRDir;
  iX = 0; if (!GetDir()) iX = -iX;
  iY = -10;
  iR = Random(360);
  iXDir = GetPhysical("Throw") / 25000; if(!GetDir()) iXDir = -iXDir;
  iYDir = -GetPhysical("Throw") / 25000;
  iRDir = Random(40) - 20;
  // Reitet? Eigengeschwindigkeit addieren
  if (GetActionTarget())
  {
    iXDir += GetXDir(GetActionTarget()) / 10;
    iYDir += GetYDir(GetActionTarget()) / 10;
  }
  // Werfen!
  Exit(pObj, iX, iY, iR, iXDir, iYDir, iRDir);  
  // Fertig
  return(1);  
}

private func Fighting()
{
  if (!Random(2)) SetAction("Punch");
  return(1);
}

private func Punching()
{
  if (!Random(3)) Sound("Kime*");
  if (!Random(5)) Sound("Punch*");
  if (!Random(2)) return(1);
  Punch(GetActionTarget());
  return(1);
}
  
private func Chopping()
{
  Sound("Chop*");
  CastParticles("Dust",Random(3)+1,6,-8+16*GetDir(),1,10,12);
  return(1);
}
  
private func Building()
{
  if (!Random(2)) Sound("Build*");
  return(1);
}

private func Processing()
{
  Sound("Build1");
  return(1);
}

private func Digging()
{
  Sound("Dig*");
  return(1);
}

protected func Scaling()
{
  var szDesiredAction;
  if (GetYDir()>0) szDesiredAction = "ScaleDown"; else szDesiredAction = "Scale";
  if (GetAction() ne szDesiredAction) SetAction(szDesiredAction);
  return(1);   
}

/* Ereignisse */
  
protected func CatchBlow()
{
  if (GetAction() eq "Dead") return(0);
  if (!Random(5)) Hurt();
  return(1);
}
  
protected func Hurt()
{
  Sound("Hurt*");
  return(1);
}
  
protected func Grab()
{
  Sound("Grab");
  return(1);
}

protected func Get()
{
  Sound("Grab");
  return(1);
}

protected func Put()
{
  Sound("Grab");
  return(1);
}

protected func Death(int iKilledBy)
{
  // Info-Broadcasts für sterbende Clonks
  GameCallEx("OnClonkDeath", this(), iKilledBy);
  
  // Der Broadcast könnte seltsame Dinge gemacht haben: Clonk ist noch tot?
  if (GetAlive()) return();
  
  // den Beutel fallenlassen
  if(GetAlchemBag()) GetAlchemBag()->~Loose();

  Sound("Die");
  DeathAnnounce();
  // Letztes Mannschaftsmitglied tot: Script benachrichtigen
  if (!GetCrew(GetOwner()))
    GameCallEx("RelaunchPlayer",GetOwner());
  return(1);
}

protected func Destruction()
{
  // Clonk war noch nicht tot: Jetzt ist er es
  if (GetAlive())
    GameCallEx("OnClonkDeath", this(), GetKiller());
  // Dies ist das letztes Mannschaftsmitglied: Script benachrichtigen
  if (GetCrew(GetOwner()) == this())
    if (GetCrewCount(GetOwner()) == 1)
      //Nur wenn der Spieler noch lebt und nicht gerade eleminiert wird
      if (GetPlayerName(GetOwner()))
        {
        GameCallEx("RelaunchPlayer",GetOwner());
        }
  return(1);
}

protected func DeepBreath()
{
  Sound("Breath");
  return(1); 
}
  
protected func CheckStuck()
{                   
  // Verhindert Festhängen am Mittelvertex
  if(!GetXDir()) if(Abs(GetYDir()) < 5)
    if(GBackSolid(0, 3))
      SetPosition(GetX(), GetY() + 1);
}

/* Status */

public func IsRiding()
{
  // Reitet der Clonk?
  return (WildcardMatch(GetAction(), "Ride*"));
}

public func IsClonk() { return(1); }

/* Kontext */

public func ContextRelease(pCaller) 
{
  [$CtxRelease$|Image=CXRL|Condition=ReleaseAllowed]
  FindObject(REAC)->Activate(GetOwner());
  return(1);
}

public func ContextConstruction(pCaller)
{
  [$CtxConstructionDesc$|Image=CXCN|Condition=HasKnowledge]
  SetCommand(this(),"Construct");
  return(1);
}

public func ContextHome(pCaller)
{
  [$CtxHomeDesc$|Image=CXHM|Condition=HasBase]
  SetCommand(this(),"Home");
  return(1);
}

/* Steuerung */

protected func ControlSpecial2()
{
  [$CtrlMenuDesc$]
  AddCommand(this(),"Context",0,0,0,this());
  return(1);
}

/* Callback beim Auswahl aus dem Construct-Kontextmenu */

public func ControlCommandConstruction(target, x, y, target2, def)
{
  // Keine Konstruktion erlaubt?
  if(def->~RejectConstruction(x - GetX(), y - GetY(), this()) )
    // Construct-Kommando beenden
    return(FinishCommand(this(), false, 0) );
}

/* Automatische Produktion */

public func ControlCommandAcquire(target, x, y, target2, def)
{
  //Falls das Teil rumliegt nur aufsammeln
  var obj = GetAvailableObject (def, target2);
  if (obj) return ();
  //Gebäude suchen worin man's herstellen kann  
  if (obj = GetProducerOf (def)) {
    AddCommand (this (), "Call", this (), 0, 0, 0, 0, "AutoProduction", 0, 1);
    obj -> HowToProduce (this(), def);
    return (1);
  }
}

public func AutoProduction() { return (1); }

public func AutoProductionFailed() 
{
  var def = GetCommand (this (), 5, 1);
  if (!FindContents(def)) {
    var obj = GetAvailableObject (def, GetCommand (this (), 4, 1));
    if (obj) return (AddCommand (this(), "Get", obj,0,0,0,40));
    AddCommand (this(), "Buy", 0, 0, 0, 0, 100, GetCommand(this(), 5, 1), 0, 3);
  }
  return (1);
}

public func GetProducerOf(def)
{
  // Nur Objekte mit typischen OCFs befragen damit's schneller geht.
  var obj, prod, ocf = OCF_Container () | OCF_Exclusive () | OCF_Entrance () | OCF_LineConstruct () | OCF_PowerConsumer ();
  while (obj = FindObject (0, -500, -250, 1000, 500, ocf,0,0,0, obj))
    if (obj -> ~IsProducerOf (this (), def))
      if (!prod || ObjectDistance (prod) > ObjectDistance (obj))
        prod = obj;
  return (prod);
}

/* Trinken */

public func Drink(object pDrink)
{
  // Trinkaktion setzen, wenn vorhanden
  if (GetActMapVal("Name", "Drink"))
    SetAction("Drink");
  // Vorsicht: erstmal nichts mit pDrink machen,
  // die Potions löschen sich meist selber...
}

/* Fahrstuhlsteuerung */

public func ContextPermanentModeTurnOn(pCaller) 
{
  [$TxtPermanentModeTurnOn$|Image=ECS1:5|Condition=GrabbingNonPermanentElevator]
  if (GetAction() eq "Push")
    if (GetActionTarget()->~IsElevator())
      GetActionTarget()->DoControlAuto(this());
}
 
public func ContextPermanentModeTurnOff(pObj) 
{
  [$TxtPermanentModeTurnOff$|Image=ECS1:5|Condition=GrabbingPermanentElevator]
  if (GetAction() eq "Push")
    if (GetActionTarget()->~IsElevator())
      GetActionTarget()->DoControlAuto(this());
}

private func GrabbingNonPermanentElevator()
{
  if (GetAction() eq "Push")
    if (GetActionTarget()->~IsElevator())
      if (GetActionTarget()->IsNotInPermanentMode())
        return(1);
  return(0);        
}

private func GrabbingPermanentElevator()
{
  if (GetAction() eq "Push")
    if (GetActionTarget()->~IsElevator())
      if (GetActionTarget()->IsInPermanentMode())
        return(1);
  return(0);        
}
