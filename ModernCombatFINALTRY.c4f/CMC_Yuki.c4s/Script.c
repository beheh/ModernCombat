/*-- Yuki --*/

#strict
#include CSTD

static aDoor1, aDoor2, aLamp1, aLamp2, aLamp3, aFlag;


/* Initalisierung */

func Initialize()
{
  //Starttitel und Musikliste zusammenstellen
  SetPlayList("CMC_Base Groove.ogg;CMC_City Lights.ogg;CMC_Friendly Unit.ogg;CMC_Getaway.ogg;CMC_Moving Squad.ogg;CMC_Offensive.ogg;CMC_Showtime.ogg;CMC_Slow Motion.ogg;CMC_Striking Force.ogg;CMC_No Good.ogg;CMC_Obsession.ogg");
  Music("CMC_Striking Force.ogg");
  //T�renteam1
  aDoor1 = [];
  //T�renteam2
  aDoor2 = [];
  //Lampen1
  aLamp1 = [];
  //Lampen2
  aLamp2 = [];
  //Lampen3
  aLamp3 = [];
  //Flaggen
  aFlag = [];
  //Szenario einrichten
  CreateFurniture();
  //Equipment plazieren
  CreateEquipment();
  //Script starten
  ScriptGo(1);
  return(1);
}

/* Plazierungslisten */

