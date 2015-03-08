/*-- Bodenluke --*/

#strict 2

#include GBDR

public func IsBulletTarget()	{return false;}


/* Initialisierung */

public func Initialize() 
{
   SetAction("Closed");
   _inherited();
   SetMaxDamage(-1);
}

/* Öffnung und Schließung */

public func OnOpen()
{
  if(GetAction() == "Open" || GetAction() == "Opened") return;
  SetAction("Open");
  Sound("Airlock1");
}

public func OnOpened()
{
  DigFreeRect(GetX()-GetDefWidth()/2,GetY()-GetDefHeight()/2,GetDefWidth(),GetDefHeight());
}

public func OnClose()
{
  if(GetAction() == "Closed" || GetAction() == "Close") return;
  SetAction("Close");
  Sound("Airlock2");
  SetSolidMask(0, 0, 20, 20,3);
}

public func SetPathFree()
{
  SetSolidMask();
}

/* Clonkerkennung */

private func SomeonesApproaching()
{
  var aClonks = CreateArray();

  //Clonks suchen
  aClonks = FindObjects(Find_InRect(-(GetObjWidth()/2),0,GetObjWidth(),20),
  			Find_NoContainer(),
  			Find_Or(Find_Category(C4D_Living), Find_Func("IsMAV")),
  			Find_Not(Find_Func("IsAlien")));
  if(!closed)
  {
    if(!GetLength(aClonks))
    {
      aClonks = FindObjects(Find_InRect(-(GetObjWidth()/2),-20,GetObjWidth(),20),
      			Find_NoContainer(),
      			Find_Or(Find_Category(C4D_Living), Find_Func("IsMAV")),
      			Find_Not(Find_Func("IsAlien")));
    }
  }
  if(GetLength(aClonks) > 0)
    return true;
  return false;
}

/* Steuerung */

public func ControlUp(object pByObj)
{
  if(GetAction() != "Destroyed" && GetAction() != "Close")
    if(!lock)
      OnOpen();
  return 1;
}

public func ControlDig(object pByObj)
{
  if(GetAction() != "Destroyed" && GetAction() != "Open")
    if(!lock)
      OnClose();
  return 1;
}

/* KI Hilfe */

public func UpdateTransferZone()
{
  SetTransferZone(-GetObjWidth()/2,-GetObjHeight()/2,GetObjWidth(),GetObjHeight());
}

public func ControlTransfer(object obj, int x, int y)
{
  if(lock && closed)
    return false;

  var dir = 1;
  if(GetY(obj) < GetY()+GetObjHeight())
    dir = -1;

  if(!closed) return false;

  if(dir == -1)
  {
    if(GetProcedure(obj) != "PUSH")
    {
      if(GetCommand(obj) != "Grab")
        return AddCommand(obj,"Grab",this,0,-10);
    }
    else
    {
      ControlUp(obj);
        return true;
    }
  }
  else
  {
    return AddCommand(obj,"MoveTo",0,GetX(),GetY()+10);
  }
}

global func DebugCmds(object pTarget, bool fLog)
{
  if(!pTarget) pTarget = this;
  if(!pTarget) return;

  var i,name,target,target_name,x,y,list = "<CMD-LIST>";
  while(name = GetCommand(pTarget,0,i))
  {
    target = GetCommand(pTarget,1,i);
    if(target)
      target_name = GetName(target);
    else
      target_name = "(none)";
    
    x = GetCommand(pTarget,2,i);
    y = GetCommand(pTarget,3,i);
    
    if(fLog)
      Log("%s - [%s/%d/%d]",name,target_name,x,y);
    else
      list = Format("|%s - [%s/%d/%d]",name,target_name,x,y);
    i++;
  }
  
  if(!fLog)
    Message("@%s",pTarget,list);
}