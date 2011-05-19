/*-- Base 1337 --*/

#strict
#include CSTD

static aFlag,aSelfDefense;


/* Initialisierung */

func Initialize()
{
  //Wartemusik einstellen
  SetPlayList("CMC_WaitingforSpawn.ogg");
  Music("CMC_WaitingforSpawn.ogg",1);
  //Bildschirmfärbung
  SetGamma(RGB(3,1,0),RGB(95,83,68),RGB(213,212,187));
  //Himmel
  SetSkyParallax(1, 40, 40, 0, 0, SkyPar_Keep, SkyPar_Keep);
  //Flaggen
  aFlag = [];
  //Selbstschussanlagen
  aSelfDefense = [];
  //Einrichtung plazieren
  CreateInterior();
  //Ausrüstung plazieren
  CreateEquipment();
  //Dekoration plazieren
  CreateDecoration();
  //Optionale Objekte plazieren
  CreateOptionalFeatures();
  return(1);
}

/* Plazierungslisten */

func CreateInterior()
{
  Log("$CreatingInterior$");

  //Rampe
  DrawMaterialQuad("Wall-Concrete3",3489,930,3489,920,3459,930,3489,930,true);

  //Aufzug
  CreateObject(LFTP, 2775, 1210, -1);
  CreateObject(LFTP, 3125, 1210, -1);

  //Leitern
  CreateObject(LADR, 970, 868, -1)->Set(10);
  CreateObject(LADR, 1030, 1216, -1)->Set(6);
  CreateObject(LADR, 1180, 930, -1)->Set(9);
  CreateObject(LADR, 1990, 1050, -1)->Set(13);
  CreateObject(LADR, 2250, 1100, -1)->Set(16);
  CreateObject(LADR, 2700, 1064, -1)->Set(12);
  CreateObject(LADR, 2930, 1072, -1)->Set(3);
  CreateObject(LADR, 3290, 1096, -1)->Set(6);
  CreateObject(LADR, 3350, 912, -1)->Set(13);
  CreateObject(LADR, 3445, 1018, -1)->Set(10);
  CreateObject(LADR, 3540, 768, -1)->Set(20);
  CreateObject(LADR, 3540, 1160, -1)->Set(14);
  CreateObject(LADR, 3710, 1012, -1)->Set(18);
  CreateObject(LADR, 3920, 1108, -1)->Set(30);

  //Bodenluken
  CreateObject(HA4K, 970, 783, -1);
  CreateObject(HA4K, 1030, 1163, -1);
  CreateObject(HA4K, 1180, 853, -1);
  CreateObject(HA4K, 2700, 963, -1);
  CreateObject(HA4K, 2930, 1043, -1);
  CreateObject(HA4K, 3290, 1043, -1);
  CreateObject(HA4K, 3350, 803, -1);
  CreateObject(HA4K, 3540, 1043, -1);
  CreateObject(HA4K, 3540, 603, -1);
  CreateObject(HA4K, 3710, 863, -1);
  CreateObject(HA4K, 3920, 863, -1);

  //Große Bodenluken
  CreateObject(H24K, 635, 1208, -1);
  CreateObject(H24K, 3445, 938, -1);

  //Schutztüren
  CreateObject(GDDR, 2840, 1040, -1)->Lock();
  CreateObject(GDDR, 2900, 1040, -1)->Lock();
  CreateObject(GDDR, 3290, 800, -1)->Lock();

  //Glasscheiben
  CreateObject(_WIN, 3292, 901, -1);
  CreateObject(_WIN, 3292, 930, -1);
  CreateObject(_WIN, 3528, 891, -1);
  CreateObject(_WIN, 3528, 920, -1);

  //Kisten
  CreateObject(WCR2, 570, 1172, -1)->AutoRespawn();
  CreateObject(WCR2, 1510, 1168, -1);
  CreateObject(WCR2, 1660, 1168, -1);
  CreateObject(WCR2, 1680, 1168, -1);
  CreateObject(WCR2, 2640, 1220, -1);
  CreateObject(WCR2, 2660, 1220, -1);
  CreateObject(WCR2, 2890, 1200, -1)->AutoRespawn();
  CreateObject(WCR2, 3080, 940, -1);
  CreateObject(WCR2, 3570, 1180, -1)->AutoRespawn();
  CreateObject(WCR2, 3755, 1040, -1);
  CreateObject(WCR2, 4050, 1130, -1)->AutoRespawn();
  CreateObject(WCR2, 4072, 1130, -1)->AutoRespawn();

  //Metallkisten
  CreateObject(MWCR, 3040, 800, -1)->AutoRespawn();
  CreateObject(MWCR, 3420, 930, -1)->AutoRespawn();
  CreateObject(MWCR, 3790, 832, -1)->AutoRespawn();
  CreateObject(MWCR, 3813, 832, -1);
  CreateObject(MWCR, 3836, 832, -1);
  CreateObject(MWCR, 3859, 832, -1);
  CreateObject(MWCR, 3830, 1040, -1);

  //Verbandskisten
  CreateObject(BECR, 1510, 1152, -1);
  CreateObject(BECR, 2800, 910, -1)->AutoRespawn();
  CreateObject(BECR, 3640, 760, -1);

  //Explosive Kisten
  CreateObject(XWCR, 3210, 800, -1)->AutoRespawn();
  CreateObject(XWCR, 3595, 760, -1);
  CreateObject(XWCR, 3680, 860, -1)->AutoRespawn();

  //Explosivfässer
  CreateObject(XBRL, 3460, 780, -1);
  CreateObject(XBRL, 3300, 1180, -1)->AutoRespawn();
  CreateObject(XBRL, 3610, 1162, -1);
  CreateObject(XBRL, 3615, 760, -1);

  //Benzinfässer
  CreateObject(PBRL, 2730, 1200, -1)->AutoRespawn();
  CreateObject(PBRL, 3180, 1040, -1)->AutoRespawn();

  //Phosphorfässer
  CreateObject(HBRL, 3190, 1040, -1)->AutoRespawn();
  CreateObject(HBRL, 3400, 930, -1)->AutoRespawn();
  CreateObject(HBRL, 3620, 1162, -1);

  //Gasflaschen
  CreateObject(GSBL, 880, 780, -1)->AutoRespawn();
  CreateObject(GSBL, 1530, 1170, -1);
  CreateObject(GSBL, 1540, 1170, -1);
  CreateObject(GSBL, 2180, 1120, -1)->AutoRespawn();
  CreateObject(GSBL, 2720, 960, -1)->AutoRespawn();
  CreateObject(GSBL, 3770, 832, -1)->AutoRespawn();
  CreateObject(GSBL, 4030, 1130, -1);

  //Hinweisschild
  CreateObject(SNPT, 2700, 960, -1)->Light();

  //Schilder
  CreateObject(MSGN, 620, 1200, -1);
  CreateObject(MSGN, 660, 1200, -1);
  CreateObject(WLSN, 1290, 935, -1)->SetPhase(2);
  CreateObject(WLSN, 3405, 900, -1)->SetPhase(1);
  CreateObject(MSGN, 2600, 1220, -1);
  CreateObject(MSGN, 2620, 1220, -1);
  CreateObject(MSGN, 2640, 1220, -1);
  CreateObject(MSGN, 2660, 1220, -1);

  //Gerüste
  CreateObject(SFFG, 2080, 940, -1)->Set(5);
  CreateObject(SFFG, 2160, 940, -1)->Set(5);
  CreateObject(SFFG, 3050, 990, -1);
  CreateObject(SFFG, 3050, 1040, -1)->Set(3);
  CreateObject(SFFG, 3200, 990, -1)->Set(5);
  CreateObject(SFFG, 3200, 1040, -1)->Set(5);
  CreateObject(SFFG, 3620, 760, -1)->Set(4);
  CreateObject(SFFG, 3620, 810, -1);
  CreateObject(SFFG, 3620, 860, -1)->Set(2);

  //Stahlbrücken
  CreateObject(_HBR, 1345, 952, -1);
  CreateObject(_HBR, 1450, 1182, -1);
  CreateObject(_HBR, 1520, 1182, -1);
  CreateObject(_HBR, 1590, 1182, -1);
  CreateObject(_HBR, 1660, 1182, -1);
  CreateObject(_HBR, 1905, 1072, -1);
  CreateObject(_HBR, 2895, 812, -1);
  CreateObject(_HBR, 3617, 1174, -1);
  CreateObject(_HBR, 3617, 1182, -1);

  //Container
  CreateObject(CON1, 550, 1200, -1);
  CreateObject(CON1, 3185, 1173, -1)->SetPerspective(2);
  CreateObject(CON1, 3170, 1200, -1)->SetPerspective(2);
  CreateObject(CON1, 3200, 1201, -1)->SetPerspective(2);
  CreateObject(CON1, 3350, 1040, -1)->SetPerspective(2);
  CreateObject(CON1, 3390, 1041, -1)->SetPerspective();
  CreateObject(CON1, 3557, 851, -1)->SetPerspective();
  CreateObject(CON1, 3751, 861, -1)->SetPerspective(2);
  CreateObject(CON1, 3795, 861, -1);
  CreateObject(CON1, 3850, 861, -1)->SetPerspective();
  CreateObject(CON1, 3990, 861, -1);

  //Rohre
  CreateObject(PIPL, 3310, 715, -1)->Solid()->Right()->Solid()->Right()->Solid()->Right()->Solid()->Right()->Solid();
  CreateObject(PIPL, 3430, 1080, -1)->Solid()->Right()->Solid()->Right()->Solid();

  //Haie
  CreateObject(SHRK, 400, 1300, -1)->AutoRespawn();
  CreateObject(SHRK, 800, 1300, -1)->AutoRespawn();
  CreateObject(SHRK, 1200, 1300, -1)->AutoRespawn();
  CreateObject(SHRK, 1600, 1300, -1)->AutoRespawn();
  CreateObject(SHRK, 2000, 1300, -1)->AutoRespawn();

  //Sandsackbarrieren
  CreateObject(SBBA, 1880, 1060, -1);
  CreateObject(SBBA, 2260, 1120, -1)->Right();
  CreateObject(SBBA, 2820, 1200, -1);
  CreateObject(SBBA, 2870, 800, -1);

  //Explosivtank
  CreateObject(XTNK, 910, 780, -1)->AutoRespawn();
  CreateObject(XTNK, 1240, 850, -1)->AutoRespawn();

  //Phosphortanks
  CreateObject(PTNK, 2210, 1120, -1)->AutoRespawn();
  CreateObject(PTNK, 3210, 1040, -1)->AutoRespawn();

  //Selbstschussanlagen
  aSelfDefense[0] = CreateObject(SEGU, 705, 1069, -1);
    aSelfDefense[0]->Arm(MISA);
    aSelfDefense[0]->SetAutoRepair(1500);
    CreateObject(CONS, 925, 855, -1)->Set(aSelfDefense[0]);

  aSelfDefense[1] = CreateObject(SEGU, 2960, 839, -1);
    aSelfDefense[1]->Arm(MISA);
    aSelfDefense[1]->SetAutoRepair(1500);
    CreateObject(CONS, 3310, 920, -1)->Set(aSelfDefense[1]);

  aSelfDefense[2] = CreateObject(SEGU, 3192, 690, -1);
    aSelfDefense[2]->SetR(90);
    aSelfDefense[2]->Arm(MISA);
    aSelfDefense[2]->SetAutoRepair(1500);
    CreateObject(CONS, 3435, 790, -1)->Set(aSelfDefense[2]);

  aSelfDefense[3] = CreateObject(SEGU, 3785, 899, -1);
    aSelfDefense[3]->Arm(MISA);
    aSelfDefense[3]->SetAutoRepair(1500);
    CreateObject(CONS, 3895, 850, -1)->Set(aSelfDefense[3]);

  //Sounds

  //Möven
  CreateObject(SE4K, 300, 1120, -1)->Set("SeaSounds*.ogg",140,35);
  CreateObject(SE4K, 700, 1120, -1)->Set("SeaSounds*.ogg",140,35);
  CreateObject(SE4K, 1500, 1120, -1)->Set("SeaSounds*.ogg",140,35);
  CreateObject(SE4K, 2300, 1120, -1)->Set("SeaSounds*.ogg",140,35);

  //Wellen
  CreateObject(SE4K, 150, 1230, -1)->Set("Wave*.ogg",105,35);
  CreateObject(SE4K, 650, 1230, -1)->Set("Wave*.ogg",105,35);
  CreateObject(SE4K, 1350, 1230, -1)->Set("Wave*.ogg",105,35);
  CreateObject(SE4K, 1800, 1230, -1)->Set("Wave*.ogg",105,35);
  CreateObject(SE4K, 2400, 1230, -1)->Set("Wave*.ogg",105,35);

  //Wind
  CreateObject(SE4K, 300, 370, -1)->Set("WindSound*.ogg",245,105);
  CreateObject(SE4K, 1400, 370, -1)->Set("WindSound*.ogg",245,105);
  CreateObject(SE4K, 2300, 370, -1)->Set("WindSound*.ogg",245,105);
  CreateObject(SE4K, 2800, 370, -1)->Set("WindSound*.ogg",245,105);

  //Hallen
  CreateObject(SE4K, 2960, 1070, -1)->Set("Interior*.ogg",665,105);
  CreateObject(SE4K, 3400, 890, -1)->Set("Interior*.ogg",665,105);
  CreateObject(SE4K, 3490, 740, -1)->Set("Interior*.ogg",665,105);
  CreateObject(SE4K, 3800, 1060, -1)->Set("Interior*.ogg",665,105);

  //Vögel
  CreateObject(SE4K, 3500, 560, -1)->Set("BirdSong*",140,35);
  CreateObject(SE4K, 4040, 510, -1)->Set("BirdSong*",140,35);

  //Erdrutsche
  CreateObject(SE4K, 4250, 400, -1)->Set("FallingDirt*.ogg",850,250);
}

