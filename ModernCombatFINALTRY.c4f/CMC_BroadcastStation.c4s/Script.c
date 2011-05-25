/*-- Broadcast Station --*/

#strict
#include CSTD


/* Initialisierung */

func Initialize()
{
  //Wartemusik einstellen
  SetPlayList("CMC_WaitingforSpawn.ogg");
  Music("CMC_WaitingforSpawn.ogg",1);
  //Bildschirmf�rbung
  SetSkyAdjust(RGBa(255,255,255,128),RGB(64,196,255));
  //Einrichtung plazieren
  CreateInterior();
  //Ausr�stung plazieren
  CreateEquipment();
  //Dekoration plazieren
  CreateDecoration();
  return(1);
}

/* Plazierungslisten */

func CreateInterior()
{
  Log("$CreatingInterior$");

  //Aufz�ge
  CreateObject(LFTP, 845, 510, -1);
  CreateObject(LFTP, 1265, 710, -1);
  var liftp = CreateObject(LFTP, 1500, 194, -1);
  liftp->DisableVertical();

  //Leitern
  CreateObject(LADR, 190, 690, -1)->Set(24);
  CreateObject(LADR, 430, 488, -1)->Set(15);
  CreateObject(LADR, 570, 692, -1)->Set(22);

  CreateObject(LADR, 810, 420, -1)->Set(9);

  CreateObject(LADR, 1050, 798, -1)->Set(10);
  CreateObject(LADR, 1145, 190, -1)->Set(5);
  CreateObject(LADR, 1360, 572, -1)->Set(33);
  CreateObject(LADR, 1470, 770, -1)->Set(9);
  CreateObject(LADR, 1595, 291, -1)->Set(13);

  //Bodenluken
  CreateObject(HA4K, 190, 493, -1);
  CreateObject(HA4K, 430, 363, -1);
  CreateObject(HA4K, 570, 513, -1);
  CreateObject(HA4K, 810, 343, -1);
  CreateObject(HA4K, 1050, 713, -1);
  CreateObject(HA4K, 1360, 303, -1);

  //Gro�e Bodenluke
  CreateObject(H24K, 1595, 188, -1);

  //Hydraulikt�ren
  var door = CreateObject(SLDR, 140, 490, -1);
  door->Lock();
  door->SetMaxDamage(-1);

  door = CreateObject(SLDR,1670, 180, -1);
  door->Lock();
  door->SetMaxDamage(-1);

  //Schutzt�ren
  CreateObject(GDDR, 690, 510, -1);
  CreateObject(GDDR, 1388, 300, -1);
  CreateObject(GDDR, 1553, 180, -1);

  //Metallkisten
  CreateObject(MWCR, 880, 340, -1)->AutoRespawn();
  CreateObject(MWCR, 1215, 590, -1)->AutoRespawn();

  //Kisten
  CreateObject(WCR2, 30, 710, -1);
  CreateObject(WCR2, 215, 710, -1)->AutoRespawn();
  CreateObject(WCR2, 310, 682, -1);
  CreateObject(WCR2, 400, 710, -1)->AutoRespawn();
  CreateObject(WCR2, 475, 500, -1)->AutoRespawn();
  CreateObject(WCR2, 885, 322, -1);
  CreateObject(WCR2, 1275, 140, -1)->AutoRespawn();

  //Explosive Kiste
  CreateObject(XWCR, 710, 710, -1)->AutoRespawn();

  //Container
  CreateObject(CON1, 300, 710, -1);
  CreateObject(CON1, 359, 710, -1);
  CreateObject(CON1, 1090, 710, -1);

  //Giftf�sser
  CreateObject(TBRL, 50, 710, -1);
  CreateObject(TBRL, 340, 680, -1)->AutoRespawn();
  CreateObject(TBRL, 420, 710, -1)->AutoRespawn();
  CreateObject(TBRL, 450, 710, -1)->AutoRespawn();
  CreateObject(TBRL, 860, 340, -1)->AutoRespawn();

  //Benzinf�sser
  CreateObject(PBRL, 1060, 150, -1)->AutoRespawn();
  CreateObject(PBRL, 1315, 300, -1)->AutoRespawn();
  CreateObject(PBRL, 1525, 655, -1)->AutoRespawn();
  CreateObject(PBRL, 1660, 180, -1)->AutoRespawn();

  //Explosivf�sser
  CreateObject(XBRL, 1075, 680, -1)->AutoRespawn();
  CreateObject(XBRL, 1380, 580, -1)->AutoRespawn();

  //Phosphorf�sser
  CreateObject(HBRL, 440, 710, -1)->AutoRespawn();
  CreateObject(HBRL, 1130, 710, -1)->AutoRespawn();

  //Palette
  CreateObject(PLLT, 895, 710, -1);

  //Rampen
  DrawMaterialQuad("Backwall-Marble1",311,490,281,480,281,490,296,490,true);
  DrawMaterialQuad("Backwall-Marble1",561,500,531,490,531,500,546,500,true);

  DrawMaterialQuad("Wall-Concrete3",311,500,281,490,281,500,296,500,true);
  DrawMaterialQuad("Wall-Concrete3",561,510,531,500,531,510,546,510,true);
  DrawMaterialQuad("Wall-Concrete3",1300,590,1330,580,1330,590,1315,590,true);
  DrawMaterialQuad("Wall-Concrete3",1290,190,1320,180,1320,190,1305,190,true);

  //Sandsackbarrieren
  CreateObject(SBBA, 635, 710, -1)->Right();
  CreateObject(SBBA, 1125, 590, -1)->Left();

  //Explosivtank
  CreateObject(XTNK, 1170, 590, -1)->AutoRespawn();

  //Gasflaschen
  CreateObject(GSBL, 320, 500, -1)->AutoRespawn();
  CreateObject(GSBL, 1180, 710, -1)->AutoRespawn();
  CreateObject(GSBL, 1330, 300, -1)->AutoRespawn();
  CreateObject(GSBL, 1690, 180, -1);

  //Tore
  CreateObject(HNG2, 100, 710, -1);
  CreateObject(HNG2, 170, 710, -1);
  door = CreateObject(HNG2, 590, 710, -1);
  door->Opened();
  door = CreateObject(HNG2, 660, 710, -1);
  door->Opened();

  //Steine
  CreateObject(STNE, 1015, 820, -1);
  CreateObject(STNE, 1580, 345, -1);
  CreateObject(STNE, 1715, 430, -1);
  CreateObject(STNE, 1805, 800, -1);

  //Stahlbr�cken
  CreateObject(_HBR, 775, 522, -1);
  CreateObject(_HBR, 805, 722, -1);
  CreateObject(_HBR, 895, 722, -1);
  CreateObject(_HBR, 985, 722, -1);

  //Sounds

  //Wind
  CreateObject(SE4K, 890, 630, -1)->Set("WindSound*.ogg",775,250);
  CreateObject(SE4K, 950, 190, -1)->Set("WindSound*.ogg",775,250);

  //Hallen
  CreateObject(SE4K, 370, 630, -1)->Set("Interior*.ogg",665,105);
  CreateObject(SE4K, 1820, 165, -1)->Set("Interior*.ogg",665,105);

  //M�ven
  CreateObject(SE4K, 860, 805, -1)->Set("SeaSounds*.ogg",140,35);
  CreateObject(SE4K, 1145, 75, -1)->Set("SeaSounds*.ogg",140,35);

  //V�gel
  CreateObject(SE4K, 210, 300, -1)->Set("BirdSong*",140,35);
  CreateObject(SE4K, 610, 375, -1)->Set("BirdSong*",140,35);

  //Erdrutsche
  CreateObject(SE4K, 1475, 825, -1)->Set("FallingDirt*.ogg",950,250);
  CreateObject(SE4K, 1550, 400, -1)->Set("FallingDirt*.ogg",850,250);

  //Lautsprecher
  CreateObject(SE4K, 1010, 460, -1)->Set("Announce*.ogg",1700,200);
}

