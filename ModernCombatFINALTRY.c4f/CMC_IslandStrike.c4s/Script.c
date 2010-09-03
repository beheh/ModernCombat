/*-- Island Strike --*/

#strict
#include CSTD

static aFlag,aSelfDefense;


/* Initalisierung */

func Initialize()
{
  //Starttitel und Musikliste zusammenstellen
  SetPlayList("CMC_Base Groove.ogg;CMC_Firehawk.ogg;CMC_Friendly Unit.ogg;CMC_Getaway.ogg;CMC_Moving Squad.ogg;CMC_Offensive.ogg;CMC_Showtime.ogg;CMC_Slow Motion.ogg;CMC_Striking Force.ogg;CMC_No Good.ogg;CMC_Obsession.ogg");
  Music("CMC_Offensive.ogg");
  //Bildschirmfärbung
  SetGamma(RGB(7,6,0), RGB(152,147,128), RGB(255,254,236) );
  //Flaggen
  aFlag = [];
  //Selbstschussanlagen
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
  //Alle Objekte folglich von links oben nach rechts unten

  //Aufzüge
  CreateObject(LFTP, 1025, 290, -1);
  CreateObject(LFTP, 2745, 555, -1);
  CreateObject(LFTP, 3655, 555, -1);
  var liftp = CreateObject(LFTP, 3785, 345, -1);
  liftp->DisableVertical();
  liftp->ContactLeft();
  CreateObject(LFTP, 4375, 555, -1);
  CreateObject(LFTP, 5665, 545, -1);
  CreateObject(LFTP, 6405, 555, -1);

  //Leitern
  CreateObject(LADR, 695, 504, -1)->Set(7);
  CreateObject(LADR, 925, 453, -1)->Set(22);
  CreateObject(LADR, 1295, 494, -1)->Set(6);
  CreateObject(LADR, 1405, 494, -1)->Set(6);
  CreateObject(LADR, 1595, 494, -1)->Set(6);
  CreateObject(LADR, 1715, 494, -1)->Set(6);

  CreateObject(LADR, 2515, 431, -1)->Set(11);

  CreateObject(LADR, 3505, 640, -1)->Set(20);
  CreateObject(LADR, 3605, 384, -1)->Set(6);
  CreateObject(LADR, 4445, 360, -1)->Set(3);
  CreateObject(LADR, 4680, 538, -1)->Set(8);
  CreateObject(LADR, 4780, 424, -1)->Set(12);

  CreateObject(LADR, 5120, 537, -1)->Set(16);
  CreateObject(LADR, 5525, 536, -1)->Set(16);

  CreateObject(LADR, 6635, 434, -1)->Set(11);

  CreateObject(LADR, 7240, 502, -1)->Set(8);
  CreateObject(LADR, 7280, 502, -1)->Set(8);
  CreateObject(LADR, 7320, 502, -1)->Set(8);
  CreateObject(LADR, 7365, 440, -1)->Set(12);
  CreateObject(LADR, 7425, 478, -1)->Set(14);
  CreateObject(LADR, 7670, 375, -1)->Set(6);

  //Bodenlucken
  CreateObject(HA4K, 2960, 373, -1);
  CreateObject(HA4K, 2890, 513, -1);
  CreateObject(HA4K, 1100, 473, -1);
  CreateObject(HA4K, 4780, 323, -1);
  CreateObject(HA4K, 5120, 403, -1);
  CreateObject(HA4K, 5760, 353, -1);
  CreateObject(HA4K, 5760, 403, -1);
  CreateObject(HA4K, 6260, 513, -1);
  CreateObject(HA4K, 7200, 363, -1);
  CreateObject(HA4K, 7240, 433, -1);
  CreateObject(HA4K, 7280, 433, -1);
  CreateObject(HA4K, 7320, 433, -1);
  CreateObject(HA4K, 7670, 323, -1);

  //Orientierungslichter
  CreateObject(OLGH, 595, 440, -1)->Set(4, 15, 1, 1, 30);
  CreateObject(OLGH, 1025, 210, -1)->Set(3, 15, 1, 1, 30);
  CreateObject(OLGH, 1500, 290, -1)->Set(4, 15, 1, 1, 30);
  CreateObject(OLGH, 2560, 340, -1)->Set(2, 15, 1, 1, 40);
  CreateObject(OLGH, 2960, 280, -1)->Set(2, 15, 1, 1, 40);
  CreateObject(OLGH, 4750, 320, -1)->Set(5, 15, 1, 1, 30);
  CreateObject(OLGH, 6590, 340, -1)->Set(2, 15, 1, 1, 40);
  CreateObject(OLGH, 7755, 320, -1)->Set(4, 15, 1, 1, 30);

  //Rampen
  DrawMaterialQuad("Wall-Concrete3",1150,400,1180,390,1180,400,1165,400,true);

  DrawMaterialQuad("Wall-Concrete2",2450,511,2480,501,2480,511,2465,511,true);

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

  DrawMaterialQuad("Wall-Concrete2",6701,510,6671,500,6671,510,6686,510,true);

  //Glasscheiben
  CreateObject(_WIN, 982, 330, -1);
  CreateObject(_WIN, 1068, 330, -1);

  CreateObject(_WIN, 5092, 470, -1);
  CreateObject(_WIN, 5148, 470, -1);

  //Rohre
  CreateObject(PIPL, 2730, 365, -1)->Up()->Right()->Right()->Right()->Right();
  CreateObject(PIPL, 2685, 365, -1)->Up()->Right();
  CreateObject(PIPL, 5150, 470, -1)->Right()->Right()->Right()->Right()->Right()->Right()->Up()->Up()->Right()->Right()->Right();
  CreateObject(PIPL, 5150, 485, -1)->Right()->Right()->Right()->Right()->Right()->Right()->Right()->Up()->Up()->Right()->Right();

  //Bojen
  CreateObject(BUOY, 445, 533, -1);
  CreateObject(BUOY, 1765, 533, -1);
  CreateObject(BUOY, 5040, 573, -1);
  CreateObject(BUOY, 5200, 573, -1);
  CreateObject(BUOY, 7665, 533, -1);

  //Satellitenschüssel
  CreateObject(SADH, 7625, 320, -1);

  //Metallkiste
  CreateObject(MWCR, 1530, 400, -1)->AutoRespawn();
  CreateObject(MWCR, 4570, 390, -1)->AutoRespawn();
  CreateObject(MWCR, 7560, 510, -1)->AutoRespawn();

  //Kisten
  CreateObject(WCR2, 1510, 400, -1);
  CreateObject(WCR2, 1275, 400, -1)->AutoRespawn();
  CreateObject(WCR2, 3605, 500, -1)->AutoRespawn();
  CreateObject(WCR2, 3775, 410, -1)->AutoRespawn();
  CreateObject(WCR2, 4250, 410, -1);
  CreateObject(WCR2, 4265, 410, -1)->AutoRespawn();
  CreateObject(WCR2, 4400, 330, -1)->AutoRespawn();
  CreateObject(WCR2, 6290, 620, -1)->AutoRespawn();
  CreateObject(WCR2, 6310, 620, -1)->AutoRespawn();
  CreateObject(WCR2, 7325, 310, -1)->AutoRespawn();

  //Explosive Kiste
  CreateObject(XWCR, 5560, 400, -1)->AutoRespawn();
  CreateObject(XWCR, 7550, 492, -1);

  //Benzinfässer
  CreateObject(PBRL, 520, 440, -1)->AutoRespawn();
  CreateObject(PBRL, 530, 440, -1)->AutoRespawn();
  CreateObject(PBRL, 1475, 400, -1)->AutoRespawn();
  CreateObject(PBRL, 1485, 400, -1)->AutoRespawn();
  CreateObject(PBRL, 1100, 270, -1)->AutoRespawn();
  CreateObject(PBRL, 1170, 470, -1)->AutoRespawn();

  CreateObject(PBRL, 2815, 591, -1)->AutoRespawn();

  CreateObject(PBRL, 3450, 630, -1)->AutoRespawn();
  CreateObject(PBRL, 3585, 500, -1)->AutoRespawn();

  CreateObject(PBRL, 4330, 410, -1)->AutoRespawn();
  CreateObject(PBRL, 4485, 580, -1)->AutoRespawn();

  CreateObject(PBRL, 6220, 510, -1)->AutoRespawn();
  CreateObject(PBRL, 6445, 410, -1)->AutoRespawn();

  CreateObject(PBRL, 7790, 320, -1)->AutoRespawn();
  CreateObject(PBRL, 7800, 320, -1)->AutoRespawn();

  //Explosivfässer
  CreateObject(XBRL, 1255, 400, -1)->AutoRespawn();

  CreateObject(XBRL, 2945, 510, -1)->AutoRespawn();
  CreateObject(XBRL, 2955, 510, -1)->AutoRespawn();

  CreateObject(XBRL, 3755, 410, -1)->AutoRespawn();

  CreateObject(XBRL, 7225, 430, -1)->AutoRespawn();

  //Gasflaschen
  CreateObject(GSBL, 4240, 580, -1)->AutoRespawn();
  CreateObject(GSBL, 5760, 550, -1)->AutoRespawn();

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
  CreateObject(CON1, 7500, 510, -1)->SetPerspective(2);

  //Ventillatoren
  CreateObject(VENT, 2705, 463, -1)->SetCon(20);
  CreateObject(VENT, 6445, 463, -1)->SetCon(20);

  //Satellitenschüsseln
  CreateObject(RADR, 1500, 290, -1);
  CreateObject(RADR, 2960, 280, -1);

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
  CreateObject(BSH2, 1875, 350, -1);
  CreateObject(BSH2, 2470, 510, -1);
  CreateObject(BSH2, 2460, 520, -1);
  CreateObject(BSH2, 3060, 640, -1);
  CreateObject(BSH2, 6035, 580, -1);
  CreateObject(BSH2, 6680, 510, -1);
  CreateObject(BSH2, 6690, 520, -1);

  //Steine
  CreateObject(STNE, 1460, 560, -1);
  CreateObject(STNE, 2030, 340, -1);
  CreateObject(STNE, 3320, 370, -1);
  CreateObject(STNE, 5815, 350, -1);
  CreateObject(STNE, 6640, 670, -1);
  CreateObject(STNE, 7410, 570, -1);

  //Verbundene Türen
  var doorw = CreateObject(GAT1, 2960, 333, -1);
  CreateObject(ROOM, 3570, 410, -1)->Connect(doorw);
  doorw = CreateObject(GAT1, 3845, 685, -1);
  CreateObject(GAT1, 3830, 525, -1)->Connect(doorw);
  doorw = CreateObject(GAT1, 4200, 685, -1);
  CreateObject(GAT1, 4215, 515, -1)->Connect(doorw);
  doorw = CreateObject(GAT1, 6095, 315, -1);
  CreateObject(ROOM, 5735, 400, -1)->Connect(doorw);

  //Dekoschleusen
  CreateObject(GAT1, 6045, 315, -1);
  CreateObject(GAT1, 6145, 315, -1);
  CreateObject(GAT1, 6195, 315, -1);

  //Panele
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
  CreateObject(LLGH, 1500, 490, -1);
  CreateObject(LLGH, 1655, 490, -1);
  CreateObject(LLGH, 5120, 540, -1);
  CreateObject(LLGH, 7280, 410, -1);

  //Sandsackbarrieren
  CreateObject(SBBA, 1380, 440, -1)->Right();
  CreateObject(SBBA, 1690, 440, -1)->Right();
  CreateObject(SBBA, 2660, 340, -1);
  CreateObject(SBBA, 3800, 410, -1)->Right();
  CreateObject(SBBA, 4204, 409, -1);
  CreateObject(SBBA, 5075, 400, -1);
  CreateObject(SBBA, 5165, 400, -1)->Right();
  CreateObject(SBBA, 5780, 350, -1)->Right();
  CreateObject(SBBA, 6340, 620, -1)->Right();
  CreateObject(SBBA, 6490, 340, -1)->Right();

  //Geländer
  CreateObject(RAI1, 510, 440, -1)->SetRail([1,1,1,1,1,1,1,1,1]);
  CreateObject(RAI1, 930, 270, -1)->SetRail([1,1]);
  CreateObject(RAI1, 1075, 270, -1)->SetRail([1,1]);
  CreateObject(RAI1, 1300, 440, -1)->SetRail([1,1,1,1,1]);
  CreateObject(RAI1, 1600, 440, -1)->SetRail([1,1,1,1,1]);
  CreateObject(RAI1, 3700, 410, -1)->SetRail([1,1,1,1,1,1]);
  CreateObject(RAI1, 4190, 410, -1)->SetRail([1,1,1,1,1,1]);
  CreateObject(RAI1, 4660, 320, -1)->SetRail([1,1,1,1,1,1,1,1,1]);
  CreateObject(RAI1, 5063, 400, -1)->SetRail([1,1,1,1,1,1]);
  CreateObject(RAI1, 5530, 400, -1)->SetRail([1,1,1,1]);
  CreateObject(RAI1, 7140, 360, -1)->SetRail([1,1,1,1,1,1,1,1,1]);
  CreateObject(RAI1, 7395, 360, -1)->SetRail([1]);
  CreateObject(RAI1, 7480, 510, -1)->SetRail([1,1,1,1,1,1]);
  CreateObject(RAI1, 7660, 320, -1)->SetRail([1,1,1,1,1,1,1,1,1]);

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

  //Explosivtanks
  CreateObject(XTNK, 1335, 440, -1)->AutoRespawn();
  CreateObject(XTNK, 4345, 330, -1)->AutoRespawn();
  CreateObject(XTNK, 4690, 320, -1)->AutoRespawn();

  //Phosphortanks
  CreateObject(PTNK, 1635, 440, -1)->AutoRespawn();
  CreateObject(PTNK, 2855, 410, -1)->AutoRespawn();
  CreateObject(PTNK, 6295, 410, -1)->AutoRespawn();

  //Sounds

  //Möven
  CreateObject(SE4K, 865, 480, -1)->Set("SeaSounds*.ogg",140,35);
  CreateObject(SE4K, 2290, 500, -1)->Set("SeaSounds*.ogg",140,35);
  CreateObject(SE4K, 2480, 460, -1)->Set("SeaSounds*.ogg",140,35);
  CreateObject(SE4K, 4775, 460, -1)->Set("SeaSounds*.ogg",140,35);
  CreateObject(SE4K, 5120, 420, -1)->Set("SeaSounds*.ogg",140,35);
  CreateObject(SE4K, 6660, 450, -1)->Set("SeaSounds*.ogg",140,35);
  CreateObject(SE4K, 6850, 500, -1)->Set("SeaSounds*.ogg",140,35);
  CreateObject(SE4K, 7560, 275, -1)->Set("SeaSounds*.ogg",140,35);

  //Wellen
  CreateObject(SE4K, 240, 510, -1)->Set("Wave*.ogg",105,35);
  CreateObject(SE4K, 640, 510, -1)->Set("Wave*.ogg",105,35);
  CreateObject(SE4K, 1620, 510, -1)->Set("Wave*.ogg",105,35);
  CreateObject(SE4K, 2090, 510, -1)->Set("Wave*.ogg",105,35);
  CreateObject(SE4K, 4000, 530, -1)->Set("Wave*.ogg",105,35);

  CreateObject(SE4K, 4910, 550, -1)->Set("Wave*.ogg",105,35);
  CreateObject(SE4K, 5370, 550, -1)->Set("Wave*.ogg",105,35);
  CreateObject(SE4K, 7050, 510, -1)->Set("Wave*.ogg",105,35);
  CreateObject(SE4K, 7660, 510, -1)->Set("Wave*.ogg",105,35);

  //Jungel
  CreateObject(SE4K, 3160, 290, -1)->Set("Jungle*.ogg",140,70);
  CreateObject(SE4K, 3375, 360, -1)->Set("Jungle*.ogg",140,70);
  CreateObject(SE4K, 5850, 340, -1)->Set("Jungle*.ogg",140,70);

  //Lautsprecher
  CreateObject(SE4K, 1140, 330, -1)->Set("Announce*.ogg",1500,130);
  CreateObject(SE4K, 4000, 430, -1)->Set("Announce*.ogg",1500,130);
  CreateObject(SE4K, 7215, 280, -1)->Set("Announce*.ogg",1500,130);

  //Kran
  var cran = CreateObject(CRN1, 4000, 359, -1);
  cran->Set(42);
  
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
  aSelfDefense[0] = CreateObject(SEGU, 1095, 309, -1);
    aSelfDefense[0]->Arm(MISA);
    aSelfDefense[0]->TurnOn();
    aSelfDefense[0]->SetAutoRepair(1500);
    CreateObject(CONS, 1155, 530, -1)->Set(aSelfDefense[0]);
  
  aSelfDefense[1] = CreateObject(SEGU, 2565, 379, -1);
    aSelfDefense[1]->Arm(MISA);
    aSelfDefense[1]->TurnOn();
    aSelfDefense[1]->SetAutoRepair(1500);
    CreateObject(CONS, 2800, 500, -1)->Set(aSelfDefense[1]);
  
  aSelfDefense[2] = CreateObject(SEGU, 6585, 379, -1);
    aSelfDefense[2]->Arm(MISA);
    aSelfDefense[2]->TurnOn();
    aSelfDefense[2]->SetAutoRepair(1500);
    CreateObject(CONS, 6355, 500, -1)->Set(aSelfDefense[2]);
  
  aSelfDefense[3] = CreateObject(SEGU, 7190, 479, -1);
    aSelfDefense[3]->Arm(MISA);
    aSelfDefense[3]->TurnOn();
    aSelfDefense[3]->SetAutoRepair(1500);
    CreateObject(CONS, 7260, 420, -1)->Set(aSelfDefense[3]);
}

