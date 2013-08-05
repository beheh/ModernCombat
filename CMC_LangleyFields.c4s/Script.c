/*-- Langley Fields --*/

#strict
#include CSTD


/* Initialisierung */

func Initialize()
{
  //Himmelparallaxität
  SetSkyParallax(1, 20, 15);
  return(1);
}

/* Relaunch */

protected func InitializePlayer(int iPlr, int iX, int iY, object pBase, int iTeam)
{
  for(var i=0, pCrew ; pCrew = GetCrew(iPlr, i) ; i++)
    Relaunch(iPlr, pCrew, 0, iTeam);
}

public func RelaunchPlayer(int iPlr, object pCrew, object pKiller, int iTeam)
{
  //Keine Respawns
}

public func Relaunch(int iPlr, object pCrew, object pKiller, int iTeam)
{
  //Kein ordentlicher Spieler?
  if(GetOwner(pCrew) == NO_OWNER || iPlr == NO_OWNER || !GetPlayerName(iPlr))
    return();

  //Kein Team
  if(!iTeam) iTeam = GetPlayerTeam(iPlr);

  //Clonk tot?
  if(!GetAlive(pCrew))
    pCrew = RelaunchClonk(iPlr, pCrew);

  if(GetPlayerType(iPlr) == C4PT_Script)
    pCrew->~SetupBot4K();

  //Zufallsposition
  var iX, iY;
  RelaunchPosition(iX, iY, iTeam);

  //Ausrüsten
  var lowest = 1000;
  for(var i = 0; i < GetPlayerCount(); i++)
  {
    if(GetPlayerTeam(GetPlayerByIndex(i)) != iTeam) continue;
    
    if(lowest > GetPlayerByIndex(i))
      lowest = GetPlayerByIndex(i);
  }

  if(lowest == 1000 || lowest == iPlr)
  {
    pCrew->CreateContents(CNKT);
    pCrew->CreateContents(FLAG);
  }
  
  if(Contained(pCrew))
    SetPosition(iX, iY, Contained(pCrew));
  else
    SetPosition(iX, iY, pCrew);

  ParachuteOut(pCrew);
}

/* Bei Relaunch */
 
public func ParachuteOut(object pClonk)
{
  AddEffect("IntPara", pClonk, 1, 1);
  Sound("Airstrike2", 0, pClonk);
}

global func FxIntParaTimer(object pTarget)
{
  CreateObject(PARA,0,0,GetOwner(pTarget))->Set(pTarget);
  return -1;
}

/* Relaunch */

public func RelaunchPosition(& iX, & iY, int iTeam)
{
  var rand = Random(2);
  if(iTeam == 1)
  {
    iX = RandomX(315,605);
    iY = -50;
  }
  else
  {
    iX = RandomX(2335,2615);
    iY = -50;
  }
}