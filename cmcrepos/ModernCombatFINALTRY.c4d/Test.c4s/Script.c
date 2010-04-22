/*-- CMC Test --*/

#strict

func Initialize()
{
  //Aufzüge
  CreateObject(LFTP, 275, 100, -1);
  //var liftp = CreateObject(LFTP, 365, 95, -1);
  //liftp->DisableVertical();
  //liftp->ContactLeft();

  //Clonks
  CreateObject(PCMK, 645, 350, -1)->AutoRespawn();
  CreateObject(PCMK, 855, 350, -1)->AutoRespawn();

  //Granatenspawnpoints
  PlaceSpawnpoint(FRAG, 740, 590);
  PlaceSpawnpoint(FRG2, 865, 555);
  PlaceSpawnpoint(SGRN, 790, 460);
  PlaceSpawnpoint(STUN, 705, 465);

  //EHP-Spawnpoint
  PlaceSpawnpoint(FAPK, 1000, 340);

  //Waffenspawnpoints
  PlaceSpawnpoint(BNEL, 235, 70);
  PlaceSpawnpoint(92FS, 235, 160);
  PlaceSpawnpoint(ASTR, 235, 220);
  PlaceSpawnpoint(M249, 235, 280);
  PlaceSpawnpoint(MP7R, 235, 340);
  PlaceSpawnpoint(OICW, 320, 160);
  PlaceSpawnpoint(P29W, 320, 220);
  PlaceSpawnpoint(PDTW, 320, 280);
  PlaceSpawnpoint(PZFW, 710, 70);
  PlaceSpawnpoint(SPAS, 790, 640);

  //Leiter
  CreateObject(LADR, 505, 585, -1)->Set(26);

  //Waffenautomaten
  SetUpStore(CreateObject(WPVM, 410, 350, -1));
  SetUpStore(CreateObject(WPVM, 600, 350, -1));

  //Sprungschanze
  CreateObject (JMPD, 10, 350, -1)->Set(90, 11);

  //Explosivfass
  CreateObject(XBRL, 320, 350, -1)->AutoRespawn();

  //Benzinfass
  CreateObject(PBRL, 370, 350, -1)->AutoRespawn();

  //Kiste
  CreateObject(WCR2, 75, 220, -1)->AutoRespawn();

  //Bildschirm
  //CreateObject(SCA2, 505, 305, -1);

  //Selbstschussanlagen und Konsolen
  var selfd = CreateObject (SEGU, 75, 245, -1);
  selfd->Arm(MISA);
  selfd->SetAutoRepair(525);
  CreateObject (CONS, 195, 340, -1)->Set(selfd);
  var selfd = CreateObject (SEGU, 855, 70, -1);
  selfd->Arm(MG2A);
  selfd->SetAutoRepair(525);
  CreateObject (CONS, 855, 175, -1)->Set(selfd);

  //CMC Theme abspielen
  //Sound("CMCTheme.ogg");
  //Sound("Clonk");

  //MusicLevel(0);
  //Schedule("Music()", 100, 0, 0);
}

public func Music()
{
  //Starttitel und Musikliste zusammenstellen
  SetPlayList("CMC_*.ogg");
  MusicLevel(100);
  Music("CMC_Base Groove.ogg");
  return(1);
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
      pClonk = CreateObject(MDIC, 0, 0, iPlr);//default
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
  iX = LandscapeWidth()/2 + RandomX(-50,+50);
  iY = LandscapeHeight()/2;
}

public func SetUpStore(pStore)
{
  pStore->AddWares("IsWeapon", -1);
  pStore->AddWares("IsAmmoPacket", -1);
  pStore->AddWares("IsUpgrade", -1);
  pStore->AddWares("IsEquipment", -1);
  pStore->SortWare("IsWeapon","IsAmmoPacket","IsUpgrade","IsEquipment");
}