func CreateFurniture()
{
  var tmp;
  Log("$CreatingFurniture$");
  //Alle Objekte folglich von links nach rechts

  //Leitern
  CreateObject(LADR, 110, 710, -1)->Set(25);
  CreateObject(LADR, 155, 485, -1)->Set(32);
  CreateObject(LADR, 265, 690, -1)->Set(21);
  CreateObject(LADR, 390, 730, -1)->Set(25);

  CreateObject(LADR, 790, 750, -1)->Set(30);
  CreateObject(LADR, 930, 750, -1)->Set(40);

  CreateObject(LADR, 1320, 810, -1)->Set(40);
  CreateObject(LADR, 1390, 730, -1)->Set(45);
  CreateObject(LADR, 1470, 805, -1)->Set(10);

  CreateObject(LADR, 1605, 620, -1)->Set(17);
  CreateObject(LADR, 1630, 360, -1)->Set(14);
  CreateObject(LADR, 1725, 550, -1)->Set(21);

  //Bodenlucken
  CreateObject(HA4K, 260, 723, -1);
  CreateObject(HA4K, 1320, 753, -1);

  //Metallkiste
  CreateObject(MWCR, 840, 680, -1)->AutoRespawn();
  CreateObject(MWCR, 1365, 830, -1);

  //Kisten
  CreateObject(WCR2, 550, 570, -1)->AutoRespawn();
  CreateObject(WCR2, 1340, 810, -1)->AutoRespawn();
  CreateObject(WCR2, 1550, 240, -1)->AutoRespawn();

  //Explosive Kiste
  CreateObject(XWCR, 980, 580, -1)->AutoRespawn();
  CreateObject(XWCR, 1645, 480, -1)->AutoRespawn();

  //Container
  CreateObject(CON1, 1480, 690, -1);

  //Giftf�sser
  CreateObject(TBRL, 720, 750, -1)->AutoRespawn();
  CreateObject(TBRL, 960, 750, -1)->AutoRespawn();

  //Benzinf�sser
  CreateObject(PBRL, 500, 550, -1)->AutoRespawn();
  CreateObject(PBRL, 1280, 560, -1)->AutoRespawn();

  //Explosivf�sser
  CreateObject(XBRL, 535, 760, -1)->AutoRespawn();
  CreateObject(XBRL, 1140, 380, -1);
  CreateObject(XBRL, 1200, 380, -1);
  CreateObject(XBRL, 1685, 380, -1)->AutoRespawn();

  //Phosphorf�sser
  CreateObject(HBRL, 1025, 430, -1)->AutoRespawn();
  CreateObject(HBRL, 1420, 560, -1);

  //Labortisch
  CreateObject(LTBL, 1480, 380, -1);

  //Monitore
  CreateObject(MONI, 1475, 368, -1);
  CreateObject(MONI, 1490, 368, -1);

  //Bildschirme
  tmp = CreateObject(SCA2, 330, 660, -1);
  tmp->SetClrModulation(RGB(255,255,255));
  CreateObject(SCR3, 1165, 725, -1);

  //Orientierungslichter
  CreateObject(OLGH, 205, 170, -1)->Set(2, 15, 1, 1, 40);
  CreateObject(OLGH, 655, 180, -1)->Set(2, 15, 1, 1, 40);
  CreateObject(OLGH, 1105, 180, -1)->Set(2, 15, 1, 1, 40);
  CreateObject(OLGH, 1535, 180, -1)->Set(2, 15, 1, 1, 40);

  //Anlagen
  CreateObject(CCP2, 310, 540, -1);
  CreateObject(CGLO, 1155, 590, -1);
  CreateObject(CCP1, 1575, 640, -1);

  //Risse
  CreateObject(FAUD, 560, 345, -1)->Set(2);
  CreateObject(FAUD, 585, 340, -1)->Set(1);
  CreateObject(FAUD, 610, 320, -1)->Set(3);
  CreateObject(FAUD, 1005, 580, -1)->Set(2);
  CreateObject(FAUD, 1480, 640, -1)->Set(3);

  //Automat
  CreateObject(CLVM, 1440, 830, -1);

  //Satellitensch�ssel
  CreateObject(RADR, 200, 170, -1);
  CreateObject(RADR, 1535, 180, -1);

  //Nebel
  CreateParticle("Fog", 90, 500,0,0,400+Random(100));
  CreateParticle("Fog", 95, 530,0,0,900+Random(100));
  CreateParticle("Fog", 105, 495,0,0,400+Random(100));
  CreateParticle("Fog", 155, 500,0,0,200+Random(100));

  CreateParticle("Fog", 355, 270,0,0,900+Random(100));
  CreateParticle("Fog", 395, 280,0,0,400+Random(100));
  CreateParticle("Fog", 395, 280,0,0,900+Random(100));
  CreateParticle("Fog", 410, 250,0,0,450+Random(100));
  CreateParticle("Fog", 450, 250,0,0,500+Random(100));

  CreateParticle("Fog", 810, 200,0,0,850+Random(100));
  CreateParticle("Fog", 850, 160,0,0,900+Random(100));
  CreateParticle("Fog", 855, 180,0,0,400+Random(100));
  CreateParticle("Fog", 860, 220,0,0,500+Random(100));
  CreateParticle("Fog", 860, 255,0,0,250+Random(100));
  CreateParticle("Fog", 885, 255,0,0,300+Random(100));
  CreateParticle("Fog", 870, 270,0,0,450+Random(100));
  CreateParticle("Fog", 910, 205,0,0,950+Random(100));

  CreateParticle("Fog", 1605, 595,0,0,900+Random(100));
  CreateParticle("Fog", 1610, 630,0,0,400+Random(100));
  CreateParticle("Fog", 1630, 600,0,0,500+Random(100));
  CreateParticle("Fog", 1640, 620,0,0,300+Random(100));
  CreateParticle("Fog", 1640, 640,0,0,950+Random(100));
  CreateParticle("Fog", 1685, 595,0,0,500+Random(100));

  //Z�une
  CreateObject(FENC, 260, 400, -1);

  CreateObject(FENC, 600, 610, -1);

  CreateObject(FENC, 990, 750, -1);
  CreateObject(FENC, 1050, 750, -1);
  CreateObject(FENC, 1160, 750, -1);

  CreateObject(FENC, 1480, 380, -1);
  CreateObject(FENC, 1590, 380, -1);
  CreateObject(FENC, 1650, 380, -1);

  //Regale
  CreateObject(FRAM, 560, 750, -1);
  CreateObject(FRAM, 580, 750, -1);
  CreateObject(FRAM, 600, 750, -1);

  CreateObject(FRAM, 910, 625, -1);
  CreateObject(FRAM, 910, 640, -1);
  CreateObject(FRAM, 910, 655, -1);
  CreateObject(FRAM, 910, 670, -1);

  CreateObject(FRAM, 950, 625, -1);
  CreateObject(FRAM, 950, 640, -1);
  CreateObject(FRAM, 950, 655, -1);
  CreateObject(FRAM, 950, 670, -1);

  CreateObject(FRAM, 1000, 580, -1);
  CreateObject(FRAM, 1020, 580, -1);

  CreateObject(FRAM, 1370, 800, -1);
  CreateObject(FRAM, 1390, 800, -1);
  CreateObject(FRAM, 1410, 800, -1);

  CreateObject(FRAM, 1570, 380, -1);
  CreateObject(FRAM, 1590, 380, -1);

  //Feuerl�cher
  CreateObject(FIEX, 1135, 745, -1);
  CreateObject(FIEX, 1695, 475, -1);

  //Benzintank
  CreateObject(XTNK, 600, 610, -1);

  //Phosphortank
  CreateObject(PTNK, 1000, 750, -1)->AutoRespawn();

  //Apparaturen
  CreateObject(GADG, 1195, 750, -1)->Set(1);
  CreateObject(GADG, 1500, 380, -1)->Set(1);

  //Markierungsschilder
  CreateObject(MSGN, 250, 720, -1);
  CreateObject(MSGN, 280, 720, -1);

  CreateObject(MSGN, 1310, 750, -1);
  CreateObject(MSGN, 1340, 750, -1);

  //Gasflaschen
  CreateObject(GSBL, 585, 610, -1)->AutoRespawn();
  CreateObject(GSBL, 1000, 580, -1)->AutoRespawn();
  CreateObject(GSBL, 1500, 660, -1);

  //Flutlichter
  CreateObject(FLGH, 240, 720, -1)->SetRotation(30);
  CreateObject(FLGH, 525, 550, -1)->SetRotation(-40);
  CreateObject(FLGH, 1415, 560, -1)->SetRotation(-15);
  CreateObject(FLGH, 1870, 480, -1)->SetRotation(-55);

  //Gl�hbirnen
  CreateObject(LAT1, 75, 465, -1);
  CreateObject(LAT1, 85, 170, -1);

  CreateObject(LAT1, 770, 470, -1);
  CreateObject(LAT1, 835, 470, -1);
  CreateObject(LAT1, 865, 430, -1);

  CreateObject(LAT1, 1230, 395, -1);
  CreateObject(LAT1, 1270, 595, -1);
  CreateObject(LAT1, 1285, 405, -1);
  CreateObject(LAT1, 1340, 375, -1);

  CreateObject(LAT1, 1775, 160, -1);
  CreateObject(LAT1, 1825, 180, -1);

  //Spinde
  CreateObject(LCKR, 140, 725, -1);
  CreateObject(LCKR, 1300, 810, -1);

  //Gel�nder
  CreateObject(RAI1, 230, 720, -1)->SetRail([1,1,1]);
  CreateObject(RAI1, 830, 680, -1)->SetRail([1,1,1]);
  CreateObject(RAI1, 1220, 560, -1)->SetRail([1,1,1]);
  CreateObject(RAI1, 1630, 480, -1)->SetRail([1,1,1]);

  //Ventillatoren
  CreateObject(MVNT, 205, 220, -1)->SetCon(50);
  CreateObject(MVNT, 655, 230, -1)->SetCon(50);
  CreateObject(MVNT, 1105, 230, -1)->SetCon(50);
  CreateObject(MVNT, 1535, 230, -1)->SetCon(50);

  //Glasr�hre
  CreateObject(GLST, 250, 400, -1);
  CreateObject(GLST, 275, 400, -1);
  CreateObject(GLST, 300, 400, -1);

  CreateObject(GLST, 695, 750, -1);
  CreateObject(GLST, 730, 750, -1);

  CreateObject(GLST, 1145, 750, -1);
  CreateObject(GLST, 1180, 750, -1);

  CreateObject(GLST, 1450, 380, -1);

  //Computer
  CreateObject(LBPC, 990, 430, -1);

  //B�sche
  CreateObject(BSH2, 90, 355, -1);
  CreateObject(BSH2, 435, 600, -1);
  CreateObject(BSH2, 695, 800, -1);
  CreateObject(BSH2, 1240, 620, -1);
  CreateObject(BSH2, 1475, 455, -1);
  CreateObject(BSH2, 1570, 510, -1);
  CreateObject(BSH2, 1920, 465, -1);

  //Notausgangslichter
  CreateObject(ETLT, 320, 500, -1);
  CreateObject(ETLT, 1050, 705, -1);
  CreateObject(ETLT, 1365, 495, -1);

  //Warnschild
  CreateObject(EXSN, 550, 520, -1);

  //Steine
  CreateObject(STNE, 100, 735, -1);
  CreateObject(STNE, 450, 310, -1);
  CreateObject(STNE, 510, 800, -1);
  CreateObject(STNE, 1220, 840, -1);
  CreateObject(STNE, 1800, 490, -1);

  //Dekoschleuse
  CreateObject(GAT1, 1160, 690, -1);

  //Stahlbr�cke
  CreateObject(_HBR, 1245, 762, -1);

  //T�renteam 1

  aDoor1[0] = CreateObject(SLDR, 205, 400, -1);
  aDoor1[0]->Lock();
  aDoor1[0]->Close();
  aDoor1[0]->SetMaxDamage(-1);

  aDoor1[1] = CreateObject(SLDR, 205, 720, -1);
  aDoor1[1]->Lock();
  aDoor1[1]->Close();
  aDoor1[1]->SetMaxDamage(-1);

  aDoor1[2] = CreateObject(SLDR, 655, 610, -1);
  aDoor1[2]->Lock();
  aDoor1[2]->Close();
  aDoor1[2]->SetMaxDamage(-1);

  aDoor1[3] = CreateObject(SLDR, 1105, 380, -1);
  aDoor1[3]->Lock();
  aDoor1[3]->Close();
  aDoor1[3]->SetMaxDamage(-1);

  aDoor1[4] = CreateObject(SLDR, 1105, 750, -1);
  aDoor1[4]->Lock();
  aDoor1[4]->Close();
  aDoor1[4]->SetMaxDamage(-1);

  aDoor1[5] = CreateObject(SLDR, 1535, 640, -1);
  aDoor1[5]->Lock();
  aDoor1[5]->Close();
  aDoor1[5]->SetMaxDamage(-1);

  //T�renteam 2

  aDoor2[0] = CreateObject(SLDR, 205, 520, -1);
  aDoor2[0]->Lock();
  aDoor2[0]->Open();
  aDoor2[0]->SetMaxDamage(-1);

  aDoor2[1] = CreateObject(SLDR, 655, 350, -1);
  aDoor2[1]->Lock();
  aDoor2[1]->Open();
  aDoor2[1]->SetMaxDamage(-1);

  aDoor2[2] = CreateObject(SLDR, 655, 750, -1);
  aDoor2[2]->Lock();
  aDoor2[2]->Open();
  aDoor2[2]->SetMaxDamage(-1);

  aDoor2[3] = CreateObject(SLDR, 1105, 590, -1);
  aDoor2[3]->Lock();
  aDoor2[3]->Open();
  aDoor2[3]->SetMaxDamage(-1);

  aDoor2[4] = CreateObject(SLDR, 1535, 380, -1);
  aDoor2[4]->Lock();
  aDoor2[4]->Open();
  aDoor2[4]->SetMaxDamage(-1);

  //Ampeln

  aLamp1[0] = CreateObject(ALGH, 234, 600, -1);
  aLamp1[0]->SetR(90);
  aLamp1[0]->TurnOff();

  aLamp2[0] = CreateObject(ALGH, 234, 620, -1);
  aLamp2[0]->SetR(90);
  aLamp2[0]->TurnOff();

  aLamp3[0] = CreateObject(ALGH, 234, 640, -1);
  aLamp3[0]->ChangeLightColor(255,255,0,80);
  aLamp3[0]->SetR(90);
  aLamp3[0]->TurnOff();


  aLamp1[1] = CreateObject(ALGH, 980, 619, -1);
  aLamp1[1]->SetR(180);
  aLamp1[1]->TurnOff();

  aLamp2[1] = CreateObject(ALGH, 1000, 619, -1);
  aLamp2[1]->SetR(180);
  aLamp2[1]->TurnOff();

  aLamp3[1] = CreateObject(ALGH, 1020, 619, -1);
  aLamp3[1]->ChangeLightColor(255,255,0,80);
  aLamp3[1]->SetR(180);
  aLamp3[1]->TurnOff();


  aLamp1[2] = CreateObject(ALGH, 1150, 669, -1);
  aLamp1[2]->SetR(180);
  aLamp1[2]->TurnOff();

  aLamp2[2] = CreateObject(ALGH, 1170, 669, -1);
  aLamp2[2]->SetR(180);
  aLamp2[2]->TurnOff();

  aLamp3[2] = CreateObject(ALGH, 1190, 669, -1);
  aLamp3[2]->ChangeLightColor(255,255,0,80);
  aLamp3[2]->SetR(180);
  aLamp3[2]->TurnOff();


  aLamp1[3] = CreateObject(ALGH, 1640, 499, -1);
  aLamp1[3]->SetR(180);
  aLamp1[3]->TurnOff();

  aLamp2[3] = CreateObject(ALGH, 1660, 499, -1);
  aLamp2[3]->SetR(180);
  aLamp2[3]->TurnOff();

  aLamp3[3] = CreateObject(ALGH, 1680, 499, -1);
  aLamp3[3]->ChangeLightColor(255,255,0,80);
  aLamp3[3]->SetR(180);
  aLamp3[3]->TurnOff();

  //Sounds

  //Wind
  CreateObject(SE4K, 400, 130, -1)->Set("WindSound*.ogg",775,250);
  CreateObject(SE4K, 880, 100, -1)->Set("WindSound*.ogg",775,250);
  CreateObject(SE4K, 1310, 110, -1)->Set("WindSound*.ogg",775,250);

  //Hallen
  CreateObject(SE4K, 250, 325, -1)->Set("Interior*.ogg",665,105);
  CreateObject(SE4K, 425, 575, -1)->Set("Interior*.ogg",665,105);

  CreateObject(SE4K, 860, 525, -1)->Set("Interior*.ogg",665,105);

  CreateObject(SE4K, 1350, 605, -1)->Set("Interior*.ogg",665,105);
  CreateObject(SE4K, 1440, 335, -1)->Set("Interior*.ogg",665,105);

  CreateObject(SE4K, 1760, 415, -1)->Set("Interior*.ogg",665,105);

  //M�ven
  CreateObject(SE4K, 870, 180, -1)->Set("SeaSounds*.ogg",140,35);

  //V�gel
  CreateObject(SE4K, 60, 190, -1)->Set("BirdSong*",140,35);
  CreateObject(SE4K, 405, 255, -1)->Set("BirdSong*",140,35);
  CreateObject(SE4K, 1270, 305, -1)->Set("BirdSong*",140,35);
  CreateObject(SE4K, 1840, 130, -1)->Set("BirdSong*",140,35);

  //Erdrutsche
  CreateObject(SE4K, 300, 870, -1)->Set("FallingDirt*.ogg",950,250);
  CreateObject(SE4K, 870, 870, -1)->Set("FallingDirt*.ogg",850,250);
  CreateObject(SE4K, 1420, 870, -1)->Set("FallingDirt*.ogg",850,250);

  //Lautsprecher
  CreateObject(SE4K, 850, 510, -1)->Set("Announce*.ogg",1700,200);
  CreateObject(SE4K, 1430, 520, -1)->Set("Announce*.ogg",1700,200);
}