func CreateEquipment()
{
  Log("$CreatingEquipment$");

  //Versorgungskisten (Kugeln)
  var crate = CreateObject (AMCT, 1020, 900, -1);
  crate->Set(ABOX);
  crate = CreateObject (AMCT, 2170, 940, -1);
  crate->Set(ABOX);
  crate = CreateObject (AMCT, 2990, 1200, -1);
  crate->Set(ABOX);

  //Versorgungskisten (Gewehrgranaten)
  crate = CreateObject (AMCT, 1150, 940, -1);
  crate->Set(GBOX);
  crate = CreateObject (AMCT, 2140, 940, -1);
  crate->Set(GBOX);
  crate = CreateObject (AMCT, 3490, 1180, -1);
  crate->Set(GBOX);

  //Versorgungskisten (Raketen)
  crate = CreateObject (AMCT, 545, 1172, -1);
  crate->Set(MBOX);
  crate = CreateObject (AMCT, 2990, 800, -1);
  crate->Set(MBOX);

  //Versorgungskiste (Raketenwerfer)
  crate = CreateObject (AMCT, 2960, 800, -1);
  crate->Set(RTLR);

  //Versorgungskiste (Dragnin)
  crate = CreateObject (AMCT, 3500, 920, -1);
  crate->Set(DGNN);

  //Artilleriebatterie
  CreateObject(ATBY,2580,950,-1);
}

