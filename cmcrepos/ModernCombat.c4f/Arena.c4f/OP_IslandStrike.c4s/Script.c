/*-- Island Strike --*/

#strict
#include CSTD

static aFlag,aSelfDefense;


func Initialize()
{
  //Bildschirmfärbung
  SetGamma(RGB(7,6,0), RGB(152,147,128), RGB(255,254,236) );
  //Musiktitel, welches für dieses Szenario bestimmt ist, zuerst abspielen
  Music("Blue Ressort");
  //Flaggen
  aFlag = [];
  //Selbstschussanlagen
  aSelfDefense = [];
  //Szenario einrichten
  CreateFurniture();
  //Ausrüstung plazieren
  CreateEquipment();
  //Himmel
  SetSkyParallax(0,15,15);
  return(1);
}

/* Plazierungslisten */

func CreateFurniture()
{
  Log("$CreatingFurniture$");
  //Alle Objekte folglich von links oben nach rechts unten

  //Bonusspawnpunkt
  PlaceBonusSpawnpoint([AMPB,HSTB,AEXB,AIMB], 3995, 320);

  //Aufzüge
  CreateObject(LFTP, 1025, 290, -1);
  CreateObject(LFTP, 2745, 555, -1);
  CreateObject(LFTP, 3505, 555, -1);
  CreateObject(LFTP, 3655, 555, -1);
  var liftp = CreateObject(LFTP, 3785, 345, -1);
  liftp->DisableVertical();
  liftp->ContactLeft();
  CreateObject(LFTP, 4375, 555, -1);
  CreateObject(LFTP, 5665, 545, -1);
  CreateObject(LFTP, 6405, 555, -1);
  CreateObject(LFTP, 7365, 400, -1);

  //Leitern
  CreateObject(LADR, 2515, 425, -1)->Set(11);
  CreateObject(LADR, 3605, 380, -1)->Set(6);
  CreateObject(LADR, 4445, 360, -1)->Set(3);
  CreateObject(LADR, 4680, 530, -1)->Set(7);
  CreateObject(LADR, 5120, 515, -1)->Set(13);
  CreateObject(LADR, 5525, 515, -1)->Set(13);
  CreateObject(LADR, 6635, 425, -1)->Set(11);
  CreateObject(LADR, 7240, 502, -1)->Set(8);
  CreateObject(LADR, 7280, 502, -1)->Set(8);
  CreateObject(LADR, 7320, 502, -1)->Set(8);
  CreateObject(LADR, 7425, 475, -1)->Set(14);

  //Bodenlucken
  CreateObject(HA4K, 2960, 373, -1);
  CreateObject(HA4K, 2890, 513, -1);
  CreateObject(HA4K, 1100, 473, -1);
  CreateObject(HA4K, 5120, 403, -1);
  CreateObject(HA4K, 5760, 353, -1);
  CreateObject(HA4K, 5760, 403, -1);
  CreateObject(HA4K, 6260, 513, -1);
  CreateObject(HA4K, 7200, 363, -1);
  CreateObject(HA4K, 7240, 433, -1);
  CreateObject(HA4K, 7280, 433, -1);
  CreateObject(HA4K, 7320, 433, -1);

  //Orientierungslichter
  CreateObject(OLGH, 1025, 210, -1)->Set(3, 15, 1, 1, 30);
  CreateObject(OLGH, 2560, 340, -1)->Set(2, 15, 1, 1, 40);
  CreateObject(OLGH, 2960, 280, -1)->Set(2, 15, 1, 1, 40);
  CreateObject(OLGH, 6590, 340, -1)->Set(2, 15, 1, 1, 40);

  //Rampen
  DrawMaterialQuad("Wall-Concrete3",1150,400,1180,390,1180,400,1165,400,true);

  DrawMaterialQuad("Sand-Swirl",2450,511,2480,501,2480,511,2465,511,true);

  DrawMaterialQuad("Wall-Concrete2",2681,510,2651,500,2651,510,2666,510,true);

  DrawMaterialQuad("Wall-Concrete3",4621,400,4591,390,4591,400,4606,400,true);
  DrawMaterialQuad("Wall-Concrete1",4651,410,4621,400,4621,410,4636,410,true);

  DrawMaterialQuad("Wall-Concrete2",4400,540,4430,530,4430,540,4415,540,true);

  DrawMaterialQuad("Wall-Bricks2",5500,590,5530,580,5530,590,5515,590,true);
  DrawMaterialQuad("Wall-Bricks2",5581,590,5551,580,5551,590,5566,590,true);

  DrawMaterialQuad("Wall-PlateRed",6060,260,6030,250,6030,260,6045,260,true);
  DrawMaterialQuad("Wall-PlateRed",6110,260,6080,250,6080,260,6095,260,true);
  DrawMaterialQuad("Wall-PlateRed",6160,260,6130,250,6130,260,6145,260,true);
  DrawMaterialQuad("Wall-PlateRed",6211,260,6181,250,6181,260,6196,260,true);

  DrawMaterialQuad("Wall-Concrete3",6211,390,6181,380,6181,390,6196,390,true);
  DrawMaterialQuad("Wall-Concrete1",6241,400,6211,390,6211,400,6226,400,true);
  DrawMaterialQuad("Wall-Concrete1",6271,410,6241,400,6241,410,6256,410,true);

  DrawMaterialQuad("Wall-Concrete2",6470,510,6500,500,6500,510,6485,510,true);

  DrawMaterialQuad("Sand-Swirl",6701,510,6671,500,6671,510,6686,510,true);

  //Rohre
  CreateObject(PIPL, 2730, 365, -1)->Up()->Right()->Right()->Right()->Right();
  CreateObject(PIPL, 2685, 365, -1)->Up()->Right();
  CreateObject(PIPL, 5150, 470, -1)->Right()->Right()->Right()->Right()->Right()->Right()->Up()->Up()->Right()->Right()->Right();
  CreateObject(PIPL, 5150, 485, -1)->Right()->Right()->Right()->Right()->Right()->Right()->Right()->Up()->Up()->Right()->Right();

  //Bojen
  CreateObject(BUOY, 445, 510, -1);
  CreateObject(BUOY, 1765, 510, -1);
  CreateObject(BUOY, 5040, 550, -1);
  CreateObject(BUOY, 5200, 550, -1);
  CreateObject(BUOY, 7665, 510, -1);

  //Satellitenschüssel
  CreateObject(SADH, 7625, 320, -1);

  //Kisten
  CreateObject(WCR2, 1275, 400, -1)->AutoRespawn();
  CreateObject(WCR2, 3605, 500, -1)->AutoRespawn();
  CreateObject(WCR2, 3775, 410, -1)->AutoRespawn();
  CreateObject(WCR2, 4250, 410, -1)->AutoRespawn();
  CreateObject(WCR2, 4265, 410, -1)->AutoRespawn();
  CreateObject(WCR2, 4400, 330, -1)->AutoRespawn();
  CreateObject(WCR2, 4570, 390, -1)->AutoRespawn();
  CreateObject(WCR2, 6290, 620, -1)->AutoRespawn();
  CreateObject(WCR2, 6310, 620, -1)->AutoRespawn();
  CreateObject(WCR2, 7325, 310, -1)->AutoRespawn();

  //Explosive Kiste
  CreateObject(XWCR, 5560, 400, -1)->AutoRespawn();

  //Benzinfässer
  CreateObject(PBRL, 1100, 270, -1)->AutoRespawn();
  CreateObject(PBRL, 1170, 470, -1)->AutoRespawn();

  CreateObject(PBRL, 2815, 591, -1)->AutoRespawn();

  CreateObject(PBRL, 3450, 630, -1)->AutoRespawn();
  CreateObject(PBRL, 3585, 500, -1)->AutoRespawn();

  CreateObject(PBRL, 4330, 410, -1)->AutoRespawn();
  CreateObject(PBRL, 4485, 580, -1)->AutoRespawn();

  CreateObject(PBRL, 6220, 510, -1)->AutoRespawn();
  CreateObject(PBRL, 6445, 410, -1)->AutoRespawn();

  //Explosivfässer
  CreateObject(XBRL, 1255, 400, -1)->AutoRespawn();

  CreateObject(XBRL, 2945, 510, -1)->AutoRespawn();
  CreateObject(XBRL, 2955, 510, -1)->AutoRespawn();

  CreateObject(XBRL, 3755, 410, -1)->AutoRespawn();

  CreateObject(XBRL, 7225, 430, -1)->AutoRespawn();

  //Haie
  CreateObject(SHRK, 100, 560, -1)->AutoRespawn();
  CreateObject(SHRK, 200, 560, -1)->AutoRespawn();
  CreateObject(SHRK, 500, 560, -1)->AutoRespawn();
  CreateObject(SHRK, 600, 560, -1)->AutoRespawn();

  CreateObject(SHRK, 1500, 545, -1)->AutoRespawn();
  CreateObject(SHRK, 1600, 545, -1)->AutoRespawn();
  CreateObject(SHRK, 1800, 550, -1)->AutoRespawn();
  CreateObject(SHRK, 1900, 550, -1)->AutoRespawn();

  CreateObject(SHRK, 3930, 580, -1)->AutoRespawn();
  CreateObject(SHRK, 4000, 580, -1)->AutoRespawn();
  CreateObject(SHRK, 4040, 600, -1)->AutoRespawn();
  CreateObject(SHRK, 4100, 600, -1)->AutoRespawn();

  CreateObject(SHRK, 4700, 585, -1)->AutoRespawn();
  CreateObject(SHRK, 4800, 585, -1)->AutoRespawn();
  CreateObject(SHRK, 5100, 585, -1)->AutoRespawn();
  CreateObject(SHRK, 5200, 585, -1)->AutoRespawn();

  CreateObject(SHRK, 7200, 550, -1)->AutoRespawn();
  CreateObject(SHRK, 7300, 550, -1)->AutoRespawn();
  CreateObject(SHRK, 7600, 550, -1)->AutoRespawn();
  CreateObject(SHRK, 7700, 550, -1)->AutoRespawn();

  //Jetwrack
  CreateObject(_JW1, 3120, 305, -1);
  CreateObject(_JW2, 3380, 260, -1);

  //Kaputte U-Boote
  CreateObject(SUBB, 240, 630, -1);
  CreateObject(SUBB, 7200, 700, -1);

  //Kaputte Boote
  CreateObject(SLBB, 1785, 550, -1);
  CreateObject(SLBB, 4000, 675, -1);
  CreateObject(SLBB, 5270, 565, -1);
  CreateObject(SLBB, 7830, 625, -1);

  //Regale
  CreateObject(FRAM, 4250, 580, -1);
  CreateObject(FRAM, 4270, 580, -1);
  CreateObject(FRAM, 4290, 580, -1);
  CreateObject(FRAM, 4310, 580, -1);
  CreateObject(FRAM, 4330, 580, -1);

  CreateObject(FRAM, 4415, 580, -1);
  CreateObject(FRAM, 4435, 580, -1);
  CreateObject(FRAM, 4455, 580, -1);
  CreateObject(FRAM, 4475, 580, -1);
  CreateObject(FRAM, 4495, 580, -1);

  //Container
  CreateObject(CON1, 2830, 620, -1);
  CreateObject(CON1, 6089, 380, -1);
  CreateObject(CON1, 6146, 380, -1);

  //Ventillatoren
  CreateObject(VENT, 2705, 463, -1)->SetCon(20);
  CreateObject(VENT, 6445, 463, -1)->SetCon(20);

  //Satellitenschüsseln
  CreateObject(RADR, 1025, 210, -1);
  CreateObject(RADR, 2960, 280, -1);
  CreateObject(RADR, 3655, 330, -1);
  CreateObject(RADR, 7350, 310, -1);

  //Markierungsschilder
  CreateObject(MSGN, 2610, 410, -1);
  CreateObject(MSGN, 2620, 410, -1);
  CreateObject(MSGN, 2630, 410, -1);

  CreateObject(MSGN, 6525, 410, -1);
  CreateObject(MSGN, 6535, 410, -1);
  CreateObject(MSGN, 6545, 410, -1);

  //Einfache Türen
  CreateObject(LBDR, 975, 270, -1);
  CreateObject(LBDR, 1063, 270, -1);
  CreateObject(LBDR, 7304, 360, -1);

  //Stahltüren
  CreateObject(STDR, 2650, 410, -1);
  CreateObject(STDR, 2700, 620, -1);
  CreateObject(STDR, 2810, 410, -1);
  CreateObject(STDR, 2990, 510, -1);
  CreateObject(STDR, 2990, 620, -1);
  CreateObject(STDR, 3420, 630, -1);
  CreateObject(STDR, 3450, 520, -1);
  CreateObject(STDR, 3690, 530, -1);
  CreateObject(STDR, 4310, 410, -1);
  CreateObject(STDR, 4430, 390, -1);
  CreateObject(STDR, 4520, 580, -1);
  CreateObject(STDR, 5610, 400, -1);
  CreateObject(STDR, 5860, 510, -1);
  CreateObject(STDR, 6160, 510, -1);
  CreateObject(STDR, 6160, 620, -1);
  CreateObject(STDR, 6340, 410, -1);
  CreateObject(STDR, 6450, 620, -1);
  CreateObject(STDR, 6500, 410, -1);

  //Pflanzen
  CreateObject(PLNT, 1120, 525, -1);
  CreateObject(PLNT, 2715, 510, -1);
  CreateObject(PLNT, 6435, 510, -1);

  //Apparaturen
  CreateObject(GADG, 1135, 540, -1)->Set(1);
  CreateObject(GADG, 4340, 580, -1)->Set(2);
  CreateObject(GADG, 5810, 510, -1)->Set(1);
  CreateObject(GADG, 5820, 510, -1)->Set(1);

  //Büsche
  CreateObject(BSH2, 2470, 510, -1);
  CreateObject(BSH2, 2460, 520, -1);
  CreateObject(BSH2, 3060, 640, -1);
  CreateObject(BSH2, 6035, 580, -1);
  CreateObject(BSH2, 6680, 510, -1);
  CreateObject(BSH2, 6690, 520, -1);

  //Verbundene Türen
  var doorw = CreateObject(GAT1, 2960, 333, -1);
  CreateObject(ROOM, 3570, 410, -1)->Connect(doorw);
  doorw = CreateObject(GAT2, 3845, 685, -1);
  CreateObject(GAT1, 3830, 525, -1)->Connect(doorw);
  doorw = CreateObject(GAT2, 4200, 685, -1);
  CreateObject(GAT1, 4215, 515, -1)->Connect(doorw);
  doorw = CreateObject(GAT1, 6095, 315, -1);
  CreateObject(ROOM, 5735, 400, -1)->Connect(doorw);

  //Dekoschleusen
  CreateObject(GAT1, 6045, 315, -1);
  CreateObject(GAT1, 6145, 315, -1);
  CreateObject(GAT1, 6195, 315, -1);

  //Panel
  CreateObject(CPP1, 2840, 510, -1);
  CreateObject(CPP1, 4465, 580, -1);

  //Spinde
  CreateObject(LCKR, 5705, 550, -1);
  CreateObject(LCKR, 5725, 550, -1);
  CreateObject(LCKR, 5745, 550, -1);

  //Wandlampen
  CreateObject(BLGH, 1025, 260, -1);
  CreateObject(BLGH, 1160, 445, -1);
  CreateObject(BLGH, 2690, 390, -1);
  CreateObject(BLGH, 3585, 475, -1);
  CreateObject(BLGH, 4335, 385, -1);
  CreateObject(BLGH, 4335, 525, -1);
  CreateObject(BLGH, 5725, 505, -1);
  CreateObject(BLGH, 6445, 385, -1);

  //Laborlampen
  CreateObject(LLGH, 1035, 520, -1);
  CreateObject(LLGH, 1250, 490, -1);
  CreateObject(LLGH, 1340, 490, -1);
  CreateObject(LLGH, 5120, 540, -1);
  CreateObject(LLGH, 7280, 410, -1);

  //Sandsackbarrieren
  CreateObject(SBBA, 1380, 440, -1)->Right();
  CreateObject(SBBA, 2660, 340, -1);
  CreateObject(SBBA, 3800, 410, -1)->Right();
  CreateObject(SBBA, 4204, 409, -1);
  CreateObject(SBBA, 5075, 400, -1);
  CreateObject(SBBA, 5165, 400, -1)->Right();
  CreateObject(SBBA, 5730, 340, -1)->Right();
  CreateObject(SBBA, 6340, 620, -1)->Right();
  CreateObject(SBBA, 6490, 340, -1)->Right();

  //Geländer
  CreateObject(RAI1, 930, 270, -1)->SetRail([1,1]);
  CreateObject(RAI1, 1075, 270, -1)->SetRail([1,1]);
  CreateObject(RAI1, 1300, 440, -1)->SetRail([1,1,1,1,1]);
  CreateObject(RAI1, 3700, 410, -1)->SetRail([1,1,1,1,1,1]);
  CreateObject(RAI1, 4190, 410, -1)->SetRail([1,1,1,1,1,1]);
  CreateObject(RAI1, 5063, 400, -1)->SetRail([1,1,1,1,1,1]);
  CreateObject(RAI1, 5530, 400, -1)->SetRail([1,1,1,1]);
  CreateObject(RAI1, 7140, 360, -1)->SetRail([1,1,1,1,1,1,1,1,1]);
  CreateObject(RAI1, 7395, 360, -1)->SetRail([1]);

  //Zäune
  CreateObject(FENC, 2850, 410, -1);
  CreateObject(FENC, 4470, 390, -1);
  CreateObject(FENC, 4531, 390, -1);
  CreateObject(FENC, 6300, 410, -1);

  //Bildschirme
  CreateObject(SCR1, 2745, 470, -1);
  CreateObject(SCR3, 3555, 595, -1);
  CreateObject(SCA1, 4375, 480, -1);
  CreateObject(SCR1, 6405, 470, -1);
  CreateObject(SCR3, 7310, 410, -1);

  //Automaten
  CreateObject(CLVM, 1000, 540, -1);
  CreateObject(CLVM, 7210, 430, -1);

  //Lüftungsgitter
  CreateObject(ENGT, 1250, 470, -1);
  CreateObject(ENGT, 3610, 610, -1);
  CreateObject(ENGT, 5725, 550, -1);
  CreateObject(ENGT, 7190, 505, -1);
  CreateObject(ENGT, 7390, 505, -1);

  //Topfpflanze
  CreateObject(PLT2, 1030, 540, -1);

  //Tische
  CreateObject(GTBL, 1105, 540, -1);

  //Schilder
  CreateObject(ESGN, 1065, 460, -1);
  CreateObject(ESGN, 2960, 505, -1);
  CreateObject(ESGN, 5830, 505, -1);
  CreateObject(ESGN, 6200, 505, -1);

  //Stationäre Gastanks
  CreateObject(GSTA, 1335, 440, -1)->AutoRespawn();
  CreateObject(GSTA, 4345, 330, -1)->AutoRespawn();

  //Sounds

  //Möven
  CreateObject(SE4K, 865, 480, -1)->Set("SeaSounds*",140,35);
  CreateObject(SE4K, 2290, 500, -1)->Set("SeaSounds*",140,35);
  CreateObject(SE4K, 2480, 460, -1)->Set("SeaSounds*",140,35);
  CreateObject(SE4K, 4775, 460, -1)->Set("SeaSounds*",140,35);
  CreateObject(SE4K, 5120, 420, -1)->Set("SeaSounds*",140,35);
  CreateObject(SE4K, 6660, 450, -1)->Set("SeaSounds*",140,35);
  CreateObject(SE4K, 6850, 500, -1)->Set("SeaSounds*",140,35);
  CreateObject(SE4K, 7560, 275, -1)->Set("SeaSounds*",140,35);

  //Wellen
  CreateObject(SE4K, 240, 510, -1)->Set("Wave*",105,35);
  CreateObject(SE4K, 640, 510, -1)->Set("Wave*",105,35);
  CreateObject(SE4K, 1620, 510, -1)->Set("Wave*",105,35);
  CreateObject(SE4K, 2090, 510, -1)->Set("Wave*",105,35);
  CreateObject(SE4K, 4000, 530, -1)->Set("Wave*",105,35);

  CreateObject(SE4K, 4910, 550, -1)->Set("Wave*",105,35);
  CreateObject(SE4K, 5370, 550, -1)->Set("Wave*",105,35);
  CreateObject(SE4K, 7050, 510, -1)->Set("Wave*",105,35);
  CreateObject(SE4K, 7660, 510, -1)->Set("Wave*",105,35);

  //Jungel
  CreateObject(SE4K, 3160, 290, -1)->Set("Jungle*",140,70);
  CreateObject(SE4K, 3375, 360, -1)->Set("Jungle*",140,70);
  CreateObject(SE4K, 5850, 340, -1)->Set("Jungle*",140,70);

  //Wind
  CreateObject(SE4K, 1025, 170, -1)->Set("WindSound*",245,105);
  CreateObject(SE4K, 4000, 280, -1)->Set("WindSound*",245,105);
  CreateObject(SE4K, 5120, 365, -1)->Set("WindSound*",245,105);
  CreateObject(SE4K, 7255, 260, -1)->Set("WindSound*",245,105);

  //Kran und Konsolen
  var cran = CreateObject(CRN1, 4000, 359, -1);
  cran->Set(42);
  CreateObject(CONS, 3710, 465, -1)->Set(cran);
  CreateObject(CONS, 4305, 465, -1)->Set(cran);
  
  //Tore und Konsolen
  var autod = CreateObject (HNG2, 2780, 620, -1);
  autod->Open();
  CreateObject (CONS, 2880, 615, -1)->Set(autod);
  
  autod = CreateObject (HNG2, 3570, 500, -1);
  autod->Open();
  CreateObject (CONS, 3695, 465, -1)->Set(autod);
  
  autod = CreateObject (HNG2, 5532, 570, -1);
  autod->Open();
  CreateObject (CONS, 5630, 530, -1)->Set(autod);
  
  autod = CreateObject (HNG2, 6370, 620, -1);
  autod->Open();
  CreateObject (CONS, 6270, 615, -1)->Set(autod);

  //Selbstschussanlagen
  aSelfDefense[0] = CreateObject (SEGU, 1095, 309, -1);
    aSelfDefense[0]->Arm(MGSA);
    aSelfDefense[0]->TurnOn(MGSA);
    CreateObject (CONS, 1155, 530, -1)->Set(aSelfDefense[0]);
  
  aSelfDefense[1] = CreateObject (SEGU, 2565, 379, -1);
    aSelfDefense[1]->Arm(MISA);
    aSelfDefense[1]->SetAutoRepair(525);
    aSelfDefense[1]->GetAttWeapon()->SetFireMode(2);
    CreateObject (CONS, 2800, 500, -1)->Set(aSelfDefense[1]);
  
  aSelfDefense[2] = CreateObject (SEGU, 6585, 379, -1);
    aSelfDefense[2]->Arm(MISA);
    aSelfDefense[2]->SetAutoRepair(525);
    aSelfDefense[2]->GetAttWeapon()->SetFireMode(2);
    CreateObject (CONS, 6355, 500, -1)->Set(aSelfDefense[2]);
  
  aSelfDefense[3] = CreateObject (SEGU, 7190, 479, -1);
    aSelfDefense[3]->Arm(MGSA);
    aSelfDefense[3]->TurnOn(MGSA);
    CreateObject (CONS, 7260, 420, -1)->Set(aSelfDefense[3]);
  
  //Flaggen
  aFlag[0] = CreateObject(OFPL,1205,390,NO_OWNER);
    aFlag[0]->Set("$Flag1$",100);
    aFlag[0]->Capture(1);

  aFlag[1] = CreateObject(OFPL,2720,340,NO_OWNER);
    aFlag[1]->Set("$Flag2$",100,2);
    aFlag[1]->Capture(1);

  aFlag[2] = CreateObject(OFPL,3580,630,NO_OWNER);
    aFlag[2]->Set("$Flag3$",100,2);
    aFlag[2]->Capture(1);

  aFlag[3] = CreateObject(OFPL,4510,390,NO_OWNER);
    aFlag[3]->Set("$Flag4$",100,2);

  aFlag[4] = CreateObject(OFPL,5665,340,NO_OWNER);
    aFlag[4]->Set("$Flag5$",100,2);
    aFlag[4]->Capture(2);

  aFlag[5] = CreateObject(OFPL,6430,340,NO_OWNER);
    aFlag[5]->Set("$Flag6$",100,2);
    aFlag[5]->Capture(2);

  aFlag[6] = CreateObject(OFPL,7260,360,NO_OWNER);
    aFlag[6]->Set("$Flag7$",100);
    aFlag[6]->Capture(2);

  //Alarmleuchten
  //Basis 1
  var warn = CreateObject (ALGH, 935, 299, -1);
    warn->SetR(-180);
    AddWarnEffect(warn,aFlag[0]);
  warn = CreateObject (ALGH, 1115, 299, -1);
    warn->SetR(-180);
    AddWarnEffect(warn,aFlag[0]);
  warn = CreateObject (ALGH, 1305, 469, -1);
    warn->SetR(-180);
    AddWarnEffect(warn,aFlag[0]);

  //Basis 2
  warn = CreateObject (ALGH, 2525, 369, -1);
    warn->SetR(-180);
    AddWarnEffect(warn,aFlag[1]);
  warn = CreateObject (ALGH, 2595, 369, -1);
    warn->SetR(-180);
    AddWarnEffect(warn,aFlag[1]);
  warn = CreateObject (ALGH, 2780, 439, -1);
    warn->SetR(-180);
    AddWarnEffect(warn,aFlag[1]);

  //Basis 3
  warn = CreateObject (ALGH, 3570, 369, -1);
    warn->SetR(-180);
    AddWarnEffect(warn,aFlag[2]);
  warn = CreateObject (ALGH, 3710, 509, -1);
    warn->SetR(-180);
    AddWarnEffect(warn,aFlag[2]);

  //Basis 4
  warn = CreateObject (ALGH, 4245, 499, -1);
    warn->SetR(-180);
    AddWarnEffect(warn,aFlag[3]);
  warn = CreateObject (ALGH, 4415, 509, -1);
    warn->SetR(-180);
    AddWarnEffect(warn,aFlag[3]);

  //Basis 5
  warn = CreateObject (ALGH, 5705, 379, -1);
    warn->SetR(-180);
    AddWarnEffect(warn,aFlag[4]);
  warn = CreateObject (ALGH, 5570, 519, -1);
    warn->SetR(-180);
    AddWarnEffect(warn,aFlag[4]);

  //Basis 6
  warn = CreateObject (ALGH, 6555, 369, -1);
    warn->SetR(-180);
    AddWarnEffect(warn,aFlag[5]);
  warn = CreateObject (ALGH, 6625, 369, -1);
    warn->SetR(-180);
    AddWarnEffect(warn,aFlag[5]);
  warn = CreateObject (ALGH, 6370, 439, -1);
    warn->SetR(-180);
    AddWarnEffect(warn,aFlag[5]);

  //Basis 7
  warn = CreateObject (ALGH, 7150, 360, -1);
    AddWarnEffect(warn,aFlag[6]);
  warn = CreateObject (ALGH, 7410, 360, -1);
    AddWarnEffect(warn,aFlag[6]);
}

