#strict 2

static const RMEN_Radius = 54;
static const RMEN_Animation = 8;

local pCallbackObject;   //Befehlsziel für Callbacks
local pTargetObject;     //Besitzobjekt (an das die Sicht geklebt wird, und dessen Besitzer die Auswahl trifft)
local iSelectedItem;     //Aktiver Menüeintrag.

local aItemTitle;        //Titel
local aItemFunc;         //Funktion
local aItemPar;          //Parameter

/* globale Funktionen */

global func CreateRingMenu(object pCommandObject, object pMenuObj)
{
  if(!pCommandObject)
  {
    pCommandObject = this;
    if(!pCommandObject) return 0;
  }
  
  if(!pMenuObj)
  {
    pMenuObj = this;
    if(!pMenuObj) return 0;
  }


  
  //Erzeugen
  var pMenu = FindObject2(Find_ID(RMEN),Find_Owner(GetOwner(pMenuObj)));

  if(!pMenu || pMenu->InUsage())
    pMenu = CreateObject(RMEN,10,10,GetOwner(pMenuObj));

  if(pMenu)
    pMenu->Create(pCommandObject, pMenuObj);

  return pMenu;
}

global func AddRingMenuItem(object pCommandObject, string szTitle, string szFunc, Parameter, id idIcon)
{
  if(!pCommandObject)
  {
    pCommandObject = this;
    if(!pCommandObject) return 0;
  }

  var pMenu = GetRingMenu(pCommandObject);
  if(!pMenu) return 0;
  return pMenu->Add(szTitle,szFunc,Parameter,idIcon);
}

global func GetRingMenu(object pCommandObject)
{
  if(!pCommandObject)
  {
    pCommandObject = this;
    if(!pCommandObject) return 0;
  }

  var pMenu = GetEffect("IntRMENCheck",pCommandObject,0,4);
  return pMenu;
}

global func RingTest()
{
  var clonk = GetCursor();
  var ring = clonk->CreateRingMenu();
  
  var overlay;
  overlay = AddRingMenuItem(clonk,"BOING","Sound(\"BOING\")",0,RICO);
  SetGraphics("1",ring,RICO,overlay,GFXOV_MODE_IngamePicture);
  
  overlay = AddRingMenuItem(clonk,"BOING","Sound(\"BOING\")",0,RICO);
  SetGraphics("2",ring,RICO,overlay,GFXOV_MODE_IngamePicture);
  
  overlay = AddRingMenuItem(clonk,"BOING","Sound(\"BOING\")",0,RICO);
  SetGraphics("3",ring,RICO,overlay,GFXOV_MODE_IngamePicture);
  
  overlay = AddRingMenuItem(clonk,"BOING","Sound(\"BOING\")",0,RICO);
  SetGraphics("4",ring,RICO,overlay,GFXOV_MODE_IngamePicture);
  
  overlay = AddRingMenuItem(clonk,"BOING","Sound(\"BOING\")",0,RICO);
  SetGraphics("5",ring,RICO,overlay,GFXOV_MODE_IngamePicture);
}


/* Allgemeines */

protected func Initialize()
{
  aItemTitle = [];
  aItemFunc  = [];
  aItemPar   = [];
  iSelectedItem = -1;//Nix gewählt.
  SetVisibility(VIS_None); 
}

public func Create(object pCommandObject, object pMenuObj)
{
  //Felder kopieren
  pCallbackObject = pCommandObject;
  pTargetObject = pMenuObj;
  SetPosition(GetX(pMenuObj),GetY(pMenuObj));
  //Aktivität setzen und Sound ausgeben
  Sound("RMEN_Open",true,0,0,GetOwner()+1); 
  SetAction("Opening");
  Opening();//Erstes Update
  //Eine volle Energieleiste sieht doch viel schöner aus :)
  DoEnergy(100);
  //Als Cursorobjekt auswählen, damit Tastaturkommandos gesendet werden und die Sicht stimmt
  SetCursor(GetOwner(),this,1,1);
  SelectCrew(GetOwner(),pMenuObj,0,1);
  SetViewCursor(GetOwner(),pMenuObj);
  //SetComDir(COMD_Stop,pMenuObj);
  //Letztes Kommando löschen, um ControlSpecial auch beim schnellen ControlSpecialDouble abzufangen
  ClearLastPlrCom(GetController(pMenuObj));
  //Überwachungseffekt fürs Schließen an den Clonk
  AddEffect("IntRMENCheck",pMenuObj,1,1,this);
  //Sichtbar machen
  SetVisibility(VIS_Owner);

  return 1;
}

