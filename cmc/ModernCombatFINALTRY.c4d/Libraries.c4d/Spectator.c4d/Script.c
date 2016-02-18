/*-- Zuschauer --*/

#strict 2

/*
 * Das Zuschauerobjekt prüft nicht selbst, ob es nicht mehr gebraucht wird (wegen Netzwerksynchronität), 
 * daher muss die PlrViewRange von dem SPEC Objekt auch auf 0 gesetzt werden, wenn es nicht mehr gebraucht wird. (s. bspw. in FKDT)
 */

/* Folgende Funktionen sollten in MenuTarget-Objekte: */
/*
public func OnMenuSelection(int iSelection, object pMenuObject)
{
  if(GetMenu(pMenuObject) == SPEC)
    return SPEC->SpectateMenuSelection(iSelection, pMenuObject);

  return true;
}

public func MenuQueryCancel(int iSelection, object pMenuObject)
{
  if(GetMenu(pMenuObject) == SPEC)
  {
    SPEC->SpectateObject(pMenuObject);
    return false;
  }

  return true;
}
*/


/* Erstellung */

local alternative;

public func Set(object pTarget, object pAlternative)
{
  SetAction("Attach", pTarget);
  alternative = pAlternative;
}

public func GetMenuItemStyle(object pCrew, id &itemID, int &iExtra, int &iColor, string &szPrefixedIcons) {
	if(FindObject(MCSL)) {
		iExtra = pCrew->~GetClass(); //Falls es in Zukunft eine solche Funktion geben sollte...
		if(!iExtra)
			iExtra = LocalN("lastclass", FindObject(MCSL))[GetOwner(pCrew)];
		itemID = MCSL;
	}
	else
		itemID = GetID(pCrew);

	if(Contained(pCrew)) {
		if(Contained(pCrew)->~IsSpawnObject() && !GetEffect("WaitingObject", Contained(pCrew))) {
			itemID = (GetName(0, SM29) && SM29) || ROCK; //Icon das anzeigt dass der Clonk am Spawnen ist. Alles nach dem = durch die neue IconID ersetzen.
			iColor = 0x777777;
			iExtra = 0;
		}
		else if(GetEffect("IntFakeDeathEffectsData", pCrew)) {
			szPrefixedIcons = "{{FKDT}}";
			iColor = 0xDD0000;
		}
	}
}

/* Menüsystem */

public func OpenSpectateMenu(object pTarget, object pMenuTarget, int iRange)
{
  CloseMenu(pTarget);

  CreateMenu(SPEC, pTarget, pMenuTarget, 0, "$SpectateMenu$", C4MN_Style_Dialog, true, true);

	var itemid, extra, clr = 0xFFFFFF, prefixedstr = "";
	GetMenuItemStyle(pTarget, itemid, extra, clr, prefixedstr);
  AddMenuItem(Format("%s<c %x>%s</c>", prefixedstr, clr, GetName(pTarget)), 
  						Format("SPEC->SpectateObject(Object(%d), Object(%d), %d, true)", ObjectNumber(pTarget), ObjectNumber(pTarget), iRange), itemid, pTarget, 0, 0, 0, 2, extra);
  for(var i = 0; i < GetPlayerCount(); i++)
  {
    var plr = GetPlayerByIndex(i);
    if(Hostile(plr, GetOwner(pTarget)))
    	continue;

    if(GetCrewCount(plr) > 1)
    {
      AddMenuItem(GetTaggedPlayerName(plr, true, true), 0, 0, pTarget);
      for(var j = 0; j < GetCrewCount(plr); j++)
      {
        if(GetCrew(plr, j) != pTarget)
        {
        	var itemid, extra, clr = GetPlrColorDw(plr), prefixedstr = "";
          GetMenuItemStyle(GetCrew(plr, j), itemid, extra, clr, prefixedstr);

          if(!Contained(GetCrew(plr, j)) || Contained(GetCrew(plr, j))->~IsSpawnObject())
            AddMenuItem(Format("  %s<c %x>%s</c>", prefixedstr, clr, GetName(GetCrew(plr, j))), Format("SPEC->SpectateObject(Object(%d), Object(%d), %d, true)", ObjectNumber(GetCrew(plr, j)), ObjectNumber(pTarget), iRange), itemid, pTarget, 0, 0, 0, 2, extra);
          else 
            AddMenuItem(Format("  %s<c %x>%s</c>", prefixedstr, clr, GetName(GetCrew(plr, j))), "SPEC->Nothing()", itemid, pTarget, 0, 0, 0, 2, extra);
        }
      }
    }
    else if(GetCrew(plr) != pTarget)
    {
      var itemid, extra, clr = GetPlrColorDw(plr), prefixedstr = "", icon = "", rank = GetRankID(GetPlayerRank(plr));
      GetMenuItemStyle(GetCrew(plr, j), itemid, extra, clr, prefixedstr);

			if(IsDeveloper(GetPlayerID(plr)))
				icon = "{{SM14}}";
      if(!Contained(GetCrew(plr)) || Contained(GetCrew(plr))->~IsSpawnObject())
        AddMenuItem(Format("%s {{%i}}%s <c %x>%s</c>", prefixedstr, rank, icon, clr, GetPlayerName(plr)), 
        						Format("SPEC->SpectateObject(Object(%d), Object(%d), %d, true)", ObjectNumber(GetCrew(plr)), ObjectNumber(pTarget), iRange), itemid, pTarget, 0, 0, 0, 2, extra);
      else
        AddMenuItem(Format("%s {{%i}}%s <c %x>%s</c>", prefixedstr, rank, icon, clr, GetPlayerName(plr)), "SPEC->Nothing()", itemid, pTarget, 0, 0, 0, 2, extra);
    }
  }

  SpectateObject(pMenuTarget, pMenuTarget, iRange);

  return true;
}

public func Nothing() {}

public func SpectateMenuSelection(int iSelection, object pTarget, int iRange)
{
  if(!iSelection)
    return SpectateObject(pTarget);

  for(var i = 0, iSel = 1; i < GetPlayerCount(); ++i) {
  	var plr = GetPlayerByIndex(i);
  	if(Hostile(plr, GetOwner(pTarget)))
    	continue;

    for(var j = 0; j < GetCrewCount(plr); (++j && ++iSel))
    {
      if(GetCrewCount(plr) > 1 && !j)
        ++iSel;

      if(GetCrew(plr, j) == pTarget)
      {
        --iSel;
        continue;
      }
      if(iSelection == iSel)
        SpectateObject(GetCrew(plr, j), pTarget, iRange);
    }
  }

  return true;
}

public func SpectateObject(object pSpectateTarget, object pTarget, int iRange, bool fCloseMenu)
{
	if(Contained(pSpectateTarget) && Contained(pSpectateTarget)->~IsSpawnObject() && !GetEffect("WaitingObject", Contained(pSpectateTarget)))
		pSpectateTarget = pTarget;

	if(!pTarget)
		pTarget = pSpectateTarget;

	if(fCloseMenu)
		CloseMenu(pTarget);

	var iPlr = GetOwner(pTarget);
  if(!iRange)
    iRange = 150;

  var spec = FindObject2(Find_ID(SPEC), Find_Owner(iPlr));
  if(!spec)
    spec = CreateObject(SPEC, 0, 0, iPlr);

  spec->Set(pSpectateTarget, pTarget);
  SetPlrView(iPlr, spec);
  SetPlrViewRange(iRange, spec);
}

public func AttachTargetLost() {
	if(alternative)
		Set(alternative);
	else
		RemoveObject();
}
