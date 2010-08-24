/*-- Seilrolle --*/

#strict 2

local pRope, pConnected;
local iPull, fNoExtend;

public func Activate(pByObj)
{
  // kein Menü bei Aktion in angefasstem Objekt
  if(GetAction(pByObj) == "Push" && GetActionTarget(0, pByObj)->~ControlDigDouble())
    return 0;
  if(pByObj->Contained()) return 0;
  // Clonk stoppen und Seilrollenmenü erzeugen
  pByObj->SetComDir(COMD_Stop);
  CreateMenu(GetID(this), pByObj);
  // Erster Menüeintrag je nach Abnehm-Möglichkeit:
  // Entweder "Enterhaken freirütteln", wenn Seilrolle zum Enterhaken gehört...
  if(GetID(pConnected) == CW5P && GetActionTarget(0, LocalN("pRope", pConnected)) == this)
    AddMenuItem("$TxtPickUpHook$", "PickUp", CX5P, pByObj, 0, pByObj);
    else
    {
      // ..."Seil abnehmen", wenn in Nähe des angeknoteten Objekts...
      if(PickUpPossible(pByObj)) AddMenuItem("$TxtPickUp$", "PickUp", CM5P, pByObj, 0, pByObj);
        // ... sonst "Abnehmen nicht möglich"
        else AddMenuItem("$TxtPickUp$", "PickUpImpossible", CQ5P, pByObj);
    }
  // Ziehen-Halten-Geben-Menü
  AddMenuItem("$TxtPull$", "SetPull(-1, 1)", CN5P, pByObj);
  AddMenuItem("$TxtHold$", "SetPull(0, 1)", CO5P, pByObj);
  AddMenuItem("$TxtExtend$", "SetPull(1, 0)", CP5P, pByObj);
  // 5. Eintrag: Mit sich selbst verbinden
  AddMenuItem(Format("$TxtConnect$", GetName(pByObj)), "EndConnect", 0, pByObj, 0, pByObj, 0, 4, pByObj);
  // Mittlere Auswahl zur schnelleren Bedienung
  SelectMenuItem(2, pByObj);
  // Anknüpfbare Objekte auflisten, zu erst aus dem Inventar...
  for(var pObj in FindObjects(Find_Container(pByObj), Find_OCF(LocalN("iOCF", pRope)), Find_Exclude(), Find_Not(Find_ID(CK5P)), Find_Not(Find_ID(CL5P))))
    AddMenuItem(Format("$TxtConnect$", GetName(pObj)), "EndConnect", 0, pByObj, 0, pObj, 0, 4, pObj);
  // ...dann alle freien Objekte in der Umgebung
  var fEnemy;
  for(var pObj in FindObjects(Find_Or(Find_AtPoint(), Find_InRect(-10, -15, 20, 30)), Find_OCF(LocalN("iOCF", pRope)), Find_NoContainer(), Find_Exclude(), Find_Exclude(pByObj), Find_Not(Find_ID(CK5P)), Find_Not(Find_ID(CL5P)), Find_Not(Find_ID(CY5P))))
  {
    if(GetProcedure(pByObj) == "FIGHT" && pObj == GetActionTarget(0, pByObj))
      fEnemy = 1;
    if(GetCategory(pObj) & C4D_Structure && GetCon(pObj) != 100)
      continue;
    AddMenuItem(Format("$TxtConnect$", GetName(pObj)), "EndConnect", 0, pByObj, 0, pObj, 0, 4, pObj);
  }
  // Auch Zweikampfgegner aufführen
  if(GetProcedure(pByObj) == "FIGHT" && GetActionTarget(0, pByObj) && !fEnemy)
    AddMenuItem(Format("$TxtConnect$", GetName(GetActionTarget(0, pByObj))), "EndConnect", 0, pByObj, 0, GetActionTarget(0, pByObj), 0, 4, GetActionTarget(0, pByObj));
  // Falls Gefangennahme-Regel aktiv, können noch Clonks gefesselt werden
  if(!FindObject(CR5P)) return 1;
  fEnemy = 0;
  for(var pObj in FindObjects(Find_Or(Find_AtPoint(), Find_InRect(-10, -15, 20, 30)), Find_OCF(OCF_CrewMember), Find_NoContainer(), Find_Exclude(pByObj)))
  {
    if(GetProcedure(pByObj) == "FIGHT" && pObj == GetActionTarget(0, pByObj))
      fEnemy = 1;
    // Schlaufen-Grafik drüberlegen...
    SetGraphics(0, pObj, CS5P, GFX_Overlay, GFXOV_MODE_Picture);
    AddMenuItem(Format("$TxtTieUp$", GetName(pObj)), "TieUp", 0, pByObj, 0, pObj, 0, 4, pObj);
    // ...und wieder entfernen, damit es nur im Menü erscheint
    SetGraphics(0, pObj, 0, GFX_Overlay);
  }
  if(GetProcedure(pByObj) == "FIGHT" && GetActionTarget(0, pByObj) && !fEnemy)
  {
    SetGraphics(0, GetActionTarget(0, pByObj), CS5P, GFX_Overlay, GFXOV_MODE_Picture);
    AddMenuItem(Format("$TxtTieUp$", GetName(GetActionTarget(0, pByObj))), "TieUp", 0, pByObj, 0, GetActionTarget(0, pByObj), 0, 4, GetActionTarget(0, pByObj));
    SetGraphics(0, GetActionTarget(0, pByObj), 0, GFX_Overlay);
  }
  return 1;
}

