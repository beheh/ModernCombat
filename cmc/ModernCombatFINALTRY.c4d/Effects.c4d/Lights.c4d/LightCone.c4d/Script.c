/*-- Lichtkegel --*/

#strict 2
#include LGHC

local fLocked;
local iSensorDistance;
local iBlindEffectDistance;
local iFlashlightAngle;

public func Lock()		{return fLocked = true;}
public func UnLock()		{return fLocked = false;}
public func IsLocked()		{return fLocked;}
public func IsModernLight()	{return true;}


/* Konfiguration */

public func ChangeSizeXY()
{
  if(IsLocked())
    return;

  return _inherited(...);
}

public func ChangeColor()
{
  if(IsLocked())
    return;

  return _inherited(...);
}

public func Update()
{
  if(IsLocked())
    return Draw();

  return _inherited(...);
}

/* Erstellung */

global func AddLightCone(int iSize, int iColor, object pTarget, int iNewFlashlightAngle, int iNewSensorDistance, int iNewBlindEffectDistance, id LightConeID)
{
  //Kein Ziel: Abbruch
  if(!pTarget && !(pTarget = this))
    return;

  if(!LightConeID)
    LightConeID = LGHC;

  var result = CreateLight(LightConeID, iSize, iColor, pTarget);
  if(result->~IsModernLight())
  {
    LocalN("iSensorDistance", result) = iNewSensorDistance;
    LocalN("iBlindEffectDistance", result) = iNewBlindEffectDistance;
    LocalN("iFlashlightAngle", result) = iNewFlashlightAngle;
  }

  return result;
}

func SpotAndBlind(object pUser, int iAngle)
{
  //Zu markierende Gefahren suchen
  for(var pObj in FindObjects(Find_Distance(Max(iSensorDistance, iBlindEffectDistance)),	//In Reichweite
  		Find_Hostile(GetController(pUser)),						//Nur feindliche Objekte markieren
  		Find_Or(Find_OCF(OCF_Alive), Find_Func("IsDetectable")),			//Lebewesen oder als identifizierbar markiert
  		Find_NoContainer(),								//Im Freien
  		Find_Exclude(this)))								//Selber ausschlieﬂen
  {
    if(pObj == pUser)
      continue;

    var iAngleMin = iAngle-iFlashlightAngle/2;
    var iAngleMax = iAngle+iFlashlightAngle/2;

    //Objekt im Suchkegel?
    var x = GetX(pUser), y = GetY(pUser), ox = GetX(pObj), oy = GetY(pObj);
    var target_angle = Normalize(Angle(x, y, ox, oy), -180);
    var iDist = Distance(x, y, ox, oy);

    if(iAngleMax < 180 && (target_angle < iAngleMin || target_angle > iAngleMax))
      continue;
    else if(iAngleMax >= 180 && (target_angle < iAngleMin && target_angle > iAngleMax-360))
      continue;

    if(iBlindEffectDistance && pObj->~IsClonk() && PathFree(x, y, ox, oy) && iDist < iBlindEffectDistance)
    {
      if(!GetEffect("FlashlightBlindness", pObj))
        AddEffect("FlashlightBlindness", pObj, 100, 1, 0, LHC3);

      EffectCall(pObj, GetEffect("FlashlightBlindness", pObj), "Refresh", pUser, iBlindEffectDistance);
    }

    if(!iSensorDistance || iDist > iSensorDistance) return;
    //Bereits markierte Objekte auslassen
    var tag;
    if(tag = FindObject2(Find_ActionTarget(pObj), Find_ID(SM08), Find_Allied(GetController(pUser))))
    {
      tag->~RefreshRemoveTimer(this);
      continue;
    }

    //Ansonsten markieren
    CreateObject(SM08, GetX(pObj), GetY(pObj), GetController(pUser))->Set(pObj, this, GetOCF(pObj) & OCF_Alive, 26);
  }
}

public func FxFlashlightBlindnessStart(object pTarget, int iNr, temp)
{
  if(temp)
    return;

  EffectVar(0, pTarget, iNr) = ScreenRGB(pTarget, RGBa(255, 255, 255, 254), 0, 0, false, SR4K_LayerLight);
  EffectVar(5, pTarget, iNr) = 0;
}

static const Flashlight_MinAlpha = 80;
static const Flashlight_MaxAlpha = 180;

public func FxFlashlightBlindnessTimer(object pTarget, int iNr)
{
  var rgb = EffectVar(0, pTarget, iNr);
  var distAlpha = EffectVar(4, pTarget, iNr);

  if(!rgb)
    rgb = EffectVar(0, pTarget, iNr) = ScreenRGB(pTarget, RGBa(255, 255, 255, 254), 0, 0, false, SR4K_LayerLight);

  var a = rgb->GetAlpha();

  if(a < Flashlight_MinAlpha)
    return -1;

  if(--EffectVar(1, pTarget, iNr) <= 0)
    rgb->DoAlpha(-5, distAlpha, 255);
  else
    rgb->DoAlpha(+18, Min(rgb->GetAlpha()+18, distAlpha), 255);

  if(!rgb)
    return -1;

  a = rgb->GetAlpha();

  if(!GetEffect("IntFlashbang", pTarget) && !Contained())
  {
    for(var i = 0; i < GetPlayerCount(); i++)
    {
      if(i == GetOwner(pTarget))
      continue;

      var pCursor = GetCursor(GetPlayerByIndex(i))->~GetRealCursor();
      if(!pCursor && !(pCursor = GetCursor(GetPlayerByIndex(i))))
        continue;

      if(Contained(pCursor))
        continue;

      var srgb = GetScreenRGB(GetPlayerByIndex(i), SR4K_LayerLight, pCursor);

      if(srgb && srgb->GetAlpha() < 50)
      {
        CustomMessage("@", pTarget, GetPlayerByIndex(i));
        continue;
      }

      srgb = GetScreenRGB(GetPlayerByIndex(i), SR4K_LayerSmoke, pCursor);

      if(srgb && srgb->GetAlpha() < 200) 
      {
        CustomMessage("@", pTarget, GetPlayerByIndex(i));
        continue;
      }

      CustomMessage(Format("<c %x>{{SM07}}</c>", RGBa(255,255,255,BoundBy(255 - a, 1, 254))), pTarget, GetPlayerByIndex(i));
    }
  }

  return true;
}

public func FxFlashlightBlindnessRefresh(object pTarget, int iNr, object pUser, int iBlindDistance)
{
  var iDistAlpha = Max(Flashlight_MinAlpha, (Flashlight_MaxAlpha * Distance(GetX(pTarget), GetY(pTarget), GetX(pUser), GetY(pUser))) / iBlindDistance);
  if(EffectVar(2, pTarget, iNr) &&
    EffectVar(2, pTarget, iNr) != pUser &&
    EffectVar(1, pTarget, iNr) >= 0 &&
    iDistAlpha > EffectVar(4, pTarget, iNr)
    )
    return false;
  EffectVar(1, pTarget, iNr) = 6;
  EffectVar(2, pTarget, iNr) = pUser;
  EffectVar(3, pTarget, iNr) = iBlindDistance;
  EffectVar(4, pTarget, iNr) = iDistAlpha;
  return true;
}

public func FxFlashlightBlindnessStop(object pTarget, int iNr)
{
  if(EffectVar(0, pTarget, iNr))
    RemoveObject(EffectVar(0, pTarget, iNr));

  return true;
}
