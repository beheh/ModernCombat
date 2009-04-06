/*-- Blackout --*/

#strict
#include CSTD

func Initialize()
{
  //Lichteinstellung
  SetGamma(RGB(3,1,0),RGB(95,83,68),RGB(213,212,187));
  SetSkyParallax(0,33,57,0,0,0,0); 
  //Musiktitel, welches für dieses Szenario bestimmt ist, zuerst abspielen
  Music("Quaddamage");
  //Szenario einrichten
  CreateFurniture();
  return(1);
}

func CreateFurniture()
{
  Log("$CreatingFurniture$");
  //Alle Objekte folglich von links nach rechts

  //Munitionskisten (Kugeln)
  CreateObject(ABOX, 600, 280, -1)->Set(AMOC);
  CreateObject(ABOX, 1250, 360, -1)->Set(AMOC);
  CreateObject(ABOX, 1900, 280, -1)->Set(AMOC);

  //Munitionskisten (Raketen)
  CreateObject(ABOX, 440, 380, -1)->Set(MIAP);
  CreateObject(ABOX, 2060, 380, -1)->Set(MIAP);

  //Feldgranatenspawnpoints
  PlaceSpawnpoint(FGRN, 850, 425);
  PlaceSpawnpoint(FGRN, 1650, 425);

  //Blendgranatenspawnpoints
  PlaceSpawnpoint(STUN, 655, 430);
  PlaceSpawnpoint(STUN, 1830, 430);

  //Rauchgranatenspawnpoints
  PlaceSpawnpoint(SGRN, 330, 300);
  PlaceSpawnpoint(SGRN, 2170, 300);

  //PzF-Spawnpoints
  PlaceSpawnpoint(PZFW, 460, 80);
  PlaceSpawnpoint(PZFW, 2040, 80);

  //Spas12-Spawnpoints
  PlaceSpawnpoint(SPAS, 520, 360);
  PlaceSpawnpoint(SPAS, 1980, 360);

  //M16-Spawnpoints
  PlaceSpawnpoint(M16A, 995, 330);
  PlaceSpawnpoint(M16A, 1505, 330);

  //OICW-Spawnpoint
  PlaceSpawnpoint(OICW, 1250, 115);

  //MP7-Spawnpoint
  PlaceSpawnpoint(MP7R, 1250, 350);

  //Predator-Spawnpoints
  PlaceSpawnpoint(PDTW, 730, 270);
  PlaceSpawnpoint(PDTW, 1770, 270);

  //Brückenteile
  CreateObject(_HBR, 490, 392, -1);
  CreateObject(_HBR, 2005, 392, -1);

  //Leitern
  CreateObject(LADR, 431, 355, -1)->Set(32);
  CreateObject(LADR, 617, 250, -1)->Set(14);
  CreateObject(LADR, 767, 335, -1)->Set(6);
  CreateObject(LADR, 1025, 425, -1)->Set(11);
  CreateObject(LADR, 1058, 340, -1)->Set(8);

  CreateObject(LADR, 1445, 340, -1)->Set(8);
  CreateObject(LADR, 1477, 425, -1)->Set(11);
  CreateObject(LADR, 1737, 335, -1)->Set(6);
  CreateObject(LADR, 1885, 250, -1)->Set(14);
  CreateObject(LADR, 2071, 355, -1)->Set(32);

  //Bodenlucken
  CreateObject(HA4K, 430, 93, -1);
  CreateObject(HA4K, 490, 133, -1);
  CreateObject(HA4K, 550, 133, -1);
  CreateObject(HA4K, 610, 393, -1);

  CreateObject(HA4K, 1890, 393, -1);
  CreateObject(HA4K, 1950, 133, -1);
  CreateObject(HA4K, 2010, 133, -1);
  CreateObject(HA4K, 2070, 93, -1);

  //Einfache Türen
  CreateObject(STDR, 720, 340, -1);
  CreateObject(STDR, 1154, 210, -1);
  CreateObject(STDR, 1346, 210, -1);
  CreateObject(STDR, 1775, 340, -1);

  //Rohre
  CreateObject(PIPL, 400, 200, -1)->Left()->Left()->Left()->Left()->Left()->Left()->Left()->Left()->Left();
  CreateObject(PIPL, 400, 230, -1)->Left()->Left()->Left()->Left()->Left()->Left()->Left()->Left()->Left();

  CreateObject(PIPL, 890, 475, -1)->Up()->Up()->Up()->Up()->Right()->Right();
  CreateObject(PIPL, 1020, 120, -1)->Right()->Right()->Right()->Right()->Right()->Right()->Right()->Right()->Right();
  CreateObject(PIPL, 1610, 475, -1)->Up()->Up()->Up()->Up()->Left()->Left();

  CreateObject(PIPL, 2100, 200, -1)->Right()->Right()->Right()->Right()->Right()->Right()->Right()->Right()->Right();
  CreateObject(PIPL, 2100, 230, -1)->Right()->Right()->Right()->Right()->Right()->Right()->Right()->Right()->Right();

  //Regale
  CreateObject(FRAM, 630, 425, -1);
  CreateObject(FRAM, 650, 425, -1);
  CreateObject(FRAM, 1850, 425, -1);
  CreateObject(FRAM, 1870, 425, -1);

  //Wandlampen
  CreateObject(BLGH, 520, 103, -1)->TurnOff();
  CreateObject(BLGH, 520, 302, -1)->TurnOff();
  CreateObject(BLGH, 1015, 155, -1)->TurnOff();
  CreateObject(BLGH, 1485, 155, -1)->TurnOff();
  CreateObject(BLGH, 1980, 103, -1)->TurnOff();
  CreateObject(BLGH, 1980, 302, -1)->TurnOff();

  //Topfpflanze
  CreateObject(PLT3, 1185, 210, -1);

  //Geländer
  CreateObject(RAI1, 480, 130, -1)->SetRail([1,3,1,3,1,3,1]);
  CreateObject(RAI1, 982, 280, -1)->SetRail([1,3,1,3,1]);
  CreateObject(RAI1, 1038, 190, -1)->SetRail([1,3,1,3,1,3,1]);
  CreateObject(RAI1, 1178, 120, -1)->SetRail([1,3,1,3,1,3,1,3,1,3,1,3,1]);
  CreateObject(RAI1, 1378, 190, -1)->SetRail([1,3,1,3,1,3,1]);
  CreateObject(RAI1, 1452, 280, -1)->SetRail([1,3,1,3,1]);
  CreateObject(RAI1, 1940, 130, -1)->SetRail([1,3,1,3,1,3,1]);

  //Poster
  CreateObject(PSTR, 1195, 195, -1)->Set(2);
  CreateObject(PSTR, 1305, 195, -1)->Set(1);

  //Feuerlöcher
  CreateObject(FIEX, 460, 375, -1);
  CreateObject(FIEX, 1275, 210, -1);
  CreateObject(FIEX, 2040, 375, -1);

  //Bildschirm
  CreateObject(SCA2, 1250, 270, -1);

  //Bäume und Büsche
  CreateObject(TRE2, 310, 315, -1);
  CreateObject(TRE3, 350, 315, -1);
  CreateObject(TRE2, 390, 315, -1);

  CreateObject(BSH2, 310, 320, -1);
  CreateObject(BSH2, 350, 320, -1);
  CreateObject(BSH2, 390, 320, -1);

  CreateObject(TRE3, 630, 285, -1);
  CreateObject(TRE2, 670, 285, -1);
  CreateObject(TRE3, 710, 285, -1);

  CreateObject(BSH2, 630, 290, -1);
  CreateObject(BSH2, 670, 290, -1);
  CreateObject(BSH2, 710, 290, -1);

  CreateObject(TRE3, 1790, 285, -1);
  CreateObject(TRE2, 1830, 285, -1);
  CreateObject(TRE3, 1870, 285, -1);

  CreateObject(BSH2, 1790, 290, -1);
  CreateObject(BSH2, 1830, 290, -1);
  CreateObject(BSH2, 1870, 290, -1);

  CreateObject(TRE2, 2110, 315, -1);
  CreateObject(TRE3, 2150, 315, -1);
  CreateObject(TRE2, 2190, 315, -1);

  CreateObject(BSH2, 2110, 320, -1);
  CreateObject(BSH2, 2150, 320, -1);
  CreateObject(BSH2, 2190, 320, -1);

  ///Rampe
  DrawMaterialQuad("Wall-Bricks2",731,400,701,390,701,400,716,400,true);
  DrawMaterialQuad("Wall-Bricks2",761,410,731,400,731,410,746,410,true);
  DrawMaterialQuad("Wall-Bricks2",1100,311,1130,301,1130,311,1115,311,true);
  DrawMaterialQuad("Wall-Bricks2",1150,370,1180,360,1180,370,1165,370,true);
  DrawMaterialQuad("Wall-Bricks2",1150,130,1180,120,1180,130,1165,130,true);
  DrawMaterialQuad("Wall-Bricks2",1242,220,1212,210,1212,220,1227,220,true);

  DrawMaterialQuad("Wall-Bricks2",1260,220,1290,210,1290,220,1275,220,true);
  DrawMaterialQuad("Wall-Bricks2",1351,131,1321,121,1321,131,1336,131,true);
  DrawMaterialQuad("Wall-Bricks2",1351,370,1321,360,1321,370,1336,370,true);
  DrawMaterialQuad("Wall-Bricks2",1401,310,1371,300,1371,310,1386,310,true);
  DrawMaterialQuad("Wall-Bricks2",1740,410,1770,400,1770,410,1755,410,true);
  DrawMaterialQuad("Wall-Bricks2",1770,401,1800,391,1800,401,1785,401,true);

  //Kisten
  CreateObject(WCR2, 1190, 120, -1)->AutoRespawn();
  CreateObject(WCR2, 1300, 120, -1)->AutoRespawn();

  //Benzinfässer
  CreateObject(PBRL, 670, 340, -1)->AutoRespawn();
  CreateObject(PBRL, 1830, 340, -1)->AutoRespawn();

  //Giftfässer
  CreateObject(TBRL, 690, 280, -1)->AutoRespawn();
  CreateObject(TBRL, 1810, 280, -1)->AutoRespawn();

  //Explosivfässer
  CreateObject(XBRL, 570, 280, -1)->AutoRespawn();
  CreateObject(XBRL, 1930, 280, -1)->AutoRespawn();

  //Flutlichter
  CreateObject(FLGH, 850, 440, -1)->SetRotation(30);
  CreateObject(FLGH, 1650, 440, -1)->SetRotation(-30);

  //Glühbirnen
  CreateObject(LBGH, 330, 340, -1);
  CreateObject(LBGH, 1075, 240, -1);
  CreateObject(LBGH, 1425, 240, -1);
  CreateObject(LBGH, 2170, 340, -1);

  //CTF-Flaggen
  CreateFlag(1,175,360,GetTeamColor(1)); 
  CreateFlag(2,2325,360,GetTeamColor(2));

  //Selbstschussanlagen und Konsolen
  var selfd = CreateObject (SEGU, 585, 50, -1);
  selfd->Arm(MISA);
  selfd->SetAutoRepair(800);
  selfd->TurnOn(MISA);
  CreateObject (CONS, 465, 125, -1)
  ->Set(selfd);
  var selfd = CreateObject (SEGU, 1915, 50, -1);
  selfd->Arm(MISA);
  selfd->SetAutoRepair(800);
  selfd->TurnOn(MISA);
  CreateObject (CONS, 2040, 125, -1)
  ->Set(selfd);

  //Sounds

  //Wind
  CreateObject(SE4K, 885, 295, -1)->Set("WindSound*",775,250);
  CreateObject(SE4K, 1625, 295, -1)->Set("WindSound*",775,250);

  //Jungel
  CreateObject(SE4K, 175, 360, -1)->Set("Jungle*",140,70);
  CreateObject(SE4K, 2325, 360, -1)->Set("Jungle*",140,70);

  //Möven
  CreateObject(SE4K, 520, 10, -1)->Set("SeaSounds*",140,35);
  CreateObject(SE4K, 1980, 10, -1)->Set("SeaSounds*",140,35);
}

public func SetUpStore(pStore)
{
  pStore->AddWares("IsWeapon", -1);
  pStore->AddWares("IsAmmoPacket", -1);
  pStore->AddWares("IsUpgrade", -1);
  pStore->AddWares("IsEquipment", -1);
  pStore->SortWare("IsWeapon","IsAmmoPacket","IsUpgrade","IsEquipment");
}

public func RelaunchPosition(& iX, & iY, int iTeam)
{
  var rand = Random(2);
  if(iTeam == 1)
  {
    if(!rand)
      { iX = 300; iY = 400; }
    if(!--rand)
      { iX = 365; iY = 300; }
  }
  else
  {
    if(!rand)
      { iX = 2200; iY = 400; }
    if(!--rand)
      { iX = 2130; iY = 300; }
  }
}

public func RelaunchPosition2(& iX, & iY, int iTeam)//Wegen Domination. //TODO: Spawnpunkte anpassen. :X
{
  return(RelaunchPosition(iX,iY,iTeam));
}