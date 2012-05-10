/*-- MTP --*/

#strict 2
#appendto AMPK
// Script kann 1:1 ins MTP-Script kopiert werden.
// Dafür muss nur die Zeile:
// #appendto AMPK
// in:
// #include PACK
// umgeändert werden.

public func IsDrawable()		{return true;}
public func HandX()			{return 4000;}
public func HandY()			{return 10;}
public func HandSize()			{return 1000;}

public func StartPoints()		{return 200;}
public func MaxPoints()			{return 200;}
public func RefillTime()		{return 20;}
public func TeamSupportTime()		{return 60;}

public func IsEquipment()       	{return !NoAmmo();}
public func MinValue()			{return 49;}
public func AI_Inventory(object pClonk)	{return true;}

local aPages, iLastPage;
static const AMPK_PrevPage = 1;
static const AMPK_NextPage = 2;

/* Initialisierung */

protected func Initialize()
{
  //Regel "Keine Munition" - Wird nicht gebraucht
  if (NoAmmo())
    return ScheduleCall(this, "RemoveObject", 1);
  
  aPages = [];
  var cnt = GetLength(AmmoTypes()); var i = 0;
  while(i < cnt)
  {
  	var left = AMPK_PrevPage;
  	var right = AMPK_NextPage;
  	var up = AmmoTypes()[i++];
  	if(cnt - i <= 3 + (!GetLength(aPages)))
  		right = AmmoTypes()[i++];
  	
  	var down = AmmoTypes()[i++];
  	
  	if(!GetLength(aPages))
  		left = AmmoTypes()[i++];
  	
  	var throw = AmmoTypes()[i++];
  	
  	aPages[GetLength(aPages)] = [up, right, down, left, throw];
  }
  
  return _inherited(...);
}

/* Munition entnehmen */

public func AmmoTypes()
{
  //[ID, Menge, Punkte].
  return [[STAM, 50, 50], [GRAM, 12, 60], [MIAM, 4, 60]];
}

