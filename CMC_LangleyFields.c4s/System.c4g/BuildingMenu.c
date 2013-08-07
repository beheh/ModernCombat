/*-- Gebäudemenü über Kontext --*/

#strict 2

#appendto HZCK

protected func ContextBuilding(object pCaller)
{
	[$CtxBuilding$|Image=CXCN]
	CreateMenu(CBAS, pCaller, this, 0, 0, 0, C4MN_Style_Context, false);
	AddMenuItem("$BuildBuilding$", "OpenBuildingMenu", FLNT, pCaller);
	
	//ToDo: Sell und Repair sollen deaktiviert/ausgegraut erscheinen, wenn nichts zu reparieren/verkaufen da ist.
	AddMenuItem("$RepairBuilding$", "OpenRepairMenu", BWTH, pCaller);
	AddMenuItem("$SellBuilding$", "OpenSellMenu", EFLN, pCaller);
	
	AddMenuItem("$Close$", Format("CloseMenu(Object(%d))", ObjectNumber(pCaller)), PCMK, pCaller);
	
	return true;
}

public func OpenBuildingMenu(dummy, object pTarget)
{
	CreateMenu(CBAS, pTarget, this, C4MN_Extra_Value, 0, 0, C4MN_Style_Context);
	
	var buildings = [], def = 0, i = 0;
	while(def = GetDefinition(++i, C4D_Structure))
		if(def->~IsCMCBuilding())
		{
			var lvl = def->~TechLevel();
			if(!buildings[lvl])
				buildings[lvl] = [];
			
			buildings[lvl][GetLength(buildings[lvl])] = def;
		}
	
	for(var lvl = 0; lvl < GetLength(buildings); lvl++)
	{
		if(!buildings[lvl])
			continue;
		
		AddMenuItem(Format("$TechLevel$", lvl), 0, 0, pTarget);
		
		for(var building in buildings[lvl])
		{
			if(!GetPlrKnowledge(GetOwner(pTarget), building))
				continue;
			
			//Falls nicht baubar ausgrauen/rot färben (Check fehlt noch)
			AddMenuItem(GetName(0, building), "StartBuilding", building, pTarget);
		}
	}
	
	return true;
}

public func OpenRepairMenu() { return true; }
public func OpenSellMenu() { return true; }

public func StartBuilding(id idBuilding, object pTarget)
{
	var pBuilding = CreateConstruction(idBuilding, 0, 10, GetOwner(pTarget), 1, true, true);
	if(!pBuilding)
		return;

	AddEffect("AutoBuild", pBuilding, 10, 1);
}

/* Bau-Effekt (by henry4k) */

global func FxAutoBuildTimer(object pTarget, int iNo, int iTemp)
{
  Sound("SF_Build", 0, pTarget, 0, 0, 1);
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
