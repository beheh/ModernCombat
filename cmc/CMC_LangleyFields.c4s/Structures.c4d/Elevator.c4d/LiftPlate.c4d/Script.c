/*-- Plattform --*/

#strict 2
#include ELEC


/* TimerCall */

protected func Timer() 
{
  timer = (++timer) % 30;
  // Lore fassen
  //GrabObjects();
  // Clonks umgreifen lassen
  ChangeClonkGrabs();
  // Feststecken -> Schachtbohrung
  if(Stuck() && GetComDir() != COMD_Stop)
    if(HasEnergy())
      DigFreeRect(GetX() - GetObjWidth()/2, GetY() - GetObjHeight()/2, GetObjWidth(), GetObjHeight());
  if(!timer) Activity();
}

protected func Riding() 
{
  var elev = Elevator();
  if(!elev) return Halt();
  // Energieverbrauch
  NoEnergy();
}

private func FindWaitingClonk()
{
  var clnk, best, proc;
  while(clnk = FindObject(0, -GetObjWidth()/2-8, RangeTop - GetY(), GetObjWidth()+16, 6000, OCF_CrewMember, 0, 0, NoContainer(), clnk))
  {
    proc = GetProcedure(clnk);
    if(GetComDir(clnk) == COMD_Stop || (proc == "SWIM" && Inside(GetXDir(clnk), -5, 5)))
    {
      if(proc == "WALK" || proc == "PUSH" || proc == "SCALE" || proc == "HANGLE" || proc == "SWIM")
        if(!Hostile(GetController(clnk), GetOwner()) )
        {
          // Nicht erreichbar?
          if(GetY(clnk) < GetY() - 7) if(!PathFree(GetX(), GetY(), GetX(), GetY(clnk)) ) continue;
          if(GetY(clnk) > GetY() + 7) if(!PathFree(GetX(), GetY() + 16, GetX(), GetY(clnk)) ) continue;
          // Priorität prüfen (Cursor hat Priorität, sonst nächster Clonk)
          if(!best)
            best = clnk;
          else if(GetCursor(GetController(clnk)) == clnk)
            best = clnk;
          else if(GetCursor(GetController(best)) != best)
            if(ObjectDistance(clnk) < ObjectDistance(best))
              best = clnk;
        }
    }
  }
  return best;
}

private func FitsInElevator(pObject)
{
  // Alle (ungedrehten) Vertices des Objekts prüfen
  var vertexCount = GetDefCoreVal("Vertices", "DefCore", pObject->GetID());
  var wdt = GetObjWidth(), hgt = GetObjHeight();
  for (var i = 0; i < vertexCount; i++) 
  {
    var vertexX = GetDefCoreVal("VertexX", "DefCore", pObject->GetID(), i);
    var vertexY = GetDefCoreVal("VertexY", "DefCore", pObject->GetID(), i);
    // Passt nicht
    if (!Inside(vertexX, -wdt/2, wdt/2) || !Inside(vertexY, -hgt/2, hgt/2)) return;
  }
  // Passt
  return true;  
}

private func GrabObjects()
{
  var pObject, did_grab, wdt = GetObjWidth()+6, hgt = GetObjHeight()-6;
  
  // Fahrzeuge
  while(pObject = FindObject(0, -wdt/2, -hgt/2, wdt, hgt, OCF_Grab, 0, 0, NoContainer(), pObject)) 
  {
    // Objekt passt in den Fahrstuhlkorb
    if(FitsInElevator(pObject))
    {
      if(!(GetCategory(pObject) & C4D_Vehicle)) continue;
      if(!Inside(GetXDir(pObject, 100), -1, +1)) continue;
      if(pObject->~IsTree() && GetCon(pObject) >= 50) continue;
      if(GetX(pObject) == GetX() && GetY(pObject) == GetY() + 1) continue;
      if(pObject->~IsElevator()) continue;
      if(GetProcedure(pObject) == "FLOAT") continue;
      did_grab = true;
      
      GrabAdjustPosition(pObject);
    }
  }
  
  return did_grab;
}

