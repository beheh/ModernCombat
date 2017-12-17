/*-- Türsteuerung --*/

#strict 2

local closed;							//Offen?
local lock;							//Tür ist verschließbar (geht nicht automatisch auf)
local destroyed;						//zerstört
local maxdmg;

public func GetMaxDamage()	{return maxdmg;}		//Maximaler Schaden
public func IsDestroyable()	{return maxdmg != -1;}		//Zerstörbar?
public func SetMaxDamage(int m)	{ maxdmg = m; }
public func RejectC4Attach()	{return true;}
public func IsSideDoor()	{return true;}
func OnDestroyed()		{return 1;}			//Zerstört
func OnOpen()			{return 1;}			//Geöffnet
func OnClose()			{return 1;}			//Geschlossen


/* Initialisierung */

public func Initialize()
{
  closed = true;
  Unlock();
  SetMaxDamage(50);	//Standardmaximalschaden
  UpdateTransferZone();
}

/* Ver- und Aufschließen */

public func Lock(bool aut)
{
  lock = 1 + aut;
  RemoveEffect("CheckOpen", this);
}

public func Unlock()
{
  lock = 0;
  StartDoorCheck();
}

/* Suchzone */

public func UpdateTransferZone()
{
  SetTransferZone(-15,-GetObjWidth()/2,30,GetObjWidth());
}

public func ControlTransfer(object obj, int x, int y)
{
  //Versperrt und verschlossen: Nichts unternehmen
  if(lock && closed)
    return false;

  var dir = 1;
  if(GetX(obj) < GetX())
    dir = -1;

  if(closed)
    return AddCommand(obj,"MoveTo",0,GetX()+10*dir,GetY());
}

/* Schlossfunktionen */

public func SetKeycardLock(int iID, int dir)		//Per Keycard
{
  var x = (dir*2-1)*(GetObjWidth()/2+15);
  var konsole = CreateObject(LKEY,x,0,GetOwner());
  konsole -> Set(this,iID);
  Lock();
  return konsole;
}

public func SetPasscodeLock(int a, int dir)		//Per Eingabecode
{
  var x = (dir*2-1)*(GetObjWidth()/2+10);
  var konsole = CreateObject(PKEY,x,0,GetOwner());
  konsole -> Set(this,a);
  Lock();
  return konsole;
}

public func SetSwitchLock(int dir)			//Per Schalter
{
  var x = (dir*2-1)*(GetObjWidth()/2+8);
  var konsole = CreateObject(SWTH,x,0,GetOwner());
  konsole -> Set(this);
  Lock();
  return konsole;
}

public func Access()
{
  if(closed)
    Open();
  else if(lock != 2)
    Close();
}

public func SwitchOn()		{Open();}
public func SwitchOff()		{Close();}

/* Aktionen */

public func Open()
{
  if(destroyed)		return;
  if(!closed)		return;
  OnOpen();
  closed = false;
}

public func Close()
{
  if(destroyed)		return;
  if(closed)		return;
  OnClose();
  closed = true;

  //Tür verfügt über SolidMask: Eventuell feststeckende Objekte freimachen
  if(!GetDefCoreVal("SolidMask",0,GetID(),3)) return;
  for(var o in FindObjects(Find_InRect(-GetObjWidth()/2,-GetObjHeight()/2,GetObjWidth(),GetObjHeight()),
  				Find_Or(Find_Category(C4D_Living), Find_Func("IsMAV")),
  				Find_NoContainer(),
  				Find_Exclude(this),
  				Find_Not(Find_ID(LADR))))
  {
   AutoUnstuck4K(o);
  }
}

/* Schaden */

protected func Damage(int iChange, int iByPlayer)
{
  if(GetDamage() > GetMaxDamage() && IsDestroyable() && !destroyed)
  {
    SetSolidMask(0);
    RemoveEffect("CheckOpen",this,0);
    destroyed = true;
    OnDestroyed();
  }
}

public func IsBulletTarget()
{
  if(!closed || destroyed)
    return false;
  return true;
}

/* Clonk-Suche beginnen */

private func StartDoorCheck()
{
  AddEffect("CheckOpen",this,1,5,this);
}

/* Clonksucheffekt */

public func FxCheckOpenTimer()
{
  if(destroyed) return -1;
  if(lock == 1) return;

  if(SomeonesApproaching())
  {
    Open();
  }
  else
  {
    Close();
  }
}

private func SomeonesApproaching()
{
  return FindObject2(Find_InRect(-35, -GetObjHeight() / 2, 70, GetObjHeight()), Find_NoContainer(), Find_Or(Find_OCF(OCF_CrewMember), Find_Func("IsMav")));
}

/* Serialisierung */

public func Serialize(array& extra)
{
  extra[GetLength(extra)] = Format("SetMaxDmg(%d)", maxdmg);
  if(lock)
    extra[GetLength(extra)] = Format("Lock(%v)", (lock == 2));
  if(destroyed)
  {
    extra[GetLength(extra)] = "SetSolidMask()";
    extra[GetLength(extra)] = "RemoveEffect(\"CheckOpen\",this,0)";
    extra[GetLength(extra)] = "LocalN(\"destroyed\")=true";
  }
}