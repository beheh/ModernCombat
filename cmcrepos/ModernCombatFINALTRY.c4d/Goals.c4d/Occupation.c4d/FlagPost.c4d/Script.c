/*-- Flaggenposten --*/

#strict

local team,process,range,flag,attacker,spawnpoints,trend;

public func GetAttacker()	{return(attacker);}
public func GetTeam()		{return(team);}
public func GetProcess()	{return(process);}
public func GetTrend()		{return(trend);}

public func IsFlagpole(){return(true);}


/* Initalisierung */

public func Initialize()
{
  spawnpoints = CreateArray();
  Set();
  if(!flag)
    flag = CreateObject(OFLG);
  UpdateFlag();
}

//Timer-Effekt
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

public func AddSpawnPoint(int iX, int iY)
{
  spawnpoints[GetLength(spawnpoints)] = iX;
  spawnpoints[GetLength(spawnpoints)] = iY;
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

public func IsCaptured()
{
  if((process > 0)&&(team)) return(team);
}


protected func Timer()
{
  var enemys,friends,opposition;
  
  trend = 0;

  for(clonk in FindObjects(Find_Distance(range),Find_OCF(OCF_Alive),Find_NoContainer()))
  {
    if(GetOwner(clonk) == NO_OWNER) continue;
    if(!PathFree4K(GetX(this()),GetY(this())-GetDefHeight(GetID())/2,GetX(clonk),GetY(clonk),4)) continue;
  
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
    DoProcess(opposition,1);
    
  if(!enemys && friends)
    DoProcess(team,1);
}

public func Capture(int iTeam)
{
  team = iTeam;
  process = 100;
  attacker = 0;
  EventInfo4K(0,Format("$MsgCaptured$",GetTeamName(iTeam),GetName()),OFLG,GetTeamColor(iTeam));
  Sound("Trumpet");
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
  UpdateFlag();
}

public func UpdateFlag()
{
  if(!flag) return();
  
  if(team)
    SetClrModulation(GetTeamColor(team),flag);
    
  SetFlagPos(process);
}

protected func SetFlagPos(int iHeight)//Prozentangaben! :D
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
  
  if(team)//Wenn er schon ein Team hat muss natürlich erstmal übernommen werden.
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
  
  if((process < 100) && (old >= 100))//feindliche Flagge übernehmen
  {
    Capturing(iTeam);
  }
    
  if((process >= 100) && (old < 100))//eigene Flagge sichern
  {
    if(attacker)
      Recaptured();
    else
      Capture(iTeam);
  }
  
  if((process <= 0) && (old > 0))//neutrale Flagge
  {
    attacker = 0;
    team = iTeam;
  }
    
  UpdateFlag();
  
  return(process);
}