protected func Activate(object pCaller)
{
  //Hat schon eine Box
  if (FindContents(CUAM, pCaller))
  {
    PlayerMessage(GetOwner(pCaller), "$NoSpace$", pCaller);
    return true;
  }

  //Falsche Aktion?
  if(!WildcardMatch(GetAction(pCaller), "*Walk*") && !WildcardMatch(GetAction(pCaller), "*Swim*") && !WildcardMatch(GetAction(pCaller), "*Crawl*") && !WildcardMatch(GetAction(pCaller), "*Jump*"))
  {
    PlayerMessage(GetOwner(pCaller), "$CantTake$", pCaller);
    return true;
  }

  //Clonk anhalten
  SetComDir(COMD_Stop, pCaller);

	var speedmenu = CreateSpeedMenu(0, pCaller);
	var aPage = aPages[iLastPage];
	
	if(aPage[0])
		speedmenu->AddUpItem(GetName(0, aPage[0][0]), "CreateAmmoPack", aPage[0], aPage[0][0]);
	if(aPage[1])
	{
		if(aPage[1] == AMPK_NextPage)
			speedmenu->AddRightItem("$NextPage$", "SwitchPage", iLastPage+1, NULL);
		else
			speedmenu->AddRightItem(GetName(0, aPage[1][0]), "CreateAmmoPack", aPage[1], aPage[1][0]);
	}
	if(aPage[2])
		speedmenu->AddDownItem(GetName(0, aPage[2][0]), "CreateAmmoPack", aPage[2], aPage[2][0]);
	if(aPage[3])
	{
		if(aPage[3] == AMPK_PrevPage)
			speedmenu->AddLeftItem("$PrevPage$", "SwitchPage", iLastPage-1, NULL);
		else
			speedmenu->AddLeftItem(GetName(0, aPage[3][0]), "CreateAmmoPack", aPage[3], aPage[3][0]);
	}
	if(aPage[4])
		speedmenu->AddThrowItem(GetName(0, aPage[4][0]), "CreateAmmoPack", aPage[4], aPage[4][0]);
	
	/*
	if(aPage[0])
		speedmenu->AddUpItem(GetName(0, aPage[0][0]), Format("CreateAmmoPack(%v, Object(%d))", aPage[0], ObjectNumber(pCaller)), 0, aPage[0][0]);
	if(aPage[1])
	{
		if(aPage[1] == AMPK_NextPage)
			speedmenu->AddRightItem("$NextPage$", Format("SwitchPage(%d, Object(%d))", iLastPage+1, ObjectNumber(pCaller)), 0, NULL);
		else
			speedmenu->AddRightItem(GetName(0, aPage[1][0]), Format("CreateAmmoPack(%v, Object(%d))", aPage[1], ObjectNumber(pCaller)), 0, aPage[1][0]);
	}
	if(aPage[2])
		speedmenu->AddDownItem(GetName(0, aPage[2][0]), Format("CreateAmmoPack(%v, Object(%d))", aPage[2], ObjectNumber(pCaller)), 0, aPage[2][0]);
	if(aPage[3])
	{
		if(aPage[3] == AMPK_PrevPage)
			speedmenu->AddLeftItem("$PrevPage$", Format("SwitchPage(%d, Object(%d))", iLastPage-1, ObjectNumber(pCaller)), 0, NULL);
		else
			speedmenu->AddLeftItem(GetName(0, aPage[3][0]), Format("CreateAmmoPack(%v, Object(%d))", aPage[3], ObjectNumber(pCaller)), 0, aPage[3][0]);
	}
	if(aPage[4])
		speedmenu->AddThrowItem(GetName(0, aPage[4][0]), Format("CreateAmmoPack(%v, Object(%d))", aPage[4], ObjectNumber(pCaller)), 0, aPage[4][0]);
	
  //Menü
  /*CreateMenu(GetID(), pCaller, this, 0, "$TakeAmmo$", 0, C4MN_Style_Context);
  for (var i = 0; i < GetLength(AmmoTypes()); i++)
  {
    var aAmmo = AmmoTypes()[i];
    AddMenuItem(Format("%d %s", aAmmo[1], GetName(0, aAmmo[0])), "CreateAmmoPack", aAmmo[0], pCaller, aAmmo[2], pCaller, 0, C4MN_Add_ForceNoDesc | 128, 0, i);
  }*/

  return true;
}

public func SwitchPage(int iPage, object pObj)
{
	if(!pObj && !(pObj = Contained()))
		return false;

	if(!aPages[iPage])
		return false;
	
	iLastPage = iPage;
	Activate(pObj);
	return true;
}

protected func CreateAmmoPack(array aAmmo, object pCaller)
{
	if(!pCaller && !(pCaller = Contained()))
		return false;

	if(!aAmmo || !GetLength(aAmmo))
		return false;
		
	if(GetPackPoints() < aAmmo[2])
	{
		PlayerMessage(GetOwner(pCaller), "$NeededPoints$", pCaller, aAmmo[2]);
    return false;
	}
	
	//Box erstellen und füllen
  var box = CreateObject(CUAM, 0, 0, GetOwner(pCaller));
  box->~SetAmmoID(aAmmo[0]);
  box->~SetAmmoCount(aAmmo[1], true);

  //Einsammeln
  if(!Collect(box, pCaller))
  {
    PlayerMessage(GetOwner(pCaller), "$NoSpace$", pCaller);
    return false;
  }

  //Punkte abziehen
  DoPackPoints(-aAmmo[2]);
  Sound("PackAmmo.ogg", false, this, 0, GetOwner(pCaller) + 1);

  //Hinwechseln
  ShiftContents(pCaller, 0, CUAM);

  return true;
}

