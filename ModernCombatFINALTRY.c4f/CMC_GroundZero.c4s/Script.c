/*-- Ground Zero --*/

#strict
#include CSTD

static aFlag;
static aFlagnames = [Format("$Flag1$"), Format("$Flag2$"), Format("$Flag3$"), Format("$Flag4$"), Format("$Flag5$")];

public func GetFlagNames() {return (aFlagnames);}


/* Regelvoreinstellung */

func ChooserRuleConfig()
{
  //Standardregelsatz: Kein FriendlyFire, Waffen bleiben, Arena, Klassenwahl
  return [NOFF,WPST,NODR,MCSL];
}

/* Initalisierung */

func Initialize()
{
  //Starttitel und Musikliste zusammenstellen
  SetPlayList("CMC_*.ogg");
  Music("CMC_No Good.ogg");
  //Himmel
  SetSkyParallax(0,15,15);
  //Flaggennamen
  aFlagnames = [Format("$Flag1$"),Format("$Flag2$"),Format("$Flag3$"),Format("$Flag4$"),Format("$Flag5$")];
  //Flaggen
  aFlag = [];
  //Szenario einrichten
  CreateFurniture();
  //Equipment plazieren
  CreateEquipment();
  return(1);
}

/* Plazierungslisten */

func CreateFurniture()
{
  Log("$CreatingFurniture$");
  //Alle Objekte folglich von links oben nach rechts unten

  //Aufzüge
  CreateObject(LFTP, 165, 500, -1);
  CreateObject(LFTP, 615, 380, -1);
  CreateObject(LFTP, 1025, 390, -1);
  var liftp = CreateObject (LFTP, 1105, 185, -1);
  liftp->DisableVertical();
  liftp->ContactLeft();
  CreateObject(LFTP, 1775, 400, -1);
  CreateObject(LFTP, 2185, 405, -1);
  CreateObject(LFTP, 2635, 435, -1);

  //Leitern
  CreateObject(LADR, 535, 320, -1)->Set(7);
  CreateObject(LADR, 695, 320, -1)->Set(7);
  CreateObject(LADR, 1025, 310, -1)->Set(17);
  CreateObject(LADR, 35, 530, -1)->Set(17);
  CreateObject(LADR, 835, 580, -1)->Set(15);
  CreateObject(LADR, 1205, 520, -1)->Set(9);
  CreateObject(LADR, 1775, 310, -1)->Set(17);
  CreateObject(LADR, 2105, 320, -1)->Set(7);
  CreateObject(LADR, 2265, 320, -1)->Set(7);
  CreateObject(LADR, 1595, 520, -1)->Set(9);
  CreateObject(LADR, 1965, 580, -1)->Set(15);
  CreateObject(LADR, 2765, 530, -1)->Set(17);

  //Bodenlucken
  CreateObject(HA4K, 40, 393, -1);
  CreateObject(HA4K, 2760, 393, -1);

  //Große Bodenlucken
  CreateObject(H24K, 1025, 178, -1);
  CreateObject(H24K, 1775, 178, -1);

  //Stahlbrücken
  CreateObject(_HBR, 305, 352, -1);
  CreateObject(_HBR, 2495, 352, -1);

  //Geländer
  CreateObject(RAI1, 0, 120, -1)->SetRail([1,1,1]);
  CreateObject(RAI1, 360, 390, -1)->SetRail([1]);
  CreateObject(RAI1, 360, 460, -1)->SetRail([1]);
  CreateObject(RAI1, 1330, 420, -1)->SetRail([1,1,1,1,1,1,1]);
  CreateObject(RAI1, 2415, 390, -1)->SetRail([1]);
  CreateObject(RAI1, 2415, 460, -1)->SetRail([1]);
  CreateObject(RAI1, 2740, 120, -1)->SetRail([1,1,1]);

  //Glasscheiben
  CreateObject(_WIN, 912, 320, -1);
  CreateObject(_WIN, 912, 370, -1);
  CreateObject(_WIN, 912, 420, -1);
  CreateObject(_WIN, 922, 260, -1);
  CreateObject(_WIN, 982, 170, -1);
  CreateObject(_WIN, 1128, 260, -1);
  CreateObject(_WIN, 1138, 320, -1);
  CreateObject(_WIN, 1138, 370, -1);
  CreateObject(_WIN, 1138, 420, -1);

  CreateObject(_WIN, 1662, 320, -1);
  CreateObject(_WIN, 1662, 370, -1);
  CreateObject(_WIN, 1662, 420, -1);
  CreateObject(_WIN, 1672, 260, -1);
  CreateObject(_WIN, 1818, 170, -1);
  CreateObject(_WIN, 1878, 260, -1);
  CreateObject(_WIN, 1888, 320, -1);
  CreateObject(_WIN, 1888, 370, -1);
  CreateObject(_WIN, 1888, 420, -1);

  //Satellitenschüsseln
  CreateObject(RADR, 170, 220, -1);
  CreateObject(RADR, 2630, 220, -1);

  //Automaten
  CreateObject(CLVM, 225, 460, -1);
  CreateObject(SPVM, 2570, 460, -1);

  //Glastische
  CreateObject(GTBL, 300, 460, -1);
  CreateObject(GTBL, 950, 370, -1);
  CreateObject(GTBL, 1100, 370, -1);

  CreateObject(GTBL, 1700, 370, -1);
  CreateObject(GTBL, 1850, 370, -1);
  CreateObject(GTBL, 2500, 460, -1);

  //Stationäre Gastanks
  CreateObject(GSTA, 300, 390, -1)->AutoRespawn();
  CreateObject(GSTA, 2500, 390, -1)->AutoRespawn();

  //Wandlampen
  CreateObject(BLGH, 940, 310, -1);
  CreateObject(BLGH, 1110, 310, -1);
  CreateObject(BLGH, 1690, 305, -1);
  CreateObject(BLGH, 1860, 305, -1);

  //Büsche
  CreateObject(BUSH, 1230, 445, -1);
  CreateObject(BUSH, 1245, 445, -1);
  CreateObject(BUSH, 1550, 445, -1);
  CreateObject(BUSH, 1565, 445, -1);

  //Bäume
  CreateObject(TRE3, 1300, 433, -1);
  CreateObject(TRE3, 1500, 433, -1);

  //Schornsteine
  CreateObject(FNNL,230,120,-1)->SetCon(100);
  CreateObject(FNNL,2130,160,-1)->SetCon(100);

  //Metallkisten
  CreateObject(MWCR, 510, 320, -1);
  CreateObject(MWCR, 2290, 320, -1);

  //Kisten
  CreateObject(WCR2, 225, 390, -1)->AutoRespawn();
  CreateObject(WCR2, 660, 390, -1)->AutoRespawn();
  CreateObject(WCR2, 1115, 420, -1)->AutoRespawn();
  CreateObject(WCR2, 1330, 630, -1)->AutoRespawn();
  CreateObject(WCR2, 1480, 630, -1)->AutoRespawn();
  CreateObject(WCR2, 1685, 420, -1)->AutoRespawn();
  CreateObject(WCR2, 2130, 390, -1)->AutoRespawn();
  CreateObject(WCR2, 2575, 390, -1)->AutoRespawn();

  //Explosive Kisten
  CreateObject(XWCR, 1155, 320, -1)->AutoRespawn();
  CreateObject(XWCR, 1645, 320, -1)->AutoRespawn();

  //Explosivfässer
  CreateObject(XBRL, 335, 280, -1)->AutoRespawn();
  CreateObject(XBRL, 425, 450, -1)->AutoRespawn();
  CreateObject(XBRL, 760, 450, -1)->AutoRespawn();
  CreateObject(XBRL, 2035, 450, -1)->AutoRespawn();
  CreateObject(XBRL, 2375, 450, -1)->AutoRespawn();
  CreateObject(XBRL, 2465, 280, -1)->AutoRespawn();

  //Phosphorfässer
  CreateObject(HBRL, 440, 450, -1)->AutoRespawn();
  CreateObject(HBRL, 2360, 450, -1)->AutoRespawn();

  //Benzintanks
  CreateObject(XTNK, 320, 280, -1)->AutoRespawn();
  CreateObject(XTNK, 2480, 280, -1)->AutoRespawn();

  //Phosphortanks
  CreateObject(PTNK, 450, 450, -1)->AutoRespawn();
  CreateObject(PTNK, 2355, 450, -1)->AutoRespawn();

  //Sandsackbarrieren
  CreateObject(SBBA, 425, 290, -1)->Right();
  CreateObject(SBBA, 1345, 420, -1);
  CreateObject(SBBA, 1455, 420, -1)->Right();
  CreateObject(SBBA, 2375, 290, -1);

  //Lüftungsgitter
  CreateObject(ENGT, 165, 510, -1);
  CreateObject(ENGT, 2635, 510, -1);

  //Topfpflanzen
  CreateObject(PLT2, 260, 290, -1);
  CreateObject(PLT2, 2540, 290, -1);

  //Scheinwerfer
  CreateObject(FLGH, 1370, 420, -1);
  CreateObject(FLGH, 1430, 420, -1);

  //Monitore
  CreateObject(MONI, 310, 448, -1);
  CreateObject(MONI, 960, 358, -1);
  CreateObject(MONI, 1090, 358, -1);
  CreateObject(MONI, 1710, 358, -1);
  CreateObject(MONI, 1840, 358, -1);
  CreateObject(MONI, 2485, 448, -1);

  //Pflanzen
  CreateObject(PLNT, 200, 460, -1);
  CreateObject(PLNT, 1010, 310, -1);
  CreateObject(PLNT, 1790, 310, -1);
  CreateObject(PLNT, 2600, 460, -1);

  //Dekoschleusen
  CreateObject(GAT1, 105, 340, -1);
  CreateObject(GAT1, 2695, 340, -1);

  //Warnschild
  CreateObject(EXSN, 200, 380, -1);
  CreateObject(EXSN, 2600, 380, -1);

  //Zäune
  CreateObject(FENC, 630, 260, -1);
  CreateObject(FENC, 2170, 260, -1);

  //Topfpflanzen
  CreateObject(PLT3, 580, 390, -1);
  CreateObject(PLT3, 2150, 390, -1);

  //Regale
  CreateObject(FRAM, 555, 440, -1);
  CreateObject(FRAM, 675, 440, -1);
  CreateObject(FRAM, 985, 420, -1);
  CreateObject(FRAM, 985, 440, -1);
  CreateObject(FRAM, 1065, 420, -1);
  CreateObject(FRAM, 1065, 440, -1);

  CreateObject(FRAM, 1735, 420, -1);
  CreateObject(FRAM, 1735, 440, -1);
  CreateObject(FRAM, 1815, 420, -1);
  CreateObject(FRAM, 1815, 440, -1);
  CreateObject(FRAM, 2125, 440, -1);
  CreateObject(FRAM, 2245, 440, -1);

  //Notausgangslichter
  CreateObject(ETLT, 325, 430, -1);
  CreateObject(ETLT, 2475, 430, -1);

  //Stahltüren
  CreateObject(SLDR, 125, 540, -1);
  CreateObject(SLDR, 205, 540, -1);
  CreateObject(SLDR, 585, 320, -1);
  CreateObject(SLDR, 645, 320, -1);

  CreateObject(SLDR, 2155, 320, -1);
  CreateObject(SLDR, 2215, 320, -1);
  CreateObject(SLDR, 2595, 540, -1);
  CreateObject(SLDR, 2675, 540, -1);

  //Einfache Türen
  CreateObject(LBDR, 225, 290, -1);
  CreateObject(LBDR, 345, 390, -1);
  CreateObject(LBDR, 345, 460, -1);

  CreateObject(LBDR, 535, 390, -1);
  CreateObject(LBDR, 525, 450, -1);
  CreateObject(LBDR, 685, 390, -1);
  CreateObject(LBDR, 695, 450, -1);

  CreateObject(LBDR, 985, 260, -1);
  CreateObject(LBDR, 985, 370, -1);
  CreateObject(LBDR, 1055, 260, -1);
  CreateObject(LBDR, 1055, 370, -1);

  CreateObject(LBDR, 1735, 260, -1);
  CreateObject(LBDR, 1735, 370, -1);
  CreateObject(LBDR, 1805, 260, -1);
  CreateObject(LBDR, 1805, 370, -1);

  CreateObject(LBDR, 2095, 450, -1);
  CreateObject(LBDR, 2105, 390, -1);
  CreateObject(LBDR, 2255, 390, -1);
  CreateObject(LBDR, 2265, 450, -1);

  CreateObject(LBDR, 2565, 290, -1);
  CreateObject(LBDR, 2445, 390, -1);
  CreateObject(LBDR, 2445, 460, -1);

  //Selbstschussanlagen und Konsolen
  var selfd = CreateObject (SEGU, 415, 329, -1);
  selfd->Arm(PPGN);
  CreateObject (CONS, 250, 385, -1)
  ->Set(selfd);
  var selfd = CreateObject (SEGU, 2385, 329, -1);
  selfd->Arm(PPGN);
  CreateObject (CONS, 2550, 385, -1)
  ->Set(selfd);

  //Tore und Konsolen
  var autod = CreateObject (HNG2, 1400, 610, -1);
  autod->Open();
  CreateObject (CONS, 1270, 605, -1)
  ->Set(autod);
  var autod = CreateObject (HNG2, 1400, 520, -1);
  autod->Open();
  CreateObject (CONS, 1530, 605, -1)
  ->Set(autod);

  //Sounds

  //Wind
  CreateObject(SE4K, 800, 160, -1)->Set("WindSound*.ogg",775,250);
  CreateObject(SE4K, 1990, 160, -1)->Set("WindSound*.ogg",775,250);

  //Hallen
  CreateObject(SE4K, 190, 340, -1)->Set("Interior*.ogg",665,105);
  CreateObject(SE4K, 615, 340, -1)->Set("Interior*.ogg",670,105);
  CreateObject(SE4K, 1025, 340, -1)->Set("Interior*.ogg",665,105);
  CreateObject(SE4K, 1775, 340, -1)->Set("Interior*.ogg",670,105);
  CreateObject(SE4K, 2185, 340, -1)->Set("Interior*.ogg",665,105);
  CreateObject(SE4K, 2610, 340, -1)->Set("Interior*.ogg",670,105);

  //Rush Hour
  CreateObject(SE4K, 820, 390, -1)->Set("Traffic*.ogg",245,70);
  CreateObject(SE4K, 1970, 390, -1)->Set("Traffic*.ogg",245,70);

  //Sirenen
  CreateObject(SE4K, 450, 350, -1)->Set("Siren*.ogg",4000,140);
  CreateObject(SE4K, 2350, 350, -1)->Set("Siren*.ogg",4000,140);
}

