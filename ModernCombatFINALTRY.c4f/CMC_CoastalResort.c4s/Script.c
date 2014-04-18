/*-- Coastal Resort --*/

#strict
#include CSTD

static aFlag,aSelfDefense,doorw1,doorw2,pRoom1,pRoom2,aLights,iLightsCounter;

func RecommendedGoals()			{return [GOCC, GASS];}	//Spielzielempfehlung
public func AssaultDefenderTeam()	{return 2;}		//Verteidigerteam bei Assault


/* Initialisierung */

func Initialize()
{
  //Wartemusik einstellen
  SetWaitingMusic();
  //Bildschirmfärbung
  SetGamma(RGB(7,6,0),RGB(152,147,128),RGB(255,254,236));
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
  //Lichter ausschalten
  LightsOff();
  return(1);
}

/* Plazierungslisten */

func CreateInterior()
{
  Log("$CreatingInterior$");

  //Rampen
  DrawMaterialQuad("Wall-Metal2",3860,920,3860,900,3790,920,3860,920,true);
  DrawMaterialQuad("Wall-Concrete3",3870,560,3870,550,3840,560,3870,560,true);
  DrawMaterialQuad("Wall-Concrete3",3961,560,3961,550,3991,560,3961,560,true);
  DrawMaterialQuad("Wall-Concrete3",4530,560,4530,550,4500,560,4530,560,true);
  DrawMaterialQuad("Wall-Concrete3",4620,560,4620,550,4650,560,4620,560,true);
  DrawMaterialQuad("Wall-Metal2",4631,920,4631,900,4701,920,4631,920,true);
  DrawMaterialQuad("Wall-Metal2",3680,960,3680,950,3670,960,3680,960,true);
  DrawMaterialQuad("Wall-Metal2",4811,960,4811,950,4821,960,4811,960,true);

  //Leitern
  CreateObject(LADR, 2340, 1010, -1)->Set(4);
  CreateObject(LADR, 2450, 1010, -1)->Set(4);
  CreateObject(LADR, 2910, 926, -1)->Set(6);
  CreateObject(LADR, 2995, 976, -1);

  CreateObject(LADR, 3615, 998, -1)->Set(4);
  CreateObject(LADR, 3755, 735, -1)->Set(16);
  CreateObject(LADR, 3815, 998, -1)->Set(4);
  CreateObject(LADR, 3930, 658, -1)->Set(5);
  CreateObject(LADR, 3930, 778, -1)->Set(5);
  CreateObject(LADR, 4150, 1004, -1)->Set(12);

  CreateObject(LADR, 4340, 1004, -1)->Set(12);
  CreateObject(LADR, 4560, 658, -1)->Set(5);
  CreateObject(LADR, 4560, 778, -1)->Set(5);
  CreateObject(LADR, 4675, 998, -1)->Set(4);
  CreateObject(LADR, 4735, 735, -1)->Set(16);
  CreateObject(LADR, 4875, 998, -1)->Set(4);

  CreateObject(LADR, 5580, 926, -1)->Set(6);
  CreateObject(LADR, 5495, 976, -1);
  CreateObject(LADR, 6200, 1010, -1)->Set(4);
  CreateObject(LADR, 6310, 1010, -1)->Set(4);

  //Bodenluken
  CreateObject(HA4K, 2340, 973, -1);
  CreateObject(HA4K, 2450, 973, -1);
  CreateObject(HA4K, 2910, 873, -1);
  CreateObject(HA4K, 3930, 613, -1);
  CreateObject(HA4K, 3930, 733, -1);
  CreateObject(HA4K, 4150, 903, -1);
  CreateObject(HA4K, 4340, 903, -1);
  CreateObject(HA4K, 4560, 613, -1);
  CreateObject(HA4K, 4560, 733, -1);
  CreateObject(HA4K, 5580, 873, -1);
  CreateObject(HA4K, 6200, 973, -1);
  CreateObject(HA4K, 6310, 973, -1);

  //Schutztüren
  CreateObject(GDDR, 3910, 610, -1)->Lock();
  CreateObject(GDDR, 3910, 670, -1)->Lock();
  CreateObject(GDDR, 3910, 730, -1)->Lock();
  CreateObject(GDDR, 3910, 790, -1)->Lock();
  CreateObject(GDDR, 4095, 900, -1)->SetColorDw(HSL(40, 210, 100, 127));
  CreateObject(GDDR, 4395, 900, -1)->SetColorDw(HSL(40, 210, 100, 127));
  CreateObject(GDDR, 4580, 610, -1)->Lock();
  CreateObject(GDDR, 4580, 670, -1)->Lock();
  CreateObject(GDDR, 4580, 730, -1)->Lock();
  CreateObject(GDDR, 4580, 790, -1)->Lock();

  //Rahmen
  CreateObject(FRME, 4172, 883, -1);
  CreateObject(FRME, 4322, 883, -1);

  //Glasscheiben
  CreateObject(PANE, 3842, 610, -1);
  CreateObject(PANE, 3842, 670, -1);
  CreateObject(PANE, 3842, 730, -1);
  CreateObject(PANE, 3852, 790, -1);
  CreateObject(PANE, 3852, 840, -1);

  CreateObject(PANE, 3978, 790, -1);
  CreateObject(PANE, 3978, 840, -1);
  CreateObject(PANE, 3988, 610, -1);
  CreateObject(PANE, 3988, 670, -1);
  CreateObject(PANE, 3988, 730, -1);

  CreateObject(PANE, 4092, 840, -1);
  CreateObject(PAN2, 4155, 795, -1);
  CreateObject(PAN2, 4155, 800, -1);
  CreateObject(PAN2, 4335, 795, -1);
  CreateObject(PAN2, 4335, 800, -1);
  CreateObject(PANE, 4398, 840, -1);

  CreateObject(PANE, 4502, 610, -1);
  CreateObject(PANE, 4502, 670, -1);
  CreateObject(PANE, 4502, 730, -1);
  CreateObject(PANE, 4512, 790, -1);
  CreateObject(PANE, 4512, 840, -1);

  CreateObject(PANE, 4638, 790, -1);
  CreateObject(PANE, 4638, 840, -1);
  CreateObject(PANE, 4648, 610, -1);
  CreateObject(PANE, 4648, 670, -1);
  CreateObject(PANE, 4648, 730, -1);

  //Kisten
  CreateObject(WCR2, 2360, 970, -1)->AutoRespawn();
  CreateObject(WCR2, 6290, 970, -1)->AutoRespawn();

  //Metallkisten
  CreateObject(MWCR, 2930, 870, -1)->AutoRespawn();
  CreateObject(MWCR, 4205, 900, -1)->AutoRespawn();
  CreateObject(MWCR, 4285, 900, -1)->AutoRespawn();
  CreateObject(MWCR, 5560, 870, -1)->AutoRespawn();

  //Verbandskisten
  CreateObject(BECR, 2380, 970, -1)->AutoRespawn();
  CreateObject(BECR, 6270, 970, -1)->AutoRespawn();

  //Gasflaschen
  CreateObject(GSBL, 3040, 930, -1)->AutoRespawn();
  CreateObject(GSBL, 3800, 610, -1)->AutoRespawn();
  CreateObject(GSBL, 4690, 610, -1)->AutoRespawn();
  CreateObject(GSBL, 5450, 930, -1)->AutoRespawn();

  //Stahlbrücken
  CreateObject(_HBR, 4005, 912, -1)->SwitchMode();
  CreateObject(_HBR, 4485, 912, -1)->SwitchMode();

  //Gerüste
  CreateObject(SFFG, 3800, 660, -1)->Set(4);
  CreateObject(SFFG, 3800, 710, -1)->Set(5);
  CreateObject(SFFG, 3800, 760, -1)->Set(5);
  CreateObject(SFFG, 3800, 810, -1)->Set(4);
  CreateObject(SFFG, 3800, 860, -1)->Set(2);
  CreateObject(SFFG, 3800, 910, -1)->Set(5);

  CreateObject(SFFG, 4690, 660, -1)->Set(4);
  CreateObject(SFFG, 4690, 710, -1)->Set(5);
  CreateObject(SFFG, 4690, 760, -1)->Set(5);
  CreateObject(SFFG, 4690, 810, -1)->Set(4);
  CreateObject(SFFG, 4690, 860, -1)->Set(3);
  CreateObject(SFFG, 4690, 910, -1)->Set(5);

  //Haie
  CreateObject(SHRK, 400, 1200, -1)->AutoRespawn();
  CreateObject(SHRK, 1630, 1200, -1)->AutoRespawn();
  CreateObject(SHRK, 2690, 1200, -1)->AutoRespawn();
  CreateObject(SHRK, 2790, 1250, -1)->AutoRespawn();
  CreateObject(SHRK, 3520, 1250, -1)->AutoRespawn();
  CreateObject(SHRK, 4970, 1250, -1)->AutoRespawn();
  CreateObject(SHRK, 5700, 1250, -1)->AutoRespawn();
  CreateObject(SHRK, 5800, 1200, -1)->AutoRespawn();
  CreateObject(SHRK, 7020, 1200, -1)->AutoRespawn();
  CreateObject(SHRK, 8250, 1200, -1)->AutoRespawn();

  //Piranha
  CreateObject(PIRA, 1430, 1100, -1)->AutoRespawn();
  CreateObject(PIRA, 2490, 1200, -1)->AutoRespawn();
  CreateObject(PIRA, 3690, 1200, -1)->AutoRespawn();
  CreateObject(PIRA, 4060, 1200, -1)->AutoRespawn();
  CreateObject(PIRA, 4250, 1100, -1)->AutoRespawn();
  CreateObject(PIRA, 4250, 1200, -1)->AutoRespawn();
  CreateObject(PIRA, 4430, 1200, -1)->AutoRespawn();
  CreateObject(PIRA, 4800, 1200, -1)->AutoRespawn();
  CreateObject(PIRA, 6160, 1200, -1)->AutoRespawn();
  CreateObject(PIRA, 7220, 1100, -1)->AutoRespawn();

  //Explosivtanks
  CreateObject(XTNK, 3015, 930, -1)->AutoRespawn();
  CreateObject(XTNK, 5475, 930, -1)->AutoRespawn();

  //Sonne umplazieren
  FindObject(LENS)->SetPosition(4250,640);

  //Seegras und Muscheln entfernen
  for(var obj in FindObjects(Find_Or(Find_ID(SWD1), Find_ID(SWD2), Find_ID(SHEL)), Find_InRect(2250, 800, 2410, 180)))
   RemoveObject(obj);
  for(var obj in FindObjects(Find_Or(Find_ID(SWD1), Find_ID(SWD2), Find_ID(SHEL)), Find_InRect(3830, 660, 830, 140)))
   RemoveObject(obj);

  //Verbundene Räume
  doorw1 = CreateObject(GAT1, 3915, 525, -1);
  pRoom1 = CreateObject(ROOM, 3100, 940, -1);
  pRoom1->Connect(doorw1);
  var doorw = CreateObject(ROM2, 3915, 900, -1);
  CreateObject(ROM2, 3960, 790, -1)->Connect(doorw);
  doorw = CreateObject(ROM2, 4575, 900, -1);
  CreateObject(ROM2, 4530, 790, -1)->Connect(doorw);
  doorw2 = CreateObject(GAT1, 4575, 525, -1);
  pRoom2 = CreateObject(ROOM, 5390, 940, -1);
  pRoom2->Connect(doorw2);

  //Säulen
  CreateObject(PILR, 3910, 980, -1)->Set("PreparePillarCollapseL");
  CreateObject(PILR, 4580, 980, -1)->Set("PreparePillarCollapseR");

  //Wasseroberfläche ebnen
  DrawMaterialQuad("Water",2280,1010,6370,1010,6370,1015,2280,1015,1);

  //Sounds

  //Wind
  CreateObject(SE4K, 1100, 300, -1)->Set("WindSound*.ogg",3000,105);
  CreateObject(SE4K, 2100, 300, -1)->Set("WindSound*.ogg",3000,105);
  CreateObject(SE4K, 3100, 300, -1)->Set("WindSound*.ogg",3000,105);
  CreateObject(SE4K, 4250, 300, -1)->Set("WindSound*.ogg",3000,105);
  CreateObject(SE4K, 5550, 300, -1)->Set("WindSound*.ogg",3000,105);
  CreateObject(SE4K, 6550, 300, -1)->Set("WindSound*.ogg",3000,105);
  CreateObject(SE4K, 7550, 300, -1)->Set("WindSound*.ogg",3000,105);

  //Hallen
  CreateObject(SE4K, 3100, 850, -1)->Set("Interior*.ogg",665,105);
  CreateObject(SE4K, 3915, 680, -1)->Set("Interior*.ogg",665,105);
  CreateObject(SE4K, 4575, 680, -1)->Set("Interior*.ogg",665,105);
  CreateObject(SE4K, 5390, 850, -1)->Set("Interior*.ogg",665,105);

  //Erdrutsche
  CreateObject(SE4K, 1065, 980, -1)->Set("FallingDirt*.ogg",950,250);
  CreateObject(SE4K, 7580, 980, -1)->Set("FallingDirt*.ogg",950,250);

  //Wellen
  CreateObject(SE4K, 500, 1010, -1)->Set("Wave*.ogg",105,35);
  CreateObject(SE4K, 1300, 1010, -1)->Set("Wave*.ogg",105,35);
  CreateObject(SE4K, 2670, 1010, -1)->Set("Wave*.ogg",105,35);
  CreateObject(SE4K, 3490, 1010, -1)->Set("Wave*.ogg",105,35);
  CreateObject(SE4K, 4250, 1010, -1)->Set("Wave*.ogg",105,35);
  CreateObject(SE4K, 4970, 1010, -1)->Set("Wave*.ogg",105,35);
  CreateObject(SE4K, 5820, 1010, -1)->Set("Wave*.ogg",105,35);
  CreateObject(SE4K, 7360, 1010, -1)->Set("Wave*.ogg",105,35);
  CreateObject(SE4K, 8150, 1010, -1)->Set("Wave*.ogg",105,35);

  //Jungel
  CreateObject(SE4K, 1520, 950, -1)->Set("Jungle*.ogg",140,70);
  CreateObject(SE4K, 1950, 780, -1)->Set("Jungle*.ogg",140,70);
  CreateObject(SE4K, 6700, 780, -1)->Set("Jungle*.ogg",140,70);
  CreateObject(SE4K, 7140, 950, -1)->Set("Jungle*.ogg",140,70);
}