func CreateDecoration()
{
  Log("$CreatingDecoration$");

  //Radio
  CreateObject(RDIO, 4075, 1111, -1);

  //Leitkegel
  CreateObject(TCCN, 2830, 960, -1);
  CreateObject(TCCN, 2860, 960, -1);
  CreateObject(TCCN, 2890, 960, -1);
  CreateObject(TCCN, 3580, 1040, -1);
  CreateObject(TCCN, 3600, 1040, -1);

  //Radare
  CreateObject(RADR, 1020, 780, -1);
  CreateObject(RADR, 3123, 750, -1);

  //Risse
  CreateObject(FAUD, 2990, 940, -1)->Set(2);
  CreateObject(FAUD, 3000, 970, -1)->Set(3);
  CreateObject(FAUD, 3730, 720, -1)->Set(2);
  CreateObject(FAUD, 3750, 740, -1)->Set(1);
  CreateObject(FAUD, 3780, 810, -1)->Set(3);
  CreateObject(FAUD, 4000, 1030, -1)->Set(1);
  CreateObject(FAUD, 4050, 1080, -1)->Set(2);
  CreateObject(FAUD, 4100, 1050, -1)->Set(3);
  CreateObject(FAUD, 4270, 1100, -1)->Set(2);

  //Orientierungslichter
  CreateObject(OLGH, 635, 1200, -1)->Set(1, 20, 1, 1, 50);
  CreateObject(OLGH, 960, 780, -1)->Set(6, 15, 1, 1, 40);
  CreateObject(OLGH, 1200, 850, -1)->Set(5, 15, 1, 1, 40);
  CreateObject(OLGH, 3540, 600, -1)->Set(1, 20, 1, 1, 50);

  //Automaten
  CreateObject(VGMN, 3820, 1160, -1)->SetPhase(20);
  var automat = CreateObject(VGMN, 3850, 1160, -1);
  automat->Set(2);
  automat->SetPhase(10);
  CreateObject(VGMN, 3880, 1160, -1)->Set(3);

  //Bojen
  CreateObject(BUOY, 380, 1253,  -1);
  CreateObject(BUOY, 400, 1253,  -1);
  CreateObject(BUOY, 2020, 1253,  -1);
  CreateObject(BUOY, 2040, 1253,  -1);

  //Lüftungsgitter
  CreateObject(ENGT, 3350, 910, -1);
  CreateObject(ENGT, 3450, 910, -1);
  CreateObject(ENGT, 3868, 800, -1);
  CreateObject(ENGT, 3868, 940, -1);
  CreateObject(ENGT, 3868, 1120, -1);

  //Bildschirm
  CreateObject(SCA1, 3500, 910, -1)->SetAction("Grenade");

  //Scheinwerfer
  CreateObject(FLGH, 710, 640, -1)->SetRotation(-80);
  CreateObject(FLGH, 730, 630, -1)->SetRotation(-30);
  CreateObject(FLGH, 4130, 1000, -1)->SetRotation(-90);

  //Regale
  CreateObject(FRAM, 3020, 1200, -1);
  CreateObject(FRAM, 3040, 1200, -1);
  CreateObject(FRAM, 3060, 1200, -1);
  CreateObject(FRAM, 3080, 1200, -1);

  CreateObject(FRAM, 3340, 915, -1);
  CreateObject(FRAM, 3340, 930, -1);
  CreateObject(FRAM, 3360, 915, -1);
  CreateObject(FRAM, 3360, 930, -1);

  CreateObject(FRAM, 3670, 1165, -1);
  CreateObject(FRAM, 3670, 1180, -1);

  //Notausgangslichter
  CreateObject(ETLT, 2645, 1190, -1);
  CreateObject(ETLT, 2805, 1020, -1);

  //Glastische
  CreateObject(GTBL, 3770, 1160, -1);
  CreateObject(GTBL, 3830, 1160, -1);
  CreateObject(GTBL, 3940, 1130, -1);
  CreateObject(GTBL, 4000, 1130, -1);

  //Flaschen
  CreateObject(BOTL, 535, 1172, -1);
  CreateObject(BOTL, 3045, 940, -1);
  CreateObject(BOTL, 3055, 940, -1);
  CreateObject(BOTL, 3060, 940, -1);
  CreateObject(BOTL, 3210, 1172, -1);
  CreateObject(BOTL, 3765, 1148, -1);
  CreateObject(BOTL, 3770, 1148, -1);
  CreateObject(BOTL, 3825, 1148, -1);
  CreateObject(BOTL, 3830, 1148, -1);
  CreateObject(BOTL, 3835, 1148, -1);
  CreateObject(BOTL, 3940, 1118, -1);
  CreateObject(BOTL, 3945, 1118, -1);
  CreateObject(BOTL, 4015, 1118, -1);
  CreateObject(BOTL, 3990, 832, -1);
  CreateObject(BOTL, 4005, 832, -1);
  CreateObject(BOTL, 4010, 832, -1);

  //Monitore
  CreateObject(MNI2, 3415, 912, -1);
  CreateObject(MNI2, 3990, 1118, -1);

  //Zäune
  CreateObject(FENC, 3000, 800, -1);
  CreateObject(FENC, 3060, 800, -1);
  CreateObject(FENC, 3190, 800, -1);
  CreateObject(FENC, 3310, 1040, -1);
  CreateObject(FENC, 3370, 1040, -1);
  CreateObject(FENC, 3430, 1040, -1);
  CreateObject(FENC, 3490, 1040, -1);

  //Büsche
  CreateObject(BSH2, 730, 1225, -1);
  CreateObject(BSH2, 1230, 1190, -1);
  CreateObject(BSH2, 1245, 1185, -1);
  CreateObject(BSH2, 1260, 1190, -1);
  CreateObject(BSH2, 2100, 1170, -1);
  CreateObject(BSH2, 2600, 1140, -1);
  CreateObject(BSH2, 3000, 1150, -1);
  CreateObject(BSH2, 4025, 840, -1);
  CreateObject(BSH2, 4040, 750, -1);
  CreateObject(BSH2, 4150, 1100, -1);
  CreateObject(BSH2, 4170, 1110, -1);
  CreateObject(BSH2, 4240, 1100, -1);
  CreateObject(BSH2, 4280, 1060, -1);
  CreateObject(BSH2, 4275, 1100, -1);
  CreateObject(BSH2, 4300, 900, -1);

  //Geländer
  CreateObject(RAI1, 519, 1200, -1)->SetRail([1,1,1,1,1,1,1,1,1,1,1]);
}