func CreateEquipment()
{
  Log("$CreatingEquipment$");
  //Alle Objekte folglich von links oben nach rechts unten

  //Munitionskisten (Kugeln)
  CreateObject(ABOX, 1060, 540, -1)->Set(AMOC);
  CreateObject(ABOX, 7300, 430, -1)->Set(AMOC);

  //Projektilmunition
  PlaceSpawnpoint(AMOC, 3160, 290);
  PlaceSpawnpoint(AMOC, 4735, 445);

  //Granatmunition
  PlaceSpawnpoint(GRDC, 4335, 320);

  //Raketen
  PlaceSpawnpoint(MIAP, 1025, 205);
  PlaceSpawnpoint(MIAP, 3150, 535);
  PlaceSpawnpoint(MIAP, 6035, 550);
  PlaceSpawnpoint(MIAP, 6520, 630);

  //Feldgranaten
  PlaceSpawnpoint(FGRN, 985, 400);
  PlaceSpawnpoint(FGRN, 7405, 350);

  //Blendgranaten
  PlaceSpawnpoint(STUN, 3550, 490);
  PlaceSpawnpoint(STUN, 5725, 540);

  //Rauchgranaten
  PlaceSpawnpoint(SGRN, 3715, 400);
  PlaceSpawnpoint(SGRN, 4415, 520);

  //Splittergranaten
  PlaceSpawnpoint(FRAG, 2940, 610);
  PlaceSpawnpoint(FRAG, 6210, 610);

  //Splittergranatenspawnpoints
  PlaceSpawnpoint(FRAG, 2940, 610);

  //P29
  PlaceSpawnpoint(P29W, 1015, 530);
  PlaceSpawnpoint(P29W, 7205, 420);

  //Spas12
  PlaceSpawnpoint(SPAS, 2830, 580);
  PlaceSpawnpoint(SPAS, 6300, 590);

  //M16
  PlaceSpawnpoint(M16A, 1335, 430);
  PlaceSpawnpoint(M16A, 7360, 300);

  //M249
  PlaceSpawnpoint(M249, 5120, 390);

  //MP7
  PlaceSpawnpoint(MP7R, 3400, 505);
  PlaceSpawnpoint(MP7R, 5580, 460);

  //PzF
  PlaceSpawnpoint(PZFW, 1000, 205);
  PlaceSpawnpoint(PZFW, 6495, 615);

  //OICW
  PlaceSpawnpoint(OICW, 4360, 315);

  //Motorboote
  SetupVehicleSpawn([INFL],CreateObject(VSPW,1245,510,-1),35*40);
  SetupVehicleSpawn([INFL],CreateObject(VSPW,4640,550,-1),35*40);
  SetupVehicleSpawn([INFL],CreateObject(VSPW,5565,550,-1),35*40);
}

