/*-- Babel Tower --*/

#strict
#include CSTD

static aFlag,aSelfDefense;


/* Regelvoreinstellung */

func ChooserRuleConfig()
{
  //Standardregelsatz: Kein FriendlyFire, Waffen bleiben, Arena, Klassenwahl
  return [NOFF,WPST,NODR,MCSL];
}

/* Initalisierung */

func Initialize()
{
  //Starttitel und Musikliste zusammenstellen
  SetPlayList("CMC_*.ogg");
  Music("CMC_Friendly Unit.ogg");
  //Bildschirmfärbung
  SetGamma(RGB(15,15,15),RGB(118,118,118),RGB(215,215,215));
  //Himmel
  SetSkyParallax(0,15,15);
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
  CreateObject(LFTP, 415, 900, -1);
  CreateObject(LFTP, 755, 1250, -1);
  CreateObject(LFTP, 585, 1650, -1);

  //Leitern
  CreateObject(LADR, 566, 285, -1)->Set(14);
  CreateObject(LADR, 606, 285, -1)->Set(14);
  CreateObject(LADR, 440, 1578, -1)->Set(10);
  CreateObject(LADR, 730, 1578, -1)->Set(10);
  CreateObject(LADR, 445, 1905, -1)->Set(12);
  CreateObject(LADR, 728, 1905, -1)->Set(12);

  //Bodenlucken
  CreateObject(HA4K, 540, 873, -1);
  CreateObject(HA4K, 630, 873, -1);
  CreateObject(H24K, 415, 1378, -1);
  CreateObject(H24K, 755, 1378, -1);
  CreateObject(HA4K, 440, 1493, -1);
  CreateObject(HA4K, 730, 1493, -1);

  //Orientierungslichter
  CreateObject(OLGH, 585, 760, -1)->Set(3, 15, 1, 1, 40);
  CreateObject(OLGH, 585, 1400, -1)->Set(5, 15, 1, 1, 40);
  CreateObject(OLGH, 585, 1830, -1)->Set(2, 15, 1, 1, 40);

  //Rampen
  DrawMaterialQuad("Wall-Concrete3",500,1940,530,1930,530,1940,515,1940,true);
  DrawMaterialQuad("Wall-Concrete3",530,1930,560,1920,560,1930,545,1930,true);

  DrawMaterialQuad("Wall-Concrete3",641,1930,611,1920,611,1930,626,1930,true);
  DrawMaterialQuad("Wall-Concrete3",671,1940,641,1930,641,1940,656,1940,true);

  //Sprungschanzen
  CreateObject (JMPD, 300, 920, -1)->Set(100, 10);
  CreateObject (JMPD, 870, 920, -1)->Set(100, -10);

  CreateObject (JMPD, 240, 2090, -1)->Set(100, 10);
  CreateObject (JMPD, 930, 2090, -1)->Set(100, -10);

  //Glasscheiben
  //Links
  CreateObject(_WIN, 372, 800, -1);
  CreateObject(_WIN, 372, 950, -1);
  CreateObject(_WIN, 372, 1000, -1);
  CreateObject(_WIN, 372, 1100, -1);
  CreateObject(_WIN, 372, 1150, -1);
  CreateObject(_WIN, 372, 1320, -1);
  CreateObject(_WIN, 372, 1370, -1);
  CreateObject(_WIN, 372, 1440, -1);
  CreateObject(_WIN, 372, 1490, -1);
  CreateObject(_WIN, 372, 1690, -1);
  CreateObject(_WIN, 372, 1750, -1);
  CreateObject(_WIN, 372, 1800, -1);
  CreateObject(_WIN, 372, 1850, -1);

  //Rechts
  CreateObject(_WIN, 798, 800, -1);
  CreateObject(_WIN, 798, 950, -1);
  CreateObject(_WIN, 798, 1000, -1);
  CreateObject(_WIN, 798, 1100, -1);
  CreateObject(_WIN, 798, 1150, -1);
  CreateObject(_WIN, 798, 1320, -1);
  CreateObject(_WIN, 798, 1370, -1);
  CreateObject(_WIN, 798, 1440, -1);
  CreateObject(_WIN, 798, 1490, -1);
  CreateObject(_WIN, 798, 1690, -1);
  CreateObject(_WIN, 798, 1750, -1);
  CreateObject(_WIN, 798, 1800, -1);
  CreateObject(_WIN, 798, 1850, -1);

  //Metallkiste
  CreateObject(MWCR, 515, 660, -1)->AutoRespawn();

  //Kisten
  CreateObject(WCR2, 655, 870, -1)->AutoRespawn();
  CreateObject(WCR2, 585, 1120, -1)->AutoRespawn();
  CreateObject(WCR2, 605, 1370, -1)->AutoRespawn();
  CreateObject(WCR2, 775, 1490, -1)->AutoRespawn();
  CreateObject(WCR2, 470, 1690, -1)->AutoRespawn();

  //Explosive Kisten
  CreateObject(XWCR, 620, 320, -1)->AutoRespawn();
  CreateObject(XWCR, 530, 980, -1)->AutoRespawn();
  CreateObject(XWCR, 520, 1250, -1)->AutoRespawn();
  CreateObject(XWCR, 790, 1940, -1)->AutoRespawn();

  //Benzinfässer
  CreateObject(PBRL, 640, 720, -1)->AutoRespawn();

  CreateObject(PBRL, 625, 1120, -1)->AutoRespawn();
  CreateObject(PBRL, 635, 1120, -1)->AutoRespawn();

  CreateObject(PBRL, 400, 1610, -1)->AutoRespawn();

  //Explosivfass
  CreateObject(XBRL, 560, 1370, -1)->AutoRespawn();
  CreateObject(XBRL, 490, 1690, -1)->AutoRespawn();

  //Satellitenschüsseln
  CreateObject(RADR, 460, 320, -1);
  CreateObject(RADR, 710, 320, -1);

  //Dekoschleuse
  CreateObject(GAT1, 585, 1050, -1);

  //Flutlichter
  CreateObject(FLGH, 460, 320, -1)->SetRotation(65);
  CreateObject(FLGH, 710, 320, -1);
  CreateObject(FLGH, 285, 470, -1)->SetRotation(65);
  CreateObject(FLGH, 885, 470, -1)->SetRotation(-65);

  //Glühbirne
  CreateObject(LBGH, 585, 900, -1);

  //Wandlampe
  CreateObject(BLGH, 585, 1320, -1);

  //Laborlichter
  CreateObject(LLGH, 540, 810, -1);
  CreateObject(LLGH, 630, 810, -1);
  CreateObject(LLGH, 530, 1460, -1);
  CreateObject(LLGH, 640, 1460, -1);
  CreateObject(LLGH, 515, 1740, -1);
  CreateObject(LLGH, 655, 1740, -1);
  CreateObject(LLGH, 515, 1880, -1);
  CreateObject(LLGH, 655, 1880, -1);

  //Deckenlichter
  CreateObject(CLGH, 585, 175, -1);
  CreateObject(CLGH, 585, 345, -1);
  CreateObject(CLGH, 585, 435, -1);
  CreateObject(CLGH, 585, 525, -1);
  CreateObject(CLGH, 585, 615, -1);

  //Waschbecken
  CreateObject(WSHB, 510, 980, -1);
  CreateObject(WSHB, 540, 980, -1);
  CreateObject(WSHB, 630, 980, -1);
  CreateObject(WSHB, 660, 980, -1);

  //Spinde
  CreateObject(LCKR, 400, 810, -1);
  CreateObject(LCKR, 760, 810, -1);
  CreateObject(LCKR, 510, 1610, -1);
  CreateObject(LCKR, 650, 1610, -1);

  //Panel
  CreateObject(CPP1, 585, 1370, -1);

  //Pflanzen
  CreateObject(PLNT, 375, 700, -1);
  CreateObject(PLNT, 795, 700, -1);
  CreateObject(PLNT, 380, 950, -1);
  CreateObject(PLNT, 790, 950, -1);
  CreateObject(PLNT, 380, 1100, -1);
  CreateObject(PLNT, 790, 1100, -1);
  CreateObject(PLNT, 380, 1250, -1);
  CreateObject(PLNT, 790, 1250, -1);
  CreateObject(PLNT, 390, 1690, -1);
  CreateObject(PLNT, 390, 1800, -1);
  CreateObject(PLNT, 785, 1690, -1);
  CreateObject(PLNT, 785, 1800, -1);
  CreateObject(PLNT, 640, 1675, -1);

  //Apparaturen
  CreateObject(GADG, 565, 870, -1)->Set(1);
  CreateObject(GADG, 605, 870, -1)->Set(1);
  CreateObject(GADG, 435, 1370, -1)->Set(3);
  CreateObject(GADG, 735, 1370, -1)->Set(3);

  //Geländer
  CreateObject(RAI1, 390, 720, -1)->SetRail([1,1]);
  CreateObject(RAI1, 740, 720, -1)->SetRail([1,1]);
  CreateObject(RAI1, 370, 1610, -1)->SetRail([1,1,1]);
  CreateObject(RAI1, 740, 1610, -1)->SetRail([1,1,1]);

  //Automaten
  CreateObject(CLVM, 500, 870, -1);
  CreateObject(CLVM, 487, 1800, -1);

  //Lüftungsgitter
  CreateObject(ENGT, 585, 1370, -1);

  //Bildschirme
  CreateObject(SCA2, 585, 810, -1);
  CreateObject(SCR3, 530, 840, -1);
  CreateObject(SCR3, 640, 840, -1);
  CreateObject(SCA2, 585, 1490, -1);
  CreateObject(SCR3, 530, 1900, -1);
  CreateObject(SCR3, 640, 1900, -1);

  //Topfpflanzen
  CreateObject(PLT3, 585, 870, -1);
  CreateObject(PLT2, 520, 1120, -1);
  CreateObject(PLT2, 650, 1120, -1);
  CreateObject(PLT3, 415, 1610, -1);
  CreateObject(PLT3, 755, 1610, -1);
  CreateObject(PLT2, 400, 1940, -1);
  CreateObject(PLT2, 770, 1940, -1);

  //Regale
  CreateObject(FRAM, 745, 810, -1);
  CreateObject(FRAM, 475, 870, -1);
  CreateObject(FRAM, 605, 1120, -1);
  CreateObject(FRAM, 625, 1120, -1);
  CreateObject(FRAM, 390, 1490, -1);
  CreateObject(FRAM, 410, 1490, -1);
  CreateObject(FRAM, 430, 1490, -1);
  CreateObject(FRAM, 685, 1800, -1);
  CreateObject(FRAM, 705, 1800, -1);

  //Gasflaschen
  CreateObject(GSBO, 495, 660, -1)->AutoRespawn();
  CreateObject(GSBO, 575, 1370, -1)->AutoRespawn();
  CreateObject(GSBO, 585, 1370, -1)->AutoRespawn();
  CreateObject(GSBO, 465, 1800, -1)->AutoRespawn();

  //Tische
  CreateObject(GTBL, 585, 1560, -1);
  CreateObject(GTBL, 530, 1690, -1);
  CreateObject(GTBL, 640, 1690, -1);

  //Schilder
  CreateObject(ESGN, 440, 800, -1);
  CreateObject(ESGN, 730, 800, -1);
  CreateObject(ESGN, 490, 1235, -1);
  CreateObject(ESGN, 680, 1235, -1);

  //Ventillatoren
  CreateObject(VENT, 500, 1670, -1)->SetCon(20);
  CreateObject(VENT, 670, 1670, -1)->SetCon(20);

  //Einfache Türen
  CreateObject(LBDR, 450, 870, -1);
  CreateObject(LBDR, 720, 870, -1);
  CreateObject(LBDR, 450, 980, -1);
  CreateObject(LBDR, 720, 980, -1);
  CreateObject(LBDR, 475, 1490, -1);
  CreateObject(LBDR, 695, 1490, -1);
  CreateObject(LBDR, 475, 1610, -1);
  CreateObject(LBDR, 695, 1610, -1);

  //Tore und Konsolen
  var autod = CreateObject (HNG2, 450, 1250, -1);
  autod->Close();
  CreateObject(CONS, 560, 1245, -1)->Set(autod);
  
  autod = CreateObject (HNG2, 720, 1250, -1);
  autod->Open();
  CreateObject(CONS, 610, 1245, -1)->Set(autod);
  
  autod = CreateObject (HNG2, 450, 1370, -1);
  autod->Open();
  CreateObject(CONS, 385, 1365, -1)->Set(autod);
  
  autod = CreateObject (HNG2, 720, 1370, -1);
  autod->Close();
  CreateObject(CONS, 780, 1365, -1)->Set(autod);
  
  autod = CreateObject (HNG2, 460, 1800, -1);
  autod->Close();
  CreateObject(CONS, 545, 1795, -1)->Set(autod);
  
  autod = CreateObject (HNG2, 460, 1940, -1);
  autod->Open();
  CreateObject(CONS, 490, 1935, -1)->Set(autod);
  
  autod = CreateObject (HNG2, 710, 1800, -1);
  autod->Close();
  CreateObject(CONS, 625, 1795, -1)->Set(autod);
  
  autod = CreateObject (HNG2, 710, 1940, -1);
  autod->Open();
  CreateObject(CONS, 680, 1935, -1)->Set(autod);

  //Hangartore und Konsolen
  autod = CreateObject (HNG1, 270, 1950, -1);
  autod->Open();
  CreateObject(CONS, 135, 1935, -1)->Set(autod);
  
  autod = CreateObject (HNG1, 900, 1950, -1);
  autod->Open();
  CreateObject (CONS, 1025, 1935, -1)->Set(autod);
  
  //Selbstschussanlagen und Konsolen
  //Basis 2
  aSelfDefense[0] = CreateObject (SEGU, 465, 698, -1);
  aSelfDefense[0]->Arm(MGSA);
  aSelfDefense[0]->SetAutoRepair(525);
  CreateObject (CONS, 465, 645, -1)->Set(aSelfDefense[0]);

  aSelfDefense[1] = CreateObject (SEGU, 705, 698, -1);
  aSelfDefense[1]->Arm(MGSA);
  aSelfDefense[1]->SetAutoRepair(525);
  CreateObject (CONS, 705, 645, -1)->Set(aSelfDefense[1]);

  //Basis 4
  aSelfDefense[2] = CreateObject (SEGU, 480, 1318, -1);
  aSelfDefense[2]->Arm(MGSA);
  aSelfDefense[2]->SetAutoRepair(525);
  CreateObject (CONS, 530, 1360, -1)->Set(aSelfDefense[2]);

  aSelfDefense[3] = CreateObject (SEGU, 690, 1318, -1);
  aSelfDefense[3]->Arm(MGSA);
  aSelfDefense[3]->SetAutoRepair(525);
  CreateObject (CONS, 640, 1360, -1)->Set(aSelfDefense[3]);

  //Sounds

  //Wind
  CreateObject(SE4K, 585, 150, -1)->Set("WindSound*.ogg",245,105);
  CreateObject(SE4K, 265, 520, -1)->Set("WindSound*.ogg",245,105);
  CreateObject(SE4K, 900, 520, -1)->Set("WindSound*.ogg",245,105);
  CreateObject(SE4K, 145, 870, -1)->Set("WindSound*.ogg",245,105);
  CreateObject(SE4K, 1025, 870, -1)->Set("WindSound*.ogg",245,105);
  CreateObject(SE4K, 225, 1325, -1)->Set("WindSound*.ogg",245,105);
  CreateObject(SE4K, 945, 1325, -1)->Set("WindSound*.ogg",245,105);
  CreateObject(SE4K, 165, 1855, -1)->Set("WindSound*.ogg",245,105);
  CreateObject(SE4K, 1005, 1855, -1)->Set("WindSound*.ogg",245,105);
  CreateObject(SE4K, 240, 2110, -1)->Set("WindSound*.ogg",245,105);
  CreateObject(SE4K, 930, 2110, -1)->Set("WindSound*.ogg",245,105);

  //Rush Hour
  CreateObject(SE4K, 225, 2090, -1)->Set("Traffic*.ogg",245,70);
  CreateObject(SE4K, 945, 2090, -1)->Set("Traffic*.ogg",245,70);

  //Sirenen
  CreateObject(SE4K, 20, 2090, -1)->Set("Siren*.ogg",280,140);
  CreateObject(SE4K, 1150, 2090, -1)->Set("Siren*.ogg",280,140);
}

