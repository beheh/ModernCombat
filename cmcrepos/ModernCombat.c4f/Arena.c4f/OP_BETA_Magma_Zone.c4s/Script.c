/*-- Magma Zone --*/

#strict

static aFlag;


func Initialize()
{
  //Bildschirmfärbung
  SetGamma(RGB(3,1,0),RGB(95,83,68),RGB(213,212,187));
  //Musiktitel, welches für dieses Szenario bestimmt ist, zuerst abspielen
  Music("Titanium City");
  //Szenario einrichten
  CreateFurniture();
  //Flaggen
  aFlag = [];
  return(1);
}

func CreateFurniture()
{
  Log("$CreatingFurniture$");
  //Alle Objekte folglich von links nach rechts

  //Flaggen
  var flg = CreateObject(OFPL,165,300,NO_OWNER);
  flg->Set("$Flag1$",100);
  flg->Capture(1);

  CreateObject(OFPL,780,390,NO_OWNER)->Set("$Flag2$",100,2);

  CreateObject(OFPL,1375,460,NO_OWNER)->Set("$Flag3$",100,2);

  CreateObject(OFPL,1705,330,NO_OWNER)->Set("$Flag4$",100,2);

  CreateObject(OFPL,2155,560,NO_OWNER)->Set("$Flag5$",100,2);

  flg = CreateObject(OFPL,2255,210,NO_OWNER);
  flg->Set("$Flag6$",100);
  flg->Capture(2);

  //Bonusspawnpunkt
  PlaceBonusSpawnpoint([AMPB,HSTB,AEXB], 1495, 515);

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
  CreateObject (JMPD, 1305, 300, -1)->Set(100, -25);
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

  //Gasflaschen
  CreateObject(GSBO, 800, 260, -1)->AutoRespawn();
  CreateObject(GSBO, 820, 390, -1)->AutoRespawn();
  CreateObject(GSBO, 1760, 370, -1)->AutoRespawn();

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
  var selfd = CreateObject (SEGU, 1305, 369, -1);
  selfd->Arm(MISA);
  CreateObject (CONS, 1300, 330, -1)
  ->Set(selfd);
}

func CreateEquipment()
{
  Log("$CreatingEquipment$");
  //Alle Objekte folglich von links oben nach rechts unten

  //Projektilmunition
  PlaceSpawnpoint(AMOC, 810, 255);
  PlaceSpawnpoint(AMOC, 1135, 570);
  PlaceSpawnpoint(AMOC, 1785, 205);

  //Raketen
  PlaceSpawnpoint(MIAP, 730, 505);
  PlaceSpawnpoint(MIAP, 2085, 470);
}