func CreateEquipment()
{
  Log("$CreatingEquipment$");
  //Alle Objekte folglich von links oben nach rechts unten

  //Munitionskisten (Kugeln)
  var tmp = CreateObject (AMCT, 435, 660, -1);
  tmp->Set(ABOX);
  tmp->SetGraphics("Normal");
  var tmp = CreateObject (AMCT, 1475, 660, -1);
  tmp->Set(ABOX);
  tmp->SetGraphics("Normal");

  //Munitionskiste (Granaten)
  var tmp = CreateObject (AMCT, 1000, 430, -1);
  tmp->Set(GBOX);
  tmp->SetGraphics("Normal");

  //Raketen
  PlaceSpawnpoint(MIAP, 1020, 575);

  //Feldgranaten
  PlaceSpawnpoint(FGRN, 320, 530);
  PlaceSpawnpoint(FGRN, 835, 390);
  PlaceSpawnpoint(FGRN, 1295, 800);
  PlaceSpawnpoint(FGRN, 1580, 370);

  //Splittergranaten
  PlaceSpawnpoint(FRAG, 65, 685);
  PlaceSpawnpoint(FRAG, 1910, 460);

  //Blendgranaten
  PlaceSpawnpoint(STUN, 560, 750);
  PlaceSpawnpoint(STUN, 1220, 550);

  //Sturmgewehr
  PlaceSpawnpoint(ASTR, 1190, 815);

  //Maschinengewehre
  PlaceSpawnpoint(MNGN, 335, 255);
  PlaceSpawnpoint(MNGN, 1520, 230);

  //Raketenwerfer
  PlaceSpawnpoint(RTLR, 1000, 575);

  //Shotgun
  PlaceSpawnpoint(PPGN, 860, 750);

  //Slingshot
  PlaceSpawnpoint(SGST, 700, 510);

  //Maschinenpistolen
  PlaceSpawnpoint(SMGN, 320, 390);
  PlaceSpawnpoint(SMGN, 1815, 570);

  //Abwehrschild
  PlaceSpawnpoint(RSHL, 75, 230);

  //Sprengfalle
  PlaceSpawnpoint(BBTP, 1415, 825);

  //EHP
  PlaceSpawnpoint(FAPK, 260, 775);

  //Automat
  var store = CreateObject(WPVM,860, 680,-1);
  store->AddWare(C4PA,-1);
  store->AddWare(FAPK,-1);
  store->AddWare(RSHL,-1);
  store->AddWare(CDBT,-1);
  store->AddWare(FGRN,-1);
  store->AddWare(FRAG,-1);
  store->AddWare(PGRN,-1);
  store->AddWare(SGRN,-1);
  store->AddWare(STUN,-1);
}

