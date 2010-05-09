/*-- Speedmenü --*/

#strict 2

static const RMEN_Radius = 54;
static const RMEN_Animation = 8;
static const SMEN_ItemCount = 5;
static const RMEN_TextDistance = 22;

local pCallbackObject;		//Befehlsziel für Callbacks
local pTargetObject;		//Besitzobjekt (an das die Sicht geklebt wird, und dessen Besitzer die Auswahl trifft)

local aTopInfo;
local aBottomInfo;

local aItemTitle;		//Titel
local aItemFunc;		//Funktion
local aItemPar;			//Parameter


/* globale Funktionen */

global func CreateSpeedMenu(object pCommandObject, object pMenuObj)
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
  var pMenu, pMenus = FindObjects(Find_ID(SMEN),Find_Owner(GetOwner(pMenuObj)));
 
  for(var pCurrentMenu in pMenus)
    if(pCurrentMenu->~GetRealCursor() == pMenuObj)
      pMenu = pCurrentMenu;
  if(!pMenu || pMenu->InUsage())
    pMenu = CreateObject(SMEN,10,10,GetOwner(pMenuObj));

  if(pMenu)
    pMenu->Create(pCommandObject, pMenuObj);

  return pMenu;
}

global func GetSpeedMenu(object pCommandObject)
{
  if(!pCommandObject)
  {
    pCommandObject = this;
    if(!pCommandObject) return 0;
  }

  var pMenu = GetEffect("IntSMENCheck",pCommandObject,0,4);
  return pMenu;
}

/* Allgemeines */

protected func Initialize()
{
  aItemTitle  = [SMEN_ItemCount];
  aItemFunc   = [SMEN_ItemCount];
  aItemPar    = [SMEN_ItemCount];
  aTopInfo    = [];
  aBottomInfo = [];
  SetVisibility(VIS_None); 
}

public func Create(object pCommandObject, object pMenuObj)
{
  //Felder kopieren
  pCallbackObject = pCommandObject;
  pTargetObject = pMenuObj;
  SetPosition(GetX(pMenuObj),GetY(pMenuObj));

  //Aktivität setzen
  SetAction("Opening");
  Opening();//Erstes Update

  //Volle Energieleiste
  DoEnergy(100);

  //Als Cursorobjekt auswählen, damit Tastaturkommandos gesendet werden und die Sicht stimmt
  SetCursor(GetOwner(),this,1,1);
  SelectCrew(GetOwner(),pMenuObj,0,1);
  SetViewCursor(GetOwner(),pMenuObj);

  //Letztes Kommando löschen, um ControlSpecial auch beim schnellen ControlSpecialDouble abzufangen
  ClearLastPlrCom(GetController(pMenuObj));

  //Überwachungseffekt fürs Schließen an den Clonk
  AddEffect("IntSMENCheck",pMenuObj,1,1,this);

  //Sichtbar machen
  SetVisibility(VIS_Owner);

  return 1;
}

public func Add(int iItem, string szTitle, string szFunc, Parameter, id idIcon)
{
  aItemTitle[iItem] = szTitle;
  aItemFunc[iItem]  = szFunc;
  aItemPar[iItem]   = Parameter;
  
  CreateItem(iItem);
  
  if(idIcon)
    SetGraphics(0,0,idIcon,iItem+1,GFXOV_MODE_IngamePicture);
  
  return iItem+1;
}

public func GetRealCursor()
{
  return pTargetObject;
}

/* Hilfen */

public func AddThrowItem(string szTitle, string szFunc, Parameter, id idIcon)
            { return Add(0,szTitle,szFunc,Parameter,idIcon); }
            
public func AddUpItem(string szTitle, string szFunc, Parameter, id idIcon)
            { return Add(1,szTitle,szFunc,Parameter,idIcon); }
            
public func AddRightItem(string szTitle, string szFunc, Parameter, id idIcon)
            { return Add(2,szTitle,szFunc,Parameter,idIcon); }
            
public func AddDownItem(string szTitle, string szFunc, Parameter, id idIcon)
            { return Add(3,szTitle,szFunc,Parameter,idIcon); }
            
public func AddLeftItem(string szTitle, string szFunc, Parameter, id idIcon)
            { return Add(4,szTitle,szFunc,Parameter,idIcon); }

public func AddTopInfoItem(string szItem)
						{ return aTopInfo[GetLength(aTopInfo)] = szItem; }

public func AddBottomInfoItem(string szItem)
						{ return aBottomInfo[GetLength(aBottomInfo)] = szItem; }

public func CloseDown()
{
  SetAction("Closing");
  return 1;
}

public func NoClose()
{
  if(GetAction() != "Closing") return;
  SetAction("Opening");
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

  //Letztes Kommando löschen
  ClearLastPlrCom(GetController(pTargetObject));

  aItemTitle = [];
  aItemTitle = [SMEN_ItemCount];
  aItemFunc  = [];
  aItemFunc  = [SMEN_ItemCount];
  aItemPar   = [];
  aItemPar   = [SMEN_ItemCount];
  pCallbackObject = 0;
  pTargetObject = 0;

  RemoveEffect("IntSMENCheck",pTargetObject);

  //Löschen
  if(FindObject2(Find_ID(SMEN),Find_Owner(GetOwner(pTargetObject))) != this)//Pro Spieler wird nur ein Speedmenü erhalten.
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
    SetAction("Closing");
  }
  return 1;
}

