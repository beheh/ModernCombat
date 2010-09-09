/*-- Magma Zone --*/

#strict
#include CSTD

static aFlag, aSelfDefense;


/* Initalisierung */

func Initialize()
{
  //Starttitel und Musikliste zusammenstellen
  SetPlayList("CMC_Base Groove.ogg;CMC_Firehawk.ogg;CMC_Friendly Unit.ogg;CMC_Getaway.ogg;CMC_Moving Squad.ogg;CMC_Offensive.ogg;CMC_Showtime.ogg;CMC_Slow Motion.ogg;CMC_Striking Force.ogg;CMC_No Good.ogg;CMC_Obsession.ogg");
  Music("CMC_Moving Squad.ogg");
  //Flaggen
  aFlag = [];
  //Selbstschussanlage
  aSelfDefense = [];
  //Szenario einrichten
  CreateFurniture();
  //Ausrüstung plazieren
  CreateEquipment();
  return(1);
}

/* Plazierungslisten */

func CreateFurniture()
{
  Log("$CreatingFurniture$");
  //Alle Objekte folglich von links nach rechts

  //Bodenlucken
  CreateObject(HA4K, 410, 333, -1);
  CreateObject(HA4K, 800, 393, -1);
  CreateObject(HA4K, 1340, 343, -1);
  CreateObject(HA4K, 1340, 363, -1);
  CreateObject(HA4K, 1740, 373, -1);
  CreateObject(HA4K, 2120, 243, -1);

  //Leitern
  CreateObject(LADR, 410, 442, -1)->Set(13);
  CreateObject(LADR, 800, 502, -1)->Set(13);
  CreateObject(LADR, 1340, 540, -1)->Set(24);
  CreateObject(LADR, 1740, 466, -1)->Set(11);
  CreateObject(LADR, 2120, 528, -1)->Set(35);

  //Sprungschanzen
  CreateObject (JMPD, 365, 330, -1)->Set(105, 20);
  CreateObject (JMPD, 1375, 300, -1)->Set(85, 10);
  CreateObject (JMPD, 1690, 370, -1)->Set(85, -80);

  //Alarmlampen
  CreateObject(ALGH, 185, 260, -1)->TurnOn();
  CreateObject(ALGH, 1340, 250, -1)->TurnOn();
  CreateObject(ALGH, 2300, 170, -1)->TurnOn();

  //Markierungsschilder
  CreateObject(MSGN, 140, 300, -1);
  CreateObject(MSGN, 235, 300, -1);
  CreateObject(MSGN, 2240, 210, -1);
  CreateObject(MSGN, 2340, 210, -1);

  //Satellitenschüsseln
  CreateObject(RADR, 2085, 240, -1);

  //Feuerlöcher
  CreateObject(FIEX, 215, 295, -1);
  CreateObject(FIEX, 765, 385, -1);
  CreateObject(FIEX, 1375, 335, -1);
  CreateObject(FIEX, 2320, 205, -1);

  //Bildschirme
  CreateObject(SCR3, 410, 271, -1);
  CreateObject(SCR3, 1340, 281, -1);
  CreateObject(SCR3, 2120, 151, -1);

  //Ventillatoren
  CreateObject(VENT, 365, 240, -1)->SetCon(20);
  CreateObject(VENT, 365, 290, -1)->SetCon(20);
  CreateObject(VENT, 365, 330, -1)->SetCon(20);

  CreateObject(VENT, 765, 300, -1)->SetCon(20);
  CreateObject(VENT, 765, 350, -1)->SetCon(20);
  CreateObject(VENT, 765, 390, -1)->SetCon(20);

  CreateObject(VENT, 1305, 375, -1)->SetCon(20);
  CreateObject(VENT, 1375, 375, -1)->SetCon(20);

  CreateObject(VENT, 1775, 330, -1)->SetCon(20);

  CreateObject(VENT, 2085, 520, -1)->SetCon(20);

  CreateObject(VENT, 2155, 155, -1)->SetCon(20);
  CreateObject(VENT, 2155, 200, -1)->SetCon(20);
  CreateObject(VENT, 2155, 240, -1)->SetCon(20);

  //Regale
  CreateObject(FRAM, 430, 330, -1);
  CreateObject(FRAM, 450, 330, -1);
  CreateObject(FRAM, 470, 330, -1);

  //Büsche
  CreateObject(BSH2, 295, 130, -1);
  CreateObject(BSH2, 2305, 515, -1);

  //Stahlbrücken
  CreateObject(_HBR, 785, 660, -1);
  CreateObject(_HBR, 1740, 475, -1);
  CreateObject(_HBR, 1925, 510, -1);

  //Benzinfässer
  CreateObject(PBRL, 625, 520, -1)->AutoRespawn();
  CreateObject(PBRL, 1790, 290, -1)->AutoRespawn();

  //Giftfässer
  CreateObject(TBRL, 1380, 340, -1)->AutoRespawn();
  CreateObject(TBRL, 1395, 340, -1)->AutoRespawn();

  //Explosivfässer
  CreateObject(XBRL, 2065, 480, -1)->AutoRespawn();
  CreateObject(XBRL, 2100, 560, -1)->AutoRespawn();
  CreateObject(XBRL, 2115, 560, -1)->AutoRespawn();

  //Explosivtank
  CreateObject(XTNK, 2100, 560, -1)->AutoRespawn();

  //Gasflaschen
  CreateObject(GSBL, 800, 260, -1)->AutoRespawn();
  CreateObject(GSBL, 820, 390, -1)->AutoRespawn();
  CreateObject(GSBL, 1760, 370, -1)->AutoRespawn();

  //Steine
  CreateObject(STNE, 170, 435, -1);
  CreateObject(STNE, 400, 655, -1);
  CreateObject(STNE, 830, 165, -1);
  CreateObject(STNE, 1140, 435, -1);
  CreateObject(STNE, 1645, 495, -1);
  CreateObject(STNE, 2270, 375, -1);

  //Kisten
  CreateObject(WCR2, 760, 260, -1)->AutoRespawn();
  CreateObject(WCR2, 770, 242, -1);
  CreateObject(WCR2, 780, 260, -1)->AutoRespawn();
  CreateObject(WCR2, 840, 390, -1)->AutoRespawn();
  CreateObject(WCR2, 844, 370, -1);
  CreateObject(WCR2, 1290, 460, -1)->AutoRespawn();
  CreateObject(WCR2, 1780, 370, -1)->AutoRespawn();

  //Flutlichter
  CreateObject(FLGH, 445, 330, -1)->SetRotation(20);
  CreateObject(FLGH, 1775, 290, -1)->SetRotation(-80);
  CreateObject(FLGH, 2160, 240, -1)->SetRotation(-20);

  //Geländer
  CreateObject(RAI1, 143, 300, -1)->SetRail([1,3,1,3,1,3,1]);
  CreateObject(RAI1, 760, 390, -1)->SetRail([1,3,1,3,1,3,1]);
  CreateObject(RAI1, 1300, 340, -1)->SetRail([1,3,1,3,1,3,1]);
  CreateObject(RAI1, 1300, 460, -1)->SetRail([1,3,1,3,1,3,1]);
  CreateObject(RAI1, 1700, 370, -1)->SetRail([1,3,1,3,1,3,1]);
  CreateObject(RAI1, 2080, 240, -1)->SetRail([1,3,1,3,1,3,1]);
  CreateObject(RAI1, 2080, 560, -1)->SetRail([1,3,1,3,1,3,1]);
  CreateObject(RAI1, 2250, 210, -1)->SetRail([1,3,1,3,1,3,1]);

  //Selbstschussanlage und Konsole
  aSelfDefense[0] = CreateObject(SEGU, 1305, 369, -1);
    aSelfDefense[0]->Arm(MISA);
    aSelfDefense[0]->SetAutoRepair(1500);
    CreateObject(CONS, 1300, 330, -1)->Set(aSelfDefense[0]);

  //Sounds

  //Erdrutsche
  CreateObject(SE4K, 470, 480, -1)->Set("FallingDirt*.ogg",800,250);
  CreateObject(SE4K, 1200, 200, -1)->Set("FallingDirt*.ogg",800,250);
  CreateObject(SE4K, 2240, 450, -1)->Set("FallingDirt*.ogg",800,250);

  //Hallen
  CreateObject(SE4K, 290, 250, -1)->Set("Interior*.ogg",665,105);
  CreateObject(SE4K, 1275, 395, -1)->Set("Interior*.ogg",765,105);
  CreateObject(SE4K, 2170, 290, -1)->Set("Interior*.ogg",665,105);

  //Lautsprecher
  CreateObject(SE4K, 545, 250, -1)->Set("Announce*.ogg",1500,130);
  CreateObject(SE4K, 1925, 210, -1)->Set("Announce*.ogg",1500,130);
}

