/*-- Enginefunktionen --*/

#strict 2

//Ranginformationen im Namen
global func GetTaggedPlayerName(int iPlr, bool fRank, bool fCompact)
{
  var rank = GetRankID(GetPlayerRank(iPlr));
  var icon = "";
  if(IsDeveloper(GetPlayerID(iPlr)))
    icon = "{{SM14}}";

  if(fRank && FindObject2(Find_Or(Find_ID(RWDS), Find_ID(CHOS))) && GetPlayerTeam(iPlr) != -1)
  {
    if(fCompact)
      return Format("{{%i}}%s <c %x>%s</c>", rank, icon, GetPlrColorDw(iPlr), GetPlayerName(iPlr));

    var icons = GetName(0, rank);
    if(icon != "") icons = Format("%s %s", icons, icon);
    return Format("{{%i}} %s <c %x>%s</c>", rank, icons, GetPlrColorDw(iPlr), GetPlayerName(iPlr));
  }
  else if(fRank && icon)
  {
    return Format("{{%i}} <c %x>%s</c>", icon, GetPlrColorDw(iPlr), GetPlayerName(iPlr));
  }

  return _inherited(iPlr);
}

//Deselection-Call
global func ShiftContents(object pObj, bool fShiftBack, id idTarget, bool fDoCalls)
{
	if(fDoCalls)
		Contents(0, pObj)->~Deselection(pObj);
	
	return _inherited(pObj, fShiftBack, idTarget, fDoCalls);
}