private func PickUpPossible(pForObj)
{
  if(Contained() != pForObj) return 0;
  for(var pObj in FindObjects(Find_Or(Find_AtPoint(), Find_InRect(-10, -15, 20, 30)), Find_Not(Find_And(Find_AnyContainer(), Find_Not(Find_Container(Contained()))))))
    if(pObj == pConnected) return 1;
  if(GetProcedure(pForObj) == "FIGHT" && GetActionTarget(0, pForObj) == pConnected)
    return 1;
  return 0;
}

private func EndConnect(idTo, pTo)
{
  // Nochmal alle möglichen Objekte durchgehen
  for(var pObj in FindObjects(Find_Or(Find_AtPoint(), Find_InRect(-10, -15, 20, 30)), Find_Not(Find_And(Find_AnyContainer(), Find_Not(Find_Container(Contained(this)))))))
    if(pObj == pTo) var fAtPos = 1;
  // Abbrechen, wenn Objekt nicht mehr da
  if(!fAtPos && !(GetProcedure(Contained()) == "FIGHT" && pTo == GetActionTarget(0, Contained())))
  {
    Sound("Error", 1, 0, 100, GetOwner(Contained()) + 1);
    return 0;
  }

  // Nur ein Seil pro Winch
  if(GetID(pTo) == CT5P)
    for(var pRope1 in FindObjects(Find_ID(CK5P), Find_Action("Connect")))
    {
      if(GetActionTarget(0, pRope1) == pTo || GetActionTarget(1, pRope1) == pTo)
      {
        Contained()->Sound("Error");
        Message("$TxtWinch$", Contained());
        return 0;
      }
    }

  // Falls des andere Seilende einen Clonk fesselt: Benachrichtigung
  if(GetActionTarget(0, pRope) == this)
  {
    for(var i = 1; i < GetEffectCount(0, GetActionTarget(1, pRope)) + 1; i++)
      if(EffectVar(0, GetActionTarget(1, pRope), i) == pRope)
        GameCallEx("OnEndTiedMember", GetActionTarget(1, pRope), Contained(), 1);
  }
  else if(GetActionTarget(1, pRope) == this)
  {
    for(var i = 1; i < GetEffectCount(0, GetActionTarget(0, pRope)) + 1; i++)
      if(EffectVar(0, GetActionTarget(0, pRope), i) == pRope)
        GameCallEx("OnEndTiedMember", GetActionTarget(0, pRope), Contained(), 1);
  }

  // Seillänge anpassen und anknüpfen
  Sound("ArrowHit");
  var iDistance = Distance(GetX(), GetY(), GetX(pTo) + GetVertex(pRope->GetMiddlestVertex(pTo), 0, pTo), GetY(pTo) + GetVertex(pRope->GetMiddlestVertex(pTo), 1, pTo));
  pRope->SetRopeLength(pRope->GetRopeLength() + iDistance);
  if(GetActionTarget(1, pRope) == this) pRope->SetObject(pTo, 1);
  else pRope->SetObject(pTo, 0);
  pConnected = pTo;

  RemoveObject();
  return 1;
}