func CreateOptionalFeatures()
{
  //Hintergründe
  CreateObject(BD01,2000,1300,-1);
  CreateObject(BD05,500,1300,-1)->SetClrModulation(RGB(125,125,125));
  CreateObject(BD03,500,730,-1);
  CreateObject(BD03,1500,1100,-1);
}

/* Bei Flaggenübernahme */

func FlagCaptured(object pPoint, int iTeam)
{
  if(pPoint == aFlag[0])
   aSelfDefense[0]->SetTeam(iTeam);

  if(pPoint == aFlag[2])
   aSelfDefense[1]->SetTeam(iTeam);

  if(pPoint == aFlag[3])
   aSelfDefense[2]->SetTeam(iTeam);

  if(pPoint == aFlag[4])
   aSelfDefense[3]->SetTeam(iTeam);
}

/* Bei Relaunch */
 
public func OnClassSelection(object pClonk, int iTeam)
{
  //Money Run-Spielziel
  if (FindObject(GMNR))
  {
   AddEffect("IntPara", pClonk, 1, 1);
   Sound("Airstrike2", 0, pClonk);
  }
}
 
global func FxIntParaTimer(object pTarget)
{
  CreateObject(PARA,0,0,GetOwner(pTarget))->Set(pTarget);
  return -1;
}

