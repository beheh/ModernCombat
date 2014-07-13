/*--- Pumpstation ---*/

#strict 2

#include DRBS
#include DRCK
#include CCBS

public func TechLevel()		{return 1;}	//Benötigte Techstufe
public func RequiredEnergy()	{return 50;}	//Benötigte Energie
public func BasementID()	{return BT05;}	//Fundament


/* Lokale Variablen */

local pBarrel;
local pDrillHead, iLiquidValue;

/* Initialisierung */

public func Initialize()
{
  //Umliegende Raffinerien beliefern
  AddEffect("SendResources", this, 1, 36, this);

  //Effekte
  if(HasEnergy())
    Sound("Building_PowerOn.ogg");
  else
    Sound("Building_PowerOff.ogg");

  return _inherited();
}

/* Bohrkopf */

private func PipeHeadCheck()
{
  //Bei Bedarf Bohrkopf und Leitung erzeugen
  if(!pDrillHead) 
  {              
    //Bohrkopf
    pDrillHead = CreateObject(PIPH, 0, 36, GetOwner());
    pDrillHead->Set(this);
    SetAction("Pump", pDrillHead);
    ObjectSetAction(pDrillHead, "Stop", this);
    CreateObject(DPLI, 0, 0, -1)->Init(this, pDrillHead);
  }
  return pDrillHead;
}

public func AcceptedMaterials()		{return [Material("Oil")];}	//Nur Öl fördern
public func LiquidValueCapacity()	{return 500;}			//Maximum an speicherbaren Credits
public func RefineryRange()		{return 300;}			//Maximale Reichweite bis zur nächsten Raffinerie
public func LineConnectType()		{return DPIP;}

/* Status */

public func AcceptTransfer()
{
  if(iLiquidValue/1000 >= LiquidValueCapacity())
    return;

  return GetAction() == "Pump";
}

/* Steuerung */

private func StartDrillHead(object pCaller)
{
  PipeHeadCheck();
  Sound("Click");
  if(!HasEnergy())
    return;

  SetComDir(COMD_Down, pDrillHead);
  ObjectSetAction(pDrillHead, "Drill");
  SetPlrView(pCaller->GetController(), pDrillHead);
  DigFreeRect(GetX(pDrillHead)-1, GetY() + GetObjHeight() / 2, 3, GetY(pDrillHead) - GetY() - GetObjHeight() / 2);
}

protected func ControlUp(pCaller)
{
  [$CtrlRetractDesc$|Image=PIPH]
  PipeHeadCheck();
  Sound("Click");
  if(!HasEnergy())
    return;

  SetComDir(COMD_Up, pDrillHead);
  ObjectSetAction(pDrillHead, "Move");
  SetPlrView(pCaller->GetController(),pDrillHead);
}

/* Pumpkontrolle */

protected func ReadyToPump()
{
  return true;
}

public func OnMaterialTransfer(int iMaterial)
{
  iLiquidValue += GetMaterialValue(iMaterial);
  return true;
}

public func FxSendResourcesTimer(object pTarget, int iNr)
{
  var ref;
  if(!(ref = FindObject2(Find_ID(CRFY), Find_Distance(RefineryRange()), Find_Allied(GetOwner()), Find_Not(Find_OCF(OCF_Construct)))))
    return true;

  ref->Process(iLiquidValue/1000);
  iLiquidValue = 0;
}

/* Zerstörung */

protected func Incineration()	{if (pBarrel) RemoveObject(pBarrel); pBarrel=0; if (pDrillHead) RemoveObject(pDrillHead); pDrillHead=0;}
protected func IncinerationEx()	{if (pBarrel) RemoveObject(pBarrel); pBarrel=0; if (pDrillHead) RemoveObject(pDrillHead); pDrillHead=0;}
protected func Destruction()	{if (pBarrel) RemoveObject(pBarrel); pBarrel=0; if (pDrillHead) RemoveObject(pDrillHead); pDrillHead=0; return _inherited(...);}