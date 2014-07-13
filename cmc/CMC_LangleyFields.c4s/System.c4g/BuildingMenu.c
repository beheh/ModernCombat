/*-- Gebäudemenü über Kontext --*/

#strict 2

#appendto HZCK


/* Kontexteintrag */

protected func ContextBuilding(object pCaller)
{
  [$CtxBuilding$|Image=CXCN]
  SetXDir(0, pCaller);
  SetComDir(COMD_Stop, pCaller);
  CreateMenu(CBAS, pCaller, this, 0, 0, 0, C4MN_Style_Context, false);

  //Bauen
  AddMenuItem("$BuildBuilding$", "OpenBuildingMenu", FLNT, pCaller);
  //ToDo: Sell und Repair sollen deaktiviert/ausgegraut erscheinen, wenn nichts zu reparieren/verkaufen da ist.
  //Reparatur
  AddMenuItem("$RepairBuilding$", "OpenRepairMenu", BWTH, pCaller);
  //Verkauf
  AddMenuItem("$SellBuilding$", "OpenSellMenu", EFLN, pCaller);

  return true;
}

public func OpenBuildingMenu(dummy, object pTarget, int iSel)
{
  AddEffect("PreviewBuilding", this, 100, 1, this);
  CreateMenu(CBAS, pTarget, this, C4MN_Extra_Value, 0, 0, C4MN_Style_Context, 0, BGRS);
  var plr = GetOwner(pTarget);

  for(var obj in FindObjects(Find_Category(C4D_Structure), Find_Allied(plr), Find_Func("BuildingRadius")))
    if(obj)
    {
      var radiusObj = CreateObject(BGRS, 0, 0, plr), size = obj->~BuildingRadius() * 1000 / 50;
      radiusObj->SetAction("Be", obj);
      radiusObj->SetObjDrawTransform(size, 0, 0, 0, size, 0);
      radiusObj->SetVisibility(VIS_Owner);

      var wdt = GetObjWidth(obj), hgt = GetObjHeight(obj), offx = GetObjectVal("Offset", 0, obj, 0), offy = GetObjectVal("Offset", 0, obj, 1);
      radiusObj->SetVertex(0, 0, wdt/2+offx+GetVertex(0, 0, obj));
      radiusObj->SetVertex(0, 1, hgt/2+offy+GetVertex(0, 1, obj));
    }

  var buildings = [], def = 0, i = 0;
  while(def = GetDefinition(++i, C4D_Structure))
    if(def->~IsCMCBuilding())
    {
      var lvl = def->~TechLevel();
      if(!buildings[lvl])
        buildings[lvl] = [];

      buildings[lvl][GetLength(buildings[lvl])] = def;
    }

  var entry;
  for(var lvl = 0; lvl < GetLength(buildings); lvl++)
  {
    if(!buildings[lvl])
      continue;
    AddMenuItem(Format("$TechLevel$", lvl), 0, 0, pTarget);
    entry++;

    for(var building in buildings[lvl])
    {
      var clr = 0xFFFFFF;
      if(!CanBuild(building, pTarget))
        clr = 0xFF0000;

      AddMenuItem(Format("<c %x>%s</c>", clr, GetName(0, building)), Format("StartBuildingPreviewMode(%i, Object(%d), %d)", building, ObjectNumber(pTarget), entry++), building, pTarget);
    }
  }

  SelectMenuItem(iSel, pTarget);

  return true;
}

local fBuildingPreview, idBuildingPreview, pBuildingPreviewTarget, iBuildingPreviewEntry;

public func StartBuildingPreviewMode(id idBuilding, object pTarget, int iEntry)
{
  fBuildingPreview = true;
  idBuildingPreview = idBuilding;
  pBuildingPreviewTarget = pTarget;
  iBuildingPreviewEntry = iEntry;

  return true;
}

public func ControlThrow()
{
  if(fBuildingPreview)
  {
    fBuildingPreview = false;
    StartBuilding(idBuildingPreview, pBuildingPreviewTarget, iBuildingPreviewEntry);
    return true;
  }
  return _inherited(...);
}

