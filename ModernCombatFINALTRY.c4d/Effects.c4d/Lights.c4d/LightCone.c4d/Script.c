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

public func Draw()
{
  var dir;
  if(!bDir) dir = 1;
  else      dir = 1-GetDir(GetActionTarget())*2;

  var alphamod, sizemod;
  CalcLight(alphamod, sizemod);

  if(IsLocked())
    sizemod = 100;

  var fsin, fcos, xoff, yoff;
  fsin = Sin(iR, 1000);
  fcos = Cos(iR, 1000);
  xoff = 0;
  yoff = +64;

  var width = iSizeX*sizemod/100, height = iSizeY*sizemod/100;

  SetObjDrawTransform(
    +fcos*width/1000, +fsin*height/1000, (((1000-fcos)*xoff - fsin*yoff)*height)/1000+iOffX*1000*dir,
    -fsin*width/1000, +fcos*height/1000, (((1000-fcos)*yoff + fsin*xoff - 64000)*height)/1000+iOffY*1000
  );
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
      tag->~RefreshRemoveTimer();
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
  EffectVar(1, pTarget, iNr) = 6;
}

static const Flashlight_MinAlpha = 100;
static const Flashlight_MaxAlpha = 200;

public func FxFlashlightBlindnessTimer(object pTarget, int iNr)
{
  var rgb = EffectVar(0, pTarget, iNr);
  var pUser = EffectVar(2, pTarget, iNr);
  var iBlindDistance = EffectVar(3, pTarget, iNr);
  var distAlpha = EffectVar(4, pTarget, iNr) = Max(Flashlight_MinAlpha, (Flashlight_MaxAlpha * Distance(GetX(pTarget), GetY(pTarget), GetX(pUser), GetY(pUser))) / iBlindDistance);

  if(!rgb)
    rgb = EffectVar(0, pTarget, iNr) = ScreenRGB(pTarget, RGBa(255, 255, 255, 254), 0, 0, false, SR4K_LayerLight);

  if(rgb->GetAlpha() < Flashlight_MinAlpha)
    return;

  if(--EffectVar(1, pTarget, iNr) <= 0)
  {
    if(GetEffect("IntFlashbang", pTarget))
      return;

    rgb->DoAlpha(-5, distAlpha, 255);
  }
  else
    rgb->DoAlpha(+18, Min(rgb->GetAlpha()+18, distAlpha), 255);

  if(!rgb)
    return -1;

  if(!GetEffect("IntFlashbang", pTarget))
  {
    if(!Contained() && rgb)
    {
      var a = rgb->~GetAlpha(), c;
      for(var i = 0; i < GetPlayerCount(); i++)
      {
        var pCursor = GetCursor(GetPlayerByIndex(i));
        if(!pCursor)
        	continue;
        
        pCursor = pCursor->~GetRealCursor();
        if(!pCursor && !(pCursor = GetCursor(GetPlayerByIndex(i))))
          continue;

        if(Contained(pCursor))
          continue;

        var srgb = GetScreenRGB(GetPlayerByIndex(i), SR4K_LayerLight, pCursor);
        var val;

        if(srgb)
          val = srgb->~GetAlpha();

        if(val && 255-a >= val)
          val = 255 - val;
        else
          val = 255 - a;

        var flag = 0;
        if(c != 0)
          flag = MSG_Multiple;

        CustomMessage(Format("<c %x>{{SM07}}</c>", RGBa(255,255,255,BoundBy(val, 1, 254))), pTarget, GetPlayerByIndex(i), 0, 0, 0, 0, 0, flag); 
        c++;
        }
      }
      else
        Message("@", pTarget); 
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