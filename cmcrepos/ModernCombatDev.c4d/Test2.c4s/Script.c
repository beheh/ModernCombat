/*-- CMC Test --*/

#strict

func ChooserRuleConfig()
{
  return [FDMG,NOFF,NOAM];
}

func Initialize()
{
  //Tigerjets
  CreateObject(TIGJ, 1180, 515, -1)->AutoRespawn();
  CreateObject(TIGJ, 3500, 515, -1)->AutoRespawn();

  //Waffenautomaten
  SetUpStore(CreateObject(WPVM, 1110, 560, -1));
  SetUpStore(CreateObject(WPVM, 3440, 560, -1));

  //Kommunikationsradare
  CreateObject(RADR, 975, 490, -1);
  CreateObject(RADR, 3300, 490, -1);

  //Explosivfass
  CreateObject(XBRL, 3035, 490, -1)->AutoRespawn();

  //Benzinfass
  CreateObject(PBRL, 705, 490, -1)->AutoRespawn();

  //Kiste
  CreateObject(WCR2, 1247, 560, -1)->AutoRespawn();

  //Bodenlucken
  CreateObject(HA4K, 950, 493, -1);
  CreateObject(HA4K, 1250, 493, -1);
  CreateObject(HA4K, 3280, 493, -1);
  CreateObject(HA4K, 3580, 493, -1);
}

/* Relaunch */

public func InitializePlayer(int iPlr, int iX, int iY, object pBase, int iTeam)
{
  //for(var i=0, pCrew ; pCrew = GetCrew(iPlr, i) ; i++)
    RelaunchPlayer(iPlr, GetCrew(iPlr), 0, iTeam);
}

public func RelaunchPlayer(int iPlr, object pCrew, object pKiller, int iTeam)
{
  // Kein ordentlicher Spieler?
  if(GetOwner(pCrew) == NO_OWNER || iPlr == NO_OWNER || !GetPlayerName(iPlr))
    return();
  // Kein Team
  if(!iTeam) iTeam = GetPlayerTeam(iPlr);
  
  // Clonk tot?
  //if(!GetAlive(pCrew))
    pCrew = RelaunchClonk(iPlr, pCrew);
    
    // Zufallsposition
    var iX, iY;
    RelaunchPosition(iX, iY, iTeam);
  
    if(Contained(pCrew))
      SetPosition(iX, iY, Contained(pCrew));
    else
      SetPosition(iX, iY, pCrew);
}

public func RelaunchClonk(int iPlr, object pCursor)
{
  var pClonk;

    if(!GetAlive(pCursor))
    {
      pClonk = CreateObject(PCMK, 0, 0, iPlr);//default
      MakeCrewMember(pClonk, iPlr);
      
      if(FindObject(WPCH))
        DoWealth(iPlr, +25);//O,o
    }
    else
    {
      pClonk = pCursor;
    }

  SetCursor(iPlr, pClonk);
  SetPlrView(iPlr, pClonk);

  return(pClonk);
}

public func RelaunchPosition(& iX, & iY, int iTeam)
{
  iX = 1220;
  iY = 555;
}

public func SetUpStore(pStore)
{
  pStore->AddWares("IsWeapon", -1);
  pStore->AddWares("IsAmmoPacket", -1);
  pStore->AddWares("IsUpgrade", -1);
  pStore->AddWares("IsEquipment", -1);
  pStore->SortWare("IsWeapon","IsAmmoPacket","IsUpgrade","IsEquipment");
}