func CreateEquipment()
{
  Log("$CreatingEquipment$");

  //Versorgungskisten (Kugeln)
  var crate = CreateObject(AMCT, 2280, 970, -1);
  crate->Set(ABOX);
  crate = CreateObject(AMCT, 6370, 970, -1);
  crate->Set(ABOX);
  crate = CreateObject(AMCT, 4180, 900, -1);
  crate->Set(ABOX);

  //Versorgungskisten (Gewehrgranaten)
  crate = CreateObject(AMCT, 2320, 970, -1);
  crate->Set(GBOX);
  crate = CreateObject(AMCT, 6330, 970, -1);
  crate->Set(GBOX);
  crate = CreateObject(AMCT, 4310, 900, -1);
  crate->Set(GBOX);

  //Raketen
  PlaceSpawnpoint(MBOX, 3890, 785);
  PlaceSpawnpoint(MBOX, 4600, 785);

  //Versorgungskisten (Dragnin)
  crate = CreateObject (AMCT, 3725, 950, -1);
  crate->Set(DGNN);
  crate = CreateObject (AMCT, 4765, 950, -1);
  crate->Set(DGNN);

  //Automat
  var store = CreateObject(WPVM,4245, 900,-1);
  store->AddWare(C4PA,-1);
  store->AddWare(FAPK,-1);
  store->AddWare(CDBT,-1);
  store->AddWare(BWTH,-1);
  store->AddWare(RSHL,-1);
  store->AddWare(ATWN,-1);
  store->AddWare(FGRN,-1);
  store->AddWare(FRAG,-1);
  store->AddWare(PGRN,-1);
  store->AddWare(SGRN,-1);
  store->AddWare(STUN,-1);
  store->AddWare(SRBL,-1);

  //MAV-Stationen
  CreateObject(MVSN, 3290, 950, -1)->Set(3135,820,1);
  CreateObject(MVSN, 5200, 950, -1)->Set(5355,820,1);

  //Geschützstellungen
  CreateObject(GNET, 3365, 960, -1)->Set(0,90);
  CreateObject(GNET, 5125, 960, -1)->Set(0,-90);
}