/* Besitznahme */

//Wenn eine Flagge übernommen wird...
func PointCaptured(object pPoint, int iTeam)
{
  if(pPoint == aFlag[0])
    aSelfDefense[0]->SetTeam(iTeam);
  
  if(pPoint == aFlag[1])
    aSelfDefense[1]->SetTeam(iTeam);
  
  if(pPoint == aFlag[5])
    aSelfDefense[2]->SetTeam(iTeam);

  if(pPoint == aFlag[6])
    aSelfDefense[3]->SetTeam(iTeam);
}

//Alarmleuchtensteuerung in Effektform
global func FxIntWarnStart(object pTarget, int iEffectNumber, int iTemp,pPoint)
{
  EffectVar(0,pTarget,iEffectNumber) = pPoint;
  return(1);
}

global func FxIntWarnTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
  if(!pTarget)
    return(-1);

  var pPoint = EffectVar(0,pTarget,iEffectNumber);
  var iLast  = EffectVar(1,pTarget,iEffectNumber);
  var iNow;
  if(pPoint->GetTrend() || pPoint->GetAttacker())
    iNow = true;
  
  if(iNow != iLast)
  {
    if((pPoint->GetTrend() >= 0) || !pPoint->GetAttacker())
      pTarget->TurnOff();
    else
      pTarget->TurnOn();
      
    EffectVar(1,pTarget,iEffectNumber) = iNow;
  }
  
  return(0);
}

func AddWarnEffect(object pTarget, object pPoint)
{
  AddEffect ("IntWarn",pTarget,100,35,pTarget,0,pPoint);
}