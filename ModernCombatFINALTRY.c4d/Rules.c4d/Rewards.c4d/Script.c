/*-- Belohnungssystem --*/

#strict 2

static aAchievementProgress, aAchievementExtra;
local aData, fEvaluation, aStats;

public func IsChooseable()	{return true;}


/* Initalisierung */

protected func Initialize()
{
  aData = CreateArray();
  fEvaluation = false;
  aStats = CreateArray();
  aAchievementProgress = CreateArray();
  aAchievementExtra = CreateArray();
}

protected func Activate(iByPlayer)
{
  StatsMenu(iByPlayer);
  return 1;
}

public func StatsMenu(int iPlr, bool fBack)
{
  if(!aStats[iPlr]) aStats[iPlr] = 0;
  var pClonk = GetCursor(iPlr);
  var iSelect = aStats[iPlr];
  var extra;

  if(!CreateMenu(GetID(),pClonk,this,0,0,0,C4MN_Style_Dialog)) return;
  AddMenuItem(" | ", "", RWDS, pClonk, 0, 0, "", 514, 0, 0);
  if(iSelect)
  {
    var iData = GetPlrExtraData(iPlr, "CMC_Achievements");
    var iIndex = iSelect;
    var idAchievement = C4Id(Format("AC%02d", iIndex));
    if(!(iData >> iSelect & 1))
    {
      AddMenuItem(Format("<c ffff33>%s</c>", GetName(0, idAchievement)), "", NONE, pClonk, 0, 0, "", 0, 0, 0);
      AddMenuItem("<i>$AchievementLocked$</i>", "", NONE, pClonk, 0, 0, "", 0, 0, 0);
    }
    else
    {
      AddMenuItem(Format("<c ffff33>%s</c>", GetName(0, idAchievement)), "", NONE, pClonk, 0, 0, "", 0, 0, 0);
      AddMenuItem(Format("<i>%s</i>", GetDesc(0, idAchievement)), "", NONE, pClonk, 0, 0, "", 0, 0, 0);
    }
  }
  else
  {
    extra = -1;
    //Erst mal einsortieren
    var aList = CreateArray();
    var szString;
    var iPlayer = 0;
    while(aData[iPlayer] != 0)
    {
      var iTeam = GetPlayerData(RWDS_PlayerTeam, iPlayer);
      if(!aList[iTeam]) aList[iTeam] = CreateArray();
      szString = Format("%s: %d $Points$", GetPlayerData(RWDS_PlayerName, iPlayer), GetPlayerPoints(RWDS_TotalPoints, iPlayer));
                 aList[iTeam][GetLength(aList[iTeam])] = szString;
                 iPlayer++;
    }

    //Nach Team ausgeben
	
    AddMenuItem("<c ffff33>$ActualPoints$</c>", "", NONE, pClonk, 0, 0, "", 0, 0, 0);
	
    for(var aTeam in aList)
      if(aTeam)
        for(var szString in aTeam)
		{
		  extra++;
		  AddMenuItem(szString, "", NONE, pClonk, 0, 0, "", 0, 0, 0);
		}
  }

  //Leerzeile
  AddMenuItem(" ", "", NONE, pClonk, 0, iSelect+1, "", 0, 0, 0);

  //Navigation
  if(iSelect+1 > 16)
  {
    AddMenuItem("<c 777777>$Continue$</c>", "StatsContinue", NONE, pClonk, 0, iPlr, "", 0, 0, 0);
  }
  else
  {
    AddMenuItem("$Continue$", "StatsContinue", NONE, pClonk, 0, iPlr, "", 0, 0, 0);
  }
  if(!fBack) SelectMenuItem(4+extra, pClonk);
  if(iSelect-1 < 0)
  {
    AddMenuItem("<c 777777>$Back$</c>", "StatsBack", NONE, pClonk, 0, iPlr, "", 0, 0, 0);
  }
  else
  {
    AddMenuItem("$Back$", "StatsBack", NONE, pClonk, 0, iPlr, "", 0, 0, 0);
  }
  if(fBack) SelectMenuItem(5+extra, pClonk);
  return true;
}

public func StatsContinue(temp, int iPlr)
{
  if(!aStats[iPlr]) aStats[iPlr] = 0;
  if(aStats[iPlr]+1 > 16) {
		Sound("Error", 1, 0, 100, iPlr+1);
	}
	else {
		Sound("Grab", 1, 0, 100, iPlr+1);
	}
  aStats[iPlr] = BoundBy(aStats[iPlr]+1, 0, 16);
  return StatsMenu(iPlr, false);
}

public func StatsBack(temp, int iPlr)
{
  if(!aStats[iPlr]) aStats[iPlr] = 0;
  if(aStats[iPlr]-1 < 0) {
		Sound("Error", 1, 0, 100, iPlr+1);
	}
	else {
		Sound("Grab", 1, 0, 100, iPlr+1);
	}
  aStats[iPlr] = BoundBy(aStats[iPlr]-1, 0, 16);
  return StatsMenu(iPlr, true);
}