/* Regelwähler */

public func ChooserFinished()
{
  inherited();

  //Starttitel und Musikliste zusammenstellen
  SetPlayList("CMC_Base Groove.ogg;CMC_Firehawk.ogg;CMC_Friendly Unit.ogg;CMC_Getaway.ogg;CMC_Moving Squad.ogg;CMC_Offensive.ogg;CMC_Showtime.ogg;CMC_Slow Motion.ogg;CMC_Striking Force.ogg;CMC_No Good.ogg;CMC_Obsession.ogg;CMC_Your Eyes.ogg");
  Music("CMC_Offensive.ogg");

  //Teams abfragen
  var aTeams = [false,false,false,false,false];
  for(var i = 0; i < GetPlayerCount(); i++)
   aTeams[GetPlayerTeam(GetPlayerByIndex(i))] = true;

  //DM/LMS-Spielziel
  if(FindObject(GTDM) || FindObject(GLMS))
  {
   //Grenzen setzen
   CreateObject(BRDR, 1790, 0, -1)->Set(0);
   CreateObject(BRDR, 4230, 0, -1)->Set(1);

   //Hinweisschilder
   CreateObject(SNPT, 1650, 1170, -1);
   var sign = CreateObject(SGNP, 2885, 960, -1);
   sign->SetPhase(1);
   sign->SetMode(1);
   sign = CreateObject(SGNP, 3575, 1040, -1);
   sign->SetPhase(1);
   sign->SetMode(1);
   CreateObject(SGNP, 3970, 540, -1);
   CreateObject(SGNP, 4165, 510, -1);

   //Objekte entfernen
   RemoveObject(aSelfDefense[0]);
   RemoveObject(aSelfDefense[2]);
  }

  //Base Assault-Spielziel
  if(FindObject(GBAS))
  {
   //Zielobjekte
   AddAssaultTarget(RADR, 2080, 940, 400, 1, "$Target1$", 1, [[1970, 1060], [2130, 1130], [2400, 1050]]);
   AddAssaultTarget(CMSN, 3050, 1200, 300, 1, "$Target2$", 0, [[2650, 1080], [2670, 1150], [2610, 1220]]);

   AddAssaultTarget(CMSN, 3490, 1040, 300, 2, "$Target2$", 1, [[3730, 1170], [3910, 1160], [4140, 1090]]);
   AddAssaultTarget(RADR, 3390, 620, 400, 2, "$Target1$", 0, [[3550, 821], [3600, 860], [3750, 831]]);

   //Grenzen setzen
   CreateObject(BRDR, 1790, 0, -1)->Set(0);
   CreateObject(BRDR, 4230, 0, -1)->Set(1);

   //Hinweisschilder
   CreateObject(SNPT, 1650, 1170, -1);
   var sign = CreateObject(SGNP, 2885, 960, -1);
   sign->SetPhase(1);
   sign->SetMode(1);
   sign = CreateObject(SGNP, 3575, 1040, -1);
   sign->SetPhase(1);
   sign->SetMode(1);
   CreateObject(SGNP, 3970, 540, -1);
   CreateObject(SGNP, 4165, 510, -1);

   //Objekte entfernen
   RemoveObject(aSelfDefense[0]);
   RemoveObject(aSelfDefense[2]);
  }

  //OP-Spielziel
  if(FindObject(GOCC))
  {
   //Flaggenposten
   aFlag[0] = CreateObject(OFPL,685,1200,NO_OWNER);
   aFlag[0] -> AddSpawnPoint(800, 710);
   aFlag[0] -> AddSpawnPoint(930, 850);
   aFlag[0] -> AddSpawnPoint(1210, 1100);
   if(aTeams[1] == true)
   {
    aFlag[0]->Set("$Flag1$",100,2);
    aFlag[0]->Capture(1,1);
   }
   else
   {
    aFlag[0]->Set("$Flag1$",0,2);
   }

   aFlag[1] = CreateObject(OFPL,2080,940,NO_OWNER);
   aFlag[1] -> AddSpawnPoint(1970,1050);
   aFlag[1] -> AddSpawnPoint(2130,1120);
   aFlag[1] -> AddSpawnPoint(2400,1040);
   aFlag[1]->Set("$Flag2$",0,2);

   aFlag[2] = CreateObject(OFPL,2775,910,NO_OWNER);
   aFlag[2] -> AddSpawnPoint(2630,1070);
   aFlag[2] -> AddSpawnPoint(2895,1070);
   aFlag[2] -> AddSpawnPoint(2695,1130);
   if(aTeams[2] == true)
   {
    aFlag[2]->Set("$Flag3$",100,2);
    aFlag[2]->Capture(2,1);
   }
   else
   {
    aFlag[2]->Set("$Flag3$",0,2);
   }

   aFlag[3] = CreateObject(OFPL,3255,1040,NO_OWNER);
   aFlag[3] -> AddSpawnPoint(3230,1190);
   aFlag[3] -> AddSpawnPoint(3420,1090);
   aFlag[3] -> AddSpawnPoint(3390,920);
   if(aTeams[2] == true)
   {
    aFlag[3]->Set("$Flag4$",100,2);
    aFlag[3]->Capture(2,1);
   }
   else
   {
    aFlag[3]->Set("$Flag4$",0,2);
   }

   aFlag[4] = CreateObject(OFPL,3865,1040,NO_OWNER);
   aFlag[4] -> AddSpawnPoint(4190,1100);
   aFlag[4] -> AddSpawnPoint(4000,821);
   aFlag[4] -> AddSpawnPoint(3600,850);

   if(aTeams[2] == true)
   {
    aFlag[4]->Set("$Flag5$",100,2);
    aFlag[4]->Capture(2,1);
   }
   else
   {
    aFlag[4]->Set("$Flag5$",0,2);
   }

   //Grenzen setzen
   CreateObject(BRDR, 400, 0, -1)->Set(0);
   CreateObject(BRDR, 4230, 0, -1)->Set(1);

   //Hinweisschilder
   var sign = CreateObject(SGNP, 780, 1160, -1);
   sign->SetPhase(1);
   sign = CreateObject(SGNP, 2885, 960, -1);
   sign->SetPhase(1);
   sign->SetMode(1);
   sign = CreateObject(SGNP, 3070, 800, -1);
   sign->SetPhase(1);
   sign->SetMode(1);
   sign = CreateObject(SGNP, 3575, 1040, -1);
   sign->SetPhase(1);
   sign->SetMode(1);
   CreateObject(SGNP, 3970, 540, -1);
   CreateObject(SGNP, 4165, 510, -1);

   //Hinweisschilder
   if(!FindObject(NOBH))
   {
    var sign = CreateObject(SGNP, 800, 720, -1);
    sign->SetPhase(2);
    CreateObject(SNPT, 1160,850, -1)->SetAction("Sign3");
    sign = CreateObject(SGNP, 1310, 820, -1);
    sign->SetPhase(2);
   }

   //SSA Besitzer setzen
   if(aTeams[1] == true)
     aSelfDefense[0]->SetTeam(1);
   if(aTeams[2] == true)
     {aSelfDefense[1]->SetTeam(2); aSelfDefense[2]->SetTeam(2); aSelfDefense[3]->SetTeam(2);}

   //SSA aktivieren
   aSelfDefense[0]->TurnOn();
   aSelfDefense[1]->TurnOn();
   aSelfDefense[2]->TurnOn();
   aSelfDefense[3]->TurnOn();

   //Blackhawks
   SetupVehicleSpawn([BKHK],DIR_Right,CreateObject(VSPW,980,750,-1),60*21,300);
   SetupVehicleSpawn([BKHK],DIR_Right,CreateObject(VSPW,1190,820,-1),60*21,300);

   //Patrouillenboote
   SetupVehicleSpawn([PBOT],DIR_Right,CreateObject(VSPW,490,1240,-1),50*21,300);
   SetupVehicleSpawn([PBOT],DIR_Right,CreateObject(VSPW,1180,1240,-1),50*21,300);
   SetupVehicleSpawn([PBOT],DIR_Left,CreateObject(VSPW,2480,1240,-1),50*21,300);
   SetupVehicleSpawn([PBOT],DIR_Left,CreateObject(VSPW,2550,1240,-1),50*21,300);
  }

  //MR-Spielziel
  if (FindObject(GMNR))
  {
   //Grenzen setzen
   CreateObject(BRDR, 1790, 0, -1)->Set(0);
   CreateObject(BRDR, 4230, 0, -1)->Set(1);

   //Hinweisschilder
   CreateObject(SNPT, 1650, 1170, -1);
   var sign = CreateObject(SGNP, 2885, 960, -1);
   sign->SetPhase(1);
   sign->SetMode(1);
   sign = CreateObject(SGNP, 3575, 1040, -1);
   sign->SetPhase(1);
   sign->SetMode(1);
   CreateObject(SGNP, 3970, 540, -1);
   CreateObject(SGNP, 4165, 510, -1);

   //Objekte entfernen
   RemoveObject(aSelfDefense[0]);
   RemoveObject(aSelfDefense[2]);

   //Geldsäcke
   AddMoneySpawn(2680, 950, [10]);
   AddMoneySpawn(3030, 1190, [10]);
   AddMoneySpawn(3015, 790, [10]);
   AddMoneySpawn(3250, 1030, [10]);
   AddMoneySpawn(3380, 920, [10]);
   AddMoneySpawn(3380, 920, [10]);
   AddMoneySpawn(3870, 1030, [10]);
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
    var rand = Random(3);
    if(!rand)
     { iX = 1970; iY = 1050; }
    if(!--rand)
     { iX = 2110; iY = 930; }
    if(!--rand)
     { iX = 2130; iY = 1120; }
   }
   if(iTeam == 2)
   {
    var rand = Random(3);
    if(!rand)
     { iX = 3980; iY = 821; }
    if(!--rand)
     { iX = 3600; iY = 850; }
    if(!--rand)
     { iX = 3515; iY = 590; }
   }
   return(1);
  }

  //MR-Spielziel
  if(FindObject(GMNR))
  {
   if(iTeam == 1)
   {
    var rand = Random(3);
    if(!rand)
     { iX = 2110; iY = 0; }
    if(!--rand)
     { iX = 2300; iY = 0; }
    if(!--rand)
     { iX = 2430; iY = 0; }
   }
   if(iTeam == 2)
   {
    var rand = Random(3);
    if(!rand)
     { iX = 3320; iY = 0; }
    if(!--rand)
     { iX = 3480; iY = 0; }
    if(!--rand)
     { iX = 3650; iY = 0; }
   }
   return(1);
  }

  //Assault-Spielziel
  if(FindObject(GASS))
  {if(FindObject(GASS)->GetRespawnPoint(iX, iY, iTeam)) return 1;}

  //Startsicht
  iX =2310; iY = 960;
}