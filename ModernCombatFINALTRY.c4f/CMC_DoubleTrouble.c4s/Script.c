/*-- Double Trouble --*/

#strict
#include CSTD

static aFlag;


/* Initialisierung */

func Initialize()
{
  //Wartemusik einstellen
  SetPlayList("CMC_WaitingforSpawn.ogg");
  Music("CMC_WaitingforSpawn.ogg",1);
  //Flaggen
  aFlag = [];
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

  //Rampen
  DrawMaterialQuad("Wall-Stripes",181,190,151,180,151,190,166,190,true);
  DrawMaterialQuad("Wall-Stripes",151,180,121,170,121,180,136,180,true);

  DrawMaterialQuad("Wall-Concrete3",381,700,351,690,351,700,366,700,true);
  DrawMaterialQuad("Wall-Concrete3",351,690,321,680,321,690,336,690,true);

  DrawMaterialQuad("Wall-Concrete1",740,700,770,690,770,700,755,700,true);
  DrawMaterialQuad("Wall-Concrete1",770,690,800,680,800,690,785,690,true);

  DrawMaterialQuad("Wall-Concrete1",871,690,841,680,841,690,856,690,true);
  DrawMaterialQuad("Wall-Concrete1",901,700,871,690,871,700,886,700,true);

  DrawMaterialQuad("Wall-Concrete3",1290,690,1320,680,1320,690,1305,690,true);
  DrawMaterialQuad("Wall-Concrete3",1260,700,1290,690,1290,700,1275,700,true);

  DrawMaterialQuad("Wall-Stripes",1490,180,1520,170,1520,180,1505,180,true);
  DrawMaterialQuad("Wall-Stripes",1460,190,1490,180,1490,190,1475,190,true);

  //Leitern
  CreateObject(LADR, 250, 600, -1)->Set(9);
  CreateObject(LADR, 285, 680, -1)->Set(9);
  CreateObject(LADR, 535, 700, -1)->Set(12);
  CreateObject(LADR, 570, 600, -1)->Set(9);

  CreateObject(LADR, 700, 252, -1)->Set(9);
  CreateObject(LADR, 940, 252, -1)->Set(9);

  CreateObject(LADR, 1070, 600, -1)->Set(9);
  CreateObject(LADR, 1105, 700, -1)->Set(12);
  CreateObject(LADR, 1355, 680, -1)->Set(9);
  CreateObject(LADR, 1390, 600, -1)->Set(9);

  //Bodenluken
  CreateObject(HA4K, 250, 523, -1);
  CreateObject(HA4K, 570, 523, -1);

  CreateObject(HA4K, 1070, 523, -1);
  CreateObject(HA4K, 1390, 523, -1);

  //Gro�e Bodenluken
  CreateObject(H24K, 285, 608, -1);
  CreateObject(H24K, 535, 608, -1);

  CreateObject(H24K, 1105, 608, -1);
  CreateObject(H24K, 1355, 608, -1);

  //Stahlbr�cken
  CreateObject(_HBR, 687, 612, -1);
  CreateObject(_HBR, 954, 612, -1);

  //Gasflaschen
  CreateObject(GSBL, 365, 520, -1);
  CreateObject(GSBL, 540, 290, -1)->AutoRespawn();
  CreateObject(GSBL, 1100, 290, -1)->AutoRespawn();
  CreateObject(GSBL, 1525, 663, -1);

  //Kisten
  CreateObject(WCR2, 70, 680, -1);
  CreateObject(WCR2, 85, 662, -1);
  CreateObject(WCR2, 85, 680, -1);
  CreateObject(WCR2, 140, 680, -1);
  CreateObject(WCR2, 240, 190, -1)->AutoRespawn();
  CreateObject(WCR2, 1400, 190, -1)->AutoRespawn();
  CreateObject(WCR2, 1490, 680, -1);
  CreateObject(WCR2, 1520, 680, -1);
  CreateObject(WCR2, 1550, 680, -1);
  CreateObject(WCR2, 1580, 520, -1);
  CreateObject(WCR2, 1580, 680, -1);

  //Metallkisten
  CreateObject(MWCR, 400, 600, -1);
  CreateObject(MWCR, 1240, 600, -1);

  //Verbandskisten
  CreateObject(BECR, 505, 520, -1)->AutoRespawn();
  CreateObject(BECR, 1135, 520, -1)->AutoRespawn();

  //Explosivkisten
  CreateObject(XWCR, 520, 270, -1)->AutoRespawn();

  CreateObject(XWCR, 640, 520, -1)->AutoRespawn();
  CreateObject(XWCR, 1000, 520, -1)->AutoRespawn();

  CreateObject(XWCR, 1120, 270, -1)->AutoRespawn();

  //Benzinf�sser
  CreateObject(PBRL, 220, 680, -1)->AutoRespawn();
  CreateObject(PBRL, 555, 600, -1)->AutoRespawn();
  CreateObject(PBRL, 1085, 600, -1)->AutoRespawn();
  CreateObject(PBRL, 1420, 680, -1)->AutoRespawn();

  //Giftf�sser
  CreateObject(TBRL, 500, 270, -1)->AutoRespawn();
  CreateObject(TBRL, 1140, 270, -1)->AutoRespawn();

  //Explosivf�sser
  CreateObject(XBRL, 260, 190, -1)->AutoRespawn();
  CreateObject(XBRL, 350, 600, -1)->AutoRespawn();
  CreateObject(XBRL, 1290, 600, -1)->AutoRespawn();
  CreateObject(XBRL, 1380, 190, -1)->AutoRespawn();

  //Container
  CreateObject(CON1, 25, 680, -1)->SetPerspective(2);
  CreateObject(CON1, 615, 230, -1)->SetPerspective(2);
  CreateObject(CON1, 1025, 230, -1)->SetPerspective(2);
  CreateObject(CON1, 1260, 270, -1);
  CreateObject(CON1, 1275, 525, -1)->SetPerspective(2);

  //Schutzt�ren
  CreateObject(GDDR, 450, 520, -1);
  CreateObject(GDDR, 595, 600, -1);
  CreateObject(GDDR, 595, 700, -1);

  CreateObject(GDDR, 1045, 600, -1);
  CreateObject(GDDR, 1045, 700, -1);
  CreateObject(GDDR, 1190, 520, -1);

  //Sandsackbarrieren
  CreateObject(SBBA, 320, 190, -1)->Right();
  CreateObject(SBBA, 455, 230, -1)->Right();
  CreateObject(SBBA, 640, 700, -1)->Right();
  CreateObject(SBBA, 1001, 700, -1);
  CreateObject(SBBA, 1181, 230, -1);
  CreateObject(SBBA, 1321, 190, -1);

  //Explosivtanks
  CreateObject(XTNK, 330, 600, -1)->AutoRespawn();
  CreateObject(XTNK, 1310, 600, -1)->AutoRespawn();

  //Glasscheiben
  CreateObject(_WIN, 698, 462, -1);
  CreateObject(_WIN, 698, 491, -1);
  CreateObject(_WIN, 698, 520, -1);

  CreateObject(_WIN, 942, 462, -1);
  CreateObject(_WIN, 942, 491, -1);
  CreateObject(_WIN, 942, 520, -1);

  //Steine
  CreateObject(STNE, 750, 370, -1);
  CreateObject(STNE, 1285, 330, -1);

  //Tore und Konsolen
  var autod = CreateObject (HNG3, 642, 600, -1);
  autod->Open();
  CreateObject(CONS, 610, 595, -1)->Set(autod);
  
  autod = CreateObject (HNG3, 995, 600, -1);
  autod->Open();
  CreateObject(CONS, 1030, 595, -1)->Set(autod);

  //Hydraulikt�ren
  var door = CreateObject(SLDR, 205, 520, -1);
  door->Lock();
  door->SetMaxDamage(-1);

  door = CreateObject(SLDR, 190, 680, -1);
  door->Lock();
  door->SetMaxDamage(-1);

  door = CreateObject(SLDR, 1435, 520, -1);
  door->Lock();
  door->SetMaxDamage(-1);

  door = CreateObject(SLDR, 1450, 680, -1);
  door->Lock();
  door->SetMaxDamage(-1);

  //R�ume
  CreateObject(ROM3, 60, 520, -1);
  CreateObject(ROM3, 1510, 520, -1);

  //Verbundene R�ume
  var doorw = CreateObject(ROOM, 70, 170, -1);
  CreateObject(ROOM, 270, 680, -1)->Connect(doorw);

  var doorw = CreateObject(ROOM, 200, 190, -1);
  CreateObject(ROOM, 410, 700, -1)->Connect(doorw);

  var doorw = CreateObject(ROOM, 1440, 190, -1);
  CreateObject(ROOM, 1230, 700, -1)->Connect(doorw);

  var doorw = CreateObject(ROOM, 1575, 170, -1);
  CreateObject(ROOM, 1380, 680, -1)->Connect(doorw);

  //Sounds

  //Wind
  CreateObject(SE4K, 250, 100, -1)->Set("WindSound*.ogg",4500,200);
  CreateObject(SE4K, 820, 150, -1)->Set("WindSound*.ogg",4500,200);
  CreateObject(SE4K, 1390, 100, -1)->Set("WindSound*.ogg",4500,200);

  //Hallen
  CreateObject(SE4K, 100, 570, -1)->Set("Interior*.ogg",670,105);
  CreateObject(SE4K, 820, 570, -1)->Set("Interior*.ogg",900,150);
  CreateObject(SE4K, 1520, 570, -1)->Set("Interior*.ogg",670,105);

  //Lautsprecher
  CreateObject(SE4K, 820, 590, -1)->Set("Announce*.ogg",4000,200);

  //Erdrutsche
  CreateObject(SE4K, 370, 340, -1)->Set("FallingDirt*.ogg",2000,500);
  CreateObject(SE4K, 1265, 580, -1)->Set("FallingDirt*.ogg",2000,500);
}

