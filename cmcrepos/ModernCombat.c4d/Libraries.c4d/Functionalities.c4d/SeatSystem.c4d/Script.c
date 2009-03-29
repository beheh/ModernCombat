/*-- Sitz --*/

#strict 2

local aSeat;
local aSeatTool;

public func Initialize()
{
  aSeat = CreateArray(SeatCount());
  aSeatTool = CreateArray(SeatCount());
  _inherited();
}

//Konstanten
public func SeatCount(){return 1;}
public func SeatTarget(int iSeat){return this;}
public func SeatContainer(int iSeat){return this;}
public func SeatToolCount(int iSeat){return 0;}
public func SeatIconFacet(int iSeat){return 0;}
public func SeatName(int iSeat){return "Schähmel";}
public func SeatVehicle(){return this;}
public func MaxPassengers(){return 1;}
public func SeatVMenuItems(int iSeat){return 0;}
public func PassengerVMenuItems(int iSeat){return 0;}


//Kontrollfunktionen
public func GetUser(int iSeat)
{
  return aSeat[iSeat];
}

public func GetVehicleCrewCount(bool bNoActive, bool bNoPassive)
{
  var iCount = 0;
  
  if(!bNoPassive)
    iCount += ObjectCount2(Find_Container(this),Find_OCF(OCF_Alive | OCF_CrewMember));
  
  if(!bNoActive)
  {
    for(var i = 0; i < SeatCount(); i++)
    {
      if(SeatContainer(i) != this)
        if(aSeat[i])
          iCount++;
    }
  }
      
  return iCount;
}

public func UseSeat(int iSeat, object pClonk)
{
  if(aSeat[iSeat])//Besetzt?
  {
    Sound("CommandFailure1",false,pClonk,0,GetOwner(pClonk)+1);
    return 0;
  }
  
  SecureEnter(SeatContainer(iSeat),pClonk);
  Sound("Connect",false,pClonk,0,GetOwner(pClonk)+1);//TODO: Sound. :O
  aSeat[iSeat] = pClonk;//Besetzt. :D
  
  return 1;
}


//Funktionalität
public func Ejection(object pObj)
{
  var i = FindInArray4K(aSeat,pObj);
  if(i > -1)
  {
    ClearSeat(i);
  }
}

private func ClearSeat(int iSeat)
{
  aSeat[iSeat] = 0;
  aSeatTool[iSeat] = 0;
  //TODO: Callback tun?
}

/*public func ControlCommand(string strCommand, object pTarget, int iTx, int iTy, object pTarget2, int iData, object pCmdObj)
{
  var i = FindInArray4K(aSeat,pCmdObj);
  if(i > -1)
    SeatTarget(i)->~ControlCommand(strCommand,pTarget,iTx,iTy,pTarget2,iData,pCmdObj);
}*/


//...
public func ContainedUpdate(object pByObj)
{
  var i = FindInArray4K(aSeat,pByObj);
  if(i > -1)
  {
    SeatTarget(i)->Update(pByObj,i,...);
    return 1;
  }

  return 0;
}

public func ContainedUp(object pByObj)
{
  var i = FindInArray4K(aSeat,pByObj);
  if(i > -1)
  {
    SeatTarget(i)->Up(pByObj,i);
    return 1;
  }
  
  for(var i = 0; i < SeatCount(); i++)
  {
    if(!aSeat[i])
    {
      if(!UseSeat(i,pByObj))
        PlayerMessage(GetOwner(pByObj),"$msg_noseat$",pByObj);
      else
        PlayerMessage(GetOwner(pByObj),"$msg_useseat$",pByObj,SeatName(i));
        
      return 1;
    }
  }

  return 0;
}

public func ContainedDown(object pByObj)
{
  var i = FindInArray4K(aSeat,pByObj);
  if(i > -1)
  {
    SeatTarget(i)->Down(pByObj,i);
    return 1;
  }

  return 0;
}

public func ContainedLeft(object pByObj)
{
  var i = FindInArray4K(aSeat,pByObj);
  if(i > -1)
  {
    SeatTarget(i)->Left(pByObj,i);
    return 1;
  }

  return 0;
}

public func ContainedRight(object pByObj)
{
  var i = FindInArray4K(aSeat,pByObj);
  if(i > -1)
  {
    SeatTarget(i)->Right(pByObj,i);
    return 1;
  }

  return 0;
}

public func ContainedThrow(object pByObj)
{
  var i = FindInArray4K(aSeat,pByObj);
  if(i > -1)
  {
    SeatTarget(i)->UseTool(pByObj,i,aSeatTool[i]+1);
    return 1;
  }

  return 0;
}

//TODO: ControlSpecial-Append
public func ContainedSpecial(object pByObj)
{
  var i = FindInArray4K(aSeat,pByObj);
  if(i > -1)
  {
    SeatContainer(i)->CycleTool(pByObj,i);
    return 1;
  }

  return 0;
}

public func ContainedSpecial2(object pByObj)
{
  var i = FindInArray4K(aSeat,pByObj);
  if(i > -1)
  {
    SeatContainer(i)->VehicleMenu(pByObj);
    return 1;
  }

  VehicleMenu(pByObj);

  return 1;
}

public func ContainedDig(object pByObj)
{
  SeatMenu(pByObj);
  return 1;
}