private func ChangeClonkGrabs()
{
  var clonk;
  var clonk_ocf = OCF_Living|OCF_NotContained;
  var wdt = GetObjWidth()+6, hgt = GetObjHeight()-6;
  // Schiebende Clonks suchen
  while(clonk = FindObject(0, -wdt/2, -hgt/2, wdt, hgt, clonk_ocf, "Push", 0, 0, clonk)) 
  {
    if(GetComDir(clonk) != COMD_Stop && GetComDir(clonk) != COMD_Up) continue;
    if(!Inside(GetXDir(clonk), -2, +2)) continue;
    if(GetCommand(clonk) && GetCommand(clonk) != "None") continue;
    if(GetMenu(clonk)) continue;
      
    // Geschobenes Objekt prüfen
    var target = GetActionTarget(0, clonk);
    if(!target) continue;
    if(target->~IsElevator())
    {
      if(target == this)
      {
        // In den Fahrstuhlkorb laufen, wenn der Clonk am Rand steht
        if(!Inside(GetX(clonk) - GetX(), -wdt/3, +wdt/3))
        {
          SetCommand(clonk, "Grab", this);
          AddCommand(clonk, "MoveTo", 0, BoundBy(clonk->GetX(), GetX()-5, GetX()+5), GetY());
          AddCommand(clonk, "UnGrab");
        }
      }

      continue;
    }

    if(!Inside(GetX(target), GetX()-wdt/3, GetX()+wdt/3)) continue;
    if(ObjectDistance(this, target) > wdt/2) continue;
    if(!Inside(GetXDir(target), -2, +2)) continue;
    if(!PathFree(GetX(this), GetY(this), GetX(target), GetY(target))) continue;
    if(!FitsInElevator(target)) continue;
      
    // Geschobenes Objekt zentrieren
    GrabAdjustPosition(target);
    // Der Clonk soll nun uns anfassen
    if(!Inside(GetX(clonk) - GetX(), -wdt/6, +wdt/6)) 
    {
      //SetCommand(clonk, "UnGrab"); unnötig, wenn wir unten SetCommand benutzen...
      //AppendCommand(clonk, "MoveTo", 0, GetX()); wtf sollte denn dieses MoveTo-Kommando zur oberen Bildschirmkante...?
      SetCommand(clonk, "Grab", this);

      // Clonk soll in die Mitte laufen, wenn das Target gerade erst
      // reingeschoben wurde, sonst steht der Clonk so blöd am Rand des
      // Fahrstuhls und lässt beim Runterfahren den Korb los.
      if(!Inside(GetX(clonk) - GetX(), -wdt/3, +wdt/3))
      {
        AddCommand(clonk, "MoveTo", 0, BoundBy(clonk->GetX(), GetX()-wdt/6, GetX()+wdt/6), GetY());
        AddCommand(clonk, "UnGrab");
      }
    } 
    else 
    {
      SetActionTargets(this, 0, clonk);
    }
  }
}

private func Drilling()
{
  // Steuermann verloren? Suche schiebenden Clonk selbst wenn er ein Fahrzeug im Fahrstuhlkorb schiebt (->Lore)
  var elev = Elevator();
  if(!elev) return Halt();

  var wdt = GetObjWidth();
  if(!FindObject2(Find_InRect(-wdt/2,-wdt/2,wdt,wdt), Find_Action("Push")))
    return Halt();
  // Energieverbrauch
  NoEnergy();

  return true;
}

private func Waiting()
{
  // Dauerbetrieb: selbständig losfahren
  if (AutoMode == 1)
    SelfMovement();

  return true;
}

private func SelfMovement()
{
  // Selbständig rauf- bzw. runterfahren
  if(AutoMoveDir) SetMoveTo(RangeTop);
  if(!AutoMoveDir) SetMoveTo(LandscapeHeight()-GetObjHeight()/2);
  AutoMoveDir = !AutoMoveDir;
  return true;
}

private func SetMoveTo(iPos)
{
  if(NoEnergy() && AutoMoveDir) return true;
  
  SetAction("Ride"); 
  var iDir = 0;
  
  if(iPos > GetY()) iDir = +3;
  if(iPos < GetY()) iDir = -3;
  SetCommand(this,"MoveTo", 0, GetX(), Max(iPos, RangeTop) + iDir);
  SetYDir(0);
  return true;
}

/* Steuerung */

protected func ControlCommand(strCommand, oTarget, iTx, iTy)
{
  if (!Elevator()) return(0);
  if (Hostile(GetController(oTarget), GetOwner())) return;
  // Bewegungskommando vertikal
  if(strCommand == "MoveTo")
    if(Inside(iTx, GetX()-GetObjWidth()/2-3, GetX()+GetObjWidth()/2+3))
      return SetMoveTo(iTy);
  // Andere Kommandos nicht auswerten
  return;
}