func CreateEquipment()
{
  Log("$CreatingEquipment$");

  //Versorgungskisten (Kugeln)
  var crate = CreateObject(AMCT, 40, 170, -1);
  crate->Set(ABOX);
  crate = CreateObject(AMCT, 1605, 170, -1);
  crate->Set(ABOX);

  //Gewehrgranaten
  PlaceSpawnpoint(GBOX, 490, 590);

  //Raketen
  PlaceSpawnpoint(MBOX, 1150, 590);
}

func CreateDecoration()
{
  Log("$CreatingDecoration$");

  //Orientierungslichter
  CreateObject(OLGH, 820, 280, -1)->Set(1, 20, 1, 1, 60);
  CreateObject(OLGH, 820, 680, -1)->Set(1, 20, 1, 1, 60);

  //Automaten
  CreateObject(VGMN, 150, 520, -1)->Set(3);
  CreateObject(VGMN, 1555, 520, -1)->Set(2);

  //Apparaturen
  CreateObject(GADG, 90, 170, -1)->Set(1);
  CreateObject(GADG, 590, 230, -1)->Set(2);
  CreateObject(GADG, 720, 700, -1)->Set(1);
  CreateObject(GADG, 920, 700, -1)->Set(1);
  CreateObject(GADG, 1050, 230, -1)->Set(2);
  CreateObject(GADG, 1550, 170, -1)->Set(1);

  //B�sche
  CreateObject(BSH2, 25, 120, -1);
  CreateObject(BSH2, 615, 260, -1);
  CreateObject(BSH2, 870, 220, -1);
  CreateObject(BSH2, 1610, 120, -1);

  //Schilder
  CreateObject(ESGN, 160, 675, -1);
  CreateObject(ESGN, 660, 590, -1);
  CreateObject(MSGN, 710, 700, -1);
  CreateObject(MSGN, 940, 700, -1);
  CreateObject(ESGN, 980, 590, -1);
  CreateObject(ESGN, 1605, 675, -1);

  //Ventillatoren
  CreateObject(VENT, 750, 600, -1)->SetCon(60);
  CreateObject(VENT, 570, 280, -1)->SetCon(20);
  CreateObject(VENT, 1070, 280, -1)->SetCon(20);
  CreateObject(VENT, 890, 600, -1)->SetCon(60);

  //Radare
  CreateObject(RADR, 5, 90, -1);
  CreateObject(RADR, 1635, 90, -1);

  //Dekoschleusen
  CreateObject(GAT1, 40, 480, -1);
  CreateObject(GAT1, 820, 240, -1);
  CreateObject(GAT1, 1600, 480, -1);

  //Spinde
  CreateObject(LCKR, 380, 520, -1);
  CreateObject(LCKR, 400, 520, -1);
  CreateObject(LCKR, 420, 520, -1);

  CreateObject(LCKR, 1215, 520, -1);
  CreateObject(LCKR, 1235, 520, -1);

  //Wandlampen
  CreateObject(BLGH, 225, 165, -1);
  CreateObject(BLGH, 505, 485, -1);
  CreateObject(BLGH, 820, 550, -1);
  CreateObject(BLGH, 1135, 485, -1);
  CreateObject(BLGH, 1415, 165, -1);

  //Z�une
  CreateObject(FENC, 50, 170, -1);
  CreateObject(FENC, 1590, 170, -1);

  //Gel�nder
  CreateObject(RAI1, 765, 280, -1)->SetRail([1,1,1,1,1,1]);
  CreateObject(RAI1, 800, 680, -1)->SetRail([1,1]);

  //L�ftungsgitter
  CreateObject(ENGT, 440, 670, -1);
  CreateObject(ENGT, 1200, 670, -1);

  //Bildschirme
  CreateObject(SCA2, 90, 490, -1)->SetAction("Wompf");
  CreateObject(SCR1, 260, 510, -1);
  CreateObject(SCA1, 305, 510, -1);
  CreateObject(SCR3, 350, 670, -1);
  CreateObject(SCR3, 1290, 670, -1);
  CreateObject(SCR1, 1380, 510, -1);
  CreateObject(SCA2, 1550, 490, -1)->SetAction("Scan2");

  //Flaschen
  CreateObject(BOTL, 400, 582, -1);
  CreateObject(BOTL, 1240, 582, -1);

  //Monitore
  CreateObject(MNI2, 20, 508, -1)->Off();
  CreateObject(MNI2, 40, 508, -1)->Off();
  CreateObject(MNI2, 80, 508, -1);
  CreateObject(MNI2, 100, 508, -1)->Off();
  CreateObject(MNI2, 1400, 172, -1);

  //Tische
  CreateObject(GTBL, 30, 520, -1);
  CreateObject(GTBL, 90, 520, -1);
  CreateObject(GTBL, 1505, 520, -1);
  CreateObject(GTBL, 1620, 520, -1);

  //Pflanzen
  CreateObject(PLNT, 300, 680, -1);
  CreateObject(PLNT, 1340, 680, -1);

  //Regale
  CreateObject(FRAM, 355, 480, -1);
  CreateObject(FRAM, 380, 480, -1);
  CreateObject(FRAM, 405, 480, -1);

  CreateObject(FRAM, 1235, 480, -1);
  CreateObject(FRAM, 1260, 480, -1);
  CreateObject(FRAM, 1285, 480, -1);

  //Notausgangslichter
  CreateObject(ETLT, 340, 640, -1);
  CreateObject(ETLT, 1300, 640, -1);

  //Rohre
  CreateObject(PIPL, 675, 755, -1)->Up()->Up()->Up()->Up();
  CreateObject(PIPL, 965, 755, -1)->Up()->Up()->Up()->Up();
}