private func TieUp(idTo, pTo)
{
  var pContained = Contained();
  if(EndConnect(idTo, pTo))
  {
    // Wenn 1, ist die Endfessel die erste Fesselung
    var fTied = GetEffect("IntTied", pTo);

    // Fessel-Effekt
    EffectVar(0, pTo, AddEffect("IntTied", pTo, 25)) = pRope;
    if(!FindObject2(Find_ID(CY5P), Find_ActionTarget(pTo)))
      ObjectSetAction(CreateObject(CY5P), "Tied", pTo);

    // Fessler für GetCapturer() speichern
    LocalN("pCapturer", pTo) = pContained;

    // Auch hier eine Weiterleitung, wenn das andere Ende an ein Objekt geknotet ist...
    if((GetActionTarget(0, pRope) == pTo && GetID(GetActionTarget(1, pRope)) != GetID())
    || (GetActionTarget(1, pRope) == pTo && GetID(GetActionTarget(0, pRope)) != GetID()))
      GameCallEx("OnEndTiedMember", pTo, pContained, fTied);

    // ...ansonsten nur Start-Fessel-Info weitergeben
    else GameCallEx("OnStartTiedMember", pTo, pContained, GetEffect("IntTied", pTo));

    // Physicals anpassen
    SetTiedPhysicals(pTo);
    return 1;
  }
}

private func PickUp(fPossible)
{
  // Enterhaken: "Freirütteln" und ziehen (nur beim Enterhakenseil)
  if(GetID(pConnected) == CW5P
  && LocalN("pRope", pConnected) == pRope)
  {
    if(GetAction(pConnected) != "PickUp")
    {
      pConnected->SetAction("Idle");
      pConnected->SetAction("PickUp");
    }
    SetPull(-1, 1);
    return 1;
  }
  // Nur abnehmen, wenn sich Objekt noch in der Nähe befindet
  if(!fPossible || !PickUpPossible(Contained()))
    return PickUpImpossible(0, Contained());
  // eventuelle Fessel-Effekte löschen
  for(var i = 0; i < GetEffectCount("IntTied", GetActionTarget(0, pRope)); i++)
    if(EffectVar(0, GetActionTarget(0, pRope), GetEffect("IntTied", GetActionTarget(0, pRope), i)) == pRope)
      ResetTiedClonk(GetActionTarget(0, pRope), i);
  for(i = 0; i < GetEffectCount("IntTied", GetActionTarget(1, pRope)); i++)
    if(EffectVar(0, GetActionTarget(1, pRope), GetEffect("IntTied", GetActionTarget(1, pRope), i)) == pRope)
      ResetTiedClonk(GetActionTarget(1, pRope), i);
  // Seilenden für spätere Löschung speichern
  if(GetID(GetActionTarget(0, pRope)) == GetID())
    var pPulley1 = GetActionTarget(0, pRope);
  if(GetID(GetActionTarget(1, pRope)) == GetID())
    var pPulley2 = GetActionTarget(1, pRope);
  // Seilobjekt zurücksetzen und von Hilfsobjekt zu Item umwandeln
  pRope->SetAction("Idle");
  pRope->SetActionTargets();
  LocalN("aPointsX", pRope) = [0,0];
  LocalN("aPointsY", pRope) = [0,0];
  pRope->SetObjDrawTransform(1000, 0, 0, 0, 1000, 0);
  pRope->SetCategory(C4D_Object);
  pRope->Enter(Contained(this));
  Sound("ArrowHit");
  // Seilenden entfernen
  if(pPulley1) pPulley1->RemoveObject();
  if(pPulley2) pPulley2->RemoveObject();  
  return 1;
}

private func PickUpImpossible(idByObj, pByObj)
{
  Sound("Error", 0, this, 100, GetOwner(pByObj) + 1);
  PlayerMessage(GetOwner(pByObj), "$TxtPickUpImpossible$", this);
}

private func SetPull(iP, fE) { iPull = iP; fNoExtend = fE; }
public func SetRope(pRp, pCc) { pRope = pRp; pConnected = pCc; }
func Departure() { iPull = 10; }
func Entrance()
{
  if(iPull != 7) iPull = 0; // 7 nur bei Entrance-Call nach Erstellung bei Enterhakenwurf
  // Zwei passende Seilenden bzw. Enterhaken werden zum Seil zusammengefasst
  for(var pObj in FindObjects(Find_Container(Contained()), Find_ID(GetID()), Find_Exclude()))
    if(GetActionTarget(0, pRope) == pObj || GetActionTarget(1, pRope) == pObj)
      PickUp(1);
  for(var pObj in FindObjects(Find_Container(Contained()), Find_ID(CW5P)))
    if(GetActionTarget(0, pRope) == pObj || GetActionTarget(1, pRope) == pObj)
      if(LocalN("pRope", pObj) == pRope)
      {
        RemoveObject();
        Sound("ArrowHit");
        pObj->SetAction("Idle");
      }
}