public func Add(string szTitle, string szFunc, Parameter, id idIcon)
{
  var i = GetItemCount();
  aItemTitle[i] = szTitle;
  aItemFunc[i]  = szFunc;
  aItemPar[i]   = Parameter;
  
  if(idIcon)
    SetGraphics(0,0,idIcon,i+1,GFXOV_MODE_IngamePicture);
  
  UpdateItems();
  
  return i+1;
}

public func CloseDown()
{
  Sound("RMEN_Close",true,0,0,GetOwner()+1); 
  SetAction("Closeing");
}

public func Close()
{
  //Alle Items löschen
  ClearItems();
  //Sicht zurücksetzen - mit Pfeil, aber ohne Rahmen
  if(GetCursor(GetOwner()) == this || !GetCursor(GetOwner()))
  {
    if (pTargetObject) SetCursor(GetOwner(), pTargetObject, 1);
    //ViewCursor zurücksetzen
    SetViewCursor(GetOwner());
  }
  
  aItemTitle = [];
  aItemFunc  = [];
  aItemPar   = [];
  iSelectedItem = -1;
  pCallbackObject = 0;
  pTargetObject = 0;
  
  RemoveEffect("IntRMENCheck",this);
  
  //Löschen
  if(FindObject2(Find_ID(RMEN),Find_Owner(GetOwner(pTargetObject))) != this)//Pro Spieler wird nur ein Ringmenü erhalten.
  {
    RemoveObject();
    //Schedule(Format("RemoveObject(Object(%d))", ObjectNumber(this)), 1);
  }
  else
  {
    SetPosition(1,1);
    SetVisibility(VIS_None);
    SetAction("Idle");
  }

  return 1;
}

protected func CrewSelection(bool fDeselect, bool fCursor)
{
  //Bei Crewabwahl schließen (mit Verzögerung)
  if(fDeselect)
  {
    SetViewCursor(GetOwner());// ViewCursor zurücksetzen
    Sound("RMEN_Close");
    SetAction("Closeing");
  }
  return 1;
}

protected func FxIntRMENCheckTimer(object pTarget)
{
  SetPosition(GetX(pTarget),GetY(pTarget));
}

protected func FxIntRMENCheckStop(object pTarget, int iEffectNumber, int iReason, bool fTemp)
{
  if(fTemp) return 0;
  if(iReason == 0) return 0;//Bei RemoveEffect nichts machen.
  Close();
}

public func InUsage()
{
  return pCallbackObject || pTargetObject || GetEffect("IntRMENCheck",this);
}


/* Von Overlays und Menü-Items... */

private func GetItemCount()
{
  return GetLength(aItemTitle);
}

private func GetItemAngle(int iItem)
{
  if(iItem < 0) return 0;
  return (360/GetItemCount())*iItem;
}

private func CreateItems()
{
  for(var i = 0, items = GetItemCount(); i < items; i++)
  {
    SetGraphics(0,0,ROCK,i+1,GFXOV_MODE_IngamePicture);//,0,GFX_BLIT_Parent);
    
    var w = GetDefCoreVal("Picture","DefCore",ROCK,2)*1000/GetDefCoreVal("Picture","DefCore",GetID(),2);
    var h = GetDefCoreVal("Picture","DefCore",ROCK,3)*1000/GetDefCoreVal("Picture","DefCore",GetID(),3);
    
    var xoff = +Sin((360/items)*i,RMEN_Radius*w);
    var yoff = -Cos((360/items)*i,RMEN_Radius*h);
    
    SetObjDrawTransform(w/2,0,xoff,0,h/2,yoff,0,i+1);
  }
  
  UpdateItems();
}