func CreateDecoration()
{
  Log("$CreatingDecoration$");

  //Sonnenschirme
  CreateObject(SNSE, 1510, 990, -1)->SetColorDw(HSL(145, 210, 100, 127));
  CreateObject(SNSE, 1545, 990, -1)->SetColorDw(HSL(145, 210, 100, 127));
  CreateObject(SNSE, 1775, 900, -1)->SetColorDw(HSL(145, 210, 100, 127));
  CreateObject(SNSE, 1980, 840, -1)->SetColorDw(HSL(145, 210, 100, 127));
  CreateObject(SNSE, 3645, 960, -1)->SetColorDw(HSL(145, 210, 100, 127));
  CreateObject(SNSE, 4075, 790, -1)->SetColorDw(HSL(145, 210, 100, 127));
  CreateObject(SNSE, 4125, 790, -1)->SetColorDw(HSL(145, 210, 100, 127));
  CreateObject(SNSE, 4185, 790, -1)->SetColorDw(HSL(145, 210, 100, 127));
  CreateObject(SNSE, 4305, 790, -1);
  CreateObject(SNSE, 4365, 790, -1);
  CreateObject(SNSE, 4415, 790, -1);
  CreateObject(SNSE, 4845, 960, -1);
  CreateObject(SNSE, 6670, 840, -1);
  CreateObject(SNSE, 6875, 900, -1);
  CreateObject(SNSE, 7105, 990, -1);
  CreateObject(SNSE, 7140, 990, -1);

  //Waschbecken
  CreateObject(WSHB, 3870, 670, -1);
  CreateObject(WSHB, 3870, 790, -1);
  CreateObject(WSHB, 4620, 670, -1);
  CreateObject(WSHB, 4620, 790, -1);

  //Toilettenkabinen
  CreateObject(TOIC, 3890, 670, -1);
  CreateObject(TOIC, 3890, 790, -1);
  CreateObject(TOIC, 4600, 670, -1);
  CreateObject(TOIC, 4600, 790, -1);

  //Topfpflanzen
  CreateObject(PLT3, 3880, 900, -1);
  CreateObject(PLT3, 3950, 900, -1);
  CreateObject(PLT3, 4070, 900, -1);
  CreateObject(PLT3, 4095, 790, -1);
  CreateObject(PLT3, 4395, 790, -1);
  CreateObject(PLT3, 4420, 900, -1);
  CreateObject(PLT3, 4540, 900, -1);
  CreateObject(PLT3, 4610, 900, -1);

  //Betten
  CreateObject(BED2, 3962, 610, -1);
  CreateObject(BED2, 3962, 730, -1);
  CreateObject(BED2, 4527, 610, -1);
  CreateObject(BED2, 4527, 730, -1);

  //Automaten
  CreateObject(VGMN, 4130, 900, -1)->Set(3);
  CreateObject(VGMN, 4175, 900, -1);

  //Geländer
  CreateObject(RAI1, 2848, 870, -1)->SetRail([1,3,1,3,1,3,1,3,1,3,1]);
  CreateObject(RAI1, 3977, 790, -1);
  CreateObject(RAI1, 4040, 900, -1)->SetRail([1,3,1,3,1,3,1]);
  CreateObject(RAI1, 4092, 790, -1)->SetRail([1,3,1]);
  var rail = CreateObject(RAI1, 4147, 790, -1);
  rail->SetCategory(C4D_StaticBack);
  rail->SetRail([1,3,1]);
  for(var i = 0; i <= 5; i++)
   rail->SetClrModulation(RGB(200, 200, 200), this, i);
  rail = CreateObject(RAI1, 4298, 790, -1);
  rail->SetCategory(C4D_StaticBack);
  rail->SetRail([1,3,1]);
  for(var i = 0; i <= 5; i++)
   rail->SetClrModulation(RGB(200, 200, 200), this, i);
  CreateObject(RAI1, 4352, 790, -1)->SetRail([1,3,1]);
  CreateObject(RAI1, 4365, 900, -1)->SetRail([1,3,1,3,1,3,1]);
  CreateObject(RAI1, 4488, 790, -1);
  CreateObject(RAI1, 5518, 870, -1)->SetRail([1,3,1,3,1,3,1,3,1,3,1]);

  //Wandlampen
  CreateObject(BLGH, 4125, 850, -1);
  CreateObject(BLGH, 4365, 850, -1);

  //Deckenlichter
  CreateObject(CLGH, 3915, 815, -1);
  CreateObject(CLGH, 4575, 815, -1);

  //Bildschirme
  CreateObject(SCA2, 3915, 870, -1)->SetAction("News");
  CreateObject(SCA2, 4575, 870, -1)->SetAction("News10");

  //Jetwrack
  CreateObject(_JW1, 3070, 1360, -1)->SetR(60);
  CreateObject(_JW2, 3160, 1375, -1)->SetR(20);

  //Kaputte U-Boote
  CreateObject(SUBB, 440, 1340, -1)->SetR(-20);
  CreateObject(SUBB, 6700, 1220, -1)->SetR(20);

  //Schiffswracks
  CreateObject(_WRK, 1630, 1360, -1);
  CreateObject(_WRK, 6990, 1360, -1);

  //Truhen
  CreateObject(CHST, 1510, 1340, -1);
  CreateObject(CHST, 7150, 1340, -1);

  //Kaputte Boote
  CreateObject(SLBB, 2240, 1310, -1)->SetR(40);
  CreateObject(SLBB, 6410, 1310, -1)->SetR(-30);

  //Bambushütten
  CreateObject(HUT1, 1510, 992, -1);
  CreateObject(HUT1, 1720, 922, -1)->SetR(-5);
  CreateObject(HUT1, 1980, 832, -1)->SetR(-1);
  CreateObject(HUT1, 3190, 932, -1)->SetR(1);
  CreateObject(HUT1, 5295, 932, -1)->SetR(-1);
  CreateObject(HUT1, 6720, 805, -1)->SetR(1);
  CreateObject(HUT1, 6860, 902, -1)->SetR(5);
  CreateObject(HUT1, 7050, 972, -1);

  //Straßenlichter
  CreateObject(SLGH, 2300, 970, -1);
  CreateObject(SLGH, 3790, 920, -1);
  CreateObject(SLGH, 4700, 920, -1);
  CreateObject(SLGH, 6350, 970, -1);

  //Büsche
  CreateObject(BSH2, 3240, 880, -1);
  CreateObject(BSH2, 3260, 890, -1);
  CreateObject(BSH2, 3280, 900, -1);
  var bush = CreateObject(BSH2, 3750, 920, -1);
  bush->SetClrModulation(RGB(125,125,125));
  bush->SetCategory(C4D_StaticBack);
  bush = CreateObject(BSH2, 3770, 920, -1);
  bush->SetClrModulation(RGB(125,125,125));
  bush->SetCategory(C4D_StaticBack);

  var bush = CreateObject(BSH2, 4720, 920, -1);
  bush->SetClrModulation(RGB(125,125,125));
  bush->SetCategory(C4D_StaticBack);
  bush = CreateObject(BSH2, 4740, 920, -1);
  bush->SetClrModulation(RGB(125,125,125));
  bush->SetCategory(C4D_StaticBack);
  CreateObject(BSH2, 5210, 900, -1);
  CreateObject(BSH2, 5230, 890, -1);
  CreateObject(BSH2, 5250, 880, -1);

  //Steine
  CreateObject(STNE, 305, 1260, -1);
  CreateObject(STNE, 1920, 1310, -1);
  CreateObject(STNE, 2770, 1320, -1);
  CreateObject(STNE, 6215, 1330, -1);
  CreateObject(STNE, 7030, 1350, -1);
  CreateObject(STNE, 8240, 1330, -1);
  CreateObject(STNE, 8350, 1260, -1);

  //Labortische
  CreateObject(LTBL, 3880, 610, -1);
  CreateObject(LTBL, 3880, 730, -1);
  CreateObject(LTBL, 4610, 610, -1);
  CreateObject(LTBL, 4610, 730, -1);

  //Monitore
  CreateObject(MNI2, 3880, 598, -1);
  CreateObject(MNI2, 3880, 718, -1);
  CreateObject(MNI2, 4610, 598, -1);
  CreateObject(MNI2, 4610, 718, -1);

  //Lüftungsgitter
  CreateObject(ENGT, 2395, 900, -1);
  CreateObject(ENGT, 3100, 890, -1);
  CreateObject(ENGT, 5390, 890, -1);
  CreateObject(ENGT, 6255, 900, -1);
}

