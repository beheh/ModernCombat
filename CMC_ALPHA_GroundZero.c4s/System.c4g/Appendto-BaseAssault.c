/*-- Neues Script --*/

#strict 2

//- Mehrere Bombenspawnpoints, aus denen zufällig ausgewählt wird

#appendto GBAS

global func SetupBombSpawnpoint()
{
	if(FindObject(GBAS))
		return FindObject(GBAS)->~SetupBombSpawnpoint(...);
	
	return false;
}

global func PlaceBombSpawnpoint()
{
	if(FindObject(GBAS))
		return FindObject(GBAS)->~PlaceBombSpawnpoint(...);
	
	return false;
}

local bombSpawnX, bombSpawnY;

public func UpdateScoreboard() { return; }

public func SetupBombSpawnpoint(int iX, int iY)
{
	bombSpawnX = iX;
  bombSpawnY = iY;
  PlaceBombSpawnpoint();
	return true;
}

public func PlaceBombSpawnpoint(int iX, int iY)
{
	if(!iX)
		iX = bombSpawnX;
	if(!iY)
		iY = bombSpawnY;
	
  PlaceSpawnpoint(C4P2, AbsX(iX), AbsY(iY), 15);
  return true;
}

public func PlantingCondition(object pTarget, object pAssaultTarget)
{
	if(GetEffect("BaseAssaultBomb", pTarget) || EffectVar(2, pAssaultTarget, GetEffect("IntAssaultTarget", pAssaultTarget)) > 1)
		return true;
	
	return false;
}

public func OnPlantingComplete(array aAttackers)
{
	for(var clonk in aAttackers)
		if(GetEffect("BaseAssaultBomb", clonk))
			RemoveEffect("BaseAssaultBomb", clonk);
	
	return true;
}

public func OnDefusingComplete(array aDefenders)
{
	var fBomb = false;
	for(var clonk in aDefenders)
		if(clonk && !(clonk->~IsFakeDeath()))
		{
			fBomb = true;
			C4P2->AddBombObject(clonk);
			break;
		}
	
	if(!fBomb)
		PlaceBombSpawnpoint();
	/*if(GetLength(aDefenders) > 0)
		C4P2->AddBombObject(aDefenders[0]);*/
	
	return true;
}

public func ReportAssaultTargetDestruction()
{
	EventInfo4K(0, "$BombSpawnDelay$");
	ScheduleCall(this, "PlaceBombSpawnpoint", 35*10);
	return _inherited(...);
}


public func OpenRelaunchMenu(object pCrew, int iSelection, int iClass)
{
  //Sichtweite setzen
  SetPlrViewRange(200, pCrew);
  //Zwischendurch alle Ziele vernichtet? Stirb!
  if(!ObjectCount2(Find_InArray(aTargets[GetPlayerTeam(GetOwner(pCrew))])))
    return EliminatePlayer(GetOwner(pCrew));
  CloseMenu(pCrew);
  CreateMenu(GBAS, pCrew, this, 0, "$ChoosePoint$", 0, 3, 0, GBAS);
  //Alle vorhandenen Ziele ins Menü setzen
  var array = FindObjects(Find_InArray(aTargets[GetPlayerTeam(GetOwner(pCrew))]));
  SortTargets(array);
  for(var obj in array)
  {
    var id = GetID(obj);
    if(id == AHBS) id = obj->GetImitationID();
    AddMenuItem(GetName(obj), Format("DoRelaunch(Object(%d), Object(%d), %d)", ObjectNumber(pCrew), ObjectNumber(obj), iClass), id, pCrew, 0, 0, GetName(obj));
  }
  SelectMenuItem(iSelection, pCrew);
}
