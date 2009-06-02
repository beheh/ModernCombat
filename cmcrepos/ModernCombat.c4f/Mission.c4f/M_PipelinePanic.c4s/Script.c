/*-- Pipeline Panic --*/

#strict
#include CSTD

static aGate;

func Initialize()
{
  //Szenario einrichten
  CreateFurniture();
  //Musiktitel, welches für dieses Szenario bestimmt ist, zuerst abspielen
  Music("Mechanize.ogg");
  return(1);
}

func CreateFurniture()
{
  Log("$CreatingFurniture$");
  //Alle Objekte folglich von links oben nach rechts unten

  //Bonusspawnpunkt
  PlaceBonusSpawnpoint([AMPB,HSTB,AEXB,AIMB], 175, 325);
  PlaceBonusSpawnpoint([AMPB,HSTB,AEXB,AIMB], 3865, 325);

  //Munitionskisten (Kugeln)
  CreateObject(ABOX, 1880, 280, -1)->Set(AMOC);
  CreateObject(ABOX, 2160, 280, -1)->Set(AMOC);

  //Munitionspawnpoints (klein)
  PlaceSpawnpoint(AMOC, 175, 200);
  PlaceSpawnpoint(AMOC, 3865, 200);

  //Feldgranatenspawnpoints
  PlaceSpawnpoint(FGRN, 450, 330);
  PlaceSpawnpoint(FGRN, 3585, 330);

  //Raketenspawnpoints
  PlaceSpawnpoint(MIAP, 1235, 300);
  PlaceSpawnpoint(MIAP, 2805, 300);

  //Berettaspawnpoints
  PlaceSpawnpoint(92FS, 1750, 200);
  PlaceSpawnpoint(92FS, 2290, 200);

  //M16-Spawnpoints
  PlaceSpawnpoint(M16A, 1540, 255);
  PlaceSpawnpoint(M16A, 2500, 255);

  //Spas12-Spawnpoints
  PlaceSpawnpoint(SPAS, 1095, 150);
  PlaceSpawnpoint(SPAS, 2945, 150);

  //PzF-Spawnpoint
  PlaceSpawnpoint(PZFW, 690, 215);
  PlaceSpawnpoint(PZFW, 3345, 215);

  //Aufzüge
  CreateObject(LFTP, 1095, 225, -1);
  CreateObject(LFTP, 1815, 225, -1);

  CreateObject(LFTP, 2225, 225, -1);
  CreateObject(LFTP, 2945, 225, -1);

  //Leitern
  CreateObject(LADR, 370, 305, -1)->Set(11);
  CreateObject(LADR, 540, 305, -1)->Set(11);
  CreateObject(LADR, 820, 305, -1)->Set(11);
  CreateObject(LADR, 1370, 305, -1)->Set(11);
  CreateObject(LADR, 1540, 305, -1)->Set(11);
  CreateObject(LADR, 2500, 305, -1)->Set(11);
  CreateObject(LADR, 2670, 305, -1)->Set(11);
  CreateObject(LADR, 3220, 305, -1)->Set(11);
  CreateObject(LADR, 3500, 305, -1)->Set(11);
  CreateObject(LADR, 3670, 305, -1)->Set(11);

  //Bodenlucken
  CreateObject(HA4K, 370, 213, -1);
  CreateObject(HA4K, 540, 213, -1);
  CreateObject(HA4K, 820, 213, -1);
  CreateObject(HA4K, 1370, 213, -1);
  CreateObject(HA4K, 1540, 213, -1);

  CreateObject(HA4K, 2500, 213, -1);
  CreateObject(HA4K, 2670, 213, -1);
  CreateObject(HA4K, 3220, 213, -1);
  CreateObject(HA4K, 3500, 213, -1);
  CreateObject(HA4K, 3670, 213, -1);

  //Rampen
  DrawMaterialQuad("Wall-Concrete2",251,340,221,330,221,340,236,340,true);
  DrawMaterialQuad("Wall-Concrete2",621,220,591,210,591,220,606,220,true);
  DrawMaterialQuad("Wall-Concrete2",770,220,800,210,800,220,785,220,true);

  DrawMaterialQuad("Wall-Concrete2",3271,220,3241,210,3241,220,3256,220,true);
  DrawMaterialQuad("Wall-Concrete2",3420,220,3450,210,3450,220,3435,220,true);
  DrawMaterialQuad("Wall-Concrete2",3790,340,3820,330,3820,340,3805,340,true);

  //Rohre
  CreateObject(PIPL, 265, 365, -1)->Up()->Left()->Left()->Left()->Left()->Left()->Left();
  CreateObject(PIPL, 240, 315, -1)->Up();
  CreateObject(PIPL, 505, 235, -1)->Up()->Up();
  CreateObject(PIPL, 505, 365, -1)->Up()->Up();
  CreateObject(PIPL, 1390, 235, -1)->Up()->Up();
  CreateObject(PIPL, 1390, 365, -1)->Up()->Up();
  CreateObject(PIPL, 2410, 235, -1)->Up()->Right()->Right()->Right()->Right()->Right()->Up();
  CreateObject(PIPL, 2410, 365, -1)->Up()->Up();
  CreateObject(PIPL, 3040, 235, -1)->Up()->Right()->Right()->Right()->Right()->Right()->Up();
  CreateObject(PIPL, 3040, 365, -1)->Up()->Up();
  CreateObject(PIPL, 3540, 235, -1)->Up()->Left()->Left()->Left()->Up();
  CreateObject(PIPL, 3540, 365, -1)->Up()->Up();
  CreateObject(PIPL, 3760, 135, -1)->Down()->Right()->Right()->Right()->Right()->Right()->Right();

  //Brückenteile
  CreateObject(_HBR, 65, 223, -1);
  CreateObject(_HBR, 285, 223, -1);
  CreateObject(_HBR, 455, 223, -1);
  CreateObject(_HBR, 955, 223, -1);
  CreateObject(_HBR, 1235, 223, -1);
  CreateObject(_HBR, 1455, 223, -1);
  CreateObject(_HBR, 1675, 223, -1);

  CreateObject(_HBR, 2365, 223, -1);
  CreateObject(_HBR, 2585, 223, -1);
  CreateObject(_HBR, 2805, 223, -1);
  CreateObject(_HBR, 3085, 223, -1);
  CreateObject(_HBR, 3585, 223, -1);
  CreateObject(_HBR, 3755, 223, -1);
  CreateObject(_HBR, 3975, 223, -1);

  //Waffenautomaten
  SetUpStore(CreateObject (WPVM, 620, 340, -1));
  SetUpStore(CreateObject (WPVM, 1580, 340, -1));
  SetUpStore(CreateObject (WPVM, 2460, 340, -1));
  SetUpStore(CreateObject (WPVM, 3435, 340, -1));

  //Deckenlichter
  CreateObject(CLGH, 20, 115, -1);
  CreateObject(CLGH, 695, 115, -1);
  CreateObject(CLGH, 910, 115, -1);
  CreateObject(CLGH, 1280, 115, -1);
  CreateObject(CLGH, 1630, 115, -1);
  CreateObject(CLGH, 1990, 100, -1);
  CreateObject(CLGH, 2050, 100, -1);
  CreateObject(CLGH, 2410, 115, -1);
  CreateObject(CLGH, 2760, 115, -1);
  CreateObject(CLGH, 3130, 115, -1);
  CreateObject(CLGH, 3345, 115, -1);
  CreateObject(CLGH, 4020, 115, -1);

  //Ventillatoren
  CreateObject(VENT, 1095, 160, -1)->SetCon(50);
  CreateObject(VENT, 1815, 160, -1)->SetCon(50);

  CreateObject(VENT, 2225, 160, -1)->SetCon(50);
  CreateObject(VENT, 2945, 160, -1)->SetCon(50);

  //Schilder
  CreateObject(HSGN, 2020, 270, -1);

  //Lüftungsgitter
  CreateObject(ENGT, 690, 340, -1);
  CreateObject(ENGT, 2020, 340, -1);
  CreateObject(ENGT, 3345, 340, -1);

  //Stahltüren
  CreateObject(STDR, 1745, 340, -1);
  CreateObject(STDR, 1885, 340, -1);

  CreateObject(STDR, 2155, 340, -1);
  CreateObject(STDR, 2295, 340, -1);

  //Stationäre Gastanks
  CreateObject(GSTA, 450, 340, -1);
  CreateObject(GSTA, 1455, 340, -1);

  CreateObject(GSTA, 2585, 340, -1);
  CreateObject(GSTA, 3585, 340, -1);

  //Verbundene Türen
  var doorw = CreateObject(ROOM, 890, 210, -1);
  CreateObject(ROOM, 890, 340, -1)->Connect(doorw);
  var doorw = CreateObject(ROOM, 3190, 210, -1);
  CreateObject(ROOM, 3190, 340, -1)->Connect(doorw);

  //Tore und Konsolen
  var autod = CreateObject (HNG1, 690, 230, -1);
  autod->Close();
  CreateObject (CONS, 570, 205, -1)
  ->Set(autod);

  var autod = CreateObject (HNG1, 2020, 220, -1);
  autod->Close();
  CreateObject (CONS, 1900, 205, -1)
  ->Set(autod);
  CreateObject (CONS, 2140, 205, -1)
  ->Set(autod);

  var autod = CreateObject (HNG1, 3345, 230, -1);
  autod->Close();
  CreateObject (CONS, 3470, 205, -1)
  ->Set(autod);

  //Schleusen
  aGate = CreateArray();
  
  //Area A
  aGate[0] = CreateObject(GAT1, 20, 190, -1);
  aGate[1] = CreateObject(GAT1, 180, 190, -1);
  aGate[2] = CreateObject(GAT1, 370, 190, -1);

  //Area B
  aGate[3] = CreateObject(GAT1, 665, 190, -1);
  aGate[4] = CreateObject(GAT1, 695, 190, -1);
  aGate[5] = CreateObject(GAT1, 725, 190, -1);

  //Area C
  aGate[6] = CreateObject(GAT1, 1025, 190, -1);
  aGate[7] = CreateObject(GAT1, 1095, 300, -1);
  aGate[8] = CreateObject(GAT1, 1165, 190, -1);

  //Area D
  aGate[9] = CreateObject(GAT1, 1325, 190, -1);
  aGate[10] = CreateObject(GAT1, 1455, 250, -1);
  aGate[11] = CreateObject(GAT1, 1590, 190, -1);

  //Area H
  aGate[12] = CreateObject(GAT1, 2455, 190, -1);
  aGate[13] = CreateObject(GAT1, 2580, 250, -1);
  aGate[14] = CreateObject(GAT1, 2715, 190, -1);

  //Area G
  aGate[15] = CreateObject(GAT1, 2875, 190, -1);
  aGate[16] = CreateObject(GAT1, 2945, 300, -1);
  aGate[17] = CreateObject(GAT1, 3015, 190, -1);

  //Area F
  aGate[18] = CreateObject(GAT1, 3305, 190, -1);
  aGate[19] = CreateObject(GAT1, 3335, 190, -1);
  aGate[20] = CreateObject(GAT1, 3365, 190, -1);

  //Area E
  aGate[21] = CreateObject(GAT1, 3670, 190, -1);
  aGate[22] = CreateObject(GAT1, 3865, 190, -1);
  aGate[23] = CreateObject(GAT1, 4020, 190, -1);

  //Sounds

  //Hallen
  CreateObject(SE4K, 910, 250, -1)->Set("Interior*.ogg",665,105);

  CreateObject(SE4K, 3130, 250, -1)->Set("Interior*.ogg",665,105);
}