func CreateOptionalFeatures()
{
  //Hintergründe
  var back = CreateObject(BD03,1200,670,-1);
  if(FindObject(STRM))
   back->SetClrModulation(RGBa(50,50,50,5));
  back = CreateObject(BD03,3300,650,-1);
  if(FindObject(STRM))
   back->SetClrModulation(RGBa(50,50,50,5));
  back = CreateObject(BD03,5400,690,-1);
  if(FindObject(STRM))
   back->SetClrModulation(RGBa(50,50,50,5));

  back = CreateObject(BD05,1550,1200,-1);
  if(FindObject(STRM))
   back->SetClrModulation(RGBa(50,50,50,5));
  back = CreateObject(BD08,400,1300,-1);
  if(FindObject(STRM))
   back->SetClrModulation(RGB(125,125,125));
  back = CreateObject(BD08,1300,1300,-1);
  if(FindObject(STRM))
   back->SetClrModulation(RGB(125,125,125));
  back = CreateObject(BD05,2750,1130,-1);
  if(FindObject(STRM))
   back->SetClrModulation(RGBa(50,50,50,5));
  back->Local()=45;

  back = CreateObject(BD08,3700,1300,-1);
  if(FindObject(STRM))
   back->SetClrModulation(RGB(125,125,125));
  back->Local()=45;
  back = CreateObject(BD08,4600,1300,-1);
  if(FindObject(STRM))
   back->SetClrModulation(RGB(125,125,125));
  back->Local()=45;
}