protected func FxIntSMENCheckTimer(object pTarget)
{
  SetPosition(GetX(pTarget),GetY(pTarget));
}

protected func FxIntSMENCheckStop(object pTarget, int iEffectNumber, int iReason, bool fTemp)
{
  if(fTemp) return 0;
  if(iReason == 0) return 0;//Bei RemoveEffect nichts machen.
  Close();
}

public func InUsage()
{
  return pCallbackObject || pTargetObject || GetEffect("IntSMENCheck",this);
}

/* Overlays und Menü-Items */

private func ItemActive(int iItem)
{
  return !!aItemFunc[iItem];
}

private func CreateItem(int i)
{
  var w,h,xoff,yoff;

  
  if(i == 0)//Item 1
  {
    SetGraphics(0,0,ROCK,i+1,GFXOV_MODE_IngamePicture);//,0,GFX_BLIT_Parent);
    
    w = GetDefCoreVal("Picture","DefCore",ROCK,2)*1000/GetDefCoreVal("Picture","DefCore",GetID(),2);
    h = GetDefCoreVal("Picture","DefCore",ROCK,3)*1000/GetDefCoreVal("Picture","DefCore",GetID(),3);
    
    SetObjDrawTransform(w/2,0,0,0,h/2,0,0,i+1);
  }
  else//Items 2 bis 5
  {
    SetGraphics(0,0,ROCK,i+1,GFXOV_MODE_IngamePicture);//,0,GFX_BLIT_Parent);
    
    w = GetDefCoreVal("Picture","DefCore",ROCK,2)*1000/GetDefCoreVal("Picture","DefCore",GetID(),2);
    h = GetDefCoreVal("Picture","DefCore",ROCK,3)*1000/GetDefCoreVal("Picture","DefCore",GetID(),3);
    
    xoff = +Sin((360/(SMEN_ItemCount-1))*(i-1),RMEN_Radius*w);
    yoff = -Cos((360/(SMEN_ItemCount-1))*(i-1),RMEN_Radius*h);
    
    SetObjDrawTransform(w/2,0,xoff,0,h/2,yoff,0,i+1);
  }
}

private func ScaleItems(int iDst)
{
  iDst = Max(iDst, 0);
  SetObjDrawTransform(iDst*1000/(GetDefWidth()/2),0,0,0,iDst*1000/(GetDefWidth()/2),0,0,0);


  //Item 1
  if(ItemActive(i))
  {
    var xoff,yoff,i = 0;
    SetObjDrawTransform(iDst*500/GetDefWidth(),0,0,0,iDst*500/GetDefHeight(),0,0,i+1);
  }

  //Items 2 bis 5
   for(i = 1; i <= 4; i++)
  {
    if(!ItemActive(i)) continue;

    xoff = +Sin((360/(SMEN_ItemCount-1))*(i-1),(iDst*RMEN_Radius/GetDefWidth())*1000);
    yoff = -Cos((360/(SMEN_ItemCount-1))*(i-1),(iDst*RMEN_Radius/GetDefHeight())*1000);
    
    SetObjDrawTransform(iDst*500/GetDefWidth(),0,xoff,0,iDst*500/GetDefHeight(),yoff,0,i+1);
  }
}

private func ClearItems()
{
  //Overlays zurücksetzen
  for(var i = 0; i < SMEN_ItemCount; i++)
  {
    SetObjDrawTransform(1000,0,0,0,1000,0,0,i+1);
    SetGraphics(0,0,0,i+1);
  }
}

/* Steuerung */

protected func ControlThrow()		{return ActivateItem(0);}
protected func ControlUp()		{return ActivateItem(1);}
protected func ControlRight()		{return ActivateItem(2);}
protected func ControlDown()		{return ActivateItem(3);}
protected func ControlLeft()		{return ActivateItem(4);}

protected func ControlDig()		{return CloseDown();}
protected func ControlSpecial()		{return CloseDown();}
protected func ControlSpecial2()	{return CloseDown();}

private func ActivateItem(int iItem)
{
  if(GetAction() == "Closing") return 1;
  
  if(aItemFunc[iItem])
    pCallbackObject->Call(aItemFunc[iItem],aItemPar[iItem]);
  
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

protected func Open() {
  var i = -GetLength(aTopInfo)-1*RMEN_TextDistance+5;
  for(var szItem in aTopInfo) {
    CustomMessage(Format("@%s", szItem), this, GetOwner(), 0, i, 0, 0, 0, MSG_Multiple);
    i += RMEN_TextDistance;
  }
  var i = 190;
  for(var szItem in aBottomInfo) {
    CustomMessage(Format("@%s", szItem), this, GetOwner(), 0, i, 0, 0, 0, MSG_Multiple);
    i += RMEN_TextDistance;
  }
}

protected func Closing()
{
  Message(" ", this, GetOwner());
  ScaleItems((GetDefWidth()/2)-(GetActTime()*(GetDefWidth()/2)/RMEN_Animation));

  if(GetActTime() >= RMEN_Animation)
  {
    Close();
  }
}
