/*-- Komplex A --*/

#strict

func Initialize()
{
  //Bildschirmfärbung
  SetSkyAdjust(RGBa(255,255,255,128),RGB(64,196,255));
  //Szenario einrichten
  CreateFurniture();
  ScriptGo(true);
  return(1);
}

func CreateFurniture()
{
  var tmp;
  //Alle Objekte folglich von links oben nach rechts unten

  //Rampen
  DrawMaterialQuad("Wall-Stripes",220,640,250,630,250,640,235,640,true);
  DrawMaterialQuad("Wall-Stripes",250,630,280,620,280,630,265,630,true);
  DrawMaterialQuad("Wall-Stripes",280,620,310,610,310,620,295,620,true);

  DrawMaterialQuad("Wall-Stripes",440,600,470,590,470,600,455,600,true);
  DrawMaterialQuad("Wall-Stripes",410,610,440,600,440,610,425,610,true);

  DrawMaterialQuad("Wall-Concrete1",1030,290,1060,280,1060,290,1045,290,true);
  DrawMaterialQuad("Wall-Concrete1",1060,280,1090,270,1090,280,1075,280,true);

  DrawMaterialQuad("Wall-Concrete1",1311,280,1281,270,1281,280,1296,280,true);
  DrawMaterialQuad("Wall-Concrete1",1341,290,1311,280,1311,290,1326,290,true);

  DrawMaterialQuad("Wall-Concrete1",1360,290,1390,280,1390,290,1375,290,true);

  DrawMaterialQuad("Wall-Concrete1",1540,420,1570,410,1570,420,1555,420,true);
  DrawMaterialQuad("Wall-Concrete1",1621,420,1591,410,1591,420,1606,420,true);

  DrawMaterialQuad("Wall-Stripes",1980,420,2010,410,2010,420,1995,420,true);
  DrawMaterialQuad("Wall-Stripes",2010,410,2040,400,2040,410,2025,410,true);

  //Benzinfässer
  CreateObject(PBRL, 20, 640, -1);
  CreateObject(PBRL, 40, 640, -1);
  CreateObject(PBRL, 2050, 400, -1);

  //Explosive Kiste
  CreateObject(XWCR, 1190, 190, -1);

  //Aufzug
  CreateObject(LFTP, 535, 475, -1);

  //Glasscheiben
  CreateObject(_WIN, 507, 325, -1);
  CreateObject(_WIN, 507, 354, -1);
  CreateObject(_WIN, 507, 383, -1);
  CreateObject(_WIN, 507, 412, -1);
  CreateObject(_WIN, 507, 441, -1);
  CreateObject(_WIN, 507, 470, -1);

  CreateObject(_WIN, 563, 325, -1);
  CreateObject(_WIN, 563, 354, -1);
  CreateObject(_WIN, 563, 383, -1);
  CreateObject(_WIN, 563, 412, -1);
  CreateObject(_WIN, 563, 441, -1);
  CreateObject(_WIN, 563, 470, -1);

  CreateObject(_WIN, 597, 561, -1);
  CreateObject(_WIN, 597, 590, -1);

  //Leitern
  CreateObject(LADR, 1350, 420, -1)->Set(15);
  CreateObject(LADR, 1155, 680, -1)->Set(20);

  //Kisten
  CreateObject(WCR2, 460, 280, -1);
  CreateObject(WCR2, 490, 280, -1);

  //Gasflasche
  CreateObject(GSBO, 570, 280, -1);

  //Munitionskiste (Kugeln)
  CreateObject(ABOX, 580, 590, -1)->Set(AMOC);

  //Labortische
  CreateObject(LTBL, 335, 610, -1);
  CreateObject(LTBL, 930, 210, -1);
  CreateObject(LTBL, 1400, 190, -1);

  //Monitore
  CreateObject(MONI, 340, 598, -1);
  CreateObject(MONI, 925, 197, -1);
  CreateObject(MONI, 1395, 177, -1);
  CreateObject(MONI, 1415, 177, -1);

  //Apparaturen
  CreateObject(GADG, 1895, 420, -1)->Set(1);
  CreateObject(GADG, 1910, 420, -1)->Set(1);
  CreateObject(GADG, 1925, 420, -1)->Set(1);

  //Satellitenschüssel
  CreateObject(SADH, 395, 470, -1);

  //Flutlichter
  CreateObject(FLGH, 90, 440, -1)->SetRotation(20);
  CreateObject(FLGH, 585, 470, -1)->SetRotation(65);
  CreateObject(FLGH, 675, 630, -1)->SetRotation(85);
  CreateObject(FLGH, 1225, 420, -1)->SetRotation(90);
  CreateObject(FLGH, 1960, 420, -1)->SetRotation(-45);
  CreateObject(FLGH, 2155, 240, -1)->SetRotation(-20);

  //Radarbildschirm
  CreateObject(SCA1, 535, 420, -1);

  //Laborlichter
  CreateObject(LLGH, 370, 555, -1);
  CreateObject(LLGH, 420, 555, -1);
  CreateObject(LLGH, 510, 555, -1);
  CreateObject(LLGH, 565, 555, -1);
  CreateObject(LLGH, 610, 270, -1);

  //Deckenlichter
  CreateObject(CLGH, 45, 615, -1);
  CreateObject(CLGH, 2170, 375, -1);

  //Geländer
  CreateObject(RAI1, 712, 280, -1)->SetRail([1,1,1,1,1,1]);
  CreateObject(RAI1, 890, 210, -1)->SetRail([1,1,1,1]);
  CreateObject(RAI1, 1170, 190, -1)->SetRail([1,1,1,1]);
  CreateObject(RAI1, 1370, 190, -1)->SetRail([1,1,1]);
  CreateObject(RAI1, 1570, 410, -1)->SetRail([1]);

  //Markierungen
  CreateObject(MSGN, 635, 280, -1);
  CreateObject(MSGN, 715, 280, -1);
  CreateObject(MSGN, 1855, 420, -1);
  CreateObject(MSGN, 1935, 420, -1);

  //Bodenlucke
  CreateObject(HA4K, 1350, 293, -1);

  //Hydrauliktüren
  tmp = CreateObject(SLDR, 95, 640, -1);
  tmp->Lock();
  tmp->SetMaxDamage(-1);
  tmp = CreateObject(SLDR, 2145, 400, -1);
  tmp->Lock();
  tmp->SetMaxDamage(-1);

  //Steine
  CreateObject(STNE, 720, 650, -1);
  CreateObject(STNE, 1085, 670, -1);
  CreateObject(STNE, 1175, 435, -1);
  CreateObject(STNE, 1575, 710, -1);
  CreateObject(STNE, 1835, 560, -1);

  //Büsche
  CreateObject(BSH2, 1130, 660, -1);
  CreateObject(BSH2, 1195, 530, -1);
  CreateObject(BSH2, 1230, 600, -1);
  CreateObject(BSH2, 1385, 620, -1);
  CreateObject(BSH2, 1435, 565, -1);
  CreateObject(BSH2, 1655, 650, -1);

  //Lianen
  CreateObject(VINE, 1180, 465, -1);
  CreateObject(VINE, 1205, 635, -1);
  CreateObject(VINE, 1225, 635, -1);
  CreateObject(VINE, 1235, 530, -1);
  CreateObject(VINE, 1270, 530, -1);
  CreateObject(VINE, 1305, 520, -1);
  CreateObject(VINE, 1325, 530, -1);
  CreateObject(VINE, 1365, 480, -1);
  CreateObject(VINE, 1455, 505, -1);
  CreateObject(VINE, 1590, 540, -1);
  CreateObject(VINE, 1625, 490, -1);
  CreateObject(VINE, 1645, 570, -1);

  //Nebel
  CreateParticle("Fog",625,595,0,0,RandomX(400,1000));
  CreateParticle("Fog",690,610,0,0,RandomX(500,1500));
  CreateParticle("Fog",840,680,0,0,RandomX(1000,1500));
  CreateParticle("Fog",925,740,0,0,RandomX(1000,1500));
  CreateParticle("Fog",1040,640,0,0,RandomX(1000,1500));
  CreateParticle("Fog",1065,575,0,0,RandomX(500,1500));
  CreateParticle("Fog",1105,505,0,0,RandomX(500,1500));
  CreateParticle("Fog",1120,450,0,0,RandomX(400,1000));
  CreateParticle("Fog",1180,490,0,0,RandomX(400,1000));
  CreateParticle("Fog",1180,500,0,0,RandomX(300,900));

  //Sounds

  //Wind
  CreateObject(SE4K, 930, 180, -1)->Set("WindSound*",775,250);
  CreateObject(SE4K, 1260, 180, -1)->Set("WindSound*",775,250);

  //Vögel
  CreateObject(SE4K, 1085, 510, -1)->Set("BirdSong*",140,35);
  CreateObject(SE4K, 1615, 325, -1)->Set("BirdSong*",140,35);

  //Hallen
  CreateObject(SE4K, 130, 500, -1)->Set("Interior*",665,105);
  CreateObject(SE4K, 2100, 300, -1)->Set("Interior*",665,105);
}


