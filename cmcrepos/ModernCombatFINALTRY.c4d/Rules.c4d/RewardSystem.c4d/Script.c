/*-- Belohnungssystem --*/

#strict 2

local aData, fEvaluation;

public func IsChooseable()	{return true;}

/* Initalisierung */

protected func Initialize()
{
  aData = CreateArray();
  fEvaluation = false;
}

protected func Activate(iByPlayer)
{
  var szMessage = Format("$ActualPoints$|");
  
  //Erst mal einsortieren
  var aList = CreateArray();
  var iPlr, szString;
  var iPlr = 0;
  while(aData[iPlr] != 0)
  {
    var iTeam = GetPlayerData(RWDS_PlayerTeam, iPlr);
    if(!aList[iTeam]) aList[iTeam] = CreateArray();
    szString = Format("%s: %d $Points$", GetPlayerData(RWDS_PlayerName, iPlr), GetPlayerPoints(RWDS_TotalPoints, iPlr));
    aList[iTeam][GetLength(aList[iTeam])] = szString;
    iPlr++;
  }
  
  //Dann Teamweise ausgeben
  var szMessage = "";
  
  for(var aTeam in aList) {
    if(!aTeam) continue;
    for(var szString in aTeam) {
      if(szMessage != "") szMessage = Format("%s|",szMessage);
      szMessage = Format("%s%s", szMessage, szString);
    }
  }
  
  MessageWindow(Format("$ActualPoints$:|%s", szMessage), iByPlayer);
  return 1;
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
    AddEvaluationData(Format("{{WCR2}}$Points$: %d", db->GetPlayerPoints(RWDS_TotalPoints, iPlr)), iPlr+1);
    iPlr++;
  }
  
  //Dann Teamweise ausgeben
  for(var aTeam in aList) {
    if(!aTeam) continue;
    for(var aStrings in aTeam) {
      AddEvaluationData(aStrings[0], 0);
      AddEvaluationData(aStrings[1], 0);
    }
  }
  return 1;
}

/* Spieler updaten */

public func UpdatePlayers() {
  for(var i = 0; i < GetPlayerCount(); i++) {
    var iPlr = GetPlayerByIndex(i);
    if(!aData[iPlr]) aData[iPlr] = CreateArray();
    if(!GetPlayerData(RWDS_PlayerName, iPlr)) SetPlayerData(Format("<c %x>%s</c>", GetPlrColorDw(iPlr), GetPlayerName(iPlr)), RWDS_PlayerName, iPlr);
    if(!GetPlayerData(RWDS_PlayerTeam, iPlr)) SetPlayerData(GetPlayerTeam(iPlr), RWDS_PlayerTeam, iPlr);
  }
}

/* Werte setzen und auslesen */

static const RWDS_PlayerName = 0;
static const RWDS_PlayerTeam = 1;
static const RWDS_TotalPoints = 2;
static const RWDS_BattlePoints = 3;
static const RWDS_TeamPoints = 4;
static const RWDS_MinusPoints = 5;

global func DoPlayerPoints(int iPoints, int iType, int iPlr, object pClonk, id idIcon) {
  var db = FindObject2(Find_ID(RWDS));
  if(!db) return;
  if(!iPoints) return;
  if(pClonk) {
    if(!idIcon) idIcon = CLNK;
    var szMsg;
    if(iPoints < 0) szMsg = Format("{{%i}} <c ff0000>%d</c>", idIcon, iPoints);
    if(iPoints > 0) szMsg = Format("{{%i}} <c 00ff00>+%d</c>", idIcon, iPoints);
    if(iPoints == 0) szMsg = Format("{{%i}} <c ffff00>+%d</c>", idIcon, iPoints);
    pClonk->AddEffect("PointMessage", pClonk, 130, 1, pClonk, 0, szMsg);
  }
  return db->SetPlayerData(db->GetPlayerPoints(iType, iPlr)+iPoints, iType, iPlr);
}

global func GetPlayerPoints(int iType, int iPlr) {
  var db = FindObject2(Find_ID(RWDS));
  if(!db) return;
  return db->GetPlayerData(iType, iPlr);
}

public func GetData() {
  return aData;
}

/* Achievements */

global func AwardAchievement(int iPlr, id idAchievement) {
	if(!idAchievement->IsAchievement()) return false;
	var iData = GetPlrExtraData(iPlr, "CMC_Achievements");
	if(iData >> idAchievement->GetSavingSlot() & 1) return;
	SetPlrExtraData(iPlr, "CMC_Achievements", iData ^ 1 << idAchievement->GetSavingSlot());
	EventInfo4K(0, Format("$AchievmentUnlocked$", GetPlrColorDw(iPlr), GetPlayerName(iPlr), GetName(0, idAchievement)), RWDS);
	CreateObject(idAchievement, 0, 0, iPlr);
	Sound("Cheer.ogg");
	return true;
}

/* Punkteanzeige */

global func FxPointMessageStart(pTarget, iNo, iTemp, szString)
{
  if(iTemp)
    return -1;

  EffectVar(0,pTarget,iNo) = szString; //Die Message
  EffectVar(1,pTarget,iNo) = CreateObject(ARHL,0,0,-1);	//Der Helper
  Sound("PointsGet.ogg");	//Sound
}

global func FxPointMessageTimer(pTarget, iNo, iTime)
{
  CustomMessage(EffectVar(0,pTarget,iNo),EffectVar(1,pTarget,iNo),NO_OWNER,0,-iTime/2,
                RGBa(255,255,255,BoundBy(-50+iTime*5,0,255)));
  if(-50+iTime*5 > 255)
  {
    RemoveObject(EffectVar(1,pTarget,iNo));
    return -1;
  }
}

/* Internes Handling */

public func SetPlayerData(xData, int iType, int iPlr) {
  if(iType == RWDS_TotalPoints) return;
  if(!aData[iPlr]) return false;
  aData[iPlr][iType] = xData;
  return true;
}

public func GetPlayerData(int iType, int iPlr) {
  if(iType == RWDS_TotalPoints) {
    return aData[iPlr][RWDS_BattlePoints] + aData[iPlr][RWDS_TeamPoints] + aData[iPlr][RWDS_MinusPoints];
  }
  return aData[iPlr][iType];
}
