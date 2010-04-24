/*-- Flaggenposten --*/

#strict

local team,process,range,flag,attacker,spawnpoints,trend,capt,pAttackers,lastowner;

public func GetAttacker()	{return(attacker);}
public func GetTeam()		{return(team);}
public func GetProcess()	{return(process);}
public func GetTrend()		{return(trend);}
public func IsFullyCaptured()	{return(capt);}

public func IsFlagpole()	{return(true);}


/* Initalisierung */

public func Initialize()
{
  spawnpoints = CreateArray();
  lastowner = 0;
  Set();
  if(!flag)
   flag = CreateObject(OFLG);
  pAttackers = CreateArray();
  UpdateFlag();
}

/* Timereffekt */

public func FxIntFlagpoleTimer(object pTarget)
{
  if(!pTarget)
   return(-1);
  pTarget->Timer();
  return(0);
}

public func Set(string szName, int iRange, int iSpeed)
{
  if(!szName) szName = "Alpha";
  SetName(szName);

  if(!iRange) iRange = 100;
  range = iRange;

  if(!iSpeed) iSpeed = 10;

  RemoveEffect("IntFlagpole",this());
  AddEffect("IntFlagpole",this(),10,iSpeed,this());
}

/* Spawnpoint setzen */

public func AddSpawnPoint(int iX, int iY)
{
  var i = GetLength(spawnpoints);
  spawnpoints[i] = CreateArray();
  spawnpoints[i][0] = AbsX(iX);
  spawnpoints[i][1] = AbsY(iY);
}

public func GetSpawnPoint(int &iX, int &iY)
{
  if(!GetLength(spawnpoints))
  {
   iY = -30;
   return();
  }

  var rnd = Random(GetLength(spawnpoints));
  iX = spawnpoints[rnd][0];
  iY = spawnpoints[rnd][1];
}

/* Wird angegriffen */

public func IsAttacked()
{
  for(clonk in FindObjects(Find_Distance(range),Find_OCF(OCF_Alive), Find_NoContainer()))
  {
   if(GetOwner(clonk) == NO_OWNER) continue;
   if(GetPlayerTeam(GetOwner(clonk)) != team)
    return(true);
  }
  return(false);
}

public func IsCaptured(bool pBool)
{
  capt = pBool;
}

protected func ResetAttackers() {
  pAttackers = CreateArray();
}

/* Timer */

protected func Timer()
{
  var enemys,friends,opposition;

  trend = 0;

  //Erst mal schauen ob noch alle da sind
  var del;
  var clonks = FindObjects(Find_Distance(range),Find_OCF(OCF_Alive),Find_NoContainer());
  for(var pClonk in pAttackers) {
    del = true;
    for(var clonk in clonks) {
      if(clonk == pClonk) del = false;
      if(!del) break;
    }
    //Und weg damit
    if(del)
      pAttackers[FindInArray4K(pAttackers, pClonk)] = 0;
  }
  
  //Leere Eintr�ge entfernen
  CleanArray4K(pAttackers);
  
  for(clonk in FindObjects(Find_Distance(range),Find_OCF(OCF_Alive),Find_NoContainer()))
  {
   if(GetOwner(clonk) == NO_OWNER) continue;
   if(!PathFree4K(GetX(this()),GetY(this())-GetDefHeight(GetID())/2,GetX(clonk),GetY(clonk),4)) continue;
   if(Contained(clonk)) continue;

   if(GetPlayerTeam(GetOwner(clonk)) == team)
    friends++;
   else
   {
    enemys++;
    opposition = GetPlayerTeam(GetOwner(clonk));
    var new = true;
    //Schauen ob wir ihn schon finden k�nnen;
    for(var pClonk in pAttackers) {
      if(pClonk == clonk) new = false;
      if(!new) break;
    }
    //Er ist neu? Dann rein damit!
    if(new) pAttackers[GetLength(pAttackers)] = clonk;
   }
  }

  attacker = opposition;

  if(enemys && !friends)
    DoProcess(opposition,Min(enemys,3));

  if(!enemys && friends)
    DoProcess(team,Min(friends,3));
  
  if(enemys && friends) {
    Message("$MsgFoughtOver$", flag, GetTeamColor(team), GetName());
  }
  else if(trend == 0) {
    Message("", flag);
  }
}

public func Capture(int iTeam, bool bSilent)
{
  process = 100;
  attacker = 0;
  team = iTeam;
  capt = true;
  var fSilent = false;
  if(!bSilent) {
    if(lastowner == team) fSilent = true;
    GameCallEx("FlagCaptured", this, team, pAttackers, fSilent);
  }
  ResetAttackers();
  lastowner = team;
  UpdateFlag();
}

protected func Capturing(int iTeam)
{
  attacker = iTeam;
  Message("$MsgCapturing$",flag,GetTeamColor(iTeam),GetTeamName(iTeam),GetName());
}

public func NoTeam()
{
  team = 0;
  process = 0;
  attacker = 0;
  capt = false;
  UpdateFlag();
}

public func UpdateFlag()
{
  if(!flag) return();

  if(team)
   SetClrModulation(GetTeamColor(team),flag);

  SetFlagPos(process);
}

protected func SetFlagPos(int iHeight)
{
  if(!flag) return();
  iHeight = ((((GetDefHeight()+GetDefOffset(GetID(flag),true))*10)/100*(iHeight*10)) / 100)-GetDefOffset(GetID(flag),true);//Prozent umrechnen.
  iHeight = BoundBy(iHeight,0,GetDefHeight()+GetDefOffset(GetID(flag),true)); //Sicherheit ist wichtig.
  SetPosition(GetX(),GetY()-iHeight,flag);//Und setzen.
}

public func DoProcess(int iTeam, int iAmount)
{
  var old = process;

  //Eventuelle Gegnerflagge abnehmen
  if(team)
  {
   if(iTeam != team && (process != 0))
    iAmount = -iAmount;
  }
  else
   team = iTeam;

  process = BoundBy(process+iAmount,0,100);

  if(old < process)
   trend = +1;

  if(old > process)
   trend = -1;

  if((old == 100 && trend < 0) || (old == 0 && trend > 0)) {
    GameCallEx("FlagAttacked", this, team, pAttackers);
  }
  
  //Flagge wird �bernommen
  if(process < 100 && trend != 0) {
    Capturing(iTeam);
  }
  
  //Flagge ist fertig �bernommen
  if((process >= 100) && (old < 100))
  {
    Capture(iTeam);
  }

  //Neutrale Flagge
  if((process <= 0) && (old > 0))
  {
   if(team && lastowner != iTeam) GameCallEx("FlagLost", this, team, iTeam, pAttackers);
   //lastowner = team;
   attacker = 0;
   capt = false;
   team = iTeam;
  }

  UpdateFlag();

  return(process);
}