/*protected func CreateAmmoPack(id idAmmo, object pCaller, bool fRight, int iIndex)
{
  //öha.
  if (!idAmmo || !pCaller)
    return false;

  //Zu wenig Punkte?
  var aAmmo = AmmoTypes()[iIndex];
  if (GetPackPoints() < aAmmo[2])
  {
    PlayerMessage(GetOwner(pCaller), "$NeededPoints$", pCaller, aAmmo[2]);
    return false;
  }

  //Box erstellen und füllen
  var box = CreateObject(CUAM, 0, 0, GetOwner(pCaller));
  box->~SetAmmoID(aAmmo[0]);
  box->~SetAmmoCount(aAmmo[1], true);

  //Einsammeln
  if (!Collect(box, pCaller))
  {
    PlayerMessage(GetOwner(pCaller), "$NoSpace$", pCaller);
    return false;
  }

  //Punkte abziehen
  DoPackPoints(-aAmmo[2]);
  Sound("PackAmmo.ogg", false, this, 0, GetOwner(pCaller) + 1);

  //Hinwechseln
  ShiftContents(pCaller, 0, CUAM);

  return true;
}*/

/* Team-Support */

public func DoTeamSupport(array aClonks)
{
  //Zu wenig Punkte
  if (GetPackPoints() < 30)
    return false;

  for (var pTarget in aClonks)
  {
    //Nur Clonks
    if (!pTarget->~IsClonk())
      continue;
      
    //Munitionsbedarf feststellen
    var highestammo = 0, ammoID = 0;
    for(var i = 0; i < ContentsCount(0, pTarget); i++)
      if(Contents(i, pTarget)->~IsWeapon())
        for (var j = 0; j < Contents(i, pTarget)->GetFMCount(); j++)
        {
          var ammocount, weapon = Contents(i, pTarget);
          if(weapon->GetFMData(FM_NoAmmoModify, j)) continue;
          if(weapon->GetFMData(FM_AmmoLoad, j) <= 3)
            ammocount = weapon->GetFMData(FM_AmmoLoad, j) * 10;
          else
          ammocount = weapon->GetFMData(FM_AmmoLoad,j) * 3;
          if(GetAmmo(weapon->GetFMData(FM_AmmoID, j), pTarget) < ammocount)
          {
            if(!ammoID)
              ammoID = weapon->GetFMData(FM_AmmoID,j);
            if(highestammo < ammocount)
              highestammo = ammocount;
          }
        }

    if(!ammoID)
      continue;

    //Munition hinzufügen
    var factor = ammoID->~GetPointFactor();
    if(ammoID->MaxAmmo() / 10 * factor > GetPackPoints() || GetAmmo(ammoID, pTarget) >= highestammo)
      continue;

    PlayerMessage(GetOwner(Contained()), "$AmmoReceived$", pTarget, ammoID->MaxAmmo() / 10, ammoID);
    PlayerMessage(GetOwner(pTarget),"$AmmoReceived$", pTarget, ammoID->MaxAmmo() / 10, ammoID);
    DoAmmo(ammoID, ammoID->MaxAmmo()/10, pTarget);
    Sound("Resupply.ogg");
    DoPackPoints(-ammoID->MaxAmmo() / 10 * factor);

    //Achievement-Fortschritt (Ammo Distributor)
    DoAchievementProgress(ammoID->MaxAmmo() / 10 * factor, AC03, GetOwner(Contained()));

    //Punkte bei Belohnungssystem (Munitionierung)
    DoPlayerPoints(BonusPoints("Restocking", ammoID->MaxAmmo() / 10 * factor), RWDS_TeamPoints, GetOwner(Contained()), Contained(), IC14);
  }
}

/* Sonstiges */

public func ControlThrow()   {return true;}

protected func Hit()
{
  Sound("AmmoBoxHit*.ogg", false, this);
}

protected func Selection()
{
  Sound("FAPK_Charge.ogg", false, this);
}