//...
public func Collection2(object pObj)
{
  if(GetOCF(pObj) && OCF_CrewMember)
    if(GetVehicleCrewCount() > (MaxPassengers()+SeatCount()))
    {
      Sound("CommandFailure1",false,pObj,0,GetOwner(pObj)+1);
      PlayerMessage (GetOwner(pObj),"$vehicle_full$",pObj); 
      return(Exit(pObj));
    }

  //Keiner der Insassen darf mit dem Clonk verfeindet sein!
  var user;
  for(user in aSeat)//Sitze prüfen
    if(user)
      if(Hostile(GetOwner(pObj),GetOwner(user)))
        return Exit(pObj);//TODO: Erklärende Nachricht ausgeben?
      
  for(clonk in FindObjects(Find_Container(this),Find_OCF(OCF_Alive | OCF_CrewMember)))//Passagiere prüfen
    if(Hostile(GetOwner(pObj),GetOwner(clonk)))
      return Exit(pObj);//S.O.
}

public func VehicleMenu(object pClonk)
{
  CloseMenu(pClonk);
  CreateMenu(GetID(),pClonk,this,0,"$vehiclemenu$",0,C4MN_Style_Context);

  AddMenuItem("$context$","Context",GetID(pClonk),pClonk,0,ObjectNumber(pClonk));
  //AddMenuItem("$get_off$","GetOff",VI4K,pClonk,0,pClonk,0,2,1);
  AddMenuItem("$passenger$","Passenger",VI4K,pClonk,0,ObjectNumber(pClonk),0,2,2);
    
  for(var i = 0; i < SeatCount(); i++)
  {
     var rgb = RGB(0,255,0);
     var user = aSeat[i];
     if(user) rgb = RGB(255,0,0);
     
    var desc;
    if(user) desc = Format("%s(%s)",GetPlayerName(GetOwner(user)),GetName(user));
	  else     desc = "$free$";
	 
     AddMenuItem(Format("<c %x>%s</c>",rgb,SeatName(i)),Format("UseSeat2(%d,%d)",i,ObjectNumber(pClonk)),VI4K,pClonk,0,pClonk,desc,2,SeatIconFacet(i));
  }
  
  AddMenuItem(" ","Dummy",NONE,pClonk,0,pClonk);//Leerstelle
  AddMenuItem("$escape_hatch$","EscapeHatch",VI4K,pClonk,0,ObjectNumber(pClonk),0,2,1);
}

public func SeatMenu(object pClonk)
{
  CloseMenu(pClonk);
  CreateMenu(GetID(),pClonk,this,0,"$seatmenu$",0,C4MN_Style_Normal);
  SetMenuSize(3,5,pClonk);

  var i = FindInArray4K(aSeat,pClonk);
  var m = 0;

  if(i > -1)
    m = SeatVMenuItems(i);
  else
    m = PassengerVMenuItems();

  if(!m)
  {
    CloseMenu(pClonk);
    return 0;
  }
    
  SelectMenuItem(4,pClonk);
  return 1;
}

func Dummy(){}

public func Context(id iId, int iClonk)
{
  var pClonk = Object(iClonk);
  pClonk->SetCommand(pClonk,"Context",0,0,0,pClonk);
  pClonk->ExecuteCommand();
}

public func Passenger(id iId, int iClonk)
{
  var pClonk = Object(iClonk);
  var i = FindInArray4K(aSeat,pClonk);
  if(i > -1)
  {
    ClearSeat(i);
  }
  SecureEnter(this,pClonk);
}

public func GetOff(id iId, int iClonk)
{
  var pClonk = Object(iClonk);
  var i = FindInArray4K(aSeat,pClonk);
  if(i > -1)
  {
    ClearSeat(i);
  }
 
  SecureEnter(this,pClonk);
  SetCommand(pClonk,"Exit");
}

public func EscapeHatch(id iId, int iClonk)
{
  var pClonk = Object(iClonk);
  var i = FindInArray4K(aSeat,pClonk);
  if(i > -1)
  {
    ClearSeat(i);
  }
 
  SecureEnter(this,pClonk);
  Exit(pClonk);
  Fling(pClonk,GetXDir()/10,GetYDir()/10);//Huii...
  //TODO: Callback!1
}

public func UseSeat2(int iSeat, int iClonk)//Fürs Menü
{
  var pClonk = Object(iClonk);
  if(aSeat[iSeat])
  {
    CreateMenu(GetID(), pClonk, this, 0, SeatName(iSeat), 0, C4MN_Style_Context);
    AddMenuItem(Format("%s (%s)",GetTaggedPlayerName(GetOwner(aSeat[iSeat])),GetName(aSeat[iSeat])),
                0,GetID(aSeat[iSeat]),pClonk);
  }
  else
    UseSeat(iSeat,pClonk);
}


//...
public func Update(object pClonk, int iSeat, int iComDir)
{
  //...
}

public func Up(object pClonk, int iSeat)
{
  //...
}

public func Down(object pClonk, int iSeat)
{
  //...
}

public func Right(object pClonk, int iSeat)
{
  //...
}

public func Left(object pClonk, int iSeat)
{
  //...
}

public func UseTool(object pClonk, int iSeat, int iTool)//iTool = aSeatTool[iSeat]+1
{
  //...
}

public func CycleTool(object pClonk, int iSeat, int iTool)
{
  Wrap4K(aSeatTool[iSeat]+1,0,SeatToolCount(iSeat));
  //TODO: Callback!
}

//Helpers
private func SecureEnter(object pContainer, object pObj)
{
  if(pObj->Contained() != pContainer)
    return pObj->Enter(pContainer);
  return false;
}