private func DoControlDown(pObj)
{  
  if(!Elevator()) return;
  NoEnergy();

  Sound("Click");
  if(Hostile(GetController(pObj), GetOwner()))
    return;

  SetMoveTo(LandscapeHeight() - GetObjHeight()/2);
}

private func DoControlUp(pObj)
{
  if(!Elevator()) return;
  NoEnergy();

  Sound("Click");
  if(Hostile(GetController(pObj), GetOwner()))
    return;

  SetMoveTo(RangeTop);
}

private func DoControlDig(pObj)
{
  if(!Elevator()) return;
  NoEnergy();

  Sound("Click");
  if(Hostile(GetController(pObj), GetOwner()))
    return;

  SetCommand(this,"None");
  SetAction("Drill");
  SetComDir(COMD_Down);
}

private func FindVehicle(vehicle) 
{
  return FindObject(0, -GetObjWidth()/3, -GetObjHeight()/3, GetObjWidth()*2/3, GetObjHeight()+4, OCF_Grab, 0, 0, NoContainer(), vehicle);
}

private func UpdateRangeBottom()
{
  var x = GetX(), y = GetY() + GetObjHeight()/2;
  while(PathFree(x, y, x, y + 5) && y < LandscapeHeight()) y += 5;
  // set Local(9) for backwards compatibility
  RangeBottom = Local(9) = Max(Local(9), y+5);
  UpdateTransferZone();
}

/* Status */

private func Passenger()
{
  // Clonk finden
  var clnk = FindObject(0, -GetObjWidth()/2,-GetObjHeight()/2,GetObjWidth(),GetObjHeight()-2, OCF_CrewMember,0,0,NoContainer());
  if(!clnk) return;
  // steht wirklich auf dem Fahrstuhl?
  if(GetProcedure(clnk) != "WALK") return;
  return clnk;
}

public func Set(object pObj)
{
	oElev = pObj;
	return true;
}

private func Elevator()
{
  return oElev;
}

public func HasEnergy() { return Elevator()->HasEnergy(); }

private func NoEnergy()
{
  if(!HasEnergy())
  {
    SetPhysical("Float", GetPhysical("Float", 1, 0, GetID())/2, PHYS_Temporary, this);
    return;
  }
  
  SetPhysical("Float", GetPhysical("Float", 1, 0, GetID()), PHYS_Temporary, this);
  //Sound("ElevatorNoEnergy");
  return true;
}

public func IsElevator() { return true; }

/* Transfer */

protected func UpdateTransferZone()
{         
  if(!Elevator()) return SetTransferZone();
  SetTransferZone(-GetObjWidth()/2, GetY(oElev) - GetY(), GetObjWidth(), RangeBottom - GetY(oElev) + GetObjHeight()/2);
  return true;
}

protected func ControlTransfer(pObj, iTx, iTy)  
{
  // Fahrstuhl gehört dem Gegner? Dann nicht.
  if(Hostile(GetController(pObj), GetOwner())) return;

  // Durchgang am Boden des Fahrstuhlschachts: nicht warten
  if(Inside(GetY(pObj) - RangeBottom, -10, +10))
    if(Inside(iTy - RangeBottom, -10, +10))
      return;

  // Maximale Obergrenze Zielposition durch Fahrstuhl
  if(!Elevator()) return;
  iTy = Max(iTy, GetY(oElev) + 15);

  // Fahrstuhl an Zielposition: erledigt
  if(Inside(iTy - GetY(), -5, +5))
    return;

  // Fahrstuhl fährt: warten
  if(GetAction() != "Wait")
    return true;

  // Fahrstuhl noch nicht beim Clonk: warten
  if(!Inside(GetY() - GetY(pObj), -15, +15))
    return true;

  // Fahrstuhl nur in der Mitte anfassen
  if(!Inside(GetX() - GetX(pObj), -GetObjWidth()/6, +GetObjWidth()/6))
    return AddCommand(pObj, "MoveTo", this);

  // Fahrstuhl anfassen
  if(!InLiquid(pObj) && GetAction(pObj) != "Push" || GetActionTarget(0, pObj) != this)
    return AddCommand(pObj, "Grab", this);
  
  // Fahrstuhl zur Zielposition steuern
  return SetMoveTo(iTy);
}


/* Zerstörung: Reduzieren des Fahrstuhls für Wiederaufbau/Reparatur */

func Destruction()
{
  if(Elevator())
  	Elevator()->OnRemoveCase();
}
