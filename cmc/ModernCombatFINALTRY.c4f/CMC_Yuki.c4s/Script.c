/*-- Yuki --*/

#strict 2
#include CSTD

static aDoor1,aDoor2,aLamp1,aLamp2,aLamp3,aFlag;

func RecommendedGoals()	{return [GOCC, GBAS];}	//Spielzielempfehlung


/* Initialisierung */

func Initialize()
{
  //Wartemusik einstellen
  SetWaitingMusic();
  //Himmelparallaxität
  SetSkyParallax(1,50,15);
  //Türenteam1
  aDoor1 = [];
  //Türenteam2
  aDoor2 = [];
  //Lampen1
  aLamp1 = [];
  //Lampen2
  aLamp2 = [];
  //Lampen3
  aLamp3 = [];
  //Flaggen
  aFlag = [];
  //Einrichtung plazieren
  CreateInterior();
  //Ausrüstung plazieren
  CreateEquipment();
  //Dekoration plazieren
  CreateDecoration();
  return 1;
}

/* Plazierungslisten */

func CreateInterior()
{
  Log("$CreatingInterior$");

  //Leitern
  CreateObject(LADR, 410, 710, -1)->Set(25);
  CreateObject(LADR, 455, 485, -1)->Set(32);
  CreateObject(LADR, 565, 690, -1)->Set(21);
  CreateObject(LADR, 690, 730, -1)->Set(25);

  CreateObject(LADR, 1090, 750, -1)->Set(30);
  CreateObject(LADR, 1230, 750, -1)->Set(40);

  CreateObject(LADR, 1620, 810, -1)->Set(40);
  CreateObject(LADR, 1690, 730, -1)->Set(45);
  CreateObject(LADR, 1770, 805, -1)->Set(10);
  CreateObject(LADR, 1795, 620, -1)->Set(15);

  CreateObject(LADR, 1905, 620, -1)->Set(17);
  CreateObject(LADR, 1930, 360, -1)->Set(14);
  CreateObject(LADR, 2025, 550, -1)->Set(21);

  //Bodenluken
  CreateObject(HA4K, 560, 723, -1);
  CreateObject(HA4K, 1620, 753, -1);

  //Kiste
  CreateObject(WCR2, 1850, 240, -1)->AutoRespawn();

  //Metallkisten
  CreateObject(MWCR, 1140, 680, -1)->AutoRespawn();
  CreateObject(MWCR, 1660, 830, -1);

  //Verbandskisten
  CreateObject(BECR, 850, 570, -1)->AutoRespawn();
  CreateObject(BECR, 1640, 810, -1)->AutoRespawn();

  //Explosive Kisten
  CreateObject(XWCR, 1280, 580, -1)->AutoRespawn();
  CreateObject(XWCR, 1945, 480, -1)->AutoRespawn();

  //Container
  CreateObject(CON1, 1780, 690, -1)->SetGraphics("5");

  //Giftfässer
  CreateObject(TBRL, 1020, 750, -1)->AutoRespawn();
  CreateObject(TBRL, 1260, 750, -1)->AutoRespawn();

  //Benzinfässer
  CreateObject(PBRL, 800, 550, -1)->AutoRespawn();
  CreateObject(PBRL, 1580, 560, -1)->AutoRespawn();

  //Explosivfässer
  CreateObject(XBRL, 835, 760, -1)->AutoRespawn();
  CreateObject(XBRL, 1440, 380, -1);
  CreateObject(XBRL, 1500, 380, -1);
  CreateObject(XBRL, 1985, 380, -1)->AutoRespawn();

  //Phosphorfässer
  CreateObject(HBRL, 1325, 430, -1)->AutoRespawn();
  CreateObject(HBRL, 1720, 560, -1);

  //Benzintank
  CreateObject(XTNK, 900, 610, -1);

  //Phosphortank
  CreateObject(PTNK, 1300, 750, -1)->AutoRespawn();

  //Stahlbrücke
  CreateObject(_HBR, 1545, 762, -1)->SwitchMode();

  //Türenteam 1
  aDoor1[0] = CreateObject(SLDR, 505, 520, -1);
  aDoor1[0]->Lock();
  aDoor1[0]->Open();
  aDoor1[0]->SetMaxDamage(-1);
  aDoor1[1] = CreateObject(SLDR, 955, 350, -1);
  aDoor1[1]->Lock();
  aDoor1[1]->Open();
  aDoor1[1]->SetMaxDamage(-1);
  aDoor1[2] = CreateObject(SLDR, 955, 750, -1);
  aDoor1[2]->Lock();
  aDoor1[2]->Open();
  aDoor1[2]->SetMaxDamage(-1);
  aDoor1[3] = CreateObject(SLDR, 1405, 380, -1);
  aDoor1[3]->Lock();
  aDoor1[3]->Open();
  aDoor1[3]->SetMaxDamage(-1);
  aDoor1[4] = CreateObject(SLDR, 1405, 750, -1);
  aDoor1[4]->Lock();
  aDoor1[4]->Open();
  aDoor1[4]->SetMaxDamage(-1);
  aDoor1[5] = CreateObject(SLDR, 1835, 500, -1);
  aDoor1[5]->Lock();
  aDoor1[5]->Open();
  aDoor1[5]->SetMaxDamage(-1);

  //Türenteam 2
  aDoor2[0] = CreateObject(SLDR, 505, 400, -1);
  aDoor2[0]->Lock();
  aDoor2[0]->Close();
  aDoor2[0]->SetMaxDamage(-1);
  aDoor2[1] = CreateObject(SLDR, 505, 720, -1);
  aDoor2[1]->Lock();
  aDoor2[1]->Close();
  aDoor2[1]->SetMaxDamage(-1);
  aDoor2[2] = CreateObject(SLDR, 955, 610, -1);
  aDoor2[2]->Lock();
  aDoor2[2]->Close();
  aDoor2[2]->SetMaxDamage(-1);
  aDoor2[3] = CreateObject(SLDR, 1405, 590, -1);
  aDoor2[3]->Lock();
  aDoor2[3]->Close();
  aDoor2[3]->SetMaxDamage(-1);
  aDoor2[4] = CreateObject(SLDR, 1835, 380, -1);
  aDoor2[4]->Lock();
  aDoor2[4]->Close();
  aDoor2[4]->SetMaxDamage(-1);
  aDoor2[5] = CreateObject(SLDR, 1835, 640, -1);
  aDoor2[5]->Lock();
  aDoor2[5]->Close();
  aDoor2[5]->SetMaxDamage(-1);

  //Lampen
  aLamp1[0] = CreateObject(ALGH, 534, 600, -1);
  aLamp1[0]->SetR(90);
  aLamp1[0]->TurnOff();
  aLamp2[0] = CreateObject(ALGH, 534, 620, -1);
  aLamp2[0]->SetR(90);
  aLamp2[0]->TurnOff();
  aLamp3[0] = CreateObject(ALGH, 534, 640, -1);
  aLamp3[0]->ChangeLightColor(255,255,0,80);
  aLamp3[0]->SetR(90);
  aLamp3[0]->TurnOff();
  aLamp1[1] = CreateObject(ALGH, 990, 679, -1);
  aLamp1[1]->SetR(180);
  aLamp1[1]->TurnOff();
  aLamp2[1] = CreateObject(ALGH, 1010, 679, -1);
  aLamp2[1]->SetR(180);
  aLamp2[1]->TurnOff();
  aLamp3[1] = CreateObject(ALGH, 1030, 679, -1);
  aLamp3[1]->ChangeLightColor(255,255,0,80);
  aLamp3[1]->SetR(180);
  aLamp3[1]->TurnOff();
  aLamp1[2] = CreateObject(ALGH, 1450, 669, -1);
  aLamp1[2]->SetR(180);
  aLamp1[2]->TurnOff();
  aLamp2[2] = CreateObject(ALGH, 1470, 669, -1);
  aLamp2[2]->SetR(180);
  aLamp2[2]->TurnOff();
  aLamp3[2] = CreateObject(ALGH, 1490, 669, -1);
  aLamp3[2]->ChangeLightColor(255,255,0,80);
  aLamp3[2]->SetR(180);
  aLamp3[2]->TurnOff();
  aLamp1[3] = CreateObject(ALGH, 1940, 499, -1);
  aLamp1[3]->SetR(180);
  aLamp1[3]->TurnOff();
  aLamp2[3] = CreateObject(ALGH, 1960, 499, -1);
  aLamp2[3]->SetR(180);
  aLamp2[3]->TurnOff();
  aLamp3[3] = CreateObject(ALGH, 1980, 499, -1);
  aLamp3[3]->ChangeLightColor(255,255,0,80);
  aLamp3[3]->SetR(180);
  aLamp3[3]->TurnOff();

  //Grenzen
  CreateObject(BRDR, 330, 0, -1)->Set(0);
  CreateObject(BRDR, 2230, 0, -1)->Set(1);
  CreateObject(BRDR, 0, 1100, -1)->Set(3,0,1);

  //Sounds

  //Wind
  CreateObject(SE4K, 700, 130, -1)->Set("WindSound*.ogg",775,250);
  CreateObject(SE4K, 1180, 100, -1)->Set("WindSound*.ogg",775,250);
  CreateObject(SE4K, 1610, 110, -1)->Set("WindSound*.ogg",775,250);

  //Hallen
  CreateObject(SE4K, 550, 325, -1)->Set("Interior*.ogg",665,105);
  CreateObject(SE4K, 725, 575, -1)->Set("Interior*.ogg",665,105);
  CreateObject(SE4K, 1160, 525, -1)->Set("Interior*.ogg",665,105);
  CreateObject(SE4K, 1650, 605, -1)->Set("Interior*.ogg",665,105);
  CreateObject(SE4K, 1740, 335, -1)->Set("Interior*.ogg",665,105);
  CreateObject(SE4K, 2060, 415, -1)->Set("Interior*.ogg",665,105);

  //Möven
  CreateObject(SE4K, 1170, 180, -1)->Set("SeaSounds*.ogg",140,35);

  //Vögel
  CreateObject(SE4K, 360, 190, -1)->Set("BirdSong*",140,35);
  CreateObject(SE4K, 705, 255, -1)->Set("BirdSong*",140,35);
  CreateObject(SE4K, 1570, 305, -1)->Set("BirdSong*",140,35);
  CreateObject(SE4K, 2140, 130, -1)->Set("BirdSong*",140,35);

  //Erdrutsche
  CreateObject(SE4K, 600, 870, -1)->Set("FallingDirt*.ogg",950,250);
  CreateObject(SE4K, 1170, 870, -1)->Set("FallingDirt*.ogg",850,250);
  CreateObject(SE4K, 1720, 870, -1)->Set("FallingDirt*.ogg",850,250);
}