/* Bei Säulenzerstörungen */

func PreparePillarCollapseL(int iPlr)
{
  //Spielerbildschirme schütteln
  ShakeViewPort(100);

  //Effekte
  AddParticlesInRect("Smoke3",[20,40],[3840,550,150,370],[10,30],[200,400]);
  AddParticlesInRect("Smoke",[20,40],[3840,550,150,370],[10,25],[200,400]);

  //Zerstörung planen
  Schedule(Format("GameCall(\"OnPillarCollapseL\", %d)", iPlr), 200);
}

func OnPillarCollapseL(int iPlr)
{
  //Effekte
  var effect = CreateObject(TIM1,3915,670);
  effect->ShakeViewPort(800, this);
  effect->Sound("StructureHit*.ogg");
  effect->Schedule("RemoveObject()",100);
  if(GetEffectData(EFSM_ExplosionEffects) > 1) CastParticles("ConcreteSplinter",10,110,3915,600,60,100);
  if(GetEffectData(EFSM_ExplosionEffects) > 1) CastParticles("ConcreteSplinter",10,110,3915,700,60,100);
  if(GetEffectData(EFSM_ExplosionEffects) > 1) CastParticles("ConcreteSplinter",10,110,3915,800,60,100);
  if(GetEffectData(EFSM_ExplosionEffects) > 1) CastParticles("ConcreteSplinter",10,110,3915,880,60,100);
  if(GetEffectData(EFSM_ExplosionEffects) > 1) CastParticles("ConcreteSplinter",10,110,3915,600,60,100);
  if(GetEffectData(EFSM_ExplosionEffects) > 1) CastParticles("ConcreteSplinter",10,110,3915,700,60,100);
  if(GetEffectData(EFSM_ExplosionEffects) > 1) CastParticles("ConcreteSplinter",10,110,3915,850,60,100);

  //Hintergrund zeichnen
  DrawMap(3840,500,151,261, "map Empty{};");

  //Lebewesen verletzen
  for(var obj in FindObjects(Find_OCF(OCF_Alive), Find_InRect(3840,550,160,240)))
   DoDmg(200, DMG_Explosion, obj, 0, iPlr + 1);

  //Dekoration entfernen
  for(var obj in FindObjects(Find_Or(Find_ID(HA4K), Find_ID(GDDR), Find_ID(PANE), Find_ID(BED2), Find_ID(WSHB), Find_ID(TOIC), Find_ID(LADR), Find_ID(LTBL)), Find_InRect(3840, 550, 150, 270)))
   RemoveObject(obj);

  //Türverbindung entfernen
  doorw1->CastSmoke("Smoke3",12,15,0,5,150,250,RGBa(255,255,255,100),RGBa(255,255,255,100));
  RemoveObject(doorw1, true);
  pRoom1->CastSmoke("Smoke3",12,15,0,5,150,250,RGBa(255,255,255,100),RGBa(255,255,255,100));
  pRoom1->Lock();
  pRoom1->SetAction("Idle");
  pRoom1->SetClrModulation(RGBa(100,100,100,5));
  var pContent;
  while(pContent = Contents(0, pRoom1))
   pRoom1->Exit(pContent);

  //Lebewesen schwer verletzen
  for(var obj in FindObjects(Find_OCF(OCF_Alive), Find_InRect(3840, 550, 150, 270)))
   DoDmg(200, DMG_Explosion, obj, 0, iPlr + 1);

  //Trümmer verschleudern
  var debris = CreateObject(DBRS, 3915,560, iPlr);
  Fling(debris, RandomX(3,6), RandomX(-2,1));
  debris = CreateObject(DBRS, 3915,620, iPlr);
  Fling(debris, RandomX(3,6), RandomX(-2,1));
  debris = CreateObject(DBRS, 3915,680, iPlr);
  Fling(debris, RandomX(3,6), RandomX(-2,1));
  debris = CreateObject(DBRS, 3915,740, iPlr);
  Fling(debris, RandomX(3,6), RandomX(-2,1));

  //Leiter
  CreateObject(LADR, 4007, 869, -1)->Set(9);

  //Geschützstellung
  var station = CreateObject(GNET, 3825, 610, -1);
  station->Set(0,90,1);
  station->FadeIn();

  //Schutt
  CreateObject(DEB1, 3920, 791, -1)->AddSmokeEffect4K(60,-2,-5,true,this);
}

func PreparePillarCollapseR(int iPlr)
{
  //Spielerbildschirme schütteln
  ShakeViewPort(100);

  //Effekte
  AddParticlesInRect("Smoke3",[20,40],[4500,550,150,370],[10,30],[200,400]);
  AddParticlesInRect("Smoke",[20,40],[4500,550,150,370],[10,25],[200,400]);

  //Zerstörung planen
  Schedule(Format("GameCall(\"OnPillarCollapseR\", %d)", iPlr), 200);
}

public func Empty()
{}

