#strict

func Initialize()
{
  SetPlayList("HalfLife.ogg");
  Music("HalfLife",true);
  ScriptGo(true);
  SetNextMission("HalfLife.c4f/SP_Demo2.c4s");
}

func Script3()
{
  if(!ObjectCount(MCOP))
  {
    var won = true;
  
    for(var i = 0; i < GetPlayerCount(C4PT_User); i++)
      for(var j=0, pCrew ; pCrew = GetCrew(GetPlayerByIndex(i, C4PT_User), j) ;j++)
        if(GetX(pCrew) < 950)
        {
          won = false;
          break;
        }

    if(won)
    {
      Message4K(0,"MISSION COMPLETE",35*3,RGB(128,128,128),RGB(255,128,0));
      GameOver(35*3);
    }
  }
  goto(0);
}



//Relaunch

protected func InitializePlayer(int iPlr)
{
  for(var i=0, pCrew ; pCrew = GetCrew(iPlr, i) ; i++)
    RelaunchPlayer(iPlr, pCrew);
}

public func RelaunchPlayer(int iPlr, object pCrew, object pKiller, int iTeam)
{
  if(GetOwner(pCrew) == NO_OWNER || iPlr == NO_OWNER || !GetPlayerName(iPlr))
    return();

  if(!GetAlive(pCrew))
    pCrew = RelaunchClonk(iPlr, pCrew);
    
  var hev = CreateObject(HEVS,10,10,iPlr);
  hev->Activate(pCrew);

  var iX, iY;
  RelaunchPosition(iX, iY, iTeam);

  SetPosition(iX, iY, pCrew);
  pCrew->SetDir(DIR_Left);
}

public func RelaunchClonk(int iPlr, object pCursor)
{
  var pClonk = CreateObject(PCMK, 10, 10, iPlr);
  if(pCursor)
    GrabObjectInfo(pCursor, pClonk);
  else
    MakeCrewMember(pClonk, iPlr);

  DoEnergy(+150, pClonk);
  SetCursor(iPlr, pClonk);
  SetPlrView(iPlr, pClonk);

  return(pClonk);
}

public func RelaunchPosition(& iX, & iY)
{
  iX = 183;
  iY = 150;
}