func CreateEquipment()
{
  var tmp;
  Log("$CreatingEquipment$");
  //Alle Objekte folglich von links oben nach rechts unten

  //Munitionskiste (Kugeln)
  tmp = CreateObject(AMCT, 1315, 460, -1);
  tmp->Set(ABOX);

  //Granatenmunition
  PlaceSpawnpoint(GBOX, 1290, 80);

  //Raketen
  PlaceSpawnpoint(MIAP, 1360, 330);

  //Sturmgewehr
  PlaceSpawnpoint(ASTR, 1070, 235);

  //Maschinengewehr
  PlaceSpawnpoint(MNGN, 895, 115);

  //Raketenwerfer
  PlaceSpawnpoint(RTLR, 1390, 335);

  //Shotguns
  PlaceSpawnpoint(PPGN, 245, 545);
  PlaceSpawnpoint(PPGN, 1835, 445);

  //Slingshot
  PlaceSpawnpoint(SGST, 1315, 100);

  //Maschinenpistolen
  PlaceSpawnpoint(SMGN, 680, 145);
  PlaceSpawnpoint(SMGN, 1845, 125);

  //C4
  PlaceSpawnpoint(C4PA, 1500, 525);

  //Sprengfalle
  PlaceSpawnpoint(BBTP, 1250, 530);

  //Dragnin
  PlaceSpawnpoint(DGNN, 670, 645);
  PlaceSpawnpoint(DGNN, 1560, 155);

  //Feldgranaten
  PlaceSpawnpoint(FGRN, 560, 525);
  PlaceSpawnpoint(FGRN, 1875, 255);

  //Automat
  var store = CreateObject(WPVM,1305, 300,-1);
  store->AddWare(FAPK,-1);
  store->AddWare(RSHL,-1);
  store->AddWare(CDBT,-1);
  store->AddWare(FGRN,-1);
  store->AddWare(FRAG,-1);
  store->AddWare(PGRN,-1);
  store->AddWare(SGRN,-1);
  store->AddWare(STUN,-1);
}

