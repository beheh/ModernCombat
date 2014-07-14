/*-- Gebäudemenü über Kontext --*/

#strict 2

#appendto HZCK


/* Kontexteintrag */

protected func ContextBuilding(object pCaller)
{
  [$CtxBuilding$|Image=CXCN]

  //Clonk anhalten
  SetComDir(COMD_Stop, pCaller);
  //Menü öffnen
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

/* Techlevel-Menüicon-IDs */

public func TechLevelMenuID(int iLevel)
{
  if(iLevel == TECHLEVEL_Start)	return ERTH;
  if(iLevel == TECHLEVEL_1)		return LOAM;
  if(iLevel == TECHLEVEL_2)		return ROCK;
  if(iLevel == TECHLEVEL_3)		return FLNT;

  return NONE;
}

public func GetTechLevelByID(id idMenuID)
{
  var lvl = 1;
  while(lvl > 0)
  {
    if(TechLevelMenuID(lvl) == idMenuID)
      return lvl;

    lvl *= 2;
  }
  return false;
}

/* Bauradienanzeige */

public func CreateBuildingRadiusPreview(object pTarget)
{
  var plr = GetOwner(pTarget);

  for(var obj in FindObjects(Find_ID(BGRS), Find_Owner(GetOwner(pTarget))))
    if(obj)
      RemoveObject(obj);

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
  
  return true;
}

public func GetTechLevelBuildings(int iLevel)
{
  var buildings = [], def = 0, i = 0;
  while(def = GetDefinition(++i, C4D_Structure))
    if(def->~IsCMCBuilding() && def->TechLevel() == iLevel)
      buildings[GetLength(buildings)] = def;

  return buildings;
}

public func OpenTechLevelMenu(int iLevel, object pTarget, int iSel)
{
  if(!GetEffect("PreviewBuilding", this))
    AddEffect("PreviewBuilding", this, 100, 1, this);

  var buildings = GetTechLevelBuildings(iLevel);
  
  CreateMenu(CBAS, pTarget, this, C4MN_Extra_Value, 0, 0, C4MN_Style_Context, 0, TechLevelMenuID(iLevel));
  
  //Gebäude auflisten
  var entry;
  for(var building in buildings)
  {
    var clr = 0xFFFFFF;
    if(!CanBuild(building, pTarget))
      clr = 0xFF0000;

    AddMenuItem(Format("<c %x>%s</c>", clr, GetName(0, building)), Format("StartBuildingPreviewMode(%i, Object(%d), %d)", building, ObjectNumber(pTarget), entry++), building, pTarget);
  }
  
  var pos;
  while(iLevel/=2)
    pos++;

  //Zurück
  AddMenuItem("$Back$", Format("OpenBuildingMenu(0, Object(%d), %d)", ObjectNumber(pTarget), pos), NONE, pTarget);
  
  SelectMenuItem(iSel, pTarget);
  
  return true;
}

public func OpenBuildingMenu(dummy, object pTarget, int iSel)
{
  //Vorschauobjekte/-effekte erstellen
  CreateBuildingRadiusPreview(pTarget);
  if(!GetEffect("PreviewBuilding", this))
    AddEffect("PreviewBuilding", this, 100, 1, this);
  
  //Technikstufe auswählen
  CreateMenu(CBAS, pTarget, this, C4MN_Extra_Value, 0, 0, C4MN_Style_Context, 0, BGRS);
  AddMenuItem("$TechLevelStart$", Format("OpenTechLevelMenu(%d, Object(%d))", TECHLEVEL_Start, ObjectNumber(this)), NONE, this);
  AddMenuItem("$TechLevel1$", Format("OpenTechLevelMenu(%d, Object(%d))", TECHLEVEL_1, ObjectNumber(this)), NONE, this);
  AddMenuItem("$TechLevel2$", Format("OpenTechLevelMenu(%d, Object(%d))", TECHLEVEL_2, ObjectNumber(this)), NONE, this);
  AddMenuItem("$TechLevel3$", Format("OpenTechLevelMenu(%d, Object(%d))", TECHLEVEL_3, ObjectNumber(this)), NONE, this);

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
    if(!CheckBuild(idBuildingPreview, this))
      return true;
  
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
    OpenTechLevelMenu(idBuildingPreview->~TechLevel(), pBuildingPreviewTarget, iBuildingPreviewEntry);
    return true;
  }

  return _inherited(...);
}

public func MenuQueryCancel(int iSelection, object pMenuObj)
{
  if(GetMenu(pMenuObj) == BGRS || GetTechLevelByID(GetMenu(pMenuObj)))
  {
    //Bauradiusvorschau löschen
    for(var obj in FindObjects(Find_ID(BGRS), Find_Owner(GetOwner(pMenuObj))))
      if(obj)
        RemoveObject(obj);

    RemoveEffect("PreviewBuilding", this);
  }

  return false;
}

public func OnMenuSelection(int iIndex, object pObj)
{
  var lvl;
  if(lvl = GetTechLevelByID(GetMenu(pObj)))
  {
    var buildings = GetTechLevelBuildings(lvl), entry = 0, b;
    for(var building in buildings)
    {
      if(entry == iIndex)
      {
        b = building;
        break;
      }
      entry++;
    }

    EffectCall(this, GetEffect("PreviewBuilding", this), "Change", b);
  }
  else
    return _inherited(...);

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

  if(!EffectVar(3, pTarget, iNr) || !EffectVar(3, pTarget, iNr)->~IsCMCBuilding())
    return true;

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
  if(!idBuilding || !idBuilding->~IsCMCBuilding())
    return false;

  //Verfügbares Geld
  if(GetValue(0, idBuilding) > GetWealth(GetOwner(pTarget)))
    return false;

  //Freigeschaltet
  if(!GetPlrKnowledge(GetOwner(pTarget), idBuilding) && !GetTeamTechLevel(GetPlayerTeam(GetOwner(pTarget)), idBuilding->~TechLevel()))
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