#strict

static start;

func Initialize()
{
  SetPlayList("HalfLife.ogg");
  Music("HalfLife",true);
  ScriptGo(true);
  
  CreateFurniture();
}

func CreateFurniture()
{
  var tmp = 0;
  start = CreateObject (ROM2, 58, 340, -1);

  //Sicherheits-Systeme
  tmp = CreateObject (HNG2, 929, 490, -1);
  CreateObject (CONS, 459, 414, -1)->Set(tmp);
  
  tmp = CreateObject (SLDR, 234, 490, -1);
  tmp->Lock();
  tmp->SetMaxDamage(-1);
  CreateObject (LKEY, 219, 483, -1)->Set(tmp,1);
  CreateObject (KCRD, 142, 328, -1)->SetKeyNumber(1);
  
  tmp = CreateObject (HNG2, 829, 220, -1);
  CreateObject (CONS, 851, 211, -1)->Set(tmp);
  
  tmp = CreateObject (SLDR, 375, 490, -1);
  tmp->Lock();
  tmp->SetMaxDamage(-1);
  CreateObject (SWTH, 395, 484, -1)->Set(tmp);
  
  tmp = CreateObject (SEDR, 381, 590, -1);
  tmp->Lock();
  tmp->SetMaxDamage(-1);
  CreateObject (SWTH, 352, 584, -1)->Set(tmp);
  
  tmp = CreateObject (STDR, 508, 421, -1);
  tmp->Lock();
  tmp->SetMaxDamage(-1);
  CreateObject (SWTH, 489, 414, -1)->Set(tmp);
  
  //Items u.Ä.
  tmp = CreateObject (LCKR, 83, 340, -1);
  tmp = tmp->CreateContents(H2PW);
  tmp->DoAmmo(tmp->GetFMData(FM_AmmoID),tmp->GetFMData(FM_AmmoLoad));
  tmp = CreateObject (LCKR, 96, 340, -1);
  tmp->SetAction("Open");
  tmp->SetPhase(2);
  
  CreateObject (WCR2, 727, 602, -1)->SetClrModulation(RGB(200,255,128));
  CreateObject (WCR2, 747, 602, -1)->SetClrModulation(RGB(200,255,128));
  tmp = CreateObject (H2LS, 728, 568, -1);
  tmp->SetCon(75);
  tmp->SetClrModulation(RGBa(255,255,255,128));

  tmp = CreateObject (H2IC, 736, 577, -1);
  tmp->CreateContents(H2GR,0,2);
  tmp = tmp->CreateContents(H2RW);
  tmp->DoAmmo(tmp->GetFMData(FM_AmmoID),tmp->GetFMData(FM_AmmoLoad));
  
  //Zeugs
  CreateObject (WCR2, 629, 221, -1);
  CreateObject (WCR2, 612, 221, -1);
  CreateObject (WCR2, 627, 431, -1);
  CreateObject (PBRL, 742, 429, -1);
  CreateObject (PBRL, 819, 460, -1);
  CreateObject (PBRL, 828, 460, -1);
  CreateObject (XBRL, 788, 489, -1);
  CreateObject (XBRL, 563, 429, -1);
  CreateObject (XBRL, 574, 429, -1);
  CreateObject (CRAT, 853, 461, -1);
  CreateObject (CRAT, 869, 490, -1);
  CreateObject (CRAT, 152, 489, -1);
  CreateObject (TABL, 145, 339, -1);
  CreateObject (BED2, 240, 309, -1);
  CreateObject (DEB1, 469, 603, -1);
  CreateObject (PLNT, 150, 328, -1);
  CreateObject (LFTP, 975, 251, -1);
  CreateObject (LFTP, 705, 504, -1);
  CreateObject (LFTP, 265, 604, -1);
  CreateObject (LFTP, 345, 343, -1);
  tmp = CreateObject (STDR, 115, 341, -1);
  tmp->Lock();
  tmp->SetMaxDamage(-1);
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
}

func Script3()
{
  if(!ObjectCount(MCOP))
  {
    var won = true;
  
    for(var i = 0; i < GetPlayerCount(C4PT_User); i++)
      for(var j=0, pCrew ; pCrew = GetCrew(GetPlayerByIndex(i, C4PT_User), j) ;j++)
        if(GetY(pCrew) > 220)
        {
          won = false;
          break;
        }

    if(won) GameOver(35*3);
  }
  goto(0);
}

func InitializePlayer(int iPlr, int iX, int iY, object pBase, int iTeam)
{
  for(var i=0, pCrew ; pCrew = GetCrew(iPlr, i) ; i++)
    RelaunchPlayer(iPlr, pCrew, 0, iTeam);
}

func RelaunchPlayer(int iPlr, object pCrew, object pKiller)
{
  if(GetOwner(pCrew) == NO_OWNER || iPlr == NO_OWNER || !GetPlayerName(iPlr))
    return();

  if(!GetAlive(pCrew))
    pCrew = RelaunchClonk(iPlr, pCrew);

  pCrew->Enter(start);
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

  return(pClonk);
}