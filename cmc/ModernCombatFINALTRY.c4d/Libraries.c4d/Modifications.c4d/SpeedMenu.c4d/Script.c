/*-- Speedmenü --*/

#strict 2

static const RMEN_Radius = 285;
static const RMEN_Animation = 6;
static const SMEN_ItemCount = 5;
static const RMEN_TextDistance = 22;

local pCallbackObject;		//Befehlsziel für Callbacks
local pTargetObject;		//Besitzobjekt (an das die Sicht geklebt wird, und dessen Besitzer die Auswahl trifft)

local aTopInfo;
local aBottomInfo;

local aItemTitle;		//Titel
local aItemFunc;		//Funktion
local aItemPar;			//Parameter
local aItemId;			//ID
local aItemBackground;	//Hintergrund

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
  aItemTitle = CreateArray(SMEN_ItemCount);
  aItemFunc = CreateArray(SMEN_ItemCount);
  aItemPar = CreateArray(SMEN_ItemCount);
  aItemId = CreateArray(SMEN_ItemCount);
  aItemBackground = CreateArray(SMEN_ItemCount);
  aTopInfo = [];
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

public func Add(int iItem, string szTitle, string szFunc, Parameter, id idIcon, string szGraphics, id idBackground, string szBackgroundGraphics)
{
  aItemTitle[iItem] = szTitle;
  aItemFunc[iItem] = szFunc;
  aItemPar[iItem] = Parameter;
  aItemId[iItem] = idIcon;
  aItemBackground[iItem] = idBackground;

  CreateItem(iItem, idIcon);
  
  if(idIcon)
    SetGraphics(szGraphics,this,idIcon,iItem*2+2,GFXOV_MODE_IngamePicture);
  if(idBackground)
    SetGraphics(szBackgroundGraphics,this,idBackground,iItem*2+1,GFXOV_MODE_IngamePicture);

  return iItem*2+2;
}

public func GetRealCursor()
{
  return pTargetObject;
}

/* Hilfen */

static const RMEN_ThrowItem =	0;
static const RMEN_UpItem =	1;
static const RMEN_RightItem =	2;
static const RMEN_DownItem =	3;
static const RMEN_LeftItem =	4;

public func AddThrowItem(string szTitle, string szFunc, Parameter, id idIcon, string szGraphics, id idBackground, string szBackgroundGraphics)
{ return Add(RMEN_ThrowItem,szTitle,szFunc,Parameter,idIcon,szGraphics,idBackground,szBackgroundGraphics); }

public func AddUpItem(string szTitle, string szFunc, Parameter, id idIcon, string szGraphics, id idBackground, string szBackgroundGraphics)
{ return Add(RMEN_UpItem,szTitle,szFunc,Parameter,idIcon,szGraphics,idBackground,szBackgroundGraphics); }

public func AddRightItem(string szTitle, string szFunc, Parameter, id idIcon, string szGraphics, id idBackground, string szBackgroundGraphics)
{ return Add(RMEN_RightItem,szTitle,szFunc,Parameter,idIcon,szGraphics,idBackground,szBackgroundGraphics); }

public func AddDownItem(string szTitle, string szFunc, Parameter, id idIcon, string szGraphics, id idBackground, string szBackgroundGraphics)
{ return Add(RMEN_DownItem,szTitle,szFunc,Parameter,idIcon,szGraphics,idBackground,szBackgroundGraphics); }

public func AddLeftItem(string szTitle, string szFunc, Parameter, id idIcon, string szGraphics, id idBackground, string szBackgroundGraphics)
{ return Add(RMEN_LeftItem,szTitle,szFunc,Parameter,idIcon,szGraphics,idBackground,szBackgroundGraphics); }

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
  if(FindObject2(Find_ID(SMEN),Find_Owner(GetOwner(pTargetObject)), Find_Exclude(this)))//Pro Spieler wird nur ein Speedmenü erhalten.
    RemoveObject();
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
    SetViewCursor(GetOwner()); //ViewCursor zurücksetzen
    SetAction("Closing");
  }
  return 1;
}

protected func FxIntSMENCheckTimer(object pTarget)
{
  SetPosition(GetX(pTarget),GetY(pTarget));
  if (!GetAlive(pTarget) || IsFakeDeath(pTarget))
  {
    Close();
    return -1;
  }
}