func CreateStippel()
{
  Log("$CreatingStippel$");
  //Alle Objekte folglich von links oben nach rechts unten

  //Königinnen
  CreateObject (KG5B, 110, 330, -1);
  CreateObject (KG5B, 210, 210, -1);
  CreateObject (KG5B, 3825, 210, -1);
  CreateObject (KG5B, 3930, 330, -1);

  //Stippel
  CreateObject (ST5B, 270, 340, -1);
  CreateObject (ST5B, 280, 340, -1);
  CreateObject (ST5B, 290, 340, -1);
  CreateObject (ST5B, 300, 340, -1);
  CreateObject (ST5B, 400, 340, -1);

  CreateObject (ST5B, 390, 210, -1);
  CreateObject (ST5B, 400, 210, -1);

  CreateObject (ST5B, 3625, 210, -1);
  CreateObject (ST5B, 3635, 210, -1);

  CreateObject (ST5B, 3600, 340, -1);
  CreateObject (ST5B, 3700, 340, -1);
  CreateObject (ST5B, 3710, 340, -1);
  CreateObject (ST5B, 3720, 340, -1);
  CreateObject (ST5B, 3730, 340, -1);
}

public func SetUpStore(pStore)
{
  pStore->AddWares("IsWeapon", -1);
  pStore->AddWares("IsAmmoPacket", -1);
  pStore->AddWares("IsUpgrade", -1);
  pStore->AddWares("IsEquipment", -1);
  pStore->SortWare("IsWeapon","IsAmmoPacket","IsUpgrade","IsEquipment");
}