func OnPillarCollapseR(int iPlr)
{
  //Effekte
  var effect = CreateObject(TIM1,4575,670);
  effect->ShakeViewPort(800, this);
  effect->Sound("StructureHit*.ogg");
  effect->Schedule("RemoveObject()",100);
  if(GetEffectData(EFSM_ExplosionEffects) > 1) CastParticles("ConcreteSplinter",10,110,4575,600,60,100);
  if(GetEffectData(EFSM_ExplosionEffects) > 1) CastParticles("ConcreteSplinter",10,110,4575,700,60,100);
  if(GetEffectData(EFSM_ExplosionEffects) > 1) CastParticles("ConcreteSplinter",10,110,4575,800,60,100);
  if(GetEffectData(EFSM_ExplosionEffects) > 1) CastParticles("ConcreteSplinter",10,110,4575,880,60,100);
  if(GetEffectData(EFSM_ExplosionEffects) > 1) CastParticles("ConcreteSplinter",10,110,4575,600,60,100);
  if(GetEffectData(EFSM_ExplosionEffects) > 1) CastParticles("ConcreteSplinter",10,110,4575,700,60,100);
  if(GetEffectData(EFSM_ExplosionEffects) > 1) CastParticles("ConcreteSplinter",10,110,3970,850,60,100);

  //Hintergrund zeichnen
  DrawMap(4500,500,151,261, "map Empty{};");

  //Lebewesen verletzen
  for(var obj in FindObjects(Find_OCF(OCF_Alive), Find_InRect(4490,550,160,240)))
   DoDmg(200, DMG_Explosion, obj, 0, iPlr + 1);

  //Dekoration entfernen
  for(var obj in FindObjects(Find_Or(Find_ID(HA4K), Find_ID(GDDR), Find_ID(PANE), Find_ID(BED2), Find_ID(WSHB), Find_ID(TOIC), Find_ID(LADR), Find_ID(LTBL)), Find_InRect(4500, 550, 150, 270)))
   RemoveObject(obj);

  //Türverbindung entfernen
  doorw2->CastSmoke("Smoke3",12,15,0,5,150,250,RGBa(255,255,255,100),RGBa(255,255,255,100));
  RemoveObject(doorw2, true);
  pRoom2->CastSmoke("Smoke3",12,15,0,5,150,250,RGBa(255,255,255,100),RGBa(255,255,255,100));
  pRoom2->Lock();
  pRoom2->SetAction("Idle");
  pRoom2->SetClrModulation(RGBa(100,100,100,5));
  var pContent;
  while(pContent = Contents(0, pRoom2))
   pRoom2->Exit(pContent);

  //Lebewesen schwer verletzen
  for(var obj in FindObjects(Find_OCF(OCF_Alive), Find_InRect(4500, 550, 150, 270)))
   DoDmg(200, DMG_Explosion, obj, 0, iPlr + 1);

  //Trümmer verschleudern
  var debris = CreateObject(DBRS, 4575,560, iPlr);
  Fling(debris, RandomX(-3,-6), RandomX(-2,1));
  debris = CreateObject(DBRS, 4575,620, iPlr);
  Fling(debris, RandomX(-3,-6), RandomX(-2,1));
  debris = CreateObject(DBRS, 4575,680, iPlr);
  Fling(debris, RandomX(-3,-6), RandomX(-2,1));
  debris = CreateObject(DBRS, 4575,740, iPlr);
  Fling(debris, RandomX(-3,-6), RandomX(-2,1));

  //Leiter
  CreateObject(LADR, 4485, 869, -1)->Set(9);

  //Geschützstellung
  var station = CreateObject(GNET, 4665, 610, -1);
  station->Set(0,-90,1);
  station->FadeIn();

  //Schutt
  var derb = CreateObject(DEB1, 4570, 791, -1);
  derb->AddSmokeEffect4K(60,-2,-5,true,this);
  derb->SetDir(1);
}

/* Bei Flaggenübernahme */

func FlagCaptured(object pPoint, int iTeam)
{
  if(pPoint == aFlag[0])
   if(aSelfDefense[0])
    aSelfDefense[0]->SetTeam(iTeam);

  if(pPoint == aFlag[4])
   if(aSelfDefense[1])
    aSelfDefense[1]->SetTeam(iTeam);
}

/* Sturmerstellung */

func FormStorm()
{
  //Sturm erstellen
  CreateObject(STRM);

  //Dunkelheit erhöhen
  if(GetDarkness() < 3)
   FadeDarkness(3,60);

  //Hintergründe verdunkeln
  var back = FindObjects(Find_Func("IsDeco"));
  for (var pObj in back)
   pObj->SetClrModulation(RGBa(50,50,50,5));

  //Lichter einschalten
  LightsOn();
}

/* Lichtverwaltung */

func LightsOff()
{
  aLights = [];
  for(var light in FindObjects(Find_Func("IsLamp"), Sort_Func("Lamp_SortX")))
    light->~TurnOff();
}

func LightsOn()
{
  aLights = FindObjects(Find_Func("IsLamp"), Sort_Func("Lamp_SortX"));
  iLightsCounter = 0;
  if(aLights[iLightsCounter])
    Schedule("LightsOnHelper();", 50);
  else
    return -1;
}

global func Lamp_SortX()
{
  return GetX();
}

func LightsOnHelper()
{
  while(!aLights[iLightsCounter])
    if(iLightsCounter >= GetLength(aLights) - 1)
      return;
    else
      iLightsCounter++;

  aLights[iLightsCounter]->~TurnOn();
  aLights[iLightsCounter] = 0;

  Schedule("LightsOnHelper();", 50);
}

/* Regelwähler */