func CreateEquipment()
{
  Log("$CreatingEquipment$");

  //Versorgungskisten (Kugeln)
  var crate = CreateObject (AMCT, 370, 683, -1);
  crate->Set(ABOX);
  crate = CreateObject (AMCT, 1255, 190, -1);
  crate->Set(ABOX);

  //Gewehrgranaten
  PlaceSpawnpoint(GBOX, 1490, 550);

  //Raketen
  PlaceSpawnpoint(MBOX, 1645, 170);
}

func CreateDecoration()
{
  Log("$CreatingDecoration$");

  //Leitkegel
  CreateObject(TCCN, 1300, 710, -1);
  CreateObject(TCCN, 1315, 710, -1);
  CreateObject(TCCN, 1330, 710, -1);

  //Raum
  CreateObject(ROM2, 50, 490, -1);

  //Alarmlampen
  CreateObject(ALGH, 905, 340, -1);
  CreateObject(ALGH, 1035, 150, -1)->TurnOn();

  //Radare
  CreateObject(RADR, 500, 360, -1);
  CreateObject(RADR, 1210, 190, -1);

  //Z�une
  CreateObject(FENC, 1070, 710, -1);
  CreateObject(FENC, 1130, 710, -1);
  CreateObject(FENC, 1190, 710, -1);
  CreateObject(FENC, 1340, 710, -1);

  //Regale
  CreateObject(FRAM, 20, 490, -1);
  CreateObject(FRAM, 85, 490, -1);
  CreateObject(FRAM, 105, 490, -1);

  CreateObject(FRAM, 340, 500, -1);
  CreateObject(FRAM, 360, 500, -1);
  CreateObject(FRAM, 380, 500, -1);
  CreateObject(FRAM, 400, 500, -1);
  CreateObject(FRAM, 420, 500, -1);
  CreateObject(FRAM, 440, 500, -1);

  //Rohr
  CreateObject(PIPL, 670, 665, -1)->Right()->Right()->Right()->Right()->Right()->Down()->Down()->Down()->Down()->Down()->Down();

  //Feuerl�cher
  CreateObject(FIEX, 540, 700, -1);
  CreateObject(FIEX, 1380, 290, -1);

  //Automat
  CreateObject(VGMN, 510, 710, -1);

  //Labortisch
  CreateObject(LTBL, 245, 490, -1);

  //Monitore
  CreateObject(MNI2, 235, 478, -1);
  CreateObject(MNI2, 255, 478, -1)->Off();

  //Flaschen
  CreateObject(BOTL, 1290, 140, -1);
  CreateObject(BOTL, 1295, 140, -1);

  //Apparaturen
  CreateObject(GADG, 660, 510, -1);
  CreateObject(GADG, 1340, 580, -1)->Set(1);
  CreateObject(GADG, 1730, 180, -1)->Set(1);

  //Schilder
  CreateObject(WLSN, 480, 700, -1)->SetPhase(1);
  CreateObject(MSGN, 765, 710, -1);
  CreateObject(MSGN, 850, 710, -1);
  CreateObject(MSGN, 940, 710, -1);
  CreateObject(MSGN, 1025, 710, -1);

  //Notausgangslichter
  CreateObject(ETLT, 160, 475, -1);
  CreateObject(ETLT, 1635, 165, -1);

  //Flutlichter
  CreateObject(FLGH, 475, 360, -1)->SetRotation(-50);
  CreateObject(FLGH, 790, 340, -1)->SetRotation(40);
  CreateObject(FLGH, 1110, 150, -1)->SetRotation(-70);

  //Spinde
  CreateObject(LCKR, 1140, 710, -1);
  CreateObject(LCKR, 1160, 710, -1);
  CreateObject(LCKR, 1400, 300, -1);

  //Topfpflanze
  CreateObject(PLT2, 520, 500, -1);

  //Gel�nder
  CreateObject(RAI1, 1, 490, -1)->SetRail([1,1,1,1,1,1,1,1,1,1,1,1,1,1,1]);
  CreateObject(RAI1, 390, 360, -1)->SetRail([1,1,1,1,1,1,1,1]);
  CreateObject(RAI1, 700, 510, -1)->SetRail([1,1]);
  CreateObject(RAI1, 670, 710, -1)->SetRail([1,1,1,1,1]);
  CreateObject(RAI1, 1110, 590, -1)->SetRail([1,1,1,1,1,1,1]);
  CreateObject(RAI1, 1080, 190, -1)->SetRail([1,1,1,1,1,1,1,1,1,1,1]);
  CreateObject(RAI1, 1730, 180, -1)->SetRail([1,1,1,1,1,1,1,1,1,1]);

  //Bildschirme
  var screen = CreateObject(SCR3, 340, 480, -1);
  screen->SetClrModulation(RGB(255,255,255));
  CreateObject(SCR3, 390, 480, -1);
  screen = CreateObject(SCR3, 485, 480, -1);
  screen->SetClrModulation(RGB(255,255,255));
  screen = CreateObject(SCR3, 630, 485, -1);
  screen->SetClrModulation(RGB(255,255,255));

  screen = CreateObject(SCR3, 1770, 170, -1);
  screen->SetClrModulation(RGB(255,255,255));
  screen = CreateObject(SCR3, 1860, 170, -1);
  screen->SetClrModulation(RGB(255,255,255));

  //Ventillatoren
  CreateObject(MVNT, 135, 700, -1)->SetCon(80);
  CreateObject(MVNT, 625, 700, -1)->SetCon(80);

  //Anlagen
  CreateObject(CCP2, 75, 710, -1);
  CreateObject(CGLO, 630, 510, -1);
  CreateObject(CCP1, 1170, 190, -1);

  //B�sche
  CreateObject(BSH2, 115, 340, -1);
  CreateObject(BSH2, 315, 325, -1);
  CreateObject(BSH2, 530, 385, -1);
  CreateObject(BSH2, 710, 555, -1);
  CreateObject(BSH2, 1150, 245, -1);
  CreateObject(BSH2, 1190, 830, -1);
  CreateObject(BSH2, 1325, 520, -1);
  CreateObject(BSH2, 1490, 565, -1);
  CreateObject(BSH2, 1600, 365, -1);

  //Dekoschleusen
  CreateObject(GAT1, 390, 670, -1);
  CreateObject(GAT1, 845, 590, -1);
  CreateObject(GAT1, 1700, 175, -1);

  //Deckenlampen
  CreateObject(CLGH, 40, 625, -1);
  CreateObject(CLGH, 100, 465, -1);
  CreateObject(CLGH, 200, 465, -1);
  CreateObject(CLGH, 280, 625, -1);
  CreateObject(CLGH, 300, 465, -1);
  CreateObject(CLGH, 340, 625, -1);
  CreateObject(CLGH, 400, 625, -1);
  CreateObject(CLGH, 460, 625, -1);
  CreateObject(CLGH, 575, 465, -1);
  CreateObject(CLGH, 1230, 655, -1);
  CreateObject(CLGH, 1300, 655, -1);
  CreateObject(CLGH, 1360, 275, -1);
  CreateObject(CLGH, 1595, 155, -1);
  CreateObject(CLGH, 1815, 155, -1);
}