func HasStippelSetup(){return(true);}

func StippelSetup()
{
  //bInfQueens = true;
  CreateStippel();
}

//Area A
func Area0Event(int iState)
{
  if(iState >= 100)
    if(!Random(10))
      SpawnContained(aGate[RandomX(0,2)], RndStippelID());
}

//Area B
func Area1Event(int iState)
{
  if(iState >= 100)
    if(!Random(20))
      SpawnContained(aGate[RandomX(3,5)], RndStippelID());
}

//Area C
func Area2Event(int iState)
{
  if(iState >= 100)
    if(!Random(30))
      SpawnContained(aGate[RandomX(6,8)], RndStippelID());
}

//Area D
func Area3Event(int iState)
{
  if(iState >= 100)
    if(!Random(40))
      SpawnContained(aGate[RandomX(9,11)], RndStippelID());
}

//Area Z
func Area4Event(int iState)
{
  //...
}

//Area H
func Area5Event(int iState)
{
  if(iState >= 100)
    if(!Random(40))
      SpawnContained(aGate[RandomX(12,14)], RndStippelID());
}

//Area G
func Area6Event(int iState)
{
  if(iState >= 100)
    if(!Random(30))
      SpawnContained(aGate[RandomX(15,17)], RndStippelID());
}

//Area H
func Area7Event(int iState)
{
  if(iState >= 100)
    if(!Random(20))
      SpawnContained(aGate[RandomX(18,20)], RndStippelID());
}