/*-- Steuerung --*/

func ControlLeft(pClonk)
{
  if(GetAction(pClonk) == "Tumble" || GetAction(pClonk) == "Dive")
    ObjectSetAction(pClonk, "Jump");
  if(!GetEffect("NoSwing", this))
  {
    SetXDir(GetXDir(pClonk) - 5, pClonk);
    AddEffect("NoSwing", this, 1, 30, this);
  }
  if(GetProcedure(pClonk) == "FLIGHT") iPull = 0;
  else if(!fNoExtend && pRope->GetRopeLength() < 25) iPull = 1;
    else if(!fNoExtend) iPull = 5;
}

func ControlRight(pClonk)
{
  if(GetAction(pClonk) == "Tumble" || GetAction(pClonk) == "Dive")
    ObjectSetAction(pClonk, "Jump");
  if(!GetEffect("NoSwing", this))
  {
    SetXDir(GetXDir(pClonk) + 5, pClonk);
    AddEffect("NoSwing", this, 1, 30, this);
  }
  if(GetProcedure(pClonk) == "FLIGHT") iPull = 0;
  else if(!fNoExtend && pRope->GetRopeLength() < 25) iPull = 1;
    else if(!fNoExtend) iPull = 5;
}

func ControlUp(pClonk)
{
  if(GetProcedure(pClonk) == "FLIGHT") iPull = -1;
  else
    if(!fNoExtend && GetProcedure(pClonk) == "WALK")
    {
      if(pRope->GetRopeLength() < 25) iPull = 1;
      else iPull = 5;
    }
    else if(!fNoExtend) iPull = 1;
}

func ControlDown(pClonk) { if(!fNoExtend || GetProcedure(pClonk) == "FLIGHT") iPull = 2; }

// Auch im Klettern usw. benutzbar
func ControlDigDouble(pByObj)
{
  // kein Menü bei angefasstem Objekt
  if(GetAction(pByObj) == "Push") return 0;
  return Activate(pByObj);
}


func FxNoSwingTimer() { RemoveEffect("NoSwing", this); }

public func RopeBreak() { return RemoveObject(); }

func RopeAskChangeLength(iLength, pObj)
{
  if(pObj != pRope) return 0;
  if(Contained(Contained())) return Activate(Contained());
  // Ohne Kontrolle dem Wunsch des Seils nachgeben
  if(!Contained()) return iLength + 1;
  // Bei zu starkem Zug kann der CLonk das Seil nicht mehr festhalten
  if(iPull <= 0) if(iLength >= 15) if(ObjectCall(pObj, "GetRopeLength") < 1000) return 1;
  // Nicht zu viel Seil lassen
  if(iPull >= 0) if(iLength <= -10) return -1;
  // Am Seil ziehen
  if(iPull < 0) if(iLength <= 3) return iPull;
  if(iPull < 0) if(iLength <= 6) return -1;
  // Seil geben, nicht zuviel
  if(iPull > 0) if(iLength >= 0) if(ObjectCall(pObj, "GetRopeLength") < 1000) return iPull;
  return 0;
}

// Wird bei versuchter Aufnahme durch Clonk aufgerufen
// Ermöglicht unabhängig des Inventarlimits das zusammenfügen von zwei passenden Seilenden
// zu einem Seil (oder einem Enterhaken, wenn eins der Enden ein Haken ist)
public func JoinPack(pClonk)
{
  for(var pObj in FindObjects(Find_Container(pClonk), Find_ID(GetID()), Find_Exclude()))
    if(GetActionTarget(0, pRope) == pObj || GetActionTarget(1, pRope) == pObj)
      return Enter(pClonk);
  for(var pObj in FindObjects(Find_Container(pClonk), Find_ID(CW5P)))
    if(GetActionTarget(0, pRope) == pObj || GetActionTarget(1, pRope) == pObj)
      if(LocalN("pRope", pObj) == pRope)
        return Enter(pClonk);
  return 0;
}

protected func Hit()
{
  Sound("MetalHit*");
  return 1;
}