func CreateEquipment()
{
  Log("$CreatingEquipment$");
  //Alle Objekte folglich von links oben nach rechts unten

  //Projektilmunition
  PlaceSpawnpoint(ABOX, 130, 285);
  PlaceSpawnpoint(ABOX, 1100, 250);
  PlaceSpawnpoint(ABOX, 1700, 250);
  PlaceSpawnpoint(ABOX, 2670, 285);

  //Granatenmunition
  PlaceSpawnpoint(GBOX, 1100, 615);
  PlaceSpawnpoint(GBOX, 1580, 615);

  //Raketen
  PlaceSpawnpoint(MIAP, 950, 250);
  PlaceSpawnpoint(MIAP, 1850, 250);

  //Sturmgewehre
  PlaceSpawnpoint(ASTR, 295, 335);
  PlaceSpawnpoint(ASTR, 2505, 335);

  //Slingshot
  PlaceSpawnpoint(SGST, 1400, 605);

  //Shotgun
  PlaceSpawnpoint(PPGN, 1400, 165);

  //C4
  PlaceSpawnpoint(C4PA, 530, 525);
  PlaceSpawnpoint(C4PA, 2270, 525);

  //Feldgranaten
  PlaceSpawnpoint(FGRN, 60, 520);
  PlaceSpawnpoint(FGRN, 950, 505);
  PlaceSpawnpoint(FGRN, 1850, 505);
  PlaceSpawnpoint(FGRN, 2740, 1785);

  //Splittergranaten
  PlaceSpawnpoint(FRAG, 670, 250);
  PlaceSpawnpoint(FRAG, 2130, 250);
}