private func ScaleItems(int iDst)
{
  SetObjDrawTransform(iDst*1000/(GetDefWidth()/2),0,0,0,iDst*1000/(GetDefWidth()/2),0,0,0);
  for(var i = 0, items = GetItemCount(); i < items; i++)
  {
    var xoff = +Sin((360/items)*i,(iDst*RMEN_Radius/GetDefWidth())*1000);
    var yoff = -Cos((360/items)*i,(iDst*RMEN_Radius/GetDefHeight())*1000);
    
    SetObjDrawTransform(iDst*500/GetDefWidth(),0,xoff,0,iDst*500/GetDefHeight(),yoff,0,i+1);
  }
}

private func UpdateItems()
{
  for(var i = 0, items = GetItemCount(); i < items; i++)
  {
    if(iSelectedItem == i)
    {
      SetClrModulation(RGBa(255,255,255,0),0,i+1); 
    }
    else
    {
      SetClrModulation(RGBa(128,128,128,64),0,i+1);
    }
  }
}

private func ClearItems()
{
  //Overlays zurücksetzen
  for(var i = 0, items = GetItemCount(); i < items; i++)
  {
    SetObjDrawTransform(1000,0,0,0,1000,0,0,i+1);
    SetGraphics(0,0,0,i+1);
  }

  //Einträge löschen
  SetLength(aItemFunc,0);
}


/* Steuerung */

protected func ControlLeft()          { return DoSelection(270); }
protected func ControlRight()         { return DoSelection(090); }
protected func ControlUp()            { return DoSelection(000); }
protected func ControlDown()          { return DoSelection(180); }

protected func ControlThrow()         { return ActivateItem(); }

protected func ControlDig()           { return CloseDown(); }
protected func ControlSpecial()       { return CloseDown(); }
protected func ControlSpecial2()      { return CloseDown(); }

/*
Beim ersten Tastendruck
wird das Item ausgewählt das direkt in der Richtung liegt.
Ansonsten wird immer ein Item in die Zielrichtung weitergesprungen.
*/
private func DoSelection(int iTargetAngle)
{
  if(GetAction() == "Closeing") return 1;

  if(iSelectedItem < 0)
  {
    SetSelection(iTargetAngle);
  }
  else
  {
    var dir = GetItemAngle(iSelectedItem)-iTargetAngle;
    if(dir == 180)
    {
      SwapSides(iTargetAngle);
    }
    else
    {
      if(dir == 0) return 1;
    
      if(dir < 180)
        TurnLeft();
      else
        TurnRight();
    }
  }
  
  UpdateItems();
  return 1;
}

private func SetSelection(int iAngle)
{
  var curOffset = 180;
  var curItem = iSelectedItem;
  
  for(var i = 0, items = GetItemCount(); i < items; i++)//Nicht so der Logik-Hammer... aber was solls. <.<
  {
    var Offset = Abs(AngleOffset4K(iAngle,GetItemAngle(i)));
    if(Offset < curOffset)
    {
      curOffset = Offset;
      curItem = i;
    }
  }
  
  iSelectedItem = curItem;
}

private func TurnLeft()
{
  if(iSelectedItem <= 0)
    iSelectedItem = GetItemCount()-1;
  else
    iSelectedItem--;
}

private func TurnRight()
{
  if(iSelectedItem >= GetItemCount()-1)
    iSelectedItem = 0;
  else
    iSelectedItem++;
}

private func SwapSides(int iAngle)
{
  //Faulheit.
  SetSelection(iAngle);
}

private func ActivateItem()
{
  if(GetAction() == "Closeing") return 1;
  if(iSelectedItem < 0) return 1;
  
  if(aItemFunc[iSelectedItem])
  {
    pCallbackObject->Call(aItemFunc[iSelectedItem],aItemPar[iSelectedItem]);
    /*if(aItemPar[iSelectedItem])
      pCallbackObject->Call(aItemFunc[iSelectedItem],aItemPar[iSelectedItem]);
    else
      pCallbackObject->eval(aItemFunc[iSelectedItem]);*/
  }
  
  CloseDown();
  
  return 1;
}


/* Aktionen */

protected func Opening()
{
  ScaleItems(GetActTime()*(GetDefWidth()/2)/RMEN_Animation);

  if(GetActTime() >= RMEN_Animation)
    SetAction("Open");
}

protected func Closeing()
{
  ScaleItems((GetDefWidth()/2)-(GetActTime()*(GetDefWidth()/2)/RMEN_Animation));

  if(GetActTime() >= RMEN_Animation)
  {
    Close();
  }
}