func CreateEquipment()
{
  Log("$CreatingEquipment$");
  //Alle Objekte folglich von links oben nach rechts unten

  //Munitionskisten (Kugeln)
  var tmp = CreateObject(AMCT, 1060, 540, -1);
  tmp->SetGraphics("Normal");
  tmp->Set(ABOX);
  var tmp = CreateObject(AMCT, 7300, 430, -1);
  tmp->SetGraphics("Normal");
  tmp->Set(ABOX);

  //Projektilmunition
  PlaceSpawnpoint(ABOX, 3160, 290);
  PlaceSpawnpoint(ABOX, 5915, 515);

  //Granatenmunition
  PlaceSpawnpoint(GBOX, 4735, 455);

  //Raketen
  PlaceSpawnpoint(MIAP, 2485, 640);
  PlaceSpawnpoint(MIAP, 6600, 650);

  //Feldgranaten
  PlaceSpawnpoint(FGRN, 985, 405);
  PlaceSpawnpoint(FGRN, 7405, 355);

  //Blendgranaten
  PlaceSpawnpoint(STUN, 3550, 495);
  PlaceSpawnpoint(STUN, 5725, 545);

  //Rauchgranaten
  PlaceSpawnpoint(SGRN, 3715, 405);
  PlaceSpawnpoint(SGRN, 4415, 525);

  //Splittergranaten
  PlaceSpawnpoint(FRAG, 2940, 615);
  PlaceSpawnpoint(FRAG, 6210, 615);

  //Sturmgewehre
  PlaceSpawnpoint(ASTR, 3570, 325);
  PlaceSpawnpoint(ASTR, 5615, 465);

  //Maschinengewehr
  PlaceSpawnpoint(MNGN, 5100, 395);

  //Raketenwerfer
  PlaceSpawnpoint(RTLR, 2425, 635);
  PlaceSpawnpoint(RTLR, 6670, 645);

  //Shotguns
  PlaceSpawnpoint(PPGN, 2830, 585);
  PlaceSpawnpoint(PPGN, 6335, 615);

  //Slingshot
  PlaceSpawnpoint(SGST, 4750, 455);

  //Maschinenpistolen
  PlaceSpawnpoint(SMGN, 1015, 535);
  PlaceSpawnpoint(SMGN, 7205, 425);

  //Einsatzschilder
  PlaceSpawnpoint(RSHL, 950, 465);
  PlaceSpawnpoint(RSHL, 7570, 405);

  //Dragnin
  PlaceSpawnpoint(DGNN, 4020, 675);
  PlaceSpawnpoint(DGNN, 6050, 375);

  //Artilleriebatterien
  CreateObject(ATBY,1025,210,-1);
  CreateObject(ATBY,3640,330,-1);
  CreateObject(ATBY,5720,340,-1);
  CreateObject(ATBY,7360,310,-1);

  //Motorboote
  SetupVehicleSpawn([INFL],DIR_Right,CreateObject(VSPW,1245,520,-1),50*21,300);
  SetupVehicleSpawn([INFL],DIR_Left,CreateObject(VSPW,2195,520,-1),50*21,300);

  SetupVehicleSpawn([INFL],DIR_Right,CreateObject(VSPW,4640,560,-1),50*21,300);
  SetupVehicleSpawn([INFL],DIR_Left,CreateObject(VSPW,5565,560,-1),50*21,300);

  SetupVehicleSpawn([INFL],DIR_Left,CreateObject(VSPW,7440,520,-1),50*21,200);

  //Blackhawks
  //SetupVehicleSpawn([H_HE],DIR_Right,CreateObject(VSPW,590,410,-1),100*21,300);
  SetupVehicleSpawn([H_HE],DIR_Left,CreateObject(VSPW,4700,290,-1),100*21,300);
  //SetupVehicleSpawn([H_HE],DIR_Left,CreateObject(VSPW,7750,290,-1),100*21,300);
}