public func ControlDig()
{
  if(fBuildingPreview)
  {
    for(var obj in FindObjects(Find_ID(BGRS), Find_Owner(GetOwner(pBuildingPreviewTarget))))
      if(obj)
        RemoveObject(obj); 

    RemoveEffect("PreviewBuilding", this);
    fBuildingPreview = false;
    OpenBuildingMenu(0, pBuildingPreviewTarget, iBuildingPreviewEntry);
    return true;
  }

  return _inherited(...);
}

public func MenuQueryCancel(int iSelection, object pMenuObj)
{
  if(GetMenu(pMenuObj) == BGRS)
  {
    for(var obj in FindObjects(Find_ID(BGRS), Find_Owner(GetOwner(pMenuObj))))
      if(obj)
        RemoveObject(obj);

    RemoveEffect("PreviewBuilding", this);
  }

  return false;
}

public func OnMenuSelection(int iIndex, object pMenuObject)
{
  var buildings = [], def = 0, i = 0;
  while(def = GetDefinition(++i, C4D_Structure))
    if(def->~IsCMCBuilding())
    {
      var lvl = def->~TechLevel();
      if(!buildings[lvl])
        buildings[lvl] = [];

      buildings[lvl][GetLength(buildings[lvl])] = def;
    }
  
  var entry = 0, b;
  for(var lvl = 0; lvl < GetLength(buildings); lvl++)
  {
    if(!buildings[lvl])
      continue;

    entry++;

    for(var building in buildings[lvl])
    {
      if(entry == iIndex)
      {
        b = building;
        break;
      }
      entry++;
    }
  }

  EffectCall(this, GetEffect("PreviewBuilding", this), "Change", b);
  return true;
}

public func FxPreviewBuildingStart(object pTarget, int iNr, temp)
{
  if(temp) return;

  var tim = EffectVar(0, pTarget, iNr) = CreateObject(TIM1, 0, 0, GetOwner(pTarget));
  tim->SetVisibility(VIS_Owner);
}

public func FxPreviewBuildingTimer(object pTarget, int iNr)
{
  var tim = EffectVar(0, pTarget, iNr);

  //Anstatt Clonks ins Nichts zu schießen, lieber Vorschau abbrechen (bspw. bei Grenzobjekten) -> noch ordentlich fixen! 
  if(!tim)
    return -1;
  SetPosition(GetX()+EffectVar(1, pTarget, iNr), GetY()+EffectVar(2, pTarget, iNr), tim);

  if(!CheckBuild(EffectVar(3, pTarget, iNr), this))
    SetClrModulation(RGB(255), tim, 1);
  else
    SetClrModulation(RGB(255, 255, 255), tim, 1);
  return true;
}

public func FxPreviewBuildingChange(object pTarget, int iNr, id idNewDef)
{
  var wdt = GetDefWidth(idNewDef), hgt = GetDefHeight(idNewDef);
  var offx = GetDefOffset(idNewDef), offy = GetDefOffset(idNewDef, 1);
  SetShape(offx, offy, wdt, hgt, EffectVar(0, pTarget, iNr));
  SetGraphics(0, EffectVar(0, pTarget, iNr), idNewDef, 1, GFXOV_MODE_Base);

  EffectVar(1, pTarget, iNr) = -(wdt/2+offx);
  EffectVar(2, pTarget, iNr) = -(hgt+offy)+10;
  EffectVar(3, pTarget, iNr) = idNewDef;
  return true;
}

public func FxPreviewBuildingStop(object pTarget, int iNr)
{
  if(EffectVar(0, pTarget, iNr))
    RemoveObject(EffectVar(0, pTarget, iNr));

  return true;
}

