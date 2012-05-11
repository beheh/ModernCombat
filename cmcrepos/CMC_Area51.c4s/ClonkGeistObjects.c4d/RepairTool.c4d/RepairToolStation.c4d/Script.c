/*-- Stationäre Schweißbrenner --*/

#strict 2

// Öffnen und Schließen

local repairtool;

protected func Initialize()
{
  //SetAction("Closed");
  CreateNewRepairTool();
  
  AddEffect("ShowWeapon", this, 20, 1, this);
  
  return 1;
}

public func WeaponAt(int &x, int &y, int &r)
{
	x = 0; y = 0; r = 0;
	return true;
}

public func FxRepairToolCableStart(object target, int nr, int temp)
{
	if(temp)
		return -1;

	var cable = CreateObject(CK5P, 0, 0, -1);
	cable->ConnectObjects(this, target);
	cable->SetRopeColor(RGB(100, 100, 100));
	cable->SetRopeLength(160);
	
	EffectVar(0, target, nr) = cable;
	
	return true;
}

public func FxRepairToolCableTimer(object target, int nr)
{
	var ydir = EffectVar(1, target, nr);
	
	/*if(ydir != 0 || GetYDir(target) != 0)
		Log("%d, %d", ydir, GetYDir(target));*/
	
	var target2 = target;
	if(Contained(target))
		target2 = Contained(target);
	
	if((ydir > 30 && GetYDir(target2) < ydir-25) || (ydir < 30 && GetYDir(target2) > ydir+25))
	{
		RemoveObject(EffectVar(0, target, nr));
		Sound("Connect", false, target, 100);
		Schedule(Format("FadeOut(Object(%d), 2)", ObjectNumber(target)), 5);
		ScheduleCall(this, "CreateNewRepairTool", 35*30); 
		return -1;
	}
	
	EffectVar(1, target, nr) = GetYDir(target2);
	return true;
}

public func CreateNewRepairTool()
{
	repairtool = CreateContents(BWTH, this);
	repairtool->SetStation(this);
  AddEffect("RepairToolCable", repairtool, 101, 5, this);
}

public func ControlDigDouble(object pCaller) { Activate(pCaller); }

public func Activate(object pByObj)
{
	if(Contained(repairtool) == this)
		repairtool->Enter(pByObj);
		
	return true;
}

public func RejectCollect(id idObj, object pObj)
{
	if(repairtool && pObj != repairtool)
		return true;
	
	return false;		
}

/*protected func Grabbed(object pClonk, bool fGrab)
{
  // Die Truhe wurde gerade angefasst oder losgelassen
  CheckGrab();
}

protected func GrabLost(object pClonk)
{
  // Ein anfassender Clonk hat seinen Halt verloren
  CheckGrab();
}

// Status überprüfen, wenn die Truhe...
// * angefasst oder losgelassen wird
// * sich fertig geöffnet oder geschlossen hat
// * auf- oder zugeschlossen wurde
// * ein anfassender Clonk seinen Halt verloren hat
// * bekanntes Problem: wird ein Clonk durch Schaden weggeschleudert, bekommt die Truhe das nicht mit (es gibt kein GrabLost)

private func CheckGrab()
{
  // Die Truhe soll sich selbst öffnen, wenn (mindestens) ein Clonk sie anfasst...
  if (FindObject(0,0,0,0,0,OCF_CrewMember(),"Push",this()))
    Open();
  // ...und schließen, wenn nicht
  else
    Close();
}

// Auf- und zuschließen

public func Lock()
{
  // Bereits abgeschlossen
  if (fLocked) return(1);
  // Abschließen    
  fLocked = true;
  Sound("Click");  
  CheckGrab();
  return(1);
}

public func Unlock()
{
  // Nicht abgeschlossen
  if (!fLocked) return(1);
  // Aufschließen
  fLocked = false;
  Sound("Click");  
  CheckGrab();
  return(1);
}

protected func RejectCollect(id idObj, object pObj)
{
  // Abgeschlossen: Hineinlegen nicht möglich
  if (fLocked)
  {
    Message("$MsgLocked$", this());
    Sound("Error");
    return(true);
  }
  // Nicht abgeschlossen: Hineinlegen okay
  return(false);
}

protected func RejectContents()
{
  // Abgeschlossen: Herausnehmen nicht möglich
  if (fLocked)
  {
    Message("$MsgLocked$", this());
    Sound("Error");
    return(true);
  }
  // Nicht abgeschlossen: Herausnehmen okay
  return(false);
}

// Sound-Effekte

protected func Opening() 
{
  Sound("ChestOpen"); 
}
  
protected func Closing() 
{
  Sound("ChestClose");
}

// Steuerung

protected func ControlDig(object pClonk)
{
  // Herausnehmen per Graben: Holen-Menü öffnen
  pClonk->SetCommand(0, "Get", this(), 0,0, 0, 1);
}
*/
