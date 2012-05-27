/*-- Statusbalken --*/

#strict 2

local obj;
local iBarCount;
local fActive;

local iBarType;

//Standard-Balkentypen:
static const BAR_Repairbar = 1;
static const BAR_Energybar = 2;
static const BAR_Ammobar = 3;

static const BAR_IconLayer = 2;
static const BAR_RowLayer = 1;

public func GetBarType()		{return iBarType;}
public func IsBar()			{return true;}
public func BarActive()			{return fActive;}
public func RejectEntrance()		{return true;}
public func HasBarType(int barType)	{return (iBarType == barType);}
global func GetBarCount(object target, int iOwner, object exclude)
{  
  if(!target) { target = this; }

  if(!exclude)
    return GetLength(FindObjects(Find_Func("IsBar"), Find_Func("BarActive"), Find_ActionTarget(target), Find_Owner(iOwner)));
  else
    return GetLength(FindObjects(Find_Func("IsBar"), Find_Func("BarActive"), Find_ActionTarget(target), Find_Owner(iOwner), Find_Exclude(exclude)));
}


/* Initialisierung */

protected func Initialize()
{
  fActive = false;
  SetVisibility(VIS_None);

  SetGraphics("Row", this, GetID(), BAR_RowLayer, 1);
}

/* Einstellung */

local tPercent, tRotation, iDefHeight;
local idIconDef;

public func Set(object target, int color, int iType, bool fIcon, string szIcon, id idSrcDef, int iXAdjust, int iYAdjust)
{
  SetVisibility(VIS_None);

  tPercent = -1;
  tRotation = GetR(target);
  obj = target;
  iBarCount = GetBarCount(obj, GetOwner());
  iBarType = iType;
  iDefHeight = GetDefHeight(GetID(target));

  //Und festsetzen
  SetAction("Attach", target);

  //Färben
  SetClrModulation(color, this, BAR_RowLayer);

  //Icon zur Identifizierung
  if(fIcon)
  {
    idIconDef = idSrcDef;
    if(!idIconDef)
      idIconDef = GetID();

    if(!iXAdjust)
    	iXAdjust = -(21675 + GetDefWidth(idIconDef) * 333); //Berechnet die Position je nach Breite des Icons

    SetGraphics(szIcon, this, idIconDef, BAR_IconLayer, 1);
    SetObjDrawTransform(1000, 0, iXAdjust, 0, 1000, iYAdjust, this, BAR_IconLayer);
  }

  //Balken setzen
  fActive = true;
  PositionToVertex();

  fActive = false;

  return true;
}

public func ResizeIcon(int iNewWidth, int iXAdjust, int iYAdjust)
{
  //Icongröße verändern
  var iNewSize = iNewWidth * 1000 / Max(GetDefWidth(idIconDef), GetDefHeight(idIconDef));
  if(!iXAdjust)
    iXAdjust = -(21675 + iNewWidth * 333);

  SetObjDrawTransform(iNewSize, 0, iXAdjust, 0, iNewSize, iYAdjust, this, BAR_IconLayer);
  return true;
}

public func SetBarCount(int iCount)
{
  iBarCount = iCount;
}

local iYPos;

public func PositionToVertex(bool fForcedYPos)
{
  if(!fActive)
    return true;

  SetVertex(0, 0, GetVertex(0, 0, obj));

  if(GetBarType() && iBarCount != GetBarCount(obj, GetOwner()))
  {
    var ypos = 10;

    for(var bar in FindObjects(Find_Func("IsBar"), Find_Func("BarActive"), Find_ActionTarget(obj), Find_Owner(GetOwner())))
    {
      if(bar->GetBarType() < GetBarType())
        ypos += 10;
    }

    SetVertex(0, 1, GetVertex(0, 1, obj) - iDefHeight / 2 - ypos);
    iBarCount = GetBarCount(obj, GetOwner());
    iYPos = ypos;
  }
  else if(GetR(obj) != tRotation || fForcedYPos)
  {
    SetVertex(0, 1, GetVertex(0, 1, obj) - iDefHeight / 2 - iYPos);
    tRotation = GetR(obj);
  }

  return true;
}

public func Update(int percent, bool fDeactivate)
{
  //Anpassen (falls andere Anzeigen vorhanden)
  //PositionToVertex();

  if(fDeactivate && fActive)
  {
    SetVisibility(VIS_None);
    fActive = false;
    return true;
  }
  else if(!fActive && !fDeactivate)
  {
    fActive = true;
    SetVisibility(VIS_Owner);
  }

  //Prozentbalken anpassen
  if(percent != tPercent)
  {
    SetObjDrawTransform(10 * percent, 0, -160 * (100 - percent), 0, 1000, 0, 0, BAR_RowLayer);
    tPercent = percent;
  }

  return true;
}

public func AttachTargetLost()
{
  return RemoveObject();
}