/* Regelw�hler */

public func ChooserFinished()
{
  inherited();

  //Starttitel und Musikliste zusammenstellen
  SetPlayList("CMC_Base Groove.ogg;CMC_Firehawk.ogg;CMC_Friendly Unit.ogg;CMC_Getaway.ogg;CMC_Eurocorps.ogg;CMC_Moving Squad.ogg;CMC_Offensive.ogg;CMC_Showtime.ogg;CMC_Slow Motion.ogg;CMC_Striking Force.ogg;CMC_No Good.ogg;CMC_Obsession.ogg;CMC_Your Eyes.ogg");
  Music("CMC_Showtime.ogg");

  //MR-Spielziel
  if (FindObject(GMNR))
  {
    //Gelds�cke
    AddMoneySpawn(490, 355, [10, 15]);
    AddMoneySpawn(850, 705, [10, 15]);
    AddMoneySpawn(1090, 145, [10, 15]);
    AddMoneySpawn(1320, 845, [10, 15]);
  }
}

/* Relaunch */

public func RelaunchPosition(& iX, & iY, int iTeam)
{
  //DM/LMS/MR-Spielziel
  if(FindObject(GTDM) || FindObject(GLMS) || FindObject(GMNR))
  {
   var rand = Random(5);
   if(!rand)
   { iX = 260; iY = 700; }
   if(!--rand)
   { iX = 595; iY = 410; }
   if(!--rand)
   { iX = 1525; iY = 650; }
   if(!--rand)
   { iX = 1635; iY = 240; }
   if(!--rand)
   { iX = 1155; iY = 185; }
   return(1);
  }

  //Startsicht
  iX = 875; iY = 630;
}