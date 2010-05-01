/*-- Bodenlucke --*/

#strict 2

#include GBDR

public func IsBulletTarget(id idBullet)	{return false;}	//Nur anfällig gegenüber Explosionen


/* Initalisierung */

public func Initialize() 
{
   SetAction("Closed");
   _inherited();
   SetMaxDamage(100);
}

/* Öffnung und Schließung */

public func OnOpen()
{
  if(GetAction() == "Open" || GetAction() == "Opened") return ;
  SetAction("Open");
  Sound("Airlock1");
}

public func OnOpened()
{
  DigFreeRect(GetX()-GetDefWidth()/2,GetY()-GetDefHeight()/2,GetDefWidth(),GetDefHeight());
}

public func OnClose()
{
  if(GetAction() == "Closed" || GetAction() == "Close") return ;
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
 		        Find_OCF(OCF_Alive),
 		        Find_Not(Find_Func("IsAlien")) );
  if(!closed)
  {
    if(!GetLength(aClonks))
    {
      aClonks = FindObjects(Find_InRect(-(GetObjWidth()/2),-20,GetObjWidth(),20),
   		          Find_NoContainer(),
 		            Find_OCF(OCF_Alive),
 		            Find_Not(Find_Func("IsAlien")) );
    }
  }

  //Etwas gefunden?
  if(GetLength(aClonks) > 0)
    return true;
  return false;
}

/* Zerstörung */

public func OnDmg(int iDmg, int iType)
{
  //Anfälligkeit gegenüber Einflüssen
  if(iType == DMG_Projectile)	return 60;
  if(iType == DMG_Melee)	return 80;
  if(iType == DMG_Energy)	return 30;
  if(iType == DMG_Explosion)	return 0;
  return 100;
}

public func OnDestroyed(iType)
{
  //Effekte
  CastParticles("MetalSplinter",2,50,0,0,40,150);
  CastParticles("Smoke3",5,10,0,0,50,200);
  Sound("StructuralDamage*.ogg");

  //Explosion
  Explode(20);
}

/* Steuerung */

public func ControlUp(object pByObj)
{
  if(GetAction() != "Destroyed")
    if(!lock)
      OnOpen();
  return 1;
}

/* KI Hilfe */

public func UpdateTransferZone()
{
  //SetTransferZone(-15,-GetObjWidth()/2,30,GetObjWidth());
  SetTransferZone(-GetObjWidth()/2,-GetObjHeight()/2,GetObjWidth(),GetObjHeight());
}

public func ControlTransfer(object obj, int x, int y)
{
  if(lock && closed)
    return false;
  
  var dir = 1;
  if(obj->GetY() < GetY()+GetObjHeight())
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
  if(!pTarget) return ;

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