func CreateEquipment()
{
  Log("$CreatingEquipment$");

  //Versorgungskisten (Kugeln)
  var crate = CreateObject (AMCT, 735, 660, -1);
  crate->Set(ABOX);
  crate = CreateObject (AMCT, 1775, 660, -1);
  crate->Set(ABOX);

  //Versorgungskiste (Gewehrgranaten)
  crate = CreateObject (AMCT, 1300, 430, -1);
  crate->Set(GBOX);

  //Versorgungskisten (Dragnin)
  crate = CreateObject (AMCT, 870, 760, -1);
  crate->Set(DGNN);
  crate = CreateObject (AMCT, 1695, 830, -1);
  crate->Set(DGNN);

  //Raketen
  PlaceSpawnpoint(MBOX, 1320, 575);

  //Automat
  var store = CreateObject(WPVM,1160, 680,-1);
  store->AddWare(C4PA,-1);
  store->AddWare(FAPK,-1);
  store->AddWare(CDBT,-1);
  store->AddWare(BWTH,-1);
  store->AddWare(RSHL,-1);
  store->AddWare(ATWN,-1);
  store->AddWare(FGRN,-1);
  store->AddWare(FRAG,-1);
  store->AddWare(PGRN,-1);
  store->AddWare(SGRN,-1);
  store->AddWare(STUN,-1);
  store->AddWare(SRBL,-1);
}

