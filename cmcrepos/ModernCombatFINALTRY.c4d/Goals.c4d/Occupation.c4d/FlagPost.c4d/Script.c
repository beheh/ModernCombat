/*-- Flaggenposten --*/

#strict 2

local team,process,range,flag,bar,attacker,spawnpoints,trend,capt,pAttackers,lastowner, iconState;

public func GetAttacker()	{return attacker;}
public func GetTeam()		{return team;}
public func GetProcess()	{return process;}
public func GetTrend()		{return trend;}
public func GetRange()		{return range;}
public func IsFullyCaptured()	{return capt;}
public func IsFlagpole()	{return true;}

static const BAR_FlagBar = 5;

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

/* Einstellungen */

public func Set(string szName, int iRange, int iSpeed)
{
  if(!szName) szName = "Alpha";
  SetName(szName);

  if(!iRange) iRange = 100;
  range = iRange;

  if(!iSpeed) iSpeed = 10;

  RemoveEffect("IntFlagpole",this);
  AddEffect("IntFlagpole",this,10,iSpeed,this);
}

/* Spawnpoints setzen */

public func AddSpawnPoint(int iX, int iY, string szFunction)
{
  var i = GetLength(spawnpoints);
  spawnpoints[i] = CreateArray();
  spawnpoints[i][0] = AbsX(iX);
  spawnpoints[i][1] = AbsY(iY);
  spawnpoints[i][2] = szFunction;  
}

public func GetSpawnPoint(int &iX, int &iY, string &szFunction, int iPlr)
{
  if(!GetLength(spawnpoints))
  {
    iY = -30;
    return;
  }

  var iX, iY, szFunction;
  szFunction = GetBestSpawnpoint(spawnpoints, iPlr, iX, iY)[2];
}

/* Wird angegriffen */

public func IsAttacked()
{
  for(clonk in FindObjects(Find_Distance(range),Find_OCF(OCF_Alive), Find_NoContainer()))
  {
    if(GetOwner(clonk) == NO_OWNER) continue;
    if(GetPlayerTeam(GetOwner(clonk)) != team)
      return true;
  }
  return false;
}

public func IsCaptured(bool pBool)
{
  capt = pBool;
}

protected func ResetAttackers()
{
  pAttackers = CreateArray();
}

/* Timer */

public func FxIntFlagpoleTimer(object pTarget)
{
  if(!pTarget)
    return(-1);
  pTarget->Timer();
  return;
}

protected func Timer()
{
  var enemys,friends,opposition;

  var iOld = trend;

  trend = 0;

  //Alle vorhanden?
  var del;
  var clonks = FindObjects(Find_Distance(range),Find_OCF(OCF_Alive),Find_NoContainer());
  for(var pClonk in pAttackers)
  {
    del = true;
    for(var clonk in clonks)
    {
      if(clonk == pClonk)
      {
        del = false;
        break;
      }
    }
    //Nein: Entfernen
    if(del)
      pAttackers[FindInArray4K(pAttackers, pClonk)] = 0;
  }

  //Leere Einträge entfernen
  CleanArray4K(pAttackers);

  var aFriends = CreateArray();
  var aEnemies = CreateArray();
  var clonks = FindObjects(Find_Distance(range),Find_OCF(OCF_Alive),Find_NoContainer());

  for(clonk in clonks)
  {
    if(GetOwner(clonk) == NO_OWNER) continue;
    if(!GetPlayerName(GetOwner(clonk)) || !GetPlayerTeam(GetOwner(clonk))) continue;
    if(!PathFree4K(GetX(this()),GetY(this())-GetDefHeight(GetID())/2,GetX(clonk),GetY(clonk),4)) continue;
    if(Contained(clonk)) continue;
    if(GetPlayerTeam(GetOwner(clonk)) == team)
    {
      friends++;
      aFriends[GetLength(aFriends)] = clonk;
    }
    else
    {
      enemys++;
      opposition = GetPlayerTeam(GetOwner(clonk));
      aEnemies[GetLength(aEnemies)] = clonk;
    }
  }
  attacker = opposition;

  if(enemys && !friends)
    DoProcess(opposition,Min(enemys,3));

  if(!enemys && friends)
    DoProcess(team,Min(friends,3));

  if((!enemys) == (!friends))
  {
    if(!friends)
    {
      if(iconState != 0)
      {
        bar->SetIcon(0, SM21, 0, 0, 32);
        bar->Update(0, true, true);
        iconState = 0;
      }
    }
    else
    {
      if(iconState != 2)
      {
        bar->SetIcon(0, SM23, 0, 0, 32);
        bar->SetBarColor(GetTeamColor(team));
        bar->Update(process);
        iconState = 2;
      }
    }
  }

  if(trend != iOld)
    ResetAttackers();

  var pClonks = CreateArray();
  if(trend < 0)
    pClonks = aEnemies;
  if(trend > 0)
    pClonks = aFriends;

  for(var clonk in pClonks)
  {
    if(!clonk) continue;
    var new = true;
    //Clonk auffindbar?
    for(var pClonk in pAttackers)
    {
      if(pClonk == clonk) new = false;
      if(!new) break;
    }
    //Neu: Einstellen
    if(new) pAttackers[GetLength(pAttackers)] = clonk;
  }
}