/* Regelw�hler */

public func ChooserFinished()
{
  inherited();

  //Teams abfragen
  var aTeams = [false,false,false,false,false];
  for(var i = 0; i < GetPlayerCount(); i++)
    aTeams[GetPlayerTeam(GetPlayerByIndex(i))] = true;

  //CTF-Spielziel
  if(FindObject(GCTF))
  {
   if(aTeams[1] == true)
   {CreateFlag(1,135,520,GetTeamColor(1));}
   if(aTeams[2] == true)
   {CreateFlag(2,1830,480,GetTeamColor(2));}
  }

  //OP-Spielziel
  if(FindObject(GOCC))
  {
   aFlag[0] = CreateObject(OFPL,150,520,NO_OWNER);
   aFlag[0] -> AddSpawnPoint(170,710);
   aFlag[0] -> AddSpawnPoint(255,390);
   aFlag[0] -> AddSpawnPoint(295,390);
   if(aTeams[1] == true)
   {
    aFlag[0]->Set("$Flag1$",100,2);
    aFlag[0]->Capture(1,1);
   }
   else
   {
    aFlag[0]->Set("$Flag1$",0,2);
   }

   aFlag[1] = CreateObject(OFPL,610,350,NO_OWNER);
   aFlag[1] -> AddSpawnPoint(415,280);
   aFlag[1] -> AddSpawnPoint(530,540);
   aFlag[1] -> AddSpawnPoint(755,360);
   aFlag[1]->Set("$Flag2$",0,2);

   aFlag[2] = CreateObject(OFPL,1040,750,NO_OWNER);
   aFlag[2] -> AddSpawnPoint(800,760);
   aFlag[2] -> AddSpawnPoint(995,650);
   aFlag[2] -> AddSpawnPoint(1200,740);
   aFlag[2]->Set("$Flag3$",0,2);

   aFlag[3] = CreateObject(OFPL,1525,240,NO_OWNER);
   aFlag[3] -> AddSpawnPoint(1405,250);
   aFlag[3] -> AddSpawnPoint(1450,370);
   aFlag[3] -> AddSpawnPoint(1490,370);
   aFlag[3]->Set("$Flag4$",0,2);

   aFlag[4] = CreateObject(OFPL,1850,480,NO_OWNER);
   aFlag[4] -> AddSpawnPoint(1580,500);
   aFlag[4] -> AddSpawnPoint(1675,600);
   aFlag[4] -> AddSpawnPoint(1790,570);
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
    var rand = Random(3);
    if(!rand)
     { iX = 205; iY = 220; }
    if(!--rand)
     { iX = 145; iY = 510; }
    if(!--rand)
     { iX = 170; iY = 640; }
   }
   if(iTeam == 2)
   {
    var rand = Random(2);
    if(!rand)
     { iX = 1580; iY = 230; }
    if(!--rand)
     { iX = 1840; iY = 470; }
    if(!--rand)
     { iX = 1670; iY = 600; }
   }
   return(1);
  }

  //CTF-Spielziel
  if(FindObject(GCTF))
  {
   var rand = Random(2);
   if(iTeam == 1)
   {
    if(!rand)
      { iX = 205; iY = 220; }
    if(!--rand)
      { iX = 270; iY = 250; }
    return(1);
   }
   else
   {
    if(!rand)
      { iX = 1580; iY = 500; }
    if(!--rand)
      { iX = 1675; iY = 600; }
    return(1);
   }
  }

  //Startsicht
  iX = 860; iY = 670;
}

/* T�rensteuerung */

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

  EventInfo4K(0,Format("$MsgDoor$"),STDR);

  aDoor1[0]->Open();
  aDoor1[1]->Open();
  aDoor1[2]->Open();
  aDoor1[3]->Open();
  aDoor1[4]->Open();
  aDoor1[5]->Open();

  aDoor2[0]->Close();
  aDoor2[1]->Close();
  aDoor2[2]->Close();
  aDoor2[3]->Close();
  aDoor2[4]->Close();

  return(1);
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

  EventInfo4K(0,Format("$MsgDoor$"),STDR);

  aDoor1[0]->Close();
  aDoor1[1]->Close();
  aDoor1[2]->Close();
  aDoor1[3]->Close();
  aDoor1[4]->Close();
  aDoor1[5]->Close();

  aDoor2[0]->Open();
  aDoor2[1]->Open();
  aDoor2[2]->Open();
  aDoor2[3]->Open();
  aDoor2[4]->Open();

  goto(0);
}