public func CanBuild(id idBuilding, object pTarget)
{
  //Verfügbares Geld
  if(GetValue(0, idBuilding) > GetWealth(GetOwner(pTarget)))
    return false;

  //Freigeschaltet
  if(!GetPlrKnowledge(GetOwner(pTarget), idBuilding) && GetTeamTechLevel(GetPlayerTeam(GetOwner(pTarget))) < idBuilding->~TechLevel())
    return false;

  return true;
}

public func CheckBuild(id idBuilding, object pTarget)
{
  if(!CanBuild(idBuilding, pTarget))
    return false;

  //Bauradius
  if(idBuilding->~NeedBuildingRadius())
    if(!FindObject2(Find_Category(C4D_Structure), Find_Allied(GetOwner(pTarget)), Find_Func("CheckBuildingRadius", GetX(), GetY()+10)))
      return false;

  //Genügend freier Platz
  Var(0) = GetX(); Var(1) = GetY();
  //FindConstructionSite funktioniert nicht mit Clonk-Calls
  if(!FindObject2(Find_ID(BGRL))->FindConstructionSite(idBuilding, 0, 1))
    return false;
  if(GetX() != Var(0) || !Inside(Var(1), GetY(), GetY()+10))
    return false;

  //Gebäudespezifische Anforderungen
  if(!idBuilding->~BuildingConditions(pTarget, GetX(), GetY()+10-GetDefHeight(idBuilding)/2))
    return false;

  return true;
}

global func CheckBuildingRadius(int iX, int iY)
{
  var rad;
  if(this->~GetCon() < 100)
    return false;

  if(!(rad = this->~BuildingRadius()))
    return false;

  return Distance(GetX(), GetY(), iX, iY) <= rad;
}

public func OpenRepairMenu()	{return true;}
public func OpenSellMenu()	{return true;}

public func StartBuilding(id idBuilding, object pTarget, int selection)
{
  for(var obj in FindObjects(Find_ID(BGRS), Find_Owner(GetOwner(pTarget))))
    if(obj)
      RemoveObject(obj);
  RemoveEffect("PreviewBuilding", this);

  if(!CheckBuild(idBuilding, pTarget)) //ToDo: Meldung ausgeben
    return false;

  //Gebäudebaustelle erstellen
  var pBuilding = CreateConstruction(idBuilding, 0, 10, GetOwner(pTarget), 1, true, true);
  if(!pBuilding)
    return;

  //Gras im Baugebiet entfernen
  var wdt = GetDefWidth(idBuilding), hgt = GetDefHeight(idBuilding);
  for(var grass in pBuilding->FindObjects(Find_ID(GRAS), pBuilding->Find_AtRect(-wdt/2, -hgt, wdt, hgt)))
    grass->OnShockwaveHit();

  //Geld abziehen
  DoWealth(GetOwner(pTarget), -GetValue(0, idBuilding));

  //Baueffekt übergeben
  AddEffect("AutoBuild", pBuilding, 10, 1);

  //Effekt
  Sound("Building_Place.ogg",false,pBuilding);
}

/* Bau-Effekt (by henry4k) */

global func FxAutoBuildStart(object pTarget, int iNo, int iTemp)
{
  Sound("SF_Build", 0, pTarget, 0, 0, +1);
}

global func FxAutoBuildTimer(object pTarget, int iNo, int iTime)
{
  var con = GetCon(pTarget);
  if(con >= 100) return -1;

  if(iTime % 10) return;

  DoCon(+1, pTarget);
  con++;

  var x = 0;
  var y = -GetDefHeight(GetID(pTarget)) * con / 100 / 2;
  var w = GetObjWidth(pTarget);

  for(var i = 10; i > 0; i--)
  {
    x = Random(w) - w / 2;
    pTarget->CreateParticle("Frazzle", x, y, RandomX(-20, 20), RandomX(-20, 20), 30, HSL(60, 150, RandomX(150, 250)), pTarget);
  }
}

global func FxAutoBuildStop(object pTarget, int iNo, int iReason)
{
  Sound("SF_Build", 0, pTarget, 0, 0, -1);
}