func CreateDecoration()
{
  Log("$CreatingDecoration$");

  //Radio
  CreateObject(RDIO, 1665, 811, -1);

  //Leitkegel
  CreateObject(TCCN, 595, 720, -1)->Light();
  CreateObject(TCCN, 725, 750, -1)->Light();
  CreateObject(TCCN, 1125, 770, -1)->Light();
  CreateObject(TCCN, 1195, 750, -1)->Light();

  //Labortisch
  CreateObject(LTBL, 1780, 380, -1);

  //Monitore
  CreateObject(MNI2, 1770, 368, -1);
  CreateObject(MNI2, 1790, 368, -1)->Off();

  //Bildschirme
  var screen = CreateObject(SCA2, 630, 660, -1);
  screen->SetClrModulation(RGB(255,255,255));
  CreateObject(SCR3, 1465, 725, -1);

  //Orientierungslichter
  CreateObject(OLGH, 505, 170, -1)->Set(2, 15, 1, 1, 40);
  CreateObject(OLGH, 955, 180, -1)->Set(2, 15, 1, 1, 40);
  CreateObject(OLGH, 1405, 180, -1)->Set(2, 15, 1, 1, 40);
  CreateObject(OLGH, 1835, 180, -1)->Set(2, 15, 1, 1, 40);

  //Anlagen
  CreateObject(CCP2, 610, 540, -1);
  CreateObject(CGLO, 1455, 590, -1);
  CreateObject(CCP1, 1875, 640, -1);

  //Risse
  CreateObject(FAUD, 860, 345, -1);
  CreateObject(FAUD, 885, 340, -1);
  CreateObject(FAUD, 910, 320, -1);
  CreateObject(FAUD, 1305, 580, -1);
  CreateObject(FAUD, 1780, 640, -1);

  //Automat
  CreateObject(VGMN, 1740, 830, -1)->Set(3);

  //Radare
  CreateObject(RADR, 500, 170, -1)->SetClrModulation(RGB(125,125,125));
  CreateObject(RADR, 1835, 180, -1)->SetClrModulation(RGB(125,125,125));

  //Nebel
  CreateParticle("Fog", 390, 500,0,0,400+Random(100));
  CreateParticle("Fog", 395, 530,0,0,900+Random(100));
  CreateParticle("Fog", 405, 495,0,0,400+Random(100));
  CreateParticle("Fog", 455, 500,0,0,200+Random(100));

  CreateParticle("Fog", 655, 270,0,0,900+Random(100));
  CreateParticle("Fog", 695, 280,0,0,400+Random(100));
  CreateParticle("Fog", 695, 280,0,0,900+Random(100));
  CreateParticle("Fog", 710, 250,0,0,450+Random(100));
  CreateParticle("Fog", 750, 250,0,0,500+Random(100));

  CreateParticle("Fog", 1110, 200,0,0,850+Random(100));
  CreateParticle("Fog", 1150, 160,0,0,900+Random(100));
  CreateParticle("Fog", 1155, 180,0,0,400+Random(100));
  CreateParticle("Fog", 1160, 220,0,0,500+Random(100));
  CreateParticle("Fog", 1160, 255,0,0,250+Random(100));
  CreateParticle("Fog", 1185, 255,0,0,300+Random(100));
  CreateParticle("Fog", 1170, 270,0,0,450+Random(100));
  CreateParticle("Fog", 1210, 205,0,0,950+Random(100));

  CreateParticle("Fog", 1905, 595,0,0,900+Random(100));
  CreateParticle("Fog", 1910, 630,0,0,400+Random(100));
  CreateParticle("Fog", 1930, 600,0,0,500+Random(100));
  CreateParticle("Fog", 1940, 620,0,0,300+Random(100));
  CreateParticle("Fog", 1940, 640,0,0,950+Random(100));
  CreateParticle("Fog", 1985, 595,0,0,500+Random(100));

  //Zäune
  CreateObject(FENC, 560, 400, -1)->Set(2);

  CreateObject(FENC, 900, 610, -1);

  CreateObject(FENC, 1290, 750, -1);
  CreateObject(FENC, 1350, 750, -1)->Set(2);
  CreateObject(FENC, 1460, 750, -1);

  CreateObject(FENC, 1780, 380, -1);
  CreateObject(FENC, 1890, 380, -1)->Set(2);
  CreateObject(FENC, 1950, 380, -1);

  //Regale
  CreateObject(FRAM, 860, 750, -1);
  CreateObject(FRAM, 880, 750, -1);
  CreateObject(FRAM, 900, 750, -1);

  CreateObject(FRAM, 1210, 625, -1);
  CreateObject(FRAM, 1210, 640, -1);
  CreateObject(FRAM, 1210, 655, -1);
  CreateObject(FRAM, 1210, 670, -1);

  CreateObject(FRAM, 1250, 625, -1);
  CreateObject(FRAM, 1250, 640, -1);
  CreateObject(FRAM, 1250, 655, -1);
  CreateObject(FRAM, 1250, 670, -1);

  CreateObject(FRAM, 1300, 580, -1);
  CreateObject(FRAM, 1320, 580, -1);

  CreateObject(FRAM, 1670, 800, -1);
  CreateObject(FRAM, 1690, 800, -1);
  CreateObject(FRAM, 1710, 800, -1);

  CreateObject(FRAM, 1870, 380, -1);
  CreateObject(FRAM, 1890, 380, -1);

  //Feuerlöscher
  CreateObject(FIE2, 485, 515, -1);
  CreateObject(FIE2, 920, 755, -1);
  CreateObject(FIE2, 1435, 745, -1);
  CreateObject(FIE2, 1995, 475, -1);

  //Apparaturen
  CreateObject(GADG, 1495, 750, -1)->Set(1);
  CreateObject(GADG, 1800, 380, -1)->Set(1);

  //Schilder
  CreateObject(MSGN, 545, 720, -1);
  CreateObject(MSGN, 575, 720, -1);
  CreateObject(WLSN, 850, 520, -1);
  CreateObject(MSGN, 1605, 750, -1);
  CreateObject(MSGN, 1635, 750, -1);

  //Scheinwerfer
  CreateObject(FLGH, 540, 720, -1)->SetRotation(30);
  CreateObject(FLGH, 825, 550, -1)->SetRotation(-40);
  CreateObject(FLGH, 1715, 560, -1)->SetRotation(-15);
  CreateObject(FLGH, 2170, 480, -1)->SetRotation(-55);

  //Glühbirnen
  CreateObject(LAT1, 125, 425, -1);
  CreateObject(LAT1, 250, 190, -1);

  CreateObject(LAT1, 625, 585, -1);
  CreateObject(LAT1, 750, 375, -1);
  CreateObject(LAT1, 785, 700, -1);

  CreateObject(LAT1, 1085, 470, -1);
  CreateObject(LAT1, 1120, 475, -1);

  CreateObject(LAT1, 1275, 340, -1);

  CreateObject(LAT1, 1570, 595, -1);
  CreateObject(LAT1, 1585, 405, -1);
  CreateObject(LAT1, 1620, 410, -1);
  CreateObject(LAT1, 1725, 570, -1);

  CreateObject(LAT1, 1985, 410, -1);
  CreateObject(LAT1, 2050, 310, -1);

  CreateObject(LAT1, 2255, 185, -1);
  CreateObject(LAT1, 2385, 345, -1);
  CreateObject(LAT1, 2405, 525, -1);

  //Spinde
  CreateObject(LCKR, 440, 725, -1);
  CreateObject(LCKR, 1600, 810, -1);

  //Geländer
  CreateObject(RAI1, 530, 720, -1)->SetRail([1,1,1]);
  CreateObject(RAI1, 1130, 680, -1)->SetRail([1,1,1]);
  CreateObject(RAI1, 1520, 560, -1)->SetRail([1,1,1]);
  CreateObject(RAI1, 1930, 480, -1)->SetRail([1,1,1]);

  //Ventillatoren
  CreateObject(MVNT, 505, 220, -1)->SetCon(50);
  CreateObject(MVNT, 955, 230, -1)->SetCon(50);
  CreateObject(MVNT, 1405, 230, -1)->SetCon(50);
  CreateObject(MVNT, 1835, 230, -1)->SetCon(50);

  //Glasröhren
  CreateObject(GLST, 550, 400, -1);
  CreateObject(GLST, 575, 400, -1);
  CreateObject(GLST, 600, 400, -1);

  CreateObject(GLST, 995, 750, -1);
  CreateObject(GLST, 1030, 750, -1);

  CreateObject(GLST, 1445, 750, -1);
  CreateObject(GLST, 1480, 750, -1);

  CreateObject(GLST, 1750, 380, -1);

  //Computer
  CreateObject(LBPC, 1290, 430, -1);

  //Büsche
  CreateObject(BSH2, 75, 490, -1);
  CreateObject(BSH2, 390, 355, -1);
  CreateObject(BSH2, 735, 600, -1);
  CreateObject(BSH2, 995, 800, -1);
  CreateObject(BSH2, 1540, 620, -1);
  CreateObject(BSH2, 1775, 455, -1);
  CreateObject(BSH2, 1870, 510, -1);
  CreateObject(BSH2, 2220, 465, -1);
  CreateObject(BSH2, 2525, 730, -1);

  //Notausgangslichter
  CreateObject(ETLT, 620, 500, -1);
  CreateObject(ETLT, 1350, 705, -1);
  CreateObject(ETLT, 1665, 495, -1);

  //Gasflaschen
  CreateObject(GSBL, 885, 610, -1)->AutoRespawn();
  CreateObject(GSBL, 1300, 580, -1)->AutoRespawn();
  CreateObject(GSBL, 1800, 660, -1);

  //Steine
  CreateObject(STNE, 220, 520, -1)->Set(4);
  CreateObject(STNE, 400, 735, -1)->Set(2);
  CreateObject(STNE, 750, 310, -1)->Set(3);
  CreateObject(STNE, 810, 800, -1)->Set(0);
  CreateObject(STNE, 1520, 840, -1)->Set(5);
  CreateObject(STNE, 2100, 490, -1)->Set(2);
  CreateObject(STNE, 2370, 660, -1)->Set(1);
}