global func RewardsActive()
{
  return FindObject2(Find_ID(RWDS)) != false;
}

/* Auswerten */

global func RewardEvaluation()
{
  var db = FindObject(RWDS);
  if(!db) return;

  db->Evaluate();
}

public func Evaluate()
{
  if(fEvaluation) return 0;
  fEvaluation = true;
  var db = this;

  //Erst mal einsortieren
  var aList = CreateArray();
  var iPlr, szComplete, szTotal;
  var iPlr = 0;
  while(db->GetData()[iPlr] != 0)
  {
    if(!aList[GetPlayerTeam(iPlr)]) aList[GetPlayerTeam(iPlr)] = CreateArray();
    szTotal = Format("$Total$",
                   db->GetPlayerPoints(RWDS_PlayerName, iPlr),
                   db->GetPlayerPoints(RWDS_TotalPoints, iPlr));
    szComplete = Format("$Complete$",
                   db->GetPlayerPoints(RWDS_BattlePoints, iPlr),
                   db->GetPlayerPoints(RWDS_TeamPoints, iPlr),
                   db->GetPlayerPoints(RWDS_MinusPoints, iPlr));
    aList[GetPlayerTeam(iPlr)][GetLength(aList[GetPlayerTeam(iPlr)])] = [szTotal, szComplete];
    AddEvaluationData(Format("{{IC01}}$Points$: %d", db->GetPlayerPoints(RWDS_TotalPoints, iPlr)), iPlr+1);
    iPlr++;
  }
  
  //Dann teamweise ausgeben
  for(var aTeam in aList)
  {
    if(!aTeam) continue;
    for(var aStrings in aTeam)
    {
      AddEvaluationData(aStrings[0], 0);
      AddEvaluationData(aStrings[1], 0);
    }
  }
  return 1;
}

/* Spieler updaten */

public func UpdatePlayers()
{
	if(!RewardsActive()) return;
  for(var i = 0; i < GetPlayerCount(); i++)
  {
    var iPlr = GetPlayerByIndex(i);
    SetPlayerData(Format("<c %x>%s</c>", GetPlrColorDw(iPlr), GetPlayerName(iPlr)), RWDS_PlayerName, iPlr);
    SetPlayerData(GetPlayerTeam(iPlr), RWDS_PlayerTeam, iPlr);
    if(!aData[iPlr]) aData[iPlr] = CreateArray();
    if(!aAchievementProgress[iPlr]) aAchievementProgress[iPlr] = CreateArray();
    if(!aAchievementExtra[iPlr]) aAchievementExtra[iPlr] = CreateArray();
  }
  return 1;
}

/* Werte setzen und auslesen */

static const RWDS_PlayerName = 0;
static const RWDS_PlayerTeam = 1;
static const RWDS_TotalPoints = 2;
static const RWDS_BattlePoints = 3;
static const RWDS_TeamPoints = 4;
static const RWDS_MinusPoints = 5;

global func DoPlayerPoints(int iPoints, int iType, int iPlr, object pClonk, id idIcon)
{
  var db = FindObject2(Find_ID(RWDS));
  if(!db) return;
  if(!iPoints) return;
  if(iType != RWDS_BattlePoints && iType != RWDS_TeamPoints && iType != RWDS_MinusPoints)
    return ErrorLog("Invalid points type for %d points at %v", iPoints, pClonk);
  if(db->SetPlayerData(db->GetPlayerPoints(iType, iPlr)+iPoints, iType, iPlr)) {
    DoAchievementProgress(iPoints, AC13, iPlr);
    if(pClonk)
    {
      if(!idIcon) idIcon = RWDS;
      var szMsg;
      if(iPoints < 0) szMsg = Format("{{%i}} <c ff0000>%d</c>", idIcon, iPoints);
      if(iPoints > 0) szMsg = Format("{{%i}} <c 00ff00>+%d</c>", idIcon, iPoints);
      if(iPoints == 0) szMsg = Format("{{%i}} <c ffff00>+%d</c>", idIcon, iPoints);
      pClonk->AddEffect("PointMessage", pClonk, 130, 1, pClonk, 0, szMsg);
      return true;
    }
  }
  return;
}

global func GetPlayerPoints(int iType, int iPlr)
{
  var db = FindObject2(Find_ID(RWDS));
  if(!db) return;
  return db->GetPlayerData(iType, iPlr);
}

public func GetData()
{
  return aData;
}

/* Achievements */

global func ResetAllPlayerAchievements()
{
  for(var i = 0; i < GetPlayerCount(); i++)
    ResetPlayerAchievements(GetPlayerByIndex(i));
  return true;
}

