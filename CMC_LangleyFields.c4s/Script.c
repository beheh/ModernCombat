/*-- Langley Fields --*/

#strict
#include CSTD


/* Initialisierung */

func Initialize()
{
  //Himmelparallaxität
  SetSkyParallax(1, 20, 15);
  //Einrichtung plazieren
  CreateInterior();
  return(1);
}

/* Plazierungsliste */

func CreateInterior()
{
  Log("$CreatingInterior$");

  //Nachschublager
  CreateObject(SYSK, 795, 440, -1);
  CreateObject(SYSK, 2980, 470, -1);
  CreateObject(SYSK, 4020, 470, -1);
  CreateObject(SYSK, 6200, 440, -1);

  //Grenzen
  CreateObject(BRDR, 660, 0, -1)->Set(0);
  CreateObject(BRDR, 6340, 0, -1)->Set(1);

/*
  //Sounds

  //Wind
  CreateObject(SE4K, 930, 220, -1)->Set("WindSound*.ogg",775,250);
  CreateObject(SE4K, 2120, 220, -1)->Set("WindSound*.ogg",775,250);
  CreateObject(SE4K, 3300, 220, -1)->Set("WindSound*.ogg",775,250);

  //Hallen
  CreateObject(SE4K, 620, 520, -1)->Set("Interior*.ogg",665,105);
  CreateObject(SE4K, 1270, 600, -1)->Set("Interior*.ogg",670,105);
  CreateObject(SE4K, 2970, 600, -1)->Set("Interior*.ogg",670,105);
  CreateObject(SE4K, 3620, 520, -1)->Set("Interior*.ogg",665,105);

  //Jungel
  CreateObject(SE4K, 200, 1100, -1)->Set("Jungle*.ogg",800,200);
  CreateObject(SE4K, 800, 1100, -1)->Set("Jungle*.ogg",300,200);
  CreateObject(SE4K, 1400, 1100, -1)->Set("Jungle*.ogg",800,200);
  CreateObject(SE4K, 2000, 1100, -1)->Set("Jungle*.ogg",300,200);
  CreateObject(SE4K, 2600, 1100, -1)->Set("Jungle*.ogg",800,200);
  CreateObject(SE4K, 3500, 1100, -1)->Set("Jungle*.ogg",300,200);
  CreateObject(SE4K, 4100, 1100, -1)->Set("Jungle*.ogg",800,200);
*/
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
    pCrew->CreateContents(FLAG);
  
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
    iX = RandomX(1260,1360);
    iY = -50;
  }
  else
  {
    iX = RandomX(5640,5740);
    iY = -50;
  }
}