/* Respawn */

protected func InitializePlayer(int iPlr, int iX, int iY, object pBase, int iTeam)
{
  for(var i=0, pCrew ; pCrew = GetCrew(iPlr, i) ; i++)
    RelaunchPlayer(iPlr, pCrew, 0, iTeam);
}

public func RelaunchPlayer(int iPlr, object pCrew, object pKiller, int iTeam)
{
  //Kein ordentlicher Spieler?
  if(GetOwner(pCrew) == NO_OWNER || iPlr == NO_OWNER || !GetPlayerName(iPlr))
    return();
  //Kein Team
  if(!iTeam) iTeam = GetPlayerTeam(iPlr);
  //Clonk tot?
  if(!GetAlive(pCrew))
    pCrew = RelaunchClonk(iPlr, pCrew);
    
  //AddSpawnEffect(pCrew, pCrew->GetColorDw());
    
  //Station für den Clonk initialisieren. :D
  GameCall(Format("Station%d",station),pCrew);
}

public func RelaunchClonk(int iPlr, object pCursor)
{
  var pClonk = CreateObject(PCMK, 10, 10, iPlr);
  if(pCursor)
    GrabObjectInfo(pCursor, pClonk);
  else
    MakeCrewMember(pClonk, iPlr);

  DoEnergy(+150, pClonk);
  SetCursor(iPlr, pClonk);
  SetPlrView(iPlr, pClonk);

  return(pClonk);
}