global func ResetPlayerAchievements(int iPlr)
{
  SetPlrExtraData(iPlr, "CMC_Achievements", 0);
  return true;
}

global func AwardAchievement(id idAchievement, int iPlr)
{
  //Keine Belohnungen?
  if(!RewardsActive()) return;

  if(GetLeague()) return false;
  if(GetPlayerType(iPlr) != C4PT_User) return false;
  if(!idAchievement->IsAchievement()) return false;
  var iData = GetPlrExtraData(iPlr, "CMC_Achievements");
  if(iData >> idAchievement->GetSavingSlot() & 1) return;
  SetPlrExtraData(iPlr, "CMC_Achievements", iData ^ 1 << idAchievement->GetSavingSlot());
  EventInfo4K(0, Format("$AchievementUnlocked$", GetPlrColorDw(iPlr), GetPlayerName(iPlr), GetName(0, idAchievement)), RWDS);
  CreateObject(idAchievement, 0, 0, iPlr);
  Sound("AchievementGet.ogg", true, 0, 100, iPlr+1);
  return true;
}

global func ResetAchievementProgress(id idAchievement, int iPlr)
{
  if(!FindObject(RWDS)) return;
  var index = idAchievement->GetSavingSlot();
  if(aAchievementProgress[iPlr][index])
  {
    aAchievementProgress[iPlr][index] = 0;
  }
  return true;
}

global func DoAchievementProgress(int iProgress, id idAchievement, int iPlr)
{
  if(!RewardsActive()) return;
  var index = idAchievement->~GetSavingSlot();
  if(!index) {
  	ErrorLog("Achievement %v without SavingSlot", idAchievement);
  	return;
  }
  aAchievementProgress[iPlr][index] += iProgress;
  if(aAchievementProgress[iPlr][index] >= idAchievement->~GetAchievementScore())
  {
    ResetAchievementProgress(idAchievement, iPlr);
    return AwardAchievement(idAchievement, iPlr);
  }
  return true;
}

global func GetAchievementProgress(id idAchievement, int iPlr)
{
  if(!RewardsActive()) return;
  var index = idAchievement->GetSavingSlot();
  if(aAchievementProgress[iPlr][index])
  {
    return aAchievementProgress[iPlr][index];
  }
  return 0;
}

global func ResetAchievementExtra(id idAchievement, int iPlr)
{
  if(!RewardsActive()) return;
  var index = idAchievement->GetSavingSlot();
  aAchievementExtra[iPlr][index] = 0;
  return true;
}

global func SetAchievementExtra(data, id idAchievement, int iPlr)
{
  if(!RewardsActive()) return;
  var index = idAchievement->GetSavingSlot();
  aAchievementExtra[iPlr][index] = data;
  return true;
}

global func GetAchievementExtra(id idAchievement, int iPlr)
{
  if(!RewardsActive()) return;
  var index = idAchievement->GetSavingSlot();
  if(aAchievementExtra[iPlr][index])
  {
    return aAchievementExtra[iPlr][index];
  }
  return false;
}

/* Punkteanzeige */

global func FxPointMessageStart(pTarget, iNo, iTemp, szString)
{
  if(iTemp)
    return -1;

  EffectVar(0,pTarget,iNo) = szString; //Nachricht
  EffectVar(2,pTarget,iNo) = 0; //Zeit
}

global func FxPointMessageTimer(pTarget, iNo)
{
  var iTime = EffectVar(2,pTarget,iNo);
  if(!iTime)
  {
    var i = 0;
    var index;
    while((index = GetEffect("PointMessage", pTarget, i)) != 0)
    {
      if(EffectVar(2,pTarget,index) > 0) return FX_OK;
      i++;
    }
    Sound("PointsGet.ogg", 0, pTarget, 100);	//Sound
    EffectVar(1,pTarget,iNo) = CreateObject(ARHL,0,0,-1);	//Helper
  }
  EffectVar(2,pTarget,iNo)++;
  CustomMessage(EffectVar(0,pTarget,iNo),EffectVar(1,pTarget,iNo),NO_OWNER,0,-iTime/2,
                RGBa(255,255,255,BoundBy(-50+iTime*5,0,255)));
  if(-50+iTime*5 > 255)
  {
    RemoveObject(EffectVar(1,pTarget,iNo));
    return -1;
  }
  return FX_OK;
}

/* Internes Handling */

public func SetPlayerData(xData, int iType, int iPlr)
{
  if(iType == RWDS_TotalPoints) return;
  if(!aData[iPlr]) return false;
  aData[iPlr][iType] = xData;
  return true;
}

public func GetPlayerData(int iType, int iPlr)
{
  if(iType == RWDS_TotalPoints)
  {return aData[iPlr][RWDS_BattlePoints] + aData[iPlr][RWDS_TeamPoints] + aData[iPlr][RWDS_MinusPoints];}
  return aData[iPlr][iType];
}