/* Regelw�hler */

public func ChooserFinished()
{
  inherited();

  //Starttitel und Musikliste zusammenstellen
  SetPlayList("CMC_Base Groove.ogg;CMC_Firehawk.ogg;CMC_Friendly Unit.ogg;CMC_Getaway.ogg;CMC_Deep Universe.ogg;CMC_Eurocorps.ogg;CMC_Moving Squad.ogg;CMC_Offensive.ogg;CMC_Rock Go On.ogg;CMC_Showtime.ogg;CMC_Slow Motion.ogg;CMC_Striking Force.ogg;CMC_No Good.ogg;CMC_Obsession.ogg;CMC_Your Eyes.ogg");
  Music("CMC_Moving Squad.ogg");

  //Teams abfragen
  var aTeams = [false,false,false,false,false];
  for(var i = 0; i < GetPlayerCount(); i++)
   aTeams[GetPlayerTeam(GetPlayerByIndex(i))] = true;

  //OP-Spielziel
  if(FindObject(GOCC))
  {
   //Flaggenposten
   aFlag[0] = CreateObject(OFPL,415,230,NO_OWNER);
   aFlag[0] -> AddSpawnPoint(560,280);
   aFlag[0] -> AddSpawnPoint(635,330);
   aFlag[0] -> AddSpawnPoint(640,220);
   if(aTeams[1] == true)
   {
    aFlag[0]->Set("$Flag1$",100,2);
    aFlag[0]->Capture(1,1);
   }
   else
   {
    aFlag[0]->Set("$Flag1$",0,2);
   }

   aFlag[1] = CreateObject(OFPL,1340,520,NO_OWNER);
   aFlag[1] -> AddSpawnPoint(1160,510);
   aFlag[1] -> AddSpawnPoint(1190,590);
   aFlag[1] -> AddSpawnPoint(965,510);
   if(aTeams[2] == true)
   {
    aFlag[1]->Set("$Flag2$",100,2);
    aFlag[1]->Capture(2,1);
   }
   else
   {
    aFlag[1]->Set("$Flag2$",0,2);
   }

   aFlag[2] = CreateObject(OFPL,300,520,NO_OWNER);
   aFlag[2] -> AddSpawnPoint(480,510);
   aFlag[2] -> AddSpawnPoint(430,590);
   aFlag[2] -> AddSpawnPoint(670,510);
   if(aTeams[3] == true)
   {
    aFlag[2]->Set("$Flag3$",100,2);
    aFlag[2]->Capture(3,1);
   }
   else
   {
    aFlag[2]->Set("$Flag3$",0,2);
   }

   aFlag[3] = CreateObject(OFPL,1220,230,NO_OWNER);
   aFlag[3] -> AddSpawnPoint(1000,220);
   aFlag[3] -> AddSpawnPoint(1005,330);
   aFlag[3] -> AddSpawnPoint(1080,280);
   if(aTeams[4] == true)
   {
    aFlag[3]->Set("$Flag4$",100,2);
    aFlag[3]->Capture(4,1);
   }
   else
   {
    aFlag[3]->Set("$Flag4$",0,2);
   }

   aFlag[4] = CreateObject(OFPL,819,280,NO_OWNER);
   aFlag[4] -> AddSpawnPoint(780,180);
   aFlag[4] -> AddSpawnPoint(860,180);
   aFlag[4]->Set("$Flag5$",0,2);

   aFlag[5] = CreateObject(OFPL,821,680,NO_OWNER);
   aFlag[5] -> AddSpawnPoint(660,720);
   aFlag[5] -> AddSpawnPoint(980,720);
   aFlag[5] -> AddSpawnPoint(835,510);
   aFlag[5]->Set("$Flag6$",0,2);
  }

  //CTF-Spielziel
  if(FindObject(GCTF))
  {
   //Flaggen
   if(aTeams[1] == true)
   {CreateFlag(1,415,230,GetTeamColor(1));}
   if(aTeams[2] == true)
   {CreateFlag(2,1325,520,GetTeamColor(2));}
   if(aTeams[3] == true)
   {CreateFlag(3,315,520,GetTeamColor(3));}
   if(aTeams[4] == true)
   {CreateFlag(4,1220,230,GetTeamColor(4));}
  }
}