public func ChooserFinished()
{
  inherited();

  //Starttitel und Musikliste zusammenstellen
  SetPlayList("CMC_Base Groove.ogg;CMC_Blue Ressort.ogg;CMC_Firehawk.ogg;CMC_Friendly Unit.ogg;CMC_Getaway.ogg;CMC_Deep Universe.ogg;CMC_Eurocorps.ogg;CMC_Moving Squad.ogg;CMC_Offensive.ogg;CMC_Rock Go On.ogg;CMC_Showtime.ogg;CMC_Slow Motion.ogg;CMC_Striking Force.ogg;CMC_No Good.ogg;CMC_Obsession.ogg;CMC_Your Eyes.ogg");
  Music("CMC_Blue Ressort.ogg");

  //Teams abfragen
  var aTeams = [false,false,false,false,false];
  for(var i = 0; i < GetPlayerCount(); i++)
   aTeams[GetPlayerTeam(GetPlayerByIndex(i))] = true;

  //Zeitverzögertes Gewitter
  Schedule("GameCall(\"FormStorm\")", RandomX(6000,10000));

  //OP-Spielziel
  if(FindObject(GOCC))
  {
   //Flaggenposten
   aFlag[0] = CreateObject(OFPL,2430,970,NO_OWNER);
   aFlag[0] -> AddSpawnPoint(1930,940);
   aFlag[0] -> AddSpawnPoint(1980,830);
   aFlag[0] -> AddSpawnPoint(2030,960);
   if(aTeams[1] == true)
   {
    aFlag[0]->Set("$Flag1$",100,2);
    aFlag[0]->Capture(1,1);
   }
   else
   {
    aFlag[0]->Set("$Flag1$",0,2);
   }

   aFlag[1] = CreateObject(OFPL,3180,930,NO_OWNER);
   aFlag[1] -> AddSpawnPoint(2860,860);
   aFlag[1] -> AddSpawnPoint(2860,950);
   aFlag[1] -> AddSpawnPoint(2955,950);
   aFlag[1]->Set("$Flag2$",0,2);

   aFlag[2] = CreateObject(OFPL,4245,790,NO_OWNER);
   aFlag[2] -> AddSpawnPoint(4110,890);
   aFlag[2] -> AddSpawnPoint(4170,890);
   aFlag[2] -> AddSpawnPoint(4320,890);
   aFlag[2] -> AddSpawnPoint(4380,890);
   aFlag[2]->Set("$Flag3$",0,2);

   aFlag[3] = CreateObject(OFPL,5310,930,NO_OWNER);
   aFlag[3] -> AddSpawnPoint(5535,950);
   aFlag[3] -> AddSpawnPoint(5630,950);
   aFlag[3] -> AddSpawnPoint(5630,860);
   aFlag[3]->Set("$Flag4$",0,2);

   aFlag[4] = CreateObject(OFPL,6220,970,NO_OWNER);
   aFlag[4] -> AddSpawnPoint(6620,960);
   aFlag[4] -> AddSpawnPoint(6675,830);
   aFlag[4] -> AddSpawnPoint(6720,940);
   if(aTeams[2] == true)
   {
    aFlag[4]->Set("$Flag5$",100,2);
    aFlag[4]->Capture(2,1);
   }
   else
   {
    aFlag[4]->Set("$Flag4$",0,2);
   }

   //Selbstschussanlagen
   aSelfDefense[0] = CreateObject(SEGU, 2155, 930, -1);
   aSelfDefense[0]->Set(0,1,1,180,0,2620);
   aSelfDefense[0]->SetTeam(1);
   aSelfDefense[1] = CreateObject(SEGU, 6495, 930, -1);
   aSelfDefense[1]->Set(0,1,1,180,5870);
   aSelfDefense[1]->SetTeam(2);

   //Geschützstellungen
   CreateObject(GNET, 2040, 870, -1)->Set(0,90,1);
   CreateObject(GNET, 2475, 970, -1)->Set(0,90,1);
   CreateObject(GNET, 6170, 970, -1)->Set(0,-90,1);
   CreateObject(GNET, 6610, 870, -1)->Set(0,-90,1);

   //Grenzen setzen
   CreateObject(BRDR, 1115, 0, -1)->Set(0);
   CreateObject(BRDR, 7535, 0, -1)->Set(1);

   //Hinweisschilder
   CreateObject(SGNP, 1130, 1010, -1);
   CreateObject(SGNP, 7520, 1010, -1);

   //Patrouillenboote
   SetupVehicleSpawn([PBOT],DIR_Right,CreateObject(VSPW,2250,1009,-1),50*21);
   SetupVehicleSpawn([PBOT],DIR_Right,CreateObject(VSPW,2360,1009,-1),50*21);
   SetupVehicleSpawn([PBOT],DIR_Right,CreateObject(VSPW,2470,1009,-1),50*21);
   SetupVehicleSpawn([PBOT],DIR_Right,CreateObject(VSPW,2955,951,-1),50*21);
   SetupVehicleSpawn([PBOT],DIR_Right,CreateObject(VSPW,3780,951,-1),50*21);
   SetupVehicleSpawn([PBOT],DIR_Left,CreateObject(VSPW,4710,951,-1),50*21);
   SetupVehicleSpawn([PBOT],DIR_Left,CreateObject(VSPW,5535,951,-1),50*21);
   SetupVehicleSpawn([PBOT],DIR_Left,CreateObject(VSPW,6180,1009,-1),50*21);
   SetupVehicleSpawn([PBOT],DIR_Left,CreateObject(VSPW,6290,1009,-1),50*21);
   SetupVehicleSpawn([PBOT],DIR_Left,CreateObject(VSPW,6400,1009,-1),50*21);
  }

  //Base Assault-Spielziel
  if(FindObject(GBAS))
  {
   //Strukturen
   AddAssaultTarget(CCP2, 2700, 990, 300, 1, "$Flag1$", 0, [[1930,950], [1980,840], [2030,970]]);
   AddAssaultTarget(CMSN, 3500, 1000, 300, 1, "$Flag2$", 1, [[2860,870], [2860,960], [2955,960]]);
   AddAssaultTarget(CCP2, 4990, 1000, 300, 2, "$Flag5$", 0, [[5535,960], [5630,960], [5630,870]]);
   AddAssaultTarget(CMSN, 5790, 990, 300, 2, "$Flag4$", 1, [[6620,970], [6675,840], [6720,950]]);

   //Sprengsatz-Spawn
   SetupBombSpawnpoint([[4245, 790],[4245,900]]);

   //Selbstschussanlagen
   var selfdef = CreateObject(SEGU, 2155, 930, -1);
   selfdef->Set(0,1,1,180,0,2620);
   selfdef->SetTeam(1);
   selfdef = CreateObject(SEGU, 6495, 930, -1);
   selfdef->Set(0,1,1,180,5870);
   selfdef->SetTeam(2);

   //Geschützstellungen
   CreateObject(GNET, 2040, 870, -1)->Set(0,90,1);
   CreateObject(GNET, 2475, 970, -1)->Set(0,90,1);
   CreateObject(GNET, 6170, 970, -1)->Set(0,-90,1);
   CreateObject(GNET, 6610, 870, -1)->Set(0,-90,1);

   //Grenzen setzen
   CreateObject(BRDR, 1115, 0, -1)->Set(0);
   CreateObject(BRDR, 7535, 0, -1)->Set(1);

   //Boden
   DrawMaterialQuad("BackWall-Stripes", 3480,1000, 3520,1000, 3520,1010, 3480,1010);
   DrawMaterialQuad("BackWall-Stripes", 4970,1000, 5010,1000, 5010,1010, 4970,1010);

   //Hinweisschilder
   CreateObject(SGNP, 1130, 1010, -1);
   CreateObject(SGNP, 7520, 1010, -1);

   //Patrouillenboote
   SetupVehicleSpawn([PBOT],DIR_Right,CreateObject(VSPW,2250,1009,-1),50*21);
   SetupVehicleSpawn([PBOT],DIR_Right,CreateObject(VSPW,2360,1009,-1),50*21);
   SetupVehicleSpawn([PBOT],DIR_Right,CreateObject(VSPW,2470,1009,-1),50*21);
   SetupVehicleSpawn([PBOT],DIR_Right,CreateObject(VSPW,2955,951,-1),50*21);
   SetupVehicleSpawn([PBOT],DIR_Right,CreateObject(VSPW,3780,951,-1),50*21);
   SetupVehicleSpawn([PBOT],DIR_Left,CreateObject(VSPW,4710,951,-1),50*21);
   SetupVehicleSpawn([PBOT],DIR_Left,CreateObject(VSPW,5535,951,-1),50*21);
   SetupVehicleSpawn([PBOT],DIR_Left,CreateObject(VSPW,6180,1009,-1),50*21);
   SetupVehicleSpawn([PBOT],DIR_Left,CreateObject(VSPW,6290,1009,-1),50*21);
   SetupVehicleSpawn([PBOT],DIR_Left,CreateObject(VSPW,6400,1009,-1),50*21);
  }

  //HTF-Spielziel
  if(FindObject(GHTF))
  {
   //Flaggenposten
   var flag = CreateObject(OFPL, 4245, 790, -1);
   flag->~Set("$Flag3$");

   //Teamgrenzen
   CreateObject(BRDR, 3710, 0, -1)->Set(0,1,0,1,1);
   CreateObject(BRDR, 4780, 0, -1)->Set(1,1,0,1,2);

   //Selbstschussanlagen
   var selfdef = CreateObject(SEGU, 3100, 810, -1);
   selfdef->Set(0,1,1,180,0,3620);
   selfdef->SetTeam(1);
   selfdef = CreateObject(SEGU, 5390, 810, -1);
   selfdef->Set(0,1,1,180,4870);
   selfdef->SetTeam(2);

   //Grenzen setzen
   CreateObject(BRDR, 2650, 0, -1)->Set(0);
   CreateObject(BRDR, 5840, 0, -1)->Set(1);

   //Hinweisschilder
   CreateObject(SGNP, 2480, 970, -1);
   CreateObject(SGNP, 6170, 970, -1);

   //Leitern
   CreateObject(LADR, 2825, 990, -1)->Set(3);
   CreateObject(LADR, 5665, 990, -1)->Set(3);

   //Patrouillenboote
   SetupVehicleSpawn([PBOT],DIR_Right,CreateObject(VSPW,2710,1009,-1),50*21);
   SetupVehicleSpawn([PBOT],DIR_Right,CreateObject(VSPW,2800,1009,-1),50*21);
   SetupVehicleSpawn([PBOT],DIR_Right,CreateObject(VSPW,2955,951,-1),50*21);
   SetupVehicleSpawn([PBOT],DIR_Right,CreateObject(VSPW,3780,951,-1),50*21);
   SetupVehicleSpawn([PBOT],DIR_Left,CreateObject(VSPW,4710,951,-1),50*21);
   SetupVehicleSpawn([PBOT],DIR_Left,CreateObject(VSPW,5535,951,-1),50*21);
   SetupVehicleSpawn([PBOT],DIR_Left,CreateObject(VSPW,5690,1009,-1),50*21);
   SetupVehicleSpawn([PBOT],DIR_Left,CreateObject(VSPW,5780,1009,-1),50*21);
  }

  //LMS/DM-Spielziel
  if(FindObject(GLMS) || FindObject(GTDM))
  {
   //Selbstschussanlagen
   var selfdef = CreateObject(SEGU, 3100, 810, -1);
   selfdef->Set(0,1,1,180,0,3620);
   selfdef->SetTeam(1);
   selfdef = CreateObject(SEGU, 5390, 810, -1);
   selfdef->Set(0,1,1,180,4870);
   selfdef->SetTeam(2);

   //Grenzen setzen
   CreateObject(BRDR, 2650, 0, -1)->Set(0);
   CreateObject(BRDR, 5840, 0, -1)->Set(1);

   //Hinweisschilder
   CreateObject(SGNP, 2480, 970, -1);
   CreateObject(SGNP, 6170, 970, -1);

   //Leitern
   CreateObject(LADR, 2825, 990, -1)->Set(3);
   CreateObject(LADR, 5665, 990, -1)->Set(3);

   //Patrouillenboote
   SetupVehicleSpawn([PBOT],DIR_Right,CreateObject(VSPW,2710,1009,-1),50*21);
   SetupVehicleSpawn([PBOT],DIR_Right,CreateObject(VSPW,2800,1009,-1),50*21);
   SetupVehicleSpawn([PBOT],DIR_Right,CreateObject(VSPW,2955,951,-1),50*21);
   SetupVehicleSpawn([PBOT],DIR_Right,CreateObject(VSPW,3780,951,-1),50*21);
   SetupVehicleSpawn([PBOT],DIR_Left,CreateObject(VSPW,4710,951,-1),50*21);
   SetupVehicleSpawn([PBOT],DIR_Left,CreateObject(VSPW,5535,951,-1),50*21);
   SetupVehicleSpawn([PBOT],DIR_Left,CreateObject(VSPW,5690,1009,-1),50*21);
   SetupVehicleSpawn([PBOT],DIR_Left,CreateObject(VSPW,5780,1009,-1),50*21);
  }
}

/* Relaunch */

public func RelaunchPosition(& iX, & iY, int iTeam)
{
  //Startsicht
  if(!g_chooserFinished)
  {
   iX = 4245; iY = 860;
   return(1);
  }

  //HTF/LMS/DM-Spielziel
  if(FindObject(GHTF) || FindObject(GLMS) || FindObject(GTDM))
  {
   if(iTeam == 1)
   {
    return [[2860, 860], [2860, 950], [2955, 950]];
   }
   if(iTeam == 2)
   {
    return [[5535, 950], [5630, 950], [5630, 860]];
   }
   return(1);
  }
}