/* Regelwähler */

public func ChooserFinished()
{
  inherited();

  //Teams abfragen
  var aTeams = [false,false,false,false,false];
  for(var i = 0; i < GetPlayerCount(); i++)
    aTeams[GetPlayerTeam(GetPlayerByIndex(i))] = true;

  //DM/LMS-Spielziel
  if(FindObject(GTDM) || FindObject(GLMS))
  {
   //Grenzen
   CreateObject(BRDR, 790, 0, -1)->Set(0);
   CreateObject(BRDR, 2010, 0, -1)->Set(1);

   //Warnschilder
   CreateObject(SGNP, 720, 390, -1);
   CreateObject(SGNP, 720, 450, -1);

   CreateObject(SGNP, 2080, 390, -1);
   CreateObject(SGNP, 2080, 450, -1);
  }

  //CTF-Spielziel
  if(FindObject(GCTF))
  {
   if(aTeams[1] == true)
   {CreateFlag(1,790,450,GetTeamColor(1));}
   if(aTeams[2] == true)
   {CreateFlag(2,2010,450,GetTeamColor(2));}

   //Grenzen
   CreateObject(BRDR, 440, 0, -1)->Set(0);
   CreateObject(BRDR, 2360, 0, -1)->Set(1);

   //Warnschilder
   CreateObject(SGNP, 410, 290, -1);
   CreateObject(SGNP, 460, 450, -1);
   CreateObject(SGNP, 2340, 450, -1);
   CreateObject(SGNP, 2385, 290, -1);
  }

  //OP-Spielziel
  if(FindObject(GOCC))
  {
   aFlag[0] = CreateObject(OFPL,100,390,NO_OWNER);
   if(aTeams[1] == true)
   {
    aFlag[0]->Set("$Flag1$",100,2);
    aFlag[0]->Capture(1,1);
   }
   else
   {
    aFlag[0]->Set("$Flag1$",0,2);
   }

   aFlag[1] = CreateObject(OFPL,780,450,NO_OWNER);
    aFlag[1]->Set("$Flag2$",0,2);

   aFlag[2] = CreateObject(OFPL,1400,420,NO_OWNER);
    aFlag[2]->Set("$Flag3$",0,2);

   aFlag[3] = CreateObject(OFPL,2020,450,NO_OWNER);
    aFlag[3]->Set("$Flag4$",0,2);

   aFlag[4] = CreateObject(OFPL,2690,390,NO_OWNER);
   if(aTeams[2] == true)
   {
    aFlag[4]->Set("$Flag5$",100,2);
    aFlag[4]->Capture(2,1);
   }
   else
   {
    aFlag[4]->Set("$Flag5$",0,2);
   }
  }
}

/* Relaunch */

public func RelaunchPosition(& iX, & iY, int iTeam)
{
  //DM/LMS-Spielziel
  if(FindObject(GTDM) || FindObject(GLMS))
  {
   if(iTeam == 1)
   {
    iX = 940;
    iY = 250;
    if(!Random(2)) iY = 410;
   }
   if(iTeam == 2)
   {
    iX = 1860;
    iY = 250;
    if(!Random(2)) iY = 410;
   }
   return(1);
  }

  //CTF-Spielziel
  if(FindObject(GCTF))
  {
   if(iTeam == 1)
   {
    iX = 560;
    iY = 310;
    if(!Random(2)) iY = 380;
   }
   if(iTeam == 2)
   {
    iX = 2240;
    iY = 310;
    if(!Random(2)) iY = 380;
   }
   return(1);
  }

  //Startsicht
  iX = 1400; iY = 300;
}