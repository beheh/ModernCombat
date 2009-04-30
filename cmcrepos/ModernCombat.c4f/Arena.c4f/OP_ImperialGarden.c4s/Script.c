/*-- Imperial Garden --*/

#strict
#include CSTD

static aFlag,aSelfDefense;


func Initialize()
{
  //Bildschirmfärbung
  SetGamma(RGB(2,3,10), RGB(131,135,158), RGB(252,253,255) );
  //Musiktitel, welches für dieses Szenario bestimmt ist, zuerst abspielen
  Music("Ghosthouse");
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

  //Bonusspawnpunkt
  PlaceBonusSpawnpoint([AIMB], 725, 535);

  //Rampen
  DrawMaterialQuad("Wall-Unknown1",200,200,230,190,230,200,215,200,true);
  DrawMaterialQuad("Wall-Unknown1",321,200,291,190,291,200,306,200,true);
  DrawMaterialQuad("Wall-Unknown1",440,280,470,270,470,280,455,280,true);
  DrawMaterialQuad("Wall-Unknown1",541,280,511,270,511,280,526,280,true);

  DrawMaterialQuad("Wall-Concrete1",770,280,800,270,800,280,785,280,true);
  DrawMaterialQuad("Wall-Concrete1",800,270,830,260,830,270,815,270,true);
  DrawMaterialQuad("Wall-Concrete1",921,200,951,190,951,200,936,200,true);

  DrawMaterialQuad("Wall-Concrete3",950,190,980,180,980,190,965,190,true);
  DrawMaterialQuad("Wall-Concrete1",980,180,1010,170,1010,180,995,180,true);

  //Rohre
  CreateObject(PIPL, 1155, 695, -1)->Up()->Left()->Left()->Down()->Left()->Down()->Down()->Down()->Down();
  CreateObject(PIPL, 20, 630, -1)->Solid()->Down()->Solid()->Down()->Solid()->Down()->Solid()->Down()->Solid()->Down()->Solid()->Down();
  CreateObject(PIPL, 250, 530, -1)->Down()->Right()->Down()->Down()->Down()->Down()->Down()->Down()->Down();
  CreateObject(PIPL, 0, 480, -1)->Solid()->Right()->Solid()->Right()->Solid()->Down()->Solid()->Left()->Solid()->Down()->Solid()->Down()->Solid()->Left()->Solid()->Left();
  CreateObject(PIPL, 100, 530, -1)->Right()->Up()->Solid()->Right()->Solid()->Right()->Solid()->Right()->Solid()->Down()->Solid()->Left()->Solid()->Up()->Solid()->Up()->Solid()->Up()->Solid()->Up()->Solid()->Right()->Solid()->Right()->Solid()->Right()->Solid()->Right()->Solid()->Right()->Solid()->Right()->Solid()->Right()->Solid()->Right()->Solid()->Right()->Solid();
  CreateObject(PIPL, 100, 530, -1)->Down()->Down()->Down()->Down()->Down()->Down()->Down()->Down();
  CreateObject(PIPL, 250, 330, -1)->Solid()->Left()->Left()->Left()->Solid()->Left()->Solid()->Left()->Solid()->Left()->Solid();
  CreateObject(PIPL, 200, 690, -1)->Right()->Right()->Right()->Right()->Down()->Left()->Left()->Left()->Left()->Left()->Left()->Left()->Left()->Left();
  CreateObject(PIPL, 470, 625, -1)->Solid()->Left()->Solid()->Down()->Solid()->Right()->Solid();
  CreateObject(PIPL, 190, 625, -1)->Solid()->Right()->Solid()->Down()->Solid()->Left()->Solid();
  CreateObject(PIPL, 460, 405, -1)->Solid()->Down()->Solid()->Down()->Solid()->Down()->Solid()->Left()->Solid()->Up()->Solid()->Up()->Solid()->Up()->Solid();
  CreateObject(PIPL, 190, 785, -1)->Solid()->Right()->Solid()->Down()->Solid()->Down()->Solid()->Down();
  CreateObject(PIPL, 464, 505, -1)->Solid()->Right()->Solid()->Right()->Solid();
  CreateObject(PIPL, 560, 410, -1)->Down()->Down()->Down()->Down()->Down()->Down()->Down()->Down()->Down()->Down();
  CreateObject(PIPL, 900, 505, -1)->Down()->Down()->Down()->Down()->Right()->Right()->Down()->Down()->Down()->Down()->Down();
  CreateObject(PIPL, 870, 405, -1)->Solid()->Down()->Solid()->Down()->Solid()->Right()->Solid()->Up()->Solid()->Right()->Solid()->Up()->Solid();
  CreateObject(PIPL, 610, 636, -1)->Down()->Down();
  CreateObject(PIPL, 620, 745, -1)->Solid()->Left()->Solid()->Down()->Solid()->Down()->Solid()->Down()->Solid()->Down();
  CreateObject(PIPL, 870, 735, -1)->Solid()->Up()->Solid()->Left()->Solid();
  CreateObject(PIPL, 855, 735, -1)->Solid()->Up()->Solid()->Up()->Solid()->Up()->Solid();
  CreateObject(PIPL, 950, 480, -1)->Solid()->Right()->Solid()->Down()->Solid();
  CreateObject(PIPL, 1170, 695, -1)->Solid()->Up()->Solid()->Up()->Solid()->Left()->Solid()->Left()->Solid()->Up()->Solid()->Right()->Solid()->Down()->Solid();
  CreateObject(PIPL, 1250, 810, -1)->Solid()->Left()->Solid()->Left()->Solid()->Left()->Solid()->Left()->Solid()->Down()->Solid()->Down()->Solid();
  CreateObject(PIPL, 1360, 580, -1)->Solid()->Left()->Solid()->Left()->Solid()->Up()->Solid()->Right()->Solid()->Right()->Solid()->Right()->Solid()->Right()->Solid()->Right()->Solid()->Right();
  CreateObject(PIPL, 1240, 370, -1)->Solid()->Left()->Solid()->Down()->Solid()->Down()->Solid();
  CreateObject(PIPL, 1070, 405, -1)->Down()->Right()->Right()->Right()->Right()->Right()->Right();
  CreateObject(PIPL, 1210, 807, -1)->Up()->Up()->Up()->Up()->Up();
  CreateObject(PIPL, 565, 565, -1)->Right()->Right()->Down();
  CreateObject(PIPL, 896, 565, -1)->Left()->Left()->Down();

  //Geländer
  CreateObject(RAI5, 120, 200, -1)->SetRail([1,1,1,1]);
  CreateObject(RAI5, 230, 190, -1)->SetRail([1,1,1]);
  CreateObject(RAI5, 320, 200, -1)->SetRail([1,1,1]);
  CreateObject(RAI5, 1010, 170, -1)->SetRail([1,1]);
  CreateObject(RAI5, 1200, 170, -1)->SetRail([1,1,1]);

  //Staturen
  CreateObject(STU1, 610, 280, -1);
  CreateObject(STU1, 1115, 170, -1);

  //Luftströme
  CreateObject(PS4K, 280, 870, -1)->Set(70,150,6);
  CreateObject(PS4K, 725, 870, -1)->Set(100,350,5);
  CreateObject(PS4K, 1020, 870, -1)->Set(50,125,5);

  //Leitern
  CreateObject(LADR, 125, 710, -1)->Set(30);
  CreateObject(LADR, 170, 448, -1)->Set(30);
  CreateObject(LADR, 320, 615, -1)->Set(20);
  CreateObject(LADR, 385, 280, -1)->Set(10);
  CreateObject(LADR, 385, 730, -1)->Set(30);
  CreateObject(LADR, 575, 555, -1)->Set(10);
  CreateObject(LADR, 750, 450, -1)->Set(21);
  CreateObject(LADR, 920, 670, -1)->Set(12);
  CreateObject(LADR, 1045, 690, -1)->Set(30);
  CreateObject(LADR, 1140, 480, -1)->Set(31);
  CreateObject(LADR, 1230, 315, -1)->Set(17);

  //Bodenlucken
  CreateObject(HA4K, 170, 203, -1);
  CreateObject(HA4K, 1230, 173, -1);

  //Orientierungslichter
  CreateObject(OLGH, 170, 110, -1)->Set(2, 15, 1, 1, 40);
  CreateObject(OLGH, 350, 110, -1)->Set(2, 15, 1, 1, 40);
  CreateObject(OLGH, 490, 190, -1)->Set(2, 15, 1, 1, 40);
  CreateObject(OLGH, 650, 190, -1)->Set(2, 15, 1, 1, 40);
  CreateObject(OLGH, 800, 190, -1)->Set(2, 15, 1, 1, 40);
  CreateObject(OLGH, 960, 130, -1)->Set(2, 15, 1, 1, 40);
  CreateObject(OLGH, 1130, 90, -1)->Set(4, 15, 1, 1, 40);
  CreateObject(OLGH, 1270, 90, -1)->Set(2, 15, 1, 1, 40);
  CreateObject(OLGH, 1390, 60, -1)->Set(2, 15, 1, 1, 40);

  //Sprungschanzen
  CreateObject (JMPD, 490, 270, -1)->Set(80, 40);
  CreateObject (JMPD, 1220, 782, -1)->Set(100, -5);

  //Kisten
  CreateObject(WCR2, 130, 200, -1)->AutoRespawn();
  CreateObject(WCR2, 195, 450, -1)->AutoRespawn();
  CreateObject(WCR2, 835, 590, -1)->AutoRespawn();
  CreateObject(WCR2, 1150, 670, -1)->AutoRespawn();
  CreateObject(WCR2, 1345, 502, -1)->AutoRespawn();

  //Explosive Kiste
  CreateObject(XWCR, 810, 590, -1)->AutoRespawn();

  //Explosivfässer
  CreateObject(XBRL, 210, 595, -1)->AutoRespawn();
  CreateObject(XBRL, 330, 200, -1)->AutoRespawn();
  CreateObject(XBRL, 800, 268, -1)->AutoRespawn();
  CreateObject(XBRL, 1220, 340, -1)->AutoRespawn();

  //Gasflaschen
  CreateObject(GSBO, 145, 200, -1)->AutoRespawn();
  CreateObject(GSBO, 795, 590, -1)->AutoRespawn();
  CreateObject(GSBO, 1135, 670, -1)->AutoRespawn();

  //Regale
  CreateObject(FRAM, 460, 730, -1);
  CreateObject(FRAM, 940, 710, -1);
  CreateObject(FRAM, 960, 710, -1);

  //Flutlichter
  CreateObject(FLGH, 350, 200, -1)->SetRotation(-60);
  CreateObject(FLGH, 490, 570, -1)->SetRotation(80);
  CreateObject(FLGH, 1135, 565, -1)->SetRotation(10);

  //Glühbirne
  CreateObject(LAT1, 650, 420, -1);
  CreateObject(LAT1, 800, 420, -1);

  //Markierungsschilder
  CreateObject(MSGN, 685, 590, -1);
  CreateObject(MSGN, 780, 590, -1);

  //Lüftungsgitter
  CreateObject(ENGT, 170, 570, -1);
  CreateObject(ENGT, 585, 680, -1);
  CreateObject(ENGT, 960, 570, -1);
  CreateObject(ENGT, 1110, 670, -1);
  CreateObject(ENGT, 1225, 420, -1);

  //Feuerlöcher
  CreateObject(FIEX, 650, 580, -1);
  CreateObject(FIEX, 1195, 770, -1);

  //Schilder
  CreateObject(ESGN, 630, 580, -1);
  CreateObject(ESGN, 1165, 777, -1);

  //Ventillatoren
  CreateObject(VENT, 75, 680, -1)->SetCon(35);
  CreateObject(VENT, 725, 600, -1)->SetCon(45);
  CreateObject(MVNT, 585, 720, -1)->SetCon(40);
  CreateObject(MVNT, 585, 760, -1)->SetCon(40);
  CreateObject(MVNT, 585, 800, -1)->SetCon(40);
  CreateObject(VENT, 1150, 390, -1)->SetCon(50);
  CreateObject(MVNT, 1440, 460, -1)->SetCon(40);
  CreateObject(MVNT, 1470, 460, -1)->SetCon(40);
  CreateObject(MVNT, 1500, 460, -1)->SetCon(40);
  CreateObject(MVNT, 1440, 580, -1)->SetCon(40);
  CreateObject(MVNT, 1470, 580, -1)->SetCon(40);
  CreateObject(MVNT, 1500, 580, -1)->SetCon(40);

  //Alarmlampen
  CreateObject(ALGH, 650, 190, -1)->TurnOn();
  CreateObject(ALGH, 1390, 60, -1)->TurnOn();

  //Pflanzen
  CreateObject(PLNT, 190, 200, -1);
  CreateObject(PLNT, 510, 270, -1);
  CreateObject(PLNT, 835, 240, -1);
  CreateObject(PLNT, 875, 220, -1);
  CreateObject(PLNT, 1210, 170, -1);

  //Apparaturen
  CreateObject(GADG, 405, 730, -1)->Set(1);
  CreateObject(GADG, 415, 730, -1)->Set(1);
  CreateObject(GADG, 890, 710, -1)->Set(1);
  CreateObject(GADG, 900, 710, -1)->Set(1);
  CreateObject(GADG, 910, 710, -1)->Set(1);
  CreateObject(GADG, 1270, 590, -1)->Set(1);
  CreateObject(GADG, 1280, 590, -1)->Set(1);
  CreateObject(GADG, 1290, 590, -1)->Set(1);

  //Einfache Türen
  CreateObject(LBDR, 1074, 781, -1);
  CreateObject(LBDR, 1174, 781, -1);

  //Verbundene Türen
  var doorw = CreateObject(GAT1, 170, 140, -1);
  CreateObject(ROOM, 435, 730, -1)->Connect(doorw);

  //Selbstschussanlagen
  aSelfDefense[0] = CreateObject (SEGU, 360, 420, -1);
    aSelfDefense[0]->Arm(MGSA);
    aSelfDefense[0]->SetAutoRepair(525);
    CreateObject (CONS, 275, 442, -1)->Set(aSelfDefense[0]);
  
  aSelfDefense[1] = CreateObject (SEGU, 1310, 420, -1);
    aSelfDefense[1]->Arm(MGSA);
    aSelfDefense[1]->SetAutoRepair(525);
    CreateObject (CONS, 1280, 492, -1)->Set(aSelfDefense[1]);

  //Flaggen
  aFlag[0] = CreateObject(OFPL,260,190,NO_OWNER);
    aFlag[0]->Set("$Flag1$",100);
    aFlag[0]->Capture(1);

  aFlag[1] = CreateObject(OFPL,170,570,NO_OWNER);
    aFlag[1]->Set("$Flag2$",100,2);
    aFlag[1]->Capture(1);

  aFlag[2] = CreateObject(OFPL,640,590,NO_OWNER);
    aFlag[2]->Set("$Flag3$",100,2);

  aFlag[3] = CreateObject(OFPL,1110,670,NO_OWNER);
    aFlag[3]->Set("$Flag4$",100,2);

  aFlag[4] = CreateObject(OFPL,1310,502,NO_OWNER);
    aFlag[4]->Set("$Flag5$",100,2);
    aFlag[4]->Capture(2);

  aFlag[5] = CreateObject(OFPL,1035,170,NO_OWNER);
    aFlag[5]->Set("$Flag6$",100);
    aFlag[5]->Capture(2);
  
  //Alarmleuchten
  //Basis 1
  var warn = CreateObject (ALGH, 235, 190, -1);
    AddWarnEffect(warn,aFlag[0]);
  warn = CreateObject (ALGH, 285, 190, -1);
    AddWarnEffect(warn,aFlag[0]);

  //Basis 6
  warn = CreateObject (ALGH, 915, 200, -1);
    AddWarnEffect(warn,aFlag[5]);
  warn = CreateObject (ALGH, 1055, 170, -1);
    AddWarnEffect(warn,aFlag[5]);

  //Sounds

  //Wind
  CreateObject(SE4K, 280, 775, -1)->Set("WindSound*.ogg",775,250);
  CreateObject(SE4K, 730, 775, -1)->Set("WindSound*.ogg",775,250);
  CreateObject(SE4K, 725, 775, -1)->Set("WindSound*.ogg",775,250);

  //Hallen
  CreateObject(SE4K, 90, 555, -1)->Set("Interior*.ogg",665,105);
  CreateObject(SE4K, 555, 555, -1)->Set("Interior*.ogg",665,105);
  CreateObject(SE4K, 880, 555, -1)->Set("Interior*.ogg",665,105);
  CreateObject(SE4K, 1230, 555, -1)->Set("Interior*.ogg",665,105);
}

