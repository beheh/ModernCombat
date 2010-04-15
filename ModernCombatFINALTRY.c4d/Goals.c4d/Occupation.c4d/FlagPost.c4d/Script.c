/*-- Flaggenposten --*/

#strict

local team,process,range,flag,attacker,spawnpoints,trend,capt;

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
  Set();
  if(!flag)
   flag = CreateObject(OFLG);
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
  spawnpoints[GetLength(spawnpoints)] = AbsX(iX);
  spawnpoints[GetLength(spawnpoints)] = AbsY(iY);
}

public func GetSpawnPoint(int &iX, int &iY)
{
  if(!GetLength(spawnpoints))
  {
   iY = -30;
   return();
  }

  var rnd = Random(GetLength(spawnpoints)/2);
  iX = spawnpoints[rnd];
  iY = spawnpoints[rnd+1];
}

/* Wird angegriffen */

public func IsAttacked()
{
  for(clonk in FindObjects(Find_Distance(range),Find_OCF(OCF_Alive)))
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

/* Timer */

protected func Timer()
{
  var enemys,friends,opposition;

  trend = 0;

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
   }
  }

  attacker = opposition;

  if(enemys && !friends)
    DoProcess(opposition,Min(enemys,3));

  if(!enemys && friends)
    DoProcess(team,Min(friends,3));
}

public func Capture(int iTeam, bool bSilent)
{
  team = iTeam;
  process = 100;
  attacker = 0;
  capt = true;
  if(!bSilent)
  {
   EventInfo4K(0,Format("$MsgCaptured$",GetTeamName(iTeam),GetName()),OFLG,GetTeamColor(iTeam));
   Sound("Trumpet");
  }
  GameCall("PointCaptured",this(),team);//Broadcasten.

  UpdateFlag();
}

protected func Capturing(int iTeam)
{
  attacker = iTeam;
  Message("$MsgCapturing$",flag,GetTeamColor(iTeam),GetTeamName(iTeam),GetName());
}

protected func Recaptured()
{
  attacker = 0;
  capt = true;
  EventInfo4K(0,Format("$MsgRecaptured$",GetTeamName(team),GetName()),OFLG,GetTeamColor(team));
}

protected func Recapturing()
{
  attacker = 0;
  Message("$MsgRecapturing$",flag,GetTeamColor(team),GetTeamName(team),GetName());
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
  //Log("%d%%",iHeight);
  iHeight = ((((GetDefHeight()+GetDefOffset(GetID(flag),true))*10)/100*(iHeight*10)) / 100)-GetDefOffset(GetID(flag),true);//Prozent umrechnen.
  //Log("%d%",iHeight);
  iHeight = BoundBy(iHeight,0,GetDefHeight()+GetDefOffset(GetID(flag),true));//Sicherheit ist wichtig.
  //Log("%d%[s]",iHeight);
  SetPosition(GetX(),GetY()-iHeight,flag);//Und setzen.
}

public func DoProcess(int iTeam, int iAmount)
{
  var old = process;

  //Eventuelle Gegnerflagge abnehmen
  if(team)
  {
   if(iTeam != team)
    iAmount = -iAmount;
   else
   if(attacker)
    if(attacker != team)
     Recapturing();
  }
  else
   team = iTeam;

  process = BoundBy(process+iAmount,0,100);

  if(old < process)
   trend = +1;

  if(old > process)
   trend = -1;

  //Feindliche Flagge übernehmen
  if((process < 100) && (old >= 100))
  {
   Capturing(iTeam);
  }

  //Eigene Flagge sichern
  if((process >= 100) && (old < 100))
  {
   if(attacker)
    Recaptured();
   else
    Capture(iTeam);
  }

  //Neutrale Flagge
  if((process <= 0) && (old > 0))
  {
   attacker = 0;
   capt = false;
   team = iTeam;
  }

  UpdateFlag();

  return(process);
}
