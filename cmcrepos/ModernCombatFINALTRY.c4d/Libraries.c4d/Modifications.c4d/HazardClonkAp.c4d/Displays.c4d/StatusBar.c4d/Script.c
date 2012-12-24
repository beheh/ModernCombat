/*-- Statusbalken --*/

#strict 2

local obj;
local iBarCount, tBarCount;
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
public func IconSize()			{return 14;}
global func GetBarCount(object target, int iOwner, object exclude)
{  
  if(!target) { target = this; }

  if(!exclude)
    return ObjectCount2(Find_ActionTarget(target), Find_Owner(iOwner), Find_Func("IsBar"), Find_Func("BarActive"));
  else
    return ObjectCount2(Find_ActionTarget(target), Find_Owner(iOwner), Find_Exclude(exclude), Find_Func("IsBar"), Find_Func("BarActive"));
}


/* Initialisierung */

protected func Initialize()
{
  fActive = false;
  SetVisibility(VIS_None);

  SetGraphics("Row", this, GetID(), BAR_RowLayer, 1);
}

/* Einstellung */

local tPercent, tRotation, iDefHeight, iLength;
local idIconDef;

public func Set(object target, int color, int iType, int iLgt, string szIcon, id idSrcDef, int iXAdjust, int iYAdjust, bool fNoResize)
{
  SetVisibility(VIS_None);

  tPercent = -1;
  tRotation = GetR(target);
  obj = target;
  iBarCount = GetBarCount(obj, GetOwner());
  iBarType = iType;
  iDefHeight = GetDefHeight(GetID(target));
  if(!iLgt)
    iLgt = 100;
  iLength = iLgt;

  //Und festsetzen
  SetAction("Attach", target);

  //Färben
  SetClrModulation(color, this, BAR_RowLayer);
  SetObjDrawTransform(1000 * iLgt / 100, 0, 0, 0, 1000, 0, this);

  //Icon zur Identifizierung
  if(szIcon || idSrcDef)
    SetIcon(szIcon, idSrcDef, iXAdjust, iYAdjust, fNoResize);

  //Balken setzen
  fActive = true;
  PositionToVertex();

  fActive = false;

  return true;
}

public func SetIcon(string szIcon, id idSrcDef, int iXAdjust, int iYAdjust, bool fNoResize)
{
  idIconDef = idSrcDef;
  if(!idIconDef)
    idIconDef = GetID();

  SetGraphics(szIcon, this, idIconDef, BAR_IconLayer, 1);
  if(!fNoResize && GetDefWidth(idIconDef) > 16)
    ResizeIcon(IconSize(), iXAdjust, iYAdjust);
  else
  {
    if(!iXAdjust)
      iXAdjust = -(200 * iLength + 1675 + GetDefWidth(idIconDef) * 333); //Berechnet die Position je nach Breite des Icons

    SetObjDrawTransform(1000, 0, iXAdjust, 0, 1000, iYAdjust, this, BAR_IconLayer);
  }

  return true;
}

public func ResizeIcon(int iNewWidth, int iXAdjust, int iYAdjust)
{
  //Icongröße verändern
  var iNewSize = iNewWidth * 1000 / Max(GetDefWidth(idIconDef), GetDefHeight(idIconDef));
  if(!iXAdjust)
    iXAdjust = -(200 * iLength + 1675 + iNewWidth * 333);

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

  if(GetBarType() && iBarCount != tBarCount)
  {
    var ypos = 10;

    for(var bar in FindObjects(Find_ActionTarget(obj), Find_Owner(GetOwner()), Find_Func("IsBar"), Find_Func("BarActive")))
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
  if(fDeactivate && fActive)
  {
    SetVisibility(VIS_None);
    fActive = false;
    CallUpdateBarCount();
    return true;
  }
  else if(!fActive && !fDeactivate)
  {
    fActive = true;
    SetVisibility(VIS_Owner);
    if(GetOwner() == NO_OWNER)
      SetVisibility(VIS_All);

    CallUpdateBarCount();
  }

  //Prozentbalken anpassen
  if(percent != tPercent)
  {
    SetObjDrawTransform((10 * iLength / 100) * percent, 0, (-160 * iLength / 100) * (100 - percent), 0, 1000, 0, 0, BAR_RowLayer);
    tPercent = percent;
  }

  return true;
}

public func CallUpdateBarCount()
{
  return FindObjects(Find_ActionTarget(obj), Find_Owner(GetOwner()), Find_Func("IsBar"), Find_Func("UpdateBarCount"));
}

public func UpdateBarCount()
{
  tBarCount = GetBarCount(obj, GetOwner());
}

public func AttachTargetLost()
{
  return RemoveObject();
}