/* Bei Flaggenübernahme */

func FlagCaptured(object pPoint, int iTeam)
{
  if(pPoint == aFlag[2])
   aSelfDefense[0]->SetTeam(iTeam);
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
   CreateObject(BRDR, 510, 0, -1)->Set(0);
   CreateObject(BRDR, 1965, 0, -1)->Set(1);

   //Warnschilder
   CreateObject(SGNP, 525, 350, -1);
   CreateObject(SGNP, 585, 650, -1);
   CreateObject(SGNP, 600, 520, -1);
   CreateObject(SGNP, 660, 150, -1);

   CreateObject(SGNP, 1850, 130, -1);
   CreateObject(SGNP, 1850, 450, -1);
   CreateObject(SGNP, 1960, 300, -1);
  }

  //CTF-Spielziel
  if(FindObject(GCTF))
  {
   if(aTeams[1] == true)
   {CreateFlag(1,455,330,GetTeamColor(1));}
   if(aTeams[2] == true)
   {CreateFlag(2,2010,310,GetTeamColor(2));}
  }

  //OP-Spielziel
  if(FindObject(GOCC))
  {
   //Flaggen
   aFlag[0] = CreateObject(OFPL,165,300,NO_OWNER);
   aFlag[0] -> AddSpawnPoint(300,490);
   aFlag[0] -> AddSpawnPoint(355,450);
   aFlag[0] -> AddSpawnPoint(545,340);
   if(aTeams[1] == true)
   {
    aFlag[0]->Set("$Flag1$",100,2);
    aFlag[0]->Capture(1,1);
   }
   else
   {
    aFlag[0]->Set("$Flag1$",0,2);
   }

   aFlag[1] = CreateObject(OFPL,780,390,NO_OWNER);
   aFlag[1] -> AddSpawnPoint(730,510);
   aFlag[1] -> AddSpawnPoint(825,570);
   aFlag[1] -> AddSpawnPoint(885,480);
   aFlag[1]->Set("$Flag2$",0,2);

   aFlag[2] = CreateObject(OFPL,1375,460,NO_OWNER);
   aFlag[2] -> AddSpawnPoint(1285,540);
   aFlag[2] -> AddSpawnPoint(1395,540);
   aFlag[2] -> AddSpawnPoint(1205,410);
   aFlag[2]->Set("$Flag3$",0,2);

   aFlag[3] = CreateObject(OFPL,1705,330,NO_OWNER);
   aFlag[3] -> AddSpawnPoint(1665,150);
   aFlag[3] -> AddSpawnPoint(1790,130);
   aFlag[3]->Set("$Flag4$",0,2);

   aFlag[4] = CreateObject(OFPL,2155,560,NO_OWNER);
   aFlag[3] -> AddSpawnPoint(1855,440);
   aFlag[4] -> AddSpawnPoint(1980,510);
   aFlag[4] -> AddSpawnPoint(2090,470);
   aFlag[4]->Set("$Flag5$",0,2);

   aFlag[5] = CreateObject(OFPL,2255,210,NO_OWNER);
   aFlag[5] -> AddSpawnPoint(2000,300);
   aFlag[5] -> AddSpawnPoint(2100,300);
   aFlag[5] -> AddSpawnPoint(2140,300);
   if(aTeams[2] == true)
   {
    aFlag[5]->Set("$Flag6$",100,2);
    aFlag[5]->Capture(2,1);
   }
   else
   {
    aFlag[5]->Set("$Flag6$",0,2);
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
    var rand = Random(2);
    if(!rand)
     { iX = 635; iY = 280; }
    if(!--rand)
     { iX = 685; iY = 390; }
   }
   if(iTeam == 2)
   {
    var rand = Random(2);
    if(!rand)
     { iX = 1825; iY = 350; }
    if(!--rand)
     { iX = 1845; iY = 230; }
   }
   return(1);
  }

  //CTF-Spielziel
  if(FindObject(GCTF))
  {
   if(iTeam == 1)
   {
    var rand = Random(2);
    if(!rand)
     { iX = 150; iY = 290; }
    if(!--rand)
     { iX = 220; iY = 290; }
   }
   if(iTeam == 2)
   {
    var rand = Random(2);
    if(!rand)
     { iX = 2155; iY = 230; }
    if(!--rand)
     { iX = 2250; iY = 200; }
   }
   return(1);
  }

  //Startsicht
  iX = 1545; iY = 350;
}

/* Assault */

public func CreateAssaultTargets() {
  AddAssaultTarget(CLVM, 780, 390, 300, 1);
  AddAssaultTarget(SADH, 680, 150, 300, 1, 0, 1);
  AddAssaultTarget(WPVM, 180, 300, 400, 1, 0, 2);
  AddAssaultTarget(CLVM, 1710, 330, 300, 2);
  AddAssaultTarget(SADH, 1840, 130, 300, 2, 0, 1);
  AddAssaultTarget(WPVM, 2280, 210, 400, 2, 0, 2);
}