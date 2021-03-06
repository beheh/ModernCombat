/*-- Geb�udemen� �ber Kontext --*/

#strict 2

#appendto HZCK


/* Kontexteintrag */

protected func ContextBuilding(object pCaller)
{
  [$CtxBuilding$|Image=CXCN]

  //Clonk anhalten
  SetComDir(COMD_Stop, pCaller);
  //Men� �ffnen
  CreateMenu(CBAS, pCaller, this, 0, 0, 0, C4MN_Style_Context, false);
  //Bauen
  AddMenuItem("$BuildBuilding$", "OpenBuildingMenu", CXCN, pCaller);
  //ToDo: Sell und Repair sollen deaktiviert/ausgegraut erscheinen, wenn nichts zu reparieren/verkaufen da ist.
  //Reparatur
  AddMenuItem("$RepairBuilding$", "OpenRepairMenu", BWTH, pCaller);
  //Verkauf
  AddMenuItem("$SellBuilding$", "OpenSellMenu", EFLN, pCaller);

  return true;
}

/* Techlevel-Men�icon-IDs */

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
    if(obj && GetCon(obj) >= 100)
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
  
  //Geb�ude auflisten
  var entry;
  for(var building in buildings)
  {
    var clr = 0xFFFFFF;
    if(!CanBuild(building, pTarget))
      clr = 0xFF0000;

    AddMenuItem(Format("<c %x>%s</c>", clr, GetName(0, building)), Format("StartBuildingPreviewMode(%i, Object(%d), %d)", building, ObjectNumber(pTarget), entry++), building, pTarget);
  }
  
  var pos, lvl = iLevel;
  while(iLevel/=2)
    pos++;

  //Zur�ck
  if(lvl != TECHLEVEL_Start)
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
  
  if(GetTeamTechLevel(GetPlayerTeam(GetOwner(pTarget)), TECHLEVEL_Start))
    return OpenTechLevelMenu(TECHLEVEL_Start, pTarget);
  
  //Technikstufe ausw�hlen
  CreateMenu(CBAS, pTarget, this, C4MN_Extra_Value, 0, 0, C4MN_Style_Context, 0, BGRS);
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
    DecoMessage("", GetOwner(pMenuObj)+1);

    //Bauradiusvorschau l�schen
    for(var obj in FindObjects(Find_ID(BGRS), Find_Owner(GetOwner(pMenuObj))))
      if(obj)
        RemoveObject(obj);

    RemoveEffect("PreviewBuilding", this);
  }

  return false;
}

public func OnMenuSelection(int iIndex, object pObj)
{
  //Techstufenauswahl
  if(GetMenu(pObj) == BGRS)
  {
    var lvl = TECHLEVEL_1;
    while(iIndex)
    {
      lvl *= 2;
      iIndex--;
    }

    //Nachricht mit Geb�ude�bersicht generieren
    var str = "@$MsgTechLevelBuildings$", buildings = GetTechLevelBuildings(lvl), start = true;
    for(var building in buildings)
    {
      var comma = ", ";
      if(start)
      {
        comma = "";
        start = false;
      }
      var clr = 0xFFFFFF;
      if(!CanBuild(building, this))
        clr = 0xFF0000;

      str = Format("%s%s{{%i}} <c %x>%s</c>", str, comma, building, clr, GetName(0, building));
    }

    DecoMessage(str, GetOwner(pObj)+1);
    return true;
  }

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

    //Nachricht mit Geb�udedetails generieren
    if(!b)
      DecoMessage("", GetOwner(pObj)+1);
    else
      BuildingDecoMessage(b, pObj);
  }
  else
    return _inherited(...);

  return true;
}

