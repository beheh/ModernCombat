#strict

func Initialize()
{
  SetPlayList("HalfLife.ogg");
  Music("HalfLife",true);
  CreateFurniture();
}

func CreateFurniture()
{
  var tmp = 0;

  //Sicherheits-Systeme (Nein... nicht im DM. :I)
  CreateObject (HNG2, 929, 490, -1);
  
  tmp = CreateObject (SLDR, 234, 490, -1);
  tmp->SetMaxDamage(-1);
  
  CreateObject (HNG2, 829, 220, -1);
  
  tmp = CreateObject (SLDR, 375, 490, -1);
  tmp->SetMaxDamage(-1);
  
  tmp = CreateObject (SEDR, 381, 590, -1);
  tmp->SetMaxDamage(-1);
  
  tmp = CreateObject (STDR, 508, 421, -1);
  tmp->SetMaxDamage(-1);
  
  tmp = CreateObject (STDR, 115, 341, -1);
  tmp->SetMaxDamage(-1);
  
  //Items u.Ä.
  CreateObject (LCKR, 83, 340, -1);
  tmp = CreateObject (LCKR, 96, 340, -1);
  tmp->SetAction("Open");
  tmp->SetPhase(2);
  
  CreateObject (WCR2, 727, 602, -1)->SetClrModulation(RGB(200,255,128));
  CreateObject (WCR2, 747, 602, -1)->SetClrModulation(RGB(200,255,128));
  tmp = CreateObject (H2LS, 728, 568, -1);
  tmp->SetCon(75);
  tmp->SetClrModulation(RGBa(255,255,255,128));
  
  //Zeugs
  CreateObject (WCR2, 629, 221, -1)->AutoRespawn();
  CreateObject (WCR2, 612, 221, -1)->AutoRespawn();
  CreateObject (WCR2, 627, 431, -1)->AutoRespawn();
  CreateObject (PBRL, 742, 429, -1)->AutoRespawn();
  CreateObject (PBRL, 819, 460, -1)->AutoRespawn();
  CreateObject (PBRL, 828, 460, -1)->AutoRespawn();
  CreateObject (XBRL, 788, 489, -1)->AutoRespawn();
  CreateObject (XBRL, 563, 429, -1)->AutoRespawn();
  CreateObject (XBRL, 574, 429, -1)->AutoRespawn();
  CreateObject (CRAT, 853, 461, -1)->AutoRespawn();
  CreateObject (CRAT, 869, 490, -1)->AutoRespawn();
  CreateObject (CRAT, 152, 489, -1)->AutoRespawn();
  CreateObject (TABL, 145, 339, -1);
  CreateObject (BED2, 240, 309, -1);
  CreateObject (DEB1, 469, 603, -1);
  CreateObject (PLNT, 150, 328, -1);
  CreateObject (LFTP, 975, 251, -1);
  CreateObject (LFTP, 705, 504, -1);
  CreateObject (LFTP, 265, 604, -1);
  CreateObject (LFTP, 345, 343, -1);
  CreateObject (LADR, 601, 578, -1)->Set(12,LADR_Front);
  CreateObject (LADR, 421, 408, -1)->Set(7,LADR_Front);
  CreateObject (LADR, 191, 328, -1)->Set(4,LADR_Front);
  CreateObject (GSTA, 894, 219, -1);
  CreateObject (CCP2, 323, 590, -1);
  CreateObject (CON1, 667, 220, -1);
  CreateObject (CON1, 825, 490, -1);
  CreateObject (CON1, 138, 463, -1);
  CreateObject (CON1, 108, 491, -1);
  CreateObject (SLDR, 295, 590, -1);
  CreateObject (SADH, 983, 130, -1);
  CreateObject (FIEX, 536, 422, -1);
  CreateObject (SCR3, 352, 572, -1)->SetCon(50);
  CreateObject (VENT, 868, 395, -1)->SetCon(50);
  CreateObject (BART, 574, 591, -1)->Set(2,1);
  CreateObject (ENGT, 440, 541, -1)->SetCon(50);
  CreateObject (ENGT, 910, 541, -1)->SetCon(50);
  CreateObject (PSTR, 275, 290, -1)->SetCon(50);
  CreateObject (LCOM, 272, 309, -1);
  CreateObject (TABL, 463, 419, -1);
  
  //Spawns
  
  //Granaten
  CreateObject (H2GR, 810, 461, -1)->AutoRespawn(35*40,40);
  
  //Shotgun
  CreateObject (H2SG, 141, 327, -1)->AutoRespawn(35*40,40);
  CreateObject (SGAP, 136, 327, -1)->AutoRespawn(35*40,40);
  
  //SMG
  CreateObject (H2MG, 459, 407, -1)->AutoRespawn(35*40,40);
  CreateObject (MGAP, 471, 407, -1)->AutoRespawn(35*40,40);
  
  //Revolver
  CreateObject (H2RW, 354, 590, -1)->AutoRespawn(35*40,40);
  CreateObject (REAP, 345, 589, -1)->AutoRespawn(35*40,40);
  
  //Heilkits
  CreateObject (H2HK, 154, 328, -1)->AutoRespawn(35*40,40);
  CreateObject (H2HK, 267, 430, -1)->AutoRespawn(35*40,40);
  
  //Kisten
  CreateObject (H2CS, 101, 461, -1)->Set(35*40,[H2GR,SGAP]);
  CreateObject (H2CS, 568, 289, -1)->Set(35*40,[H2GR,MGAP]);
  CreateObject (H2CS, 734, 582, -1)->Set(35*40,[H2GR,REAP]);
}