protected func FxIntSMENCheckStop(object pTarget, int iEffectNumber, int iReason, bool fTemp)
{
  if(fTemp) return 0;
  if(iReason == 0) return 0; //Bei RemoveEffect nichts machen.
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

private func CreateItem(int i, id idIcon)
{
  var w,h,xoff,yoff;
  if(idIcon) {
    aItemId[i] = idIcon;
  }
  else {
    idIcon = ROCK;
  }

  //SetGraphics(0,0,idIcon,i*2+1,GFXOV_MODE_IngamePicture);
  SetGraphics(0,0,idIcon,i*2+2,GFXOV_MODE_IngamePicture);
  w = GetDefCoreVal("Picture","DefCore",idIcon,2)*1000/GetDefCoreVal("Picture","DefCore",GetID(),2);
  h = GetDefCoreVal("Picture","DefCore",idIcon,3)*1000/GetDefCoreVal("Picture","DefCore",GetID(),3);

  if(i == 0) { //Item 1
    SetObjDrawTransform(w/2,0,0,0,h/2,0,0,i+2);
  }
  else
  {
    //Items 2 bis 5
    xoff = +Sin((360/(SMEN_ItemCount-1))*(i-1),RMEN_Radius*w);
    yoff = -Cos((360/(SMEN_ItemCount-1))*(i-1),RMEN_Radius*h);
    SetObjDrawTransform(w/2,0,xoff,0,h/2,yoff,0,i+2);
  }
}

private func ScaleItems(int iDst)
{
  iDst = Max(iDst, 0);
  SetObjDrawTransform(iDst*1000/(GetDefWidth()/2),0,0,0,iDst*1000/(GetDefWidth()/2),0,0,0);


  //Item 1
  var i = 0;
  if(ItemActive(i))
  {
    var xoff,yoff = 0;
    var idIcon = aItemId[i];
    if(!idIcon) idIcon = ROCK;
    var idBackground = aItemBackground[i];

    if(aItemBackground[i]) SetObjDrawTransform(iDst*500/GetDefWidth(),0,0,0,iDst*500/GetDefHeight(),0,0,i*2+1);
    SetObjDrawTransform(iDst*500/GetDefWidth(),0,0,0,iDst*500/GetDefHeight(),0,0,i*2+2);
  }

  //Items 2 bis 5
  for(i = 1; i <= 4; i++)
  {
    if(!ItemActive(i)) continue;
    var idIcon = aItemId[i];
    if(!idIcon) idIcon = ROCK;
    var idBackground = aItemBackground[i];

    if(aItemBackground[i]) {
      xoff = +Sin((360/(SMEN_ItemCount-1))*(i-1), iDst*RMEN_Radius*GetDefCoreVal("Picture","DefCore",idBackground,2)/64);
      yoff = -Cos((360/(SMEN_ItemCount-1))*(i-1), iDst*RMEN_Radius*GetDefCoreVal("Picture","DefCore",idBackground,3)/64);
      SetObjDrawTransform(iDst*500/GetDefWidth(),0,xoff,0,iDst*500/GetDefHeight(),yoff,0,i*2+1);
    }
    
    xoff = +Sin((360/(SMEN_ItemCount-1))*(i-1), iDst*RMEN_Radius*GetDefCoreVal("Picture","DefCore",idIcon,2)/64);
    yoff = -Cos((360/(SMEN_ItemCount-1))*(i-1), iDst*RMEN_Radius*GetDefCoreVal("Picture","DefCore",idIcon,3)/64);
    SetObjDrawTransform(iDst*500/GetDefWidth(),0,xoff,0,iDst*500/GetDefHeight(),yoff,0,i*2+2);
  }
}

private func ClearItems()
{
  //Overlays zurücksetzen
  for(var i = 0; i < SMEN_ItemCount; i++)
  {
    SetObjDrawTransform(1000,0,0,0,1000,0,0,i+2);
    SetGraphics(0,0,0,i+2);
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
  Open();

  if(GetAction() == "Closing") return 1;
  
  //Kontrolle unsichtbar übergeben
  if(pTargetObject) SetCursor(GetOwner(), pTargetObject, 1, 1);

  if(aItemFunc[iItem] && GetType(aItemFunc[iItem]) == C4V_String)
    pCallbackObject->Call(aItemFunc[iItem], aItemPar[iItem]);

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

protected func Open()
{
  Message(" ", this, GetOwner());
  var i = -(GetLength(aTopInfo)-1)*RMEN_TextDistance;
  for(var szItem in aTopInfo)
  {
    CustomMessage(Format("@%s", szItem), this, GetOwner(), 0, i, 0, 0, 0, MSG_Multiple);
    i += RMEN_TextDistance;
  }
  var i = 190;
  for(var szItem in aBottomInfo)
  {
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