public func BuildingDecoMessage(id idBuilding, object pObj)
{
  var b = idBuilding;

  var clr = 0x00FF00, error = -1;
  if((error = CheckBuild(b, this, error)) != -1)
    clr = 0xFF0000;

  var str = Format("<c %x>%s</c>||%s", clr, GetName(0, b), GetDesc(0, b)), info = "";

  Concat(info, Format("{{MNYS}} %d ", GetValue(0, b)));
  if(b->~MaxDamage())
    Concat(info, Format("{{SM12}} %d ", b->MaxDamage()));
  if(b->~IsBase())
    Concat(info, Format("{{HUT1}} $True$ "));
  if(b->~ProvideTechLevel())
  {
    var lvl = -1, i = b->ProvideTechLevel();
    while(i)
    {
      i /= 2;
      lvl++;
    }
    Concat(info, Format("{{SM01}} %d ", lvl));
  }
  if(b->~BuildingRadius())
    Concat(info, Format("{{CXCN}} %d ", b->BuildingRadius()));
  if(b->~RequiredEnergy())
    Concat(info, Format("{{CXEC}} %d ", b->RequiredEnergy()));
  if(b->~EnergyProduction())
    Concat(info, Format("{{ENRG}} %d ", b->EnergyProduction()));

  var errorstr = BuildingErrors(b, this, error);
  if(GetLength(errorstr))
    errorstr = Format("||<c ff0000>%s</c>", errorstr);

  str = Format("@%s%s||%s", str, errorstr, info);
  DecoMessage(str, GetOwner(pObj)+1, Format("%i", b));
  return true;
}

public func BuildingErrors(id idBuilding, object pTarget, int iError)
{
  if(iError == BERROR_WrongBuildingID) return "$ErrWrongBuildingID$";
  if(iError == BERROR_NotEnoughMoney)  return "$ErrNotEnoughMoney$";
  if(iError == BERROR_NotAvailable)    return "$ErrNotAvailable$";
  if(iError == BERROR_BuildingRadius)  return "$ErrBuildingRadius$";
  if(iError == BERROR_NotEnoughSpace)  return "$ErrNotEnoughSpace$";
  if(iError == BERROR_SpecialError)    return idBuilding->~BuildingConditions(pTarget, GetX(), GetY()+10-GetDefHeight(idBuilding)/2, true);

  return "";
}

public func Concat(string &szStr, string szStr2)
{
  szStr = Format("%s%s", szStr, szStr2);
  return true;
}

public func FxPreviewBuildingStart(object pTarget, int iNr, temp)
{
  if(temp) return;

  var tim = EffectVar(0, pTarget, iNr) = CreateObject(TIM1, 0, 0, GetOwner(pTarget));
  tim->SetVisibility(VIS_Owner);
}

public func FxPreviewBuildingTimer(object pTarget, int iNr, int iTime)
{
  var tim = EffectVar(0, pTarget, iNr);

  //Anstatt Clonks ins Nichts zu schie�en, lieber Vorschau abbrechen (bspw. bei Grenzobjekten) -> noch ordentlich fixen! 
  if(!tim)
    return -1;
  SetPosition(GetX()+EffectVar(1, pTarget, iNr), GetY()+EffectVar(2, pTarget, iNr), tim);

  if(!EffectVar(3, pTarget, iNr) || !EffectVar(3, pTarget, iNr)->~IsCMCBuilding())
    return true;

  if(!(iTime % 5))
    BuildingDecoMessage(EffectVar(3, pTarget, iNr), this);

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
  DecoMessage("", GetOwner(pTarget)+1);

  if(EffectVar(0, pTarget, iNr))
    RemoveObject(EffectVar(0, pTarget, iNr));

  return true;
}

static const BERROR_WrongBuildingID = 1;
static const BERROR_NotEnoughMoney = 2;
static const BERROR_NotAvailable = 3;
static const BERROR_BuildingRadius = 4;
static const BERROR_NotEnoughSpace = 5;
static const BERROR_SpecialError = 6;

//iError muss -1 sein, damit gespeichert wird
public func CanBuild(id idBuilding, object pTarget, int iError)
{
  if(!idBuilding || !idBuilding->~IsCMCBuilding())
  {
    if(iError)
      iError = BERROR_WrongBuildingID;
    return iError;
  }

  //Verf�gbares Geld
  if(GetValue(0, idBuilding) > GetWealth(GetOwner(pTarget)))
  {
    if(iError)
      iError = BERROR_NotEnoughMoney;
    return iError;
  }

  //Freigeschaltet
  if(!GetTeamTechLevel(GetPlayerTeam(GetOwner(pTarget)), idBuilding->~TechLevel()))
  {
    if(iError)
      iError = BERROR_NotAvailable;
    return iError;
  }

  if(iError)
    return -1;

  return true;
}

