/*-- Belohnungssystem --*/

#strict 2

local aData;

public func IsChooseable()	{return true;}

/* Initalisierung */

protected func Initialize()
{
  aData = CreateArray();
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
    if(!aList[GetPlayerTeam(iPlr)]) aList[GetPlayerTeam(iPlr)] = CreateArray();
    szString = Format("%s: %d $Points$", GetPlayerData(RWDS_PlayerName, iPlr), GetPlayerPoints(RWDS_TotalPoints, iPlr));
    aList[GetPlayerTeam(iPlr)][GetLength(aList[GetPlayerTeam(iPlr)])] = szString;
    iPlr++;
  }
  
  //Dann Teamweise ausgeben
  var szMessage = "";
  for(var aTeam in aList) {
    for(var szString in aTeam) {
      if(szMessage != "") szMessage = Format("%s|",szMessage);
      szMessage = Format("%s%s", szMessage, szString);
    }
  }
  
  MessageWindow(Format("$ActualPoints$:|%s", szMessage), iByPlayer);
  return(1);
}

/* Auswerten */

global func DoEvaluateStats()
{
  var db = FindObject(RWDS);
  if(!db) return;
  
  //Erst mal einsortieren
  var aList = CreateArray();
  var iPlr, szComplete, szTotal;
  var iPlr = 0;
  while(db->GetData() != 0)
  {
    if(!aList[GetPlayerTeam(iPlr)]) aList[GetPlayerTeam(iPlr)] = CreateArray();
    szComplete = Format("$Complete$",
                   db->GetPlayerPoints(RWDS_PlayerName, iPlr),
                   db->GetPlayerPoints(RWDS_BattlePoints, iPlr),
                   db->GetPlayerPoints(RWDS_TeamPoints, iPlr),
                   db->GetPlayerPoints(RWDS_MinusPoints, iPlr));
    szTotal = Format("$Total$",
                db->GetPlayerPoints(RWDS_PlayerName, iPlr),
                db->GetPlayerPoints(RWDS_TotalPoints, iPlr));
    aList[GetPlayerTeam(iPlr)][GetLength(aList[GetPlayerTeam(iPlr)])] = [szTotal, szComplete];
    iPlr++;
  }
  
  //Dann Teamweise ausgeben
  for(var aTeam in aList) {
    for(var aStrings in aTeam) {
      AddEvaluationData(aStrings[0], 0);
      AddEvaluationData(aStrings[1], 0);      
    }
  }
}

/* Spieler updaten */

protected func InitializePlayer(iPlr)
{
  aData[iPlr] = CreateArray();
  UpdatePlayers();
  return;
}

protected func UpdatePlayers() {
  for(var i = 0; i < GetPlayerCount(); i++) {
    var iPlr = GetPlayerByIndex(i);
    if(!GetPlayerData(RWDS_PlayerTeam, iPlr)) SetPlayerData(Format("<c %x>%s</c>", GetPlrColorDw(iPlr), GetPlayerName(iPlr)), RWDS_PlayerName, iPlr);
    if(!GetPlayerData(RWDS_PlayerTeam, iPlr)) SetPlayerData(GetPlayerTeam(iPlr), RWDS_PlayerTeam, iPlr);
  }
}

public func GetPlayerCount()
{
  return GetLength(aData);
}

/* Punkte setzen und auslesen */

static const RWDS_PlayerName = 0;
static const RWDS_PlayerTeam = 1;
static const RWDS_TotalPoints = 2;
static const RWDS_BattlePoints = 3;
static const RWDS_TeamPoints = 4;
static const RWDS_MinusPoints = 5;

global func DoPlayerPoints(int iPoints, int iType, int iPlr, object pClonk, id idIcon) {
  var db = FindObject(RWDS);
  if(!db) return;
  if(!iPoints) return;
  if(pClonk) {
    Log("%i", idIcon);
    if(!idIcon) idIcon = CLNK;
    var szMsg;
    if(iPoints < 0) szMsg = Format("{{%i}} <c ff0000>%d</c>", idIcon, iPoints);
    if(iPoints > 0) szMsg = Format("{{%i}} <c 00ff00>+%d</c>", idIcon, iPoints);
    pClonk->AddEffect("PointMessage", pClonk, 130, 1, pClonk, 0, szMsg);
  }
  db->SetPlayerData(db->GetPlayerPoints(iType, iPlr)+iPoints, iType, iPlr);
}

global func GetPlayerPoints(int iType, int iPlr) {
  var db = FindObject2(Find_ID(RWDS));
  if(!db) return;
  return db->GetPlayerData(iType, iPlr);
}

public func GetData() {
  return aData;
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
  aData[iPlr][iType] = xData;
}

public func GetPlayerData(int iType, int iPlr) {
  if(iType == RWDS_TotalPoints) {
    return aData[iPlr][RWDS_BattlePoints] + aData[iPlr][RWDS_TeamPoints] + aData[iPlr][RWDS_MinusPoints];
  }
  return aData[iPlr][iType];
}