func InitializePlayer(int iPlr, int iX, int iY, object pBase)
{
  for(var i=0, pCrew ; pCrew = GetCrew(iPlr, i) ; i++)
    RelaunchPlayer(iPlr,pCrew,0);
}

func RelaunchPlayer(int iPlr, object pCrew, object pKiller)
{
  if(GetOwner(pCrew) == NO_OWNER || iPlr == NO_OWNER || !GetPlayerName(iPlr))
    return();

  if(!GetAlive(pCrew))
    pCrew = RelaunchClonk(iPlr, pCrew);
    
  var iX, iY;
  RelaunchPosition(iX, iY);

  if(!FindObject(IGIB))
  {
    var pistol = CreateContents(H2PW, pCrew);
    pistol->DoAmmo(pistol->GetFMData(FM_AmmoID),pistol->GetFMData(FM_AmmoLoad));
    pCrew->DoAmmo(pistol->GetFMData(FM_AmmoID),pistol->GetFMData(FM_AmmoID)->MaxAmmo());
  }
  
  var hev = CreateObject(HEVS,10,10,iPlr);
  hev->Activate(pCrew);

  if(Contained(pCrew))
    SetPosition(iX, iY, Contained(pCrew));
  else
    SetPosition(iX, iY, pCrew);
}

func RelaunchClonk(int iPlr, object pCursor)
{
  var pClonk = CreateObject(PCMK, 10, 10, iPlr);
  if(pCursor)
    GrabObjectInfo(pCursor, pClonk);
  else
    MakeCrewMember(pClonk, iPlr);
    
  DoEnergy(+150, pClonk);
  SetCursor(iPlr, pClonk);
  SetPlrView(iPlr, pClonk);
  
  // Wegstecken
  var tim = CreateObject(TIM2, 10, 10, -1);
  pClonk->Enter(tim);
  PlayerMessage(iPlr, Format("@%s", GetName(pClonk)), tim);

  return(pClonk);
}

func RelaunchPosition(& iX, & iY)
{
  var rand = Random(7);
  if(!rand)
    { iX = 58; iY = 330; }
  if(!--rand)
    { iX = 191; iY = 480; }
  if(!--rand)
    { iX = 249; iY = 300; }
  if(!--rand)
    { iX = 450; iY = 350; }
  if(!--rand)
    { iX = 639; iY = 280; }
  if(!--rand)
    { iX = 905; iY = 480; }
  if(!--rand)
    { iX = 601; iY = 580; }
}