public func CheckBuild(id idBuilding, object pTarget, int iError)
{
  var err = CanBuild(idBuilding, pTarget, iError);
  if(!err && !iError)
    return false;
  if(err != -1 && iError)
    return err;

  //Bauradius
  if(idBuilding->~NeedBuildingRadius())
    if(!FindObject2(Find_Category(C4D_Structure), Find_Allied(GetOwner(pTarget)), Find_Func("CheckBuildingRadius", GetX(), GetY()+10)))
    {
      if(iError)
        iError = BERROR_BuildingRadius;
      return iError;
    }

  //Gen�gend freier Platz
  Var(0) = GetX(); Var(1) = GetY();
  //FindConstructionSite funktioniert nicht mit Clonk-Calls
  if(!FindObject2(Find_ID(BGRL))->FindConstructionSite(idBuilding, 0, 1))
  {
    if(iError)
      iError = BERROR_NotEnoughSpace;

    return iError;
  }
  if(GetX() != Var(0) || !Inside(Var(1), GetY(), GetY()+10))
  {
    if(iError)
      iError = BERROR_NotEnoughSpace;

    return iError;
  }

  //Geb�udespezifische Anforderungen
  if(!idBuilding->~BuildingConditions(pTarget, GetX(), GetY()+10-GetDefHeight(idBuilding)/2))
  {
    if(iError)
      iError = BERROR_SpecialError;
    return iError;
  }

  if(iError)
    return -1;

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

/* Verkaufsmen� */

public func OpenSellMenu(dummy, object pMenuObj, object pBuilding) 
{
  //Objekte z�hlen
  var count = ObjectCount2(Find_AtPoint(), Find_Func("IsCMCBuilding"), Find_Allied(GetOwner()), Find_OCF(OCF_Fullcon), Find_Not(Find_Func("CanNotBeSold")));
  
  //Kein passendes Objekt gefunden
  if(!count)
    return PlayerMessage(GetOwner(), "$NoObjectToSell$", this);

  //Geb�ude �berschneiden sich? Auswahlmen� �ffnen
  if(!pBuilding && count > 1)
    return OpenSellChooseBuildingMenu();
  
  if(!pBuilding)
    pBuilding = FindObject2(Find_AtPoint(), Find_Func("IsCMCBuilding"), Find_Allied(GetOwner()), Find_OCF(OCF_Fullcon), Find_Not(Find_Func("CanNotBeSold")));
  
  //Sicherheitsabfrage
  CreateMenu(EFLN, this, this, 0, "$SellMenu$", 0, C4MN_Style_Context, 0, NULL);
  
  AddMenuItem(Format("$SellBuildingInfo$", GetName(pBuilding), GetValue(pBuilding)/2), 0, GetID(pBuilding), this);
  AddMenuItem("$Yes$", "SellBuilding", NONE, this, 0, pBuilding, ""); //Geb�ude verkaufen
  AddMenuItem("$No$", "", NONE, this); //Men� abbrechen
  
  return true;
}

public func OpenSellChooseBuildingMenu(object pMenuObj)
{
  CreateMenu(EFLN, this, this, 0, "$SellMenu$", 0, C4MN_Style_Context, 0, NULL);
  
  //Geb�ude auflisten
  for(var obj in FindObjects(Find_AtPoint(), Find_Func("IsCMCBuilding"), Find_Allied(GetOwner()), Find_OCF(OCF_Fullcon)))
    AddMenuItem(GetName(obj), Format("OpenSellMenu(0, 0, Object(%d))", ObjectNumber(obj)), GetID(obj), this, GetValue(obj)/2);

  return true;
}

public func SellBuilding(dummy, object pBuilding)
{
  //Ersatzbeh�lter suchen
  var container = FindObject2(Find_ID(GetID(pBuilding)), Find_Allied(GetOwner()), Find_Exclude(pBuilding), Find_OCF(OCF_Fullcon));

  //Kein Ersatzgeb�ude gefunden? Dann Kiste daf�r erstellen
  if(!container)
  {
    container = CreateObject(SPCT, 0, 0, GetOwner());
    container->Set(FindObject2(Find_ID(CBAS), Find_Owner(GetOwner())));
  }

  //Inhalte evakuieren
  var content;
  while(content = Contents(0, pBuilding))
  {
    //Lebewesen und Fahrzeuge rausschciken
    if(GetCategory(content) & C4D_Living || GetCategory(content) & C4D_Vehicle)
      content->Exit();
    //Sonstiges in Ersatzbeh�lter verschieben
    else
      Enter(container, content);
  }

  AddEffect("AutoSell", pBuilding, 10, 1, 0, GetID(), GetOwner());
  
  return true;
}

public func FxAutoSellStart(object pTarget, int iNr, int iTemp, int iPlr)
{
  if(iTemp)
    return;

  //Ger�ste aufstellen
  EffectVar(0, pTarget, iNr) = CreateBuildScaffolds(pTarget);
  EffectVar(1, pTarget, iNr) = iPlr;

  //(Sound-)Effekte
  
  return true;
}

public func FxAutoSellTimer(object pTarget, int iNr)
{
  //Geb�udeabbau
  DoCon(-1, pTarget);
  
  //Abbaueffekte
  
  return true;
}

public func FxAutoSellStop(object pTarget, int iNr)
{
  var plr = EffectVar(1, pTarget, iNr);

  //Ger�ste abbauen
  RemoveBuildScaffolds(EffectVar(0, pTarget, iNr));
  
  //Geld zur�ckzahlen
  SetWealth(plr, GetWealth(plr)+(GetValue(pTarget)/2));
  
  return true;
}

/* Geb�udebau starten */

public func StartBuilding(id idBuilding, object pTarget, int selection)
{
  for(var obj in FindObjects(Find_ID(BGRS), Find_Owner(GetOwner(pTarget))))
    if(obj)
      RemoveObject(obj);
  RemoveEffect("PreviewBuilding", this);

  if(!CheckBuild(idBuilding, pTarget)) //ToDo: Meldung ausgeben
    return false;

  //Geb�udebaustelle erstellen
  var pBuilding = CreateConstruction(idBuilding, 0, 10, GetOwner(pTarget), 1, true, true);
  if(!pBuilding)
    return;
  
  //Baugebiet planieren
  DigFreeRect(GetX(pBuilding)+GetDefOffset(idBuilding), GetY(pBuilding)+(GetObjHeight(pBuilding)+(GetDefOffset(idBuilding, 1)*GetCon(pBuilding)/100))-GetDefHeight(idBuilding), GetDefWidth(idBuilding), GetDefHeight(idBuilding)+2);

  //Gras im Baugebiet entfernen
  var wdt = GetDefWidth(idBuilding), hgt = GetDefHeight(idBuilding);
  for(var grass in pBuilding->FindObjects(Find_Or(Find_ID(GRAS), Find_ID(BSH2)), pBuilding->Find_AtRect(-wdt/2, -hgt, wdt, hgt)))
  {
    grass->~DoDmg(150);
    if(grass)
      grass->~OnShockwaveHit();
  }

  //Geld abziehen
  DoWealth(GetOwner(pTarget), -GetValue(0, idBuilding));

  //Baueffekt �bergeben
  AddEffect("AutoBuild", pBuilding, 10, 1);

  //Effekte
  CastSmoke("Smoke3",20,40,-10,0,100,200);
  CastSmoke("Smoke3",20,40,10,0,100,200);
  Sound("Building_Place.ogg",false,pBuilding);

  return true;
}

/* Bau-Effekt (by henry4k) */

global func FxAutoBuildStart(object pTarget, int iNo, int iTemp)
{
  //Effekt
  Sound("SF_Build", 0, pTarget, 0, 0, +1);
}

global func FxAutoBuildTimer(object pTarget, int iNo, int iTime)
{
  var con = GetCon(pTarget);

  //Abbruch sobald fertiggestellt
  if(con >= 100) return -1;

  if(iTime % 10) return;

  //Geb�ude hochziehen
  DoCon(+1, pTarget);
  con++;

  var x = 0;
  var y = -GetDefHeight(GetID(pTarget)) * con / 100 / 2;
  var w = GetObjWidth(pTarget);

  //Effekte
  for(var i = 10; i > 0; i--)
  {
    x = Random(w) - w / 2;
    pTarget->CreateParticle("Frazzle", x, y, RandomX(-20, 20), RandomX(-20, 20), 30, HSL(60, 150, RandomX(150, 250)), pTarget);
  }
}

global func FxAutoBuildStop(object pTarget, int iNo, int iReason)
{
  //Effekt
  Sound("SF_Build", 0, pTarget, 0, 0, -1);
}