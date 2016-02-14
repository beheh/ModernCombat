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
    SPEC->SpectateObject(pMenuObject, GetOwner(pMenuObject));
    return false;
  }

  return true;
}
*/


/* Erstellung */

public func Set(object pTarget)
{
  SetAction("Attach", pTarget);
}

/* Menüsystem */

public func OpenSpectateMenu(object pTarget, object pMenuTarget, int iRange)
{
  CloseMenu(pTarget);

  CreateMenu(SPEC, pTarget, pMenuTarget, 0, "$SpectateMenu$", C4MN_Style_Dialog, true, false);

  AddMenuItem(GetName(pTarget), Format("SPEC->SpectateObject(Object(%d), %d)", ObjectNumber(pTarget), GetOwner(pTarget)), FKDT, pTarget);
  for(var i = 0; i < GetPlayerCount(); i++)
  {
    var plr = GetPlayerByIndex(i);
    if(GetCrewCount(plr) > 1)
    {
      AddMenuItem(GetTaggedPlayerName(plr, true, true), 0, 0, pTarget);
      for(var j = 0; j < GetCrewCount(plr); j++)
      {
        if(GetCrew(plr, j) != pTarget)
        {
          var itemid = GetID(GetCrew(plr, j));
          if(GetEffect("IntFakeDeathEffectsData", GetCrew(plr, j)))
            itemid = FKDT;

          if(!Contained(GetCrew(plr)) || GetID(Contained(GetCrew(plr, j))) != TIM1)
            AddMenuItem(Format("  %s", GetName(GetCrew(plr, j))), Format("SPEC->SpectateObject(Object(%d), %d, %d)", ObjectNumber(GetCrew(plr, j)), GetOwner(pTarget), iRange), itemid, pTarget);
          else
            AddMenuItem(Format("  <c 777777>%s</c>", GetName(GetCrew(plr, j))), 0, itemid, pTarget);
        }
      }
    }
    else if(GetCrew(plr) != pTarget)
    {
      var itemid = GetID(GetCrew(plr));
      if(GetEffect("IntFakeDeathEffectsData", GetCrew(plr)))
        itemid = FKDT;

      if(!Contained(GetCrew(plr)) || GetID(Contained(GetCrew(plr))) != TIM1)
        AddMenuItem(GetTaggedPlayerName(plr, true, true), Format("SPEC->SpectateObject(Object(%d), %d, %d)", ObjectNumber(GetCrew(plr)), GetOwner(pTarget), iRange), itemid, pTarget);
      else
      {
        var rank = GetRankID(GetPlayerRank(plr));
        var icon = "";
        if(IsDeveloper(GetPlayerID(plr)))
          icon = "{{SM14}}";
        AddMenuItem(Format("{{%i}}%s <c 777777>%s</c>", rank, icon, GetPlayerName(plr)), 0, itemid, pTarget);
      }
    }
  }

  SpectateObject(pMenuTarget, GetOwner(pMenuTarget), iRange);

  return true;
}

public func SpectateMenuSelection(int iSelection, object pTarget, int iRange)
{
  if(!iSelection)
    return SpectateObject(pTarget, GetOwner(pTarget));

  for(var i = 0, iSel = 1; i < GetPlayerCount(); ++i)
    for(var j = 0, plr = GetPlayerByIndex(i); j < GetCrewCount(plr); (++j && ++iSel))
    {
      if(GetCrewCount(plr) > 1 && !j)
        ++iSel;

      if(GetCrew(plr, j) == pTarget)
      {
        --iSel;
        continue;
      }
      if(iSelection == iSel)
        SpectateObject(GetCrew(plr, j), GetOwner(pTarget), iRange);
    }

  return true;
}

public func SpectateObject(object pSpectateTarget, int iPlr, int iRange)
{
  if(!iRange)
    iRange = 150;

  var spec = FindObject2(Find_ID(SPEC), Find_Owner(iPlr));
  if(!spec)
    spec = CreateObject(SPEC, 0, 0, iPlr);

  spec->Set(pSpectateTarget);
  SetPlrView(iPlr, spec);
  SetPlrViewRange(iRange, spec);
}