func CreateEquipment()
{
  Log("$CreatingEquipment$");
  //Alle Objekte folglich von links oben nach rechts unten

  //Munitionskisten (Kugeln)
  CreateObject(ABOX, 70, 452, -1)->Set(AMOC);
  CreateObject(ABOX, 1180, 270, -1)->Set(AMOC);

  //Projektilmunition
  PlaceSpawnpoint(AMOC, 690, 300);

  //Raketen
  PlaceSpawnpoint(MIAP, 510, 470);
  PlaceSpawnpoint(MIAP, 995, 445);

  //Handgranaten
  PlaceSpawnpoint(FGRN, 90, 190);
  PlaceSpawnpoint(FGRN, 1270, 160);

  //Blendgranaten
  PlaceSpawnpoint(STUN, 170, 715);
  PlaceSpawnpoint(STUN, 1330, 580);

  //Rauchgranaten
  PlaceSpawnpoint(SGRN, 935, 455);

  //Splittergranaten
  PlaceSpawnpoint(FRAG, 590, 705);
  PlaceSpawnpoint(FRAG, 960, 700);

  //Predator
  PlaceSpawnpoint(PDTW, 30, 440);
  PlaceSpawnpoint(PDTW, 1205, 330);

  //PzF
  PlaceSpawnpoint(PZFW, 750, 350);

  //MP7
  PlaceSpawnpoint(MP7R, 350, 560);
  PlaceSpawnpoint(MP7R, 1090, 465);

  //P29
  PlaceSpawnpoint(P29W, 870, 640);

  //Tavor
  PlaceSpawnpoint(TAVR, 650, 150);
}

/* Besitznahme */

//Wenn eine Flagge übernommen wird...
func PointCaptured(object pPoint, int iTeam)
{
  if(pPoint == aFlag[0])
    aSelfDefense[0]->SetTeam(iTeam);
  
  if(pPoint == aFlag[5])
    aSelfDefense[1]->SetTeam(iTeam);
}

func PointCaptured(object pPoint, int iTeam)
{
  if(pPoint == aFlag[0])
    aSelfDefense[0]->SetTeam(iTeam);

  if(pPoint == aFlag[5])
    aSelfDefense[1]->SetTeam(iTeam);
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