/* Relaunch */

public func RelaunchPosition(& iX, & iY, int iTeam)
{
  //Startsicht
  if(!g_chooserFinished)
  {
   iX = 820; iY = 540;
   return(1);
  }

  //CTF-Spielziel
  if(FindObject(GCTF))
  {
   var rand = Random(2);
   if(iTeam == 1)
   {
    if(!rand)
     { iX = 635; iY = 330; }
    if(!--rand)
     { iX = 640; iY = 220; }
   }
   if(iTeam == 2)
   {
    if(!rand)
     { iX = 960; iY = 510; }
    if(!--rand)
     { iX = 1030; iY = 510; }
   }
   if(iTeam == 3)
   {
    if(!rand)
     { iX = 610; iY = 510; }
    if(!--rand)
     { iX = 670; iY = 510; }
   }
   if(iTeam == 4)
   {
    if(!rand)
     { iX = 1005; iY = 330; }
    if(!--rand)
     { iX = 1000; iY = 220; }
   }
   return(1);
  }

  //LMS/DM-Spielziel
  if(FindObject(GLMS) || FindObject(GTDM))
  {
   var rand = Random(2);
   if(iTeam == 1)
   {
    if(!rand)
     { iX = 420; iY = 220; }
    if(!--rand)
     { iX = 580; iY = 280; }
   }
   if(iTeam == 2)
   {
    if(!rand)
     { iX = 1230; iY = 510; }
    if(!--rand)
     { iX = 1360; iY = 510; }
   }
   if(iTeam == 3)
   {
    if(!rand)
     { iX = 280; iY = 510; }
    if(!--rand)
     { iX = 400; iY = 510; }
   }
   if(iTeam == 4)
   {
    if(!rand)
     { iX = 1060; iY = 280; }
    if(!--rand)
     { iX = 1220; iY = 220; }
   }
   return(1);
  }
}