/* Bei Flaggenübernahme */

func FlagCaptured(object pPoint, int iTeam)
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

/* Alarmleuchtensteuerung */

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
   CreateObject(BRDR, 3370, 0, -1)->Set(0);
   CreateObject(BRDR, 5980, 0, -1)->Set(1);

   //Warnschilder
   CreateObject(SGNP, 3295, 370, -1);
   CreateObject(SGNP, 3415, 370, -1);
   CreateObject(SGNP, 3320, 580, -1);

   CreateObject(SGNP, 5865, 350, -1);
   CreateObject(SGNP, 5885, 510, -1);
   CreateObject(SGNP, 5950, 350, -1);
  }

  //CTF-Spielziel
  if(FindObject(GCTF))
  {
   if(aTeams[1] == true)
   {CreateFlag(1,3580,630,GetTeamColor(1));}
   if(aTeams[2] == true)
   {CreateFlag(2,6430,340,GetTeamColor(2));}

   //Grenzen
   CreateObject(BRDR, 3370, 0, -1)->Set(0);
   CreateObject(BRDR, 6870, 0, -1)->Set(1);

   //Warnschilder
   CreateObject(SGNP, 3295, 370, -1);
   CreateObject(SGNP, 3415, 370, -1);
   CreateObject(SGNP, 3320, 580, -1);

   CreateObject(SGNP, 6725, 510, -1);
   CreateObject(SGNP, 6855, 510, -1);
  }

  //OP-Spielziel
  if(FindObject(GOCC))
  {
   //SSA Besitzer setzen
   if(aTeams[1] == true)
   {aSelfDefense[0]->SetTeam(1); aSelfDefense[1]->SetTeam(1);}
   if(aTeams[2] == true)
   {aSelfDefense[2]->SetTeam(2); aSelfDefense[3]->SetTeam(2);}

   //Grenzen
   CreateObject(BRDR, 440, 0, -1)->Set(0);
   CreateObject(BRDR, 7860, 0, -1)->Set(1);

   //Flaggen
   aFlag[0] = CreateObject(OFPL,1205,390,NO_OWNER);
   aFlag[0] -> AddSpawnPoint(950,260);
   aFlag[0] -> AddSpawnPoint(985,460);
   aFlag[0] -> AddSpawnPoint(1170,530);
   if(aTeams[1] == true)
   {
    aFlag[0]->Set("$Flag1$",100,2);
    aFlag[0]->Capture(1,1);
   }
   else
   {
    aFlag[0]->Set("$Flag1$",0,2);
   }

   aFlag[1] = CreateObject(OFPL,2720,340,NO_OWNER);
   aFlag[1] -> AddSpawnPoint(2640,630);
   aFlag[1] -> AddSpawnPoint(2710,500);
   aFlag[1] -> AddSpawnPoint(2870,500);
   if(aTeams[1] == true)
   {
    aFlag[1]->Set("$Flag2$",100,2);
    aFlag[1]->Capture(1,1);
   }
   else
   {
    aFlag[1]->Set("$Flag2$",0,2);
   }

   aFlag[2] = CreateObject(OFPL,3580,630,NO_OWNER);
   aFlag[2] -> AddSpawnPoint(3395,510);
   aFlag[2] -> AddSpawnPoint(3610,400);
   aFlag[2] -> AddSpawnPoint(3640,320);
   if(aTeams[1] == true)
   {
    aFlag[2]->Set("$Flag3$",100,2);
    aFlag[2]->Capture(1,1);
   }
   else
   {
    aFlag[2]->Set("$Flag3$",0,2);
   }

   aFlag[3] = CreateObject(OFPL,4510,390,NO_OWNER);
   aFlag[3] -> AddSpawnPoint(4230,400);
   aFlag[3] -> AddSpawnPoint(4335,570);
   aFlag[3] -> AddSpawnPoint(4550,570);
   aFlag[3]->Set("$Flag4$",0,2);

   aFlag[4] = CreateObject(OFPL,5665,340,NO_OWNER);
   aFlag[4] -> AddSpawnPoint(5760,540);
   aFlag[4] -> AddSpawnPoint(5785,500);
   aFlag[4] -> AddSpawnPoint(5890,500);
   if(aTeams[2] == true)
   {
    aFlag[4]->Set("$Flag5$",100,2);
    aFlag[4]->Capture(2,1);
   }
   else
   {
    aFlag[4]->Set("$Flag5$",0,2);
   }

   aFlag[5] = CreateObject(OFPL,6430,340,NO_OWNER);
   aFlag[5] -> AddSpawnPoint(6260,610);
   aFlag[5] -> AddSpawnPoint(6440,500);
   aFlag[5] -> AddSpawnPoint(6510,630);
   if(aTeams[2] == true)
   {
    aFlag[5]->Set("$Flag6$",100,2);
    aFlag[5]->Capture(2,1);
   }
   else
   {
    aFlag[5]->Set("$Flag6$",0,2);
   }

   aFlag[6] = CreateObject(OFPL,7260,360,NO_OWNER);
   aFlag[6] -> AddSpawnPoint(7500,475);
   aFlag[6] -> AddSpawnPoint(7580,500);
   aFlag[6] -> AddSpawnPoint(7620,310);
   if(aTeams[2] == true)
   {
    aFlag[6]->Set("$Flag7$",100,2);
    aFlag[6]->Capture(2,1);
   }
   else
   {
    aFlag[6]->Set("$Flag7$",0,2);
   }

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
     { iX = 3600; iY = 400; }
    if(!--rand)
     { iX = 3675; iY = 320; }
   }
   if(iTeam == 2)
   {
    var rand = Random(2);
    if(!rand)
     { iX = 5700; iY = 330; }
    if(!--rand)
     { iX = 5785; iY = 500; }
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
     { iX = 3600; iY = 400; }
    if(!--rand)
     { iX = 3675; iY = 320; }
   }
   if(iTeam == 2)
   {
    var rand = Random(2);
    if(!rand)
     { iX = 6325; iY = 500; }
    if(!--rand)
     { iX = 6460; iY = 500; }
   }
   return(1);
  }

  //Startsicht
  iX = 4690; iY = 450;
}