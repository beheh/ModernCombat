/*-- Bau-Cursor --*/

#strict 2

local iYUp,iYDown,iXLeft,iXRight;
local arrayID,selectID,selectVal,selItem;
local placemode;

/* Initalisierung */

func Initialize()
{
  if(GetPlayerTeam(GetOwner()) == 1)
  {
    iXLeft = 640;
    iXRight = 1470;
  }
  else
  {
    iXLeft = 2020;
    iXRight = 2860;
  }
  iYUp = 130;
  iYDown = 530;
  SetPlrViewRange(500);
  
  //Setzbare Items
  arrayID = [
  [WCR2,2],[XWCR,10],[MWCR,10],[SHLF,20],
  [PBRL,10],[XBRL,15],[HBRL,15],[TBRL,10],[GSBL,10],
  [CON1,30],[XTNK,10],[PTNK,20],
  [FENC,20],[PLLT,15],[SBBA,5]];
  
  SetAction("Exist");
}

func Check()
{
  if(!Inside(GetX(),iXLeft,iXRight))
  {
    SetXDir(0);
    if(GetX() < iXLeft)
      SetPosition(iXLeft,GetY());
    else
      SetPosition(iXRight,GetY());
  }
  if(!Inside(GetY(),iYUp,iYDown))
  {
    SetYDir(0);
    if(GetY() < iYUp)
      SetPosition(GetX(),iYUp);
    else
      SetPosition(GetX(),iYDown);
  }
  
  //Item vorhanden? Neues Item gebraucht? Item bewegen.
  if(selectID)
  {
    if(selItem)
      SetPosition(GetX(),GetY(),selItem);
    else
    {
      selItem = CreateObject(selectID,0,0,-1);
      if(selItem->~IsExplosive())
        SetOwner(GetOwner(),selItem);
    }
    SetYDir(0,selItem);
  }
    
}

protected func ControlUp()
{
  SetYDir(GetYDir(0,1)-1,0,1);
}

protected func ControlDown()
{
  SetYDir(GetYDir(0,1)+1,0,1);
}

protected func ControlLeft()
{
  if(placemode)
    ModifyItem(0);
  else
    SetXDir(GetXDir(0,1)-1,0,1);
}

protected func ControlRight()
{
  if(placemode)
    ModifyItem(1);
  else
    SetXDir(GetXDir(0,1)+1,0,1);
}

protected func ControlDig()
{
  SetXDir(0);
  SetYDir(0);
  Sound("CatapultSet"); 
}

protected func ControlDigDouble()
{
  CreateMenu(GetID(),this,this,0,"$SelectObject");
  for(var objid in arrayID)
    AddMenuItem(Format("%s:  %d $",GetName(0,objid[0]),objid[1]),"SelectItem",objid[0],this,0,objid[1]);
}
  
public func SelectItem(id idItem, int iValue)
{
  if(selItem)
    RemoveObject(selItem);
    
  selectID = idItem;
  selItem = CreateObject(idItem,0,0,GetOwner());
  selectVal = iValue;
}

protected func ControlThrow()
{
  ControlDig(); //Anhalten
  
  if(!selItem)
    return;
  
  if(selItem->~IsExplosive())
  {
    if(FindObject2(Find_Func("IsExplosive"),Find_Distance(20),Find_Exclude(selItem)))
      return PlayerMessage(GetOwner(),"$TooMuch$",this);
    ScheduleCall(selItem,"Check",100);
    SetOwner(GetOwner());
  }
  
  if(GetWealth(GetOwner()) < selectVal)
    return PlayerMessage(GetOwner(),"$NoFunds$",this);
    
  if(placemode)
  {
    selItem = 0; //selItem = 0 lässt das aktuelle Item fallen
    placemode = 0;
    DoWealth(GetOwner(),-selectVal);
    Sound("Cash",1,0,0,GetOwner()+1);
    return;
  }
  
  if(selectID == SBBA || selectID == CON1) //Sonderbehandlung: Plazierung
    placemode = 1;
  else
  {
    selItem = 0;
    Sound("Cash",1,0,0,GetOwner()+1);
    DoWealth(GetOwner(),-selectVal);
    return;
  }
}

private func ModifyItem(int iDir)
{
  if(selectID == SBBA) //Sandsack
  {
    if(iDir == 0)
      selItem->Left();
    else
      selItem->Right();
  }
  
  if(selectID == CON1) //Container
  {
    if(iDir == 0)
      selItem->SetPerspective(Random(2));
    else
      selItem->SetPerspective(2);
  }
}