//Area E
func Area8Event(int iState)
{
  if(iState >= 100)
    if(!Random(10))
      SpawnContained(aGate[RandomX(21,23)], RndStippelID());
}

func AreasOvertaken()
{
  if(!Random(2))
    CreateObject(ST5B,1960,10,NO_OWNER);
  if(!Random(2))
    CreateObject(ST5B,2020,10,NO_OWNER);
  if(!Random(2))
    CreateObject(ST5B,2080,10,NO_OWNER);
}

func AreasCleared()
{
  for(var gate in aGate)
  {
    if(gate)
      if(!Random(10))
        gate->Explode(20+Random(10));
  }
  
  for(var obj in FindObjects(Find_Func("IsStippel")))
    obj->Explode(10+Random(5 ));
}

func SpawnContained(object pContainer, id idType)
{
  /*if(ObjectCount2(Find_ID(idType),Find_Distance(100)) >= 20)
    return();*/

  var pObject = CreateContents(idType,pContainer);
  if(!pObject) return();//Bäähhh!
  pObject -> SetOwner(NO_OWNER);
  SetCommand(pObject,"Exit");
  return(pObject);
}

func RndStippelID()
{
  if(!Random(10))
    return(MF5B);
  return(ST5B);
}

/* Relaunch */

public func RelaunchPosition(& iX, & iY, int iTeam)
{
  iX = LandscapeWidth()/2 + RandomX(-50,+50);
  iY = 330;
}