public func Capture(int iTeam, bool bSilent)
{
  process = 100;
  attacker = 0;
  team = iTeam;
  capt = true;
  var fRegained = false;
  if(!bSilent)
  {
    if(lastowner == team) fRegained = true;
    GameCallEx("FlagCaptured", this, team, pAttackers, fRegained);
  }
  ResetAttackers();
  lastowner = team;
  UpdateFlag();
}

protected func Capturing(int iTeam)
{
  attacker = iTeam;
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
  if(!flag) return;

  if(!bar)
  {
    bar = CreateObject(SBAR, 0, 0, -1);
    bar->Set(this, RGB(255, 255, 255), BAR_FlagBar, 100, 0, SM21, 0, 0, true);
    bar->ChangeDefOffset(GetDefOffset(GetID(), 1)+5);
    //bar->PositionToVertex(0, true);
    bar->SetIcon(0, SM21, 0, 0, 32);
    bar->Update(0, true, true);
    iconState = 0;
  }
  if(team)
  {
    SetColorDw(RGB(0,0,0), flag);
    for(var i = 0; i < GetPlayerCount(); i++)
    {
      if(GetPlayerTeam(GetPlayerByIndex(i)) != team) continue;
      flag->SetOwner(GetPlayerByIndex(i));
      break;
    }
  }
  else
  {
    SetOwner(NO_OWNER, flag);
    SetColorDw(RGB(255, 255, 255), flag);
  }

  SetFlagPos(process);
}

protected func SetFlagPos(int iPercentage)
{
  if(!flag) return;
  var iMaximum = GetDefHeight() - GetDefHeight(GetID(flag));
  var iHeight = iPercentage * iMaximum / 100 + GetDefHeight(GetID(flag))/2;
  SetPosition(GetX(),GetY() - iHeight, flag);
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

  if((old == 100 && trend < 0) || (old == 0 && trend > 0))
  {
    GameCallEx("FlagAttacked", this, team, pAttackers);
  }

  //Flagge wird übernommen
  if(process < 100 && trend != 0)
  {
    Capturing(iTeam);
  }

  //Flagge ist fertig übernommen
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
  bar->SetBarColor(GetTeamColor(iTeam));
  if(process >= 100)
  {
    if(iconState != 0)
    {
      bar->SetIcon(0, SM21, 0, 0, 32);
      bar->Update(0, true, true);
      iconState = 0;
    }
  }
  else if(iconState != 1)
  {
    bar->SetIcon(0, SM22, 0, 0, 32);
    iconState = 1;
  }
  if(iconState != 0)
    bar->Update(process);

  return process;
}

/* Flaggenposten verschieben */

public func MoveFlagpost(int iX, int iY, bool fNeutral, string szName)
{
  //Effekte
  for(var i = -80; i < -20; i += 10)
    CastParticles("MetalSplinter",1,20,0,i,50,80);
  if(GetEffectData(EFSM_ExplosionEffects) > 0) CastSmoke("Smoke3",8,15,0,-5,250,200,RGBa(255,255,255,100),RGBa(255,255,255,130));
  CastParticles("Sandbag", 10, 70, 0,-10, 35, 45, RGBa(228,228,228,0), RGBa(250,250,250,50));
  Sound("FenceDestruct.ogg");
  Sound("StructureHit*.ogg");
  Sound("StructureDebris*.ogg");

  //Namen ändern
  if(szName)
    SetName(szName);

  //Besitzer neutralisieren
  if(fNeutral)
    NoTeam();

  //Spawnpunkte anpassen
  var curX = GetX(), curY = GetY();
  for(var i = 0; i < GetLength(spawnpoints); i++)
  {
    spawnpoints[i][0] -= iX - curX;
    spawnpoints[i][1] -= iY - curY;
  }

  //Verschieben und einblenden
  SetPosition(iX, iY);
  UpdateFlag();
  FadeIn();
  flag->FadeIn();
}