func CreateOptionalFeatures()
{
  //Hintergrund
  CreateObject(BD01,700,1000,-1)->SetClrModulation(RGB(125,125,125));
}

/* Regelwähler */

public func ChooserFinished()
{
  inherited();

  //Script starten
  ScriptGo(1);

  //Starttitel und Musikliste zusammenstellen
  SetPlayList("CMC_Base Groove.ogg;CMC_Firehawk.ogg;CMC_Friendly Unit.ogg;CMC_Getaway.ogg;CMC_Deep Universe.ogg;CMC_Eurocorps.ogg;CMC_Moving Squad.ogg;CMC_Offensive.ogg;CMC_Rock Go On.ogg;CMC_Showtime.ogg;CMC_Slow Motion.ogg;CMC_Striking Force.ogg;CMC_No Good.ogg;CMC_Obsession.ogg;CMC_Your Eyes.ogg");
  Music("CMC_Striking Force.ogg");

  //Teams abfragen
  var aTeams = [false,false,false,false,false];
  for(var i = 0; i < GetPlayerCount(); i++)
   aTeams[GetPlayerTeam(GetPlayerByIndex(i))] = true;

  //OP-Spielziel
  if(FindObject(GOCC))
  {
   //Flaggenposten
   aFlag[0] = CreateObject(OFPL,440,520,NO_OWNER);
   aFlag[0] -> AddSpawnPoint(470,710);
   aFlag[0] -> AddSpawnPoint(555,390);
   aFlag[0] -> AddSpawnPoint(595,390);
   if(aTeams[1] == true)
   {
    aFlag[0]->Set("$Flag1$",100,2);
    aFlag[0]->Capture(1,1);
   }
   else
   {
    aFlag[0]->Set("$Flag1$",0,2);
   }

   aFlag[1] = CreateObject(OFPL,910,350,NO_OWNER);
   aFlag[1] -> AddSpawnPoint(715,280);
   aFlag[1] -> AddSpawnPoint(830,540);
   aFlag[1] -> AddSpawnPoint(1055,360);
   aFlag[1]->Set("$Flag2$",0,2);

   aFlag[2] = CreateObject(OFPL,1340,750,NO_OWNER);
   aFlag[2] -> AddSpawnPoint(1100,760);
   aFlag[2] -> AddSpawnPoint(1295,650);
   aFlag[2] -> AddSpawnPoint(1500,740);
   aFlag[2]->Set("$Flag3$",0,2);

   aFlag[3] = CreateObject(OFPL,1825,240,NO_OWNER);
   aFlag[3] -> AddSpawnPoint(1705,250);
   aFlag[3] -> AddSpawnPoint(1750,370);
   aFlag[3] -> AddSpawnPoint(1790,370);
   aFlag[3]->Set("$Flag4$",0,2);

   aFlag[4] = CreateObject(OFPL,2150,480,NO_OWNER);
   aFlag[4] -> AddSpawnPoint(1880,550);
   aFlag[4] -> AddSpawnPoint(1975,600);
   aFlag[4] -> AddSpawnPoint(2090,570);
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

  //Base Assault-Spielziel
  if(FindObject(GBAS))
  {
   //Zielobjekte
   AddAssaultTarget(RADR, 450, 520, 350, 1, "$Target1$", 0, [[470, 650], [510, 230], [630, 270]]);
   AddAssaultTarget(CMSN, 645, 720, 350, 1, "$Target2$", 1, [[620, 540], [655, 400], [825, 550]]);
   AddAssaultTarget(CMSN, 1670, 690, 350, 2, "$Target2$", 2, [[1555, 340], [1720, 830], [1735, 380]]);
   AddAssaultTarget(RADR, 2140, 480, 350, 2, "$Target1$", 3, [[1770, 250], [2115, 580], [2145, 250]]);

   //Sprengsatz-Spawn
   SetupBombSpawnpoint([[1140, 400],[1160,680],[1210,750]]);
  }

  //HTF-Spielziel
  if(FindObject(GHTF))
  {
   //Flaggenposten
   var flag = CreateObject(OFPL, 1160,680, -1);
   flag->~Set("$Flag3$");

   //Teamgrenzen
   CreateObject(BRDR, 920, 0, -1)->Set(0,1,0,1,1);
   CreateObject(BRDR, 1440, 0, -1)->Set(1,1,0,1,2);

   //Waffenautomat entfernen
   RemoveAll(WPVM);

   //Zusätzliche Munition
   if(!FindObject(NOAM))
   {
    //Versorgungskiste (Kugeln)
    var crate = CreateObject (AMCT, 1310, 660, -1);
    crate->Set(ABOX);
   }
  }

  //CTF-Spielziel
  if(FindObject(GCTF))
  {
   //Flaggen
   if(aTeams[1] == true)
   {CreateFlag(1,635,540,GetTeamColor(1));}
   if(aTeams[2] == true)
   {CreateFlag(2,1740,560,GetTeamColor(2));}

   //Boden
   DrawMaterialQuad("Wall-Stripes", 590,540, 650,540, 650,551, 590,551);
   DrawMaterialQuad("Wall-Stripes", 1700,560, 1760,560, 1760,571, 1700,571);

   //Objekt entfernen
   RemoveObject(FindObject2(Find_ID(HBRL),Find_InRect(1710, 540, 20, 30)));
  }
}

/* Relaunch */

public func RelaunchPosition(& iX, & iY, int iTeam)
{
  //Startsicht
  if(!g_chooserFinished)
  {
   iX = 1160; iY = 670;
   return 1;
  }

  //HTF/LMS/DM-Spielziel
  if(FindObject(GHTF) || FindObject(GLMS) || FindObject(GTDM))
  {
   if(iTeam == 1)
   {
    return [[505, 220], [445, 510], [470, 640]];
   }
   if(iTeam == 2)
   {
    return [[1880, 230], [2140, 470], [1970, 600]];
   }
   return 1;
  }

  //CTF-Spielziel
  if(FindObject(GCTF))
  {
   if(iTeam == 1)
   {
    return [[470, 640], [505, 220], [570, 250]];
   }
   else
   {
    return [[1825, 230], [1975, 600], [2075, 470]];
   }
   return 1;
  }
}

/* Türensteuerung */

protected func Script220()
{
  aLamp1[0]->TurnOn();
  aLamp1[1]->TurnOn();
  aLamp1[2]->TurnOn();
  aLamp1[3]->TurnOn();
}

protected func Script230()
{
  aLamp1[0]->TurnOff();
  aLamp1[1]->TurnOff();
  aLamp1[2]->TurnOff();
  aLamp1[3]->TurnOff();

  aLamp2[0]->TurnOn();
  aLamp2[1]->TurnOn();
  aLamp2[2]->TurnOn();
  aLamp2[3]->TurnOn();

  Sound("Announce*.ogg",1);
}

protected func Script240()
{
  aLamp2[0]->TurnOff();
  aLamp2[1]->TurnOff();
  aLamp2[2]->TurnOff();
  aLamp2[3]->TurnOff();

  aLamp3[0]->TurnOn();
  aLamp3[1]->TurnOn();
  aLamp3[2]->TurnOn();
  aLamp3[3]->TurnOn();
}

protected func Script250()
{
  aLamp3[0]->TurnOff();
  aLamp3[1]->TurnOff();
  aLamp3[2]->TurnOff();
  aLamp3[3]->TurnOff();

  EventInfo4K(0,Format("$MsgDoor$"),STDR, 0, 0, 0, "Info_Event.ogg");

  aDoor1[0]->Close();
  aDoor1[1]->Close();
  aDoor1[2]->Close();
  aDoor1[3]->Close();
  aDoor1[4]->Close();
  aDoor1[5]->Close();

  aDoor2[0]->Open();
  aDoor2[0]->AddLightFlash(100,0,0,RGB(0,0,255));
  aDoor2[1]->Open();
  aDoor2[1]->AddLightFlash(100,0,0,RGB(0,0,255));
  aDoor2[2]->Open();
  aDoor2[2]->AddLightFlash(100,0,0,RGB(0,0,255));
  aDoor2[3]->Open();
  aDoor2[3]->AddLightFlash(100,0,0,RGB(0,0,255));
  aDoor2[4]->Open();
  aDoor2[4]->AddLightFlash(100,0,0,RGB(0,0,255));
  aDoor2[5]->Open();
  aDoor2[5]->AddLightFlash(100,0,0,RGB(0,0,255));

  return 1;
}

protected func Script470()
{
  aLamp1[0]->TurnOn();
  aLamp1[1]->TurnOn();
  aLamp1[2]->TurnOn();
  aLamp1[3]->TurnOn();
}

protected func Script480()
{
  aLamp1[0]->TurnOff();
  aLamp1[1]->TurnOff();
  aLamp1[2]->TurnOff();
  aLamp1[3]->TurnOff();

  aLamp2[0]->TurnOn();
  aLamp2[1]->TurnOn();
  aLamp2[2]->TurnOn();
  aLamp2[3]->TurnOn();

  Sound("Announce*.ogg",1);
}

protected func Script490()
{
  aLamp2[0]->TurnOff();
  aLamp2[1]->TurnOff();
  aLamp2[2]->TurnOff();
  aLamp2[3]->TurnOff();

  aLamp3[0]->TurnOn();
  aLamp3[1]->TurnOn();
  aLamp3[2]->TurnOn();
  aLamp3[3]->TurnOn();
}

protected func Script500()
{
  aLamp3[0]->TurnOff();
  aLamp3[1]->TurnOff();
  aLamp3[2]->TurnOff();
  aLamp3[3]->TurnOff();

  EventInfo4K(0,Format("$MsgDoor$"),STDR, 0, 0, 0, "Info_Event.ogg");

  aDoor1[0]->Open();
  aDoor1[0]->AddLightFlash(100,0,0,RGB(0,0,255));
  aDoor1[1]->Open();
  aDoor1[1]->AddLightFlash(100,0,0,RGB(0,0,255));
  aDoor1[2]->Open();
  aDoor1[2]->AddLightFlash(100,0,0,RGB(0,0,255));
  aDoor1[3]->Open();
  aDoor1[3]->AddLightFlash(100,0,0,RGB(0,0,255));
  aDoor1[4]->Open();
  aDoor1[4]->AddLightFlash(100,0,0,RGB(0,0,255));
  aDoor1[5]->Open();
  aDoor1[5]->AddLightFlash(100,0,0,RGB(0,0,255));

  aDoor2[0]->Close();
  aDoor2[1]->Close();
  aDoor2[2]->Close();
  aDoor2[3]->Close();
  aDoor2[4]->Close();
  aDoor2[5]->Close();

  goto(0);
}