func CreateEquipment()
{
  Log("$CreatingEquipment$");
  //Alle Objekte folglich von links oben nach rechts unten

  //Munitionskisten (Kugeln)
  var tmp = CreateObject(AMCT, 570, 320, -1);
  tmp->SetGraphics("Normal");
  tmp->Set(ABOX);
  var tmp = CreateObject(AMCT, 730, 1940, -1);
  tmp->SetGraphics("Normal");
  tmp->Set(ABOX);

  //Projektilmunition
  PlaceSpawnpoint(ABOX, 740, 800);
  PlaceSpawnpoint(ABOX, 390, 1360);

  //Granatmunition
  PlaceSpawnpoint(GBOX, 300, 465);
  PlaceSpawnpoint(GBOX, 930, 1785);

  //Raketen
  PlaceSpawnpoint(MIAP, 585, 490);
  PlaceSpawnpoint(MIAP, 585, 1550);

  //Feldgranaten
  PlaceSpawnpoint(FGRN, 585, 405);
  PlaceSpawnpoint(FGRN, 685, 1795);

  //Blendgranaten
  PlaceSpawnpoint(STUN, 585, 940);
  PlaceSpawnpoint(STUN, 775, 1605);

  //Splittergranaten
  PlaceSpawnpoint(FRAG, 145, 1345);
  PlaceSpawnpoint(FRAG, 1025, 1345);

  //Sturmgewehre
  PlaceSpawnpoint(ASTR, 585, 585);
  PlaceSpawnpoint(ASTR, 485, 1795);

  //Slingshots
  PlaceSpawnpoint(SGST, 270, 465);
  PlaceSpawnpoint(SGST, 945, 1785);

  //Shotgun
  PlaceSpawnpoint(PPGN, 300, 1115);

  //Maschinengewehr
  PlaceSpawnpoint(MNGN, 870, 1115);

  //Raketenwerfer
  PlaceSpawnpoint(RTLR, 560, 495);
  PlaceSpawnpoint(RTLR, 610, 1555);

  //Sprengfallen
  PlaceSpawnpoint(BBTP, 415, 805);
  PlaceSpawnpoint(BBTP, 695, 1685);
}

/* Besitznahme */

//Wenn eine Flagge übernommen wird...
func PointCaptured(object pPoint, int iTeam)
{
  if(pPoint == aFlag[1])
  {
    if(aSelfDefense[0])
      aSelfDefense[0]->SetTeam(iTeam);
      
    if(aSelfDefense[1])
      aSelfDefense[1]->SetTeam(iTeam);
  }
  
  if(pPoint == aFlag[3])
  {
    if(aSelfDefense[2])
      aSelfDefense[2]->SetTeam(iTeam);
      
    if(aSelfDefense[3])
      aSelfDefense[3]->SetTeam(iTeam);
  }
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

/* Jetpack bei Ausrüstung */

public func OnClassSelection(object pClonk)
{
  CreateContents(JTPK, pClonk)->Activate(pClonk);
}

/* Regelwähler */

public func ChooserFinished()
{
  inherited();

  //Teams abfragen
  var aTeams = [false,false,false,false,false];
  for(var i = 0; i < GetPlayerCount(); i++)
    aTeams[GetPlayerTeam(GetPlayerByIndex(i))] = true;

  //OP-Spielziel
  if(FindObject(GOCC))
  {
   //Flaggen
   aFlag[0] = CreateObject(OFPL,585,160,NO_OWNER);
   if(aTeams[1] == true)
   {
    aFlag[0]->Set("$Flag1$",100,2);
    aFlag[0]->Capture(1,1);
   }
   else
   {
    aFlag[0]->Set("$Flag1$",0,2);
   }

   aFlag[1] = CreateObject(OFPL,585,720,NO_OWNER);
   if(aTeams[1] == true)
   {
    aFlag[1]->Set("$Flag2$",100,2);
    aFlag[1]->Capture(1,1);
   }
   else
   {
    aFlag[1]->Set("$Flag2$",0,2);
   }

   aFlag[2] = CreateObject(OFPL,145,880,NO_OWNER);
    aFlag[2]->Set("$Flag3$",0,2);

   aFlag[3] = CreateObject(OFPL,585,1250,NO_OWNER);
    aFlag[3]->Set("$Flag4$",0,2);

   aFlag[4] = CreateObject(OFPL,1025,1440,NO_OWNER);
   if(aTeams[2] == true)
   {
    aFlag[4]->Set("$Flag5$",100,2);
    aFlag[4]->Capture(2,1);
   }
   else
   {
    aFlag[0]->Set("$Flag5$",0,2);
   }

   aFlag[5] = CreateObject(OFPL,145,1940,NO_OWNER);
   if(aTeams[2] == true)
   {
    aFlag[5]->Set("$Flag6$",100,2);
    aFlag[5]->Capture(2,1);
   }
   else
   {
    aFlag[5]->Set("$Flag6$",0,2);
   }

   aFlag[6] = CreateObject(OFPL,585,1920,NO_OWNER);
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
   var warn = CreateObject (ALGH, 525, 320, -1);
    AddWarnEffect(warn,aFlag[0]);
   warn = CreateObject (ALGH, 645, 320, -1);
    AddWarnEffect(warn,aFlag[0]);

   //Basis 2
   warn = CreateObject (ALGH, 380, 700, -1);
    AddWarnEffect(warn,aFlag[1]);
   warn = CreateObject (ALGH, 790, 700, -1);
    AddWarnEffect(warn,aFlag[1]);

   //Basis 4
   warn = CreateObject (ALGH, 490, 1189, -1);
    warn->SetR(-180);
    AddWarnEffect(warn,aFlag[3]);
   warn = CreateObject (ALGH, 680, 1189, -1);
    warn->SetR(-180);
    AddWarnEffect(warn,aFlag[3]);

   //Basis 7
   warn = CreateObject (ALGH, 380, 1889, -1);
    warn->SetR(-180);
    AddWarnEffect(warn,aFlag[6]);
   warn = CreateObject (ALGH, 790, 1889, -1);
    warn->SetR(-180);
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
     { iX = 460; iY = 310; }
    if(!--rand)
     { iX = 710; iY = 310; }
    return(1);
   }
   if(iTeam == 2)
   {
    var rand = Random(2);
    if(!rand)
     { iX = 500; iY = 1680; }
    if(!--rand)
     { iX = 670; iY = 1680; }
    return(1);
   }
  }

  //Startsicht
  iX = 585; iY = 1310;
}