/*-- Payload --*/

#strict 2
#include TEAM

local aCheckpoints;
local iEnd, aStart;
local iTimer;
local iMinutes, iSeconds;

public func Activate(iPlr)
{
  var text = "";
  if(GetPlayerTeam(iPlr) == 1) {
    text = "$TaskAttack$";
  }
  else {
    text = "$TaskDefend$";  
  }
  return MessageWindow(text,iPlr);
}

public func Initialize() {
  aCheckpoints = CreateArray();
  aStart = CreateArray();
  iEnd = 0;
  iMinutes = 4;
  iSeconds = 0;
  iTimer = 0;
}

public func IsFulfilled() {
  return !FindObject(CHOS) && !FindObject(PLBM);
}

public func AddCheckpoint(int iX, int iY) {
  aCheckpoints[GetLength(aCheckpoints)] = iX;
  var pObj = CreateObject(MSGN, iX, iY, -1);
  pObj->SetCategory(1);
}

public func SetEnd(int iX, int iY) {
  iEnd = iX;
  var pObj = CreateObject(EXSN, iX, iY, -1);
  pObj->SetCategory(1);
}

public func SetStart(int iX, int iY) {
  AddCheckpoint(iX, iY);
  aStart = [iX, iY];
}

public func InitBomb() {
  var pBomb = CreateObject(PLBM, aStart[0], aStart[1], -1);
  pBomb->SetTeam(1);
  pBomb->SetCheckpoints(aCheckpoints);
  pBomb->SetEnd(iEnd);
}

/* Kein Scoreboard */

private func InitMultiplayerTeam(int iTeam) {}
private func RemoveMultiplayerTeam(int iTeam) {}
private func InitSingleplayerTeam(int iPlr) {}
private func RemoveSingleplayerTeam(int iPlr) {}
private func InitPlayer(int iPlr) {}
public func UpdateHUD() {}

/* Scoreboard */

private func InitScoreboard()
{  
  SetScoreboardData(SBRD_Caption, SBRD_Caption, "{{GPL_}}", SBRD_Caption);
  SetScoreboardData(SBRD_Caption, 1, Format("<c %x>{{PLBM}}</c>", GetTeamColor(1)), SBRD_Caption);
  SetScoreboardData(SBRD_Caption, 2, "{{TIME}}", SBRD_Caption);
  Log("1");
  UpdateScoreboard();
}

private func UpdateScoreboard()
{
  var pBomb = FindObject(PLBM);
  if(pBomb) {
    var dist = Abs((pBomb->GetNextCheckpoint())-(pBomb->GetLastCheckpoint()));
    var done = Abs((pBomb->GetX())-(pBomb->GetLastCheckpoint()));
    SetScoreboardData(1,1,Format("%d%", (100*1000/dist*done)/1000));
  }
  else {
    SetScoreboardData(1,1,Format("0%"));
  }
  SetScoreboardData(1,2,Format("%d:%02d", iMinutes, iSeconds));  
}

public func AddPayloadTime(int iMinutesAdd, int iSecondsAdd) {
  iMinutes += iMinutesAdd;
  iSeconds += iSecondsAdd;
}

protected func Timer() {
  if(!(iTimer % 5)) UpdateScoreboard();
  if(!(iTimer % 36)) iSeconds--;
  iTimer++;
  if(iSeconds < 0) {
    iMinutes--;
    iSeconds = 59;
  }
  if(iMinutes < 0) {
    GameOver();
  }
}

public func ChooserFinished() {
  if(!iEnd) return ScheduleCall(this, "ChooserFinished", 1);
  InitBomb();
}
