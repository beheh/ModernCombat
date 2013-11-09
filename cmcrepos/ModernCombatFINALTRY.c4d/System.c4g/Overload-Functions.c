/*-- Enginefunktionen --*/

//Überlädt die Namensabfrage von Spielern mit zusätzlichen Ranginformationen.

#strict 2


/* Ranginformationen im Namen */

global func GetTaggedPlayerName(int iPlr, bool fRank, bool fCompact)
{
  var rank = GetRankID(GetPlayerRank(iPlr));
  var icon = "";
  if(IsDeveloper(GetPlayerID(iPlr)))
    icon = "{{SM14}}";

  var clr = GetPlrColorDw(iPlr);
  if(FindObject2(Find_ID(CHOS)) && FindObject2(Find_ID(CHOS))->IsInRandomTeam(iPlr))
    clr = 0x777777;

  if(fRank && FindObject2(Find_Or(Find_ID(RWDS), Find_ID(CHOS))) && GetPlayerTeam(iPlr) != -1)
  {
    if(fCompact)
      return Format("{{%i}}%s <c %x>%s</c>", rank, icon, clr, GetPlayerName(iPlr));

    var icons = GetName(0, rank);
    if(icon != "") icons = Format("%s %s", icons, icon);
    return Format("{{%i}} %s <c %x>%s</c>", rank, icons, clr, GetPlayerName(iPlr));
  }
  else if(fRank && icon)
  {
    return Format("{{%i}} <c %x>%s</c>", icon, clr, GetPlayerName(iPlr));
  }

  return _inherited(iPlr);
}

/* Abwahl-Aufruf */

global func ShiftContents(object pObj, bool fShiftBack, id idTarget, bool fDoCalls)
{
	if(!pObj)
		pObj = this;

  if(fDoCalls)
    Contents(0, pObj)->~Deselection(pObj);

  return _inherited(pObj, fShiftBack, idTarget, fDoCalls);
}

/* Playlist speichern */

static CMC_SavedPlayList;

global func SetPlayList(string szPlayList, bool fNoSave)
{
  if(!fNoSave)
    CMC_SavedPlayList = szPlayList;

  return _inherited(szPlayList);
}

global func ResetPlayList(bool fPlayRandomMusic) 
{ 
  SetPlayList(CMC_SavedPlayList); 

  if(fPlayRandomMusic && CMC_SavedPlayList && CMC_SavedPlayList != "*.*")
  {
    var j = 0, music = [];

    //Playlist in Musikstücke aufteilen. (Semikolon ist Trenner)
    for(var i = 0; i < GetLength(CMC_SavedPlayList); i++)
    {
      if(!music[j])
        music[j] = "";

      var char = GetChar(CMC_SavedPlayList, i);
      if(char == 59) // Semikolon
      {
        j++;
        continue;
      }

      music[j] = Format("%s%c", music[j], char);
    }

    //Zufälliges Musikstück abspielen.
    Music(music[Random(GetLength(music))]);
  }

  return true;
}
