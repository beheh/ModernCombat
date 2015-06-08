/*-- Lobby --*/

#strict 2

#include TSYS

public func UsedRules() { return [NOFD, NOFF, NOAM]; } 
public func SectionName() { return "IDQuest"; } 
public func HideInMenu() { return true; } 
public func IsCMCMinigame() { return true; }

local container, plrscores;
local triggers;
local definitionlist;

public func Initialize()
{  
	SetPlayList("@CMC_Blue Ressort.ogg");

	container = CreateObject(TIM1, 170, 220, NO_OWNER);
  g_Gamemaster->Enter(container);
  plrscores = [];
  triggers = [];
  
  FakeScriptGo(true);
  
	definitionlist = [BRDR, RSPT, VSPW, CIVC, MDIC, PCMK, HBAR, ABAR, BOOM, LHC2, BHUL, ELGT, DOFO, SE4K, STRM, WRAE, GASS, CASS, AHBS, EBR2, GBAS, GHTF, GMNR, MNYS,
	GOCC, OSPW, OFPL, OFLG, SM02, SM03, TK01, TK02, TK03, TK04, TK05, TK06, TK07, TK08, TK09, MCAM, ABOX, GBOX, MBOX, CUAM, STAM, GRAM, MIAM, PPAR, PARA, FGRN, FRAG,
	SHRP, NADE, GRNS, PGRN, PSPR, SRBL, SM08, SM09, SGRN, SM4K, STUN, SM07, AMPK, BBTP, C4PA, C4EX, DGNN, FAPK, PACK, RSHL, RSLH, CDBT, ATWN, AAMS, ASTR, ESHL, FSHL,
	SSHL, MNGN, PSTL, SHTX, TRLX, TRDT, RVLR, RTLR, MISL, PPGN, SGST, SMGN, WPN2, CSTR, CVHC, CSTD, EI4K, IDSP, IHLT, CRH2, 1HUD, RBAR, HCHA, KILL, SM10, SM04, SM05,
	FKDT, SM06, SM01, SMEN, SMIN, S24K, MCSL, EFMN, FDMG, LIMT, LSGT, SHLP, NOBH, NOFD, NMTX, RWDS, ACHV, AC01, AC02, AC03, AC04, AC05, AC06, AC07, AC08, AC09, AC10,
	AC11, AC12, AC13, AC14, AC15, AC16, AC17, AC18, AC19, AC20, AC21, AC22, AC23, AC24, AC25, AC26, AC27, AC28, AC29, AC30, AC31, ARHL, IC01, IC02, IC03, IC04, IC05,
	IC06, IC07, IC08, IC09, IC10, IC11, IC12, IC13, IC14, IC15, IC16, IC17, IC18, IC19, IC20, RG00, RG01, RG02, RG03, RG04, RG05, RG06, RG07, RG08, RG09, RG10, RG11,
	RG12, RG13, RG14, RG15, RG16, RG17, RG18, RG19, RG20, RG21, RG22, RG23, RG24, RG25, RG26, RG27, RG28, RG29, RG30, PBAR, SBUL, SICD, BOTL, MNI2, _WIN, XBRL, GSBL,
	PBRL, HBRL, TBRL, CCAR, CON1, FENC, PLLT, SBBA, SADH, SFFG, BECR, BDGE, XWCR, MWCR, XTNK, PTNK, ATBY, CNON, ARCR, ABLT, CONS, DGND, SEGU, MISA, AMCT, WPVM, CMSN,
	FAUD, _JW1, _JW2, MUMT, PLT3, RDIO, SCA1, SCA2, VGMN, VEN3, ETLT, SLGH, BDOR, GAT1, ROOM, ROM2, GDDR, HNG3, H24K, GBDR, HA4K, MRMP, _HBR, _HBB, BUOY, OLGH, MSGN,
	SGNP, SNPT, TCCN, WLSN, BSH2, STNE, BKHK, FLRE, BHUD, BARW, BRTN, CK5P, WNRK, ACCN, RLSA, LRML, BHWK, PBOT, OBMT, SPBT, BPBT];
	
	InitializeScoreboard();
	
  return _inherited(...);
}

public func InitializeScoreboard()
{
	SetScoreboardData(SBRD_Caption, 0, "$Points$");
	for(var i = 0; i < GetPlayerCount(C4PT_User); i++)
	{
		var plr = GetPlayerByIndex(i, C4PT_User);
		SetScoreboardData(GetPlayerID(plr)-1, SBRD_Caption, GetTaggedPlayerName(plr));
		SetScoreboardData(GetPlayerID(plr)-1, 0, "0", plrscores[plr]);
	}
	
	DoScoreboardShow(true);
}

public func InitializePlayer(int iPlr)
{
	SetFoW(false, iPlr);
	if(GetType(arRespawn[iPlr]) != C4V_Array)
		arRespawn[iPlr] = [];
	
	SetRespawnPosition(10, 10, iPlr);
	
	return _inherited(iPlr, ...);
}

public func OnClonkRelaunch(object pCrew) 
{ 
	return pCrew->Enter(CreateObject(TIM1, 3040, 1100, GetOwner(pCrew)));
}

public func CreateTriggers()
{
	BindTrigger(triggers[0] = CreateObject(TIM1, 5420, 1060, -1), TRIGGER_Find, [[Find_ID(BKHK), triggers[0]->Find_Distance(100)]], "FoundBlackhawk", this);
	BindTrigger(triggers[1] = CreateObject(TIM1, 6510,  780, -1), TRIGGER_Find, [[Find_ID(BKHK), triggers[1]->Find_Distance(100)]], "FoundBlackhawk", this);
	BindTrigger(triggers[2] = CreateObject(TIM1, 4835,  250, -1), TRIGGER_Find, [[Find_ID(BKHK), triggers[2]->Find_Distance(100)]], "FoundBlackhawk", this);
	BindTrigger(triggers[3] = CreateObject(TIM1, 2090,  200, -1), TRIGGER_Find, [[Find_ID(BKHK), triggers[3]->Find_Distance(100)]], "FoundBlackhawk", this);
	BindTrigger(triggers[4] = CreateObject(TIM1,  835, 1010, -1), TRIGGER_Find, [[Find_ID(BKHK), triggers[4]->Find_Distance(100)]], "FoundBlackhawk", this);
	BindTrigger(triggers[5] = CreateObject(TIM1, 1390,  765, -1), TRIGGER_Find, [[Find_ID(BKHK), triggers[5]->Find_Distance(100)]], "FoundBlackhawk", this);
	BindTrigger(triggers[6] = CreateObject(TIM1, 2960, 1140, -1), TRIGGER_Find, [[Find_ID(BKHK), triggers[6]->Find_Distance(100)]], "FoundBlackhawk", this);
	BindTrigger(triggers[7] = CreateObject(TIM1, 3515,  720, -1), TRIGGER_Find, [[Find_ID(BKHK), triggers[7]->Find_Distance(100)]], "FoundBlackhawk", this);
	BindTrigger(triggers[8] = CreateObject(TIM1, 3900,  430, -1), TRIGGER_Find, [[Find_ID(BKHK), triggers[8]->Find_Distance(100)]], "FoundBlackhawk", this);
  return true;
}

public func FoundBlackhawk(array arBlackhawks, object pTrigger)
{
	for(var blackhawk in arBlackhawks)
	{
		var trigger = triggers[Random(GetLength(triggers))];
		if(trigger != pTrigger && PathFree(GetX(blackhawk), GetY(blackhawk), GetX(trigger), GetY(trigger)))
			blackhawk->SetAutopilot(0, GetX(trigger), GetY(trigger));
		else
			arBlackhawks[GetLength(arBlackhawks)] = blackhawk;
	}
	
	return true;
}

public func LoadingFinished()
{
	Wait(35*5, -1, true, "StartGame()");
	
	return true;
}

public func StartGame()
{
	CreateQuestion(-1);
	return true;
}

local definition;

public func CreateQuestion()
{
	AddEffect("CMCQuestion", this, 101, 1, this);

	/*if(iPlr == -1)
	{
		definition = GetDefinition(Random(g_Defcount));
		for(var i = 0; i < GetPlayerCount(); i++)
		{
			iPlr = GetPlayerByIndex(i);
			CreateMenu(NULL, GetCursor(iPlr), this, 0, 0, 0, C4MN_Style_Dialog);
			AddMenuItem(Format("%s", GetName(0, definition)), 0, definition, GetCursor(iPlr));
			AddMenuItem("$Answer$", "AnswerQuestion", CHOS, GetCursor(iPlr), 0, iPlr);
			
			StartTimer(35*20, "EvalQuestion", this, false);
		}
	}
	else
	{
		CreateMenu(NULL, GetCursor(iPlr), this, 0, 0, 0, C4MN_Style_Dialog);
		AddMenuItem(Format("%s", GetName(0, definition)), 0, definition, GetCursor(iPlr));
		AddMenuItem("$Answer$", "AnswerQuestion", CHOS, GetCursor(iPlr), 0, iPlr);
	}*/
}

public func FxCMCQuestionStart(object target, int nr)
{
	definition = definitionlist[Random(GetLength(definitionlist))];
	EffectVar(1, target, nr) = [];
	EffectCall(target, nr, "Menu");
	//Log("Definition: %i", definition);
	
	return true;
}

public func FxCMCQuestionTimer(object target, int nr)
{
	++EffectVar(0, target, nr);
	if(EffectVar(0, target, nr) > 35*30)
		return -1;
	
	if(!(EffectVar(0, target, nr) % 35))
	{
		EffectCall(target, nr, "Menu");
		/*for(var i = 0; i < GetPlayerCount(); i++)
		{
			var iPlr = GetPlayerByIndex(i);
			CreateMenu(definition, GetCursor(iPlr), this, 0, 0, 0, C4MN_Style_Dialog);
			AddMenuItem(Format("Name des Objektes: %s|Verbleibende Zeit: %d", GetName(0, definition), (35*30-EffectVar(0, target, nr))), 0, definition, GetCursor(iPlr));
			AddMenuItem("$Answer$", "AnswerQuestion", CHOS, GetCursor(iPlr), 0, iPlr);
			
			//StartTimer(35*20, "EvalQuestion", this, false);
		}*/
	}
}

public func FxCMCQuestionMenu(object target, int nr)
{
	for(var i = 0; i < GetPlayerCount(); i++)
	{
		var iPlr = GetPlayerByIndex(i);
		CreateMenu(definition, GetCursor(iPlr), this, 0, 0, 0, C4MN_Style_Dialog, true);
		AddMenuItem(Format("$Question$", GetName(0, definition), (35*30-EffectVar(0, target, nr))/35), 0, definition, GetCursor(iPlr));
		if(GetIndexOf(iPlr, EffectVar(1, target, nr)) == -1)
			AddMenuItem("$Answer$", "AnswerQuestion", CHOS, GetCursor(iPlr), 0, iPlr);
		else
			AddMenuItem("$AnsweredRight$", 0, CHOS, GetCursor(iPlr), 0, iPlr);
			
		//StartTimer(35*20, "EvalQuestion", this, false);
	}
	
	return true;
}

static const MIDQ_MaxScore = 10;

public func FxCMCQuestionStop(object target, int nr)
{
	var plrs = EffectVar(1, target, nr);
	for(var plr in plrs)
		plrscores[plr]++;
	
	EventInfo4K(0, Format("$TheRightAnswerWas$", definition));
	
	if(GetIndexOf(MIDQ_MaxScore, plrscores) > -1)
		return EvalRound();
	
	return ScheduleCall(this, "CreateQuestion", 1);
}

public func FxCMCQuestionAnswerRight(object target, int nr, int plr)
{
	EffectVar(1, target, nr)[GetLength(EffectVar(1, target, nr))] = plr;
	Sound("Applause", true, 0, 100, plr);
	EventInfo4K(0, Format("$HasAnsweredRight$", GetTaggedPlayerName(plr)));
	SetScoreboardData(GetPlayerID(plr)-1, 0, Format("%d", plrscores[plr]+1), plrscores[plr]+1);
	SortScoreboard(0);
	
	if(GetLength(EffectVar(1, target, nr)) == GetPlayerCount(C4PT_User))
		return -1;
	
	return EffectCall(target, nr, "Menu");
}

public func AnswerQuestion(dummy, int iPlr)
{
	CallMessageBoard(this, true, " ID: ", iPlr);
}

public func InputCallback(string szID, int plr)
{
	if(szID == Format("%i", definition))
		return EffectCall(this, GetEffect("CMCQuestion", this), "AnswerRight", plr);
	
	return EffectCall(this, GetEffect("CMCQuestion", this), "Menu");
}

public func EvalRound()
{
	for(var i = 0; i < GetPlayerCount(); i++)
		SetFoW(true, GetPlayerByIndex(i));

	var players = [];
	for(var plr = 0; plr < GetLength(plrscores); plr++)
		if(plrscores[plr] >= 10)
			players[GetLength(players)] = plr;
	
	g_aLastRoundWinners = players;
	return LoadMinigame(MLBW);
}

public func Script1()
{
	if(ObjectCount(BKHK) < 5)
	{
		while(ObjectCount(BKHK) < 5)
		{
			var trigger = triggers[Random(GetLength(triggers))];
			var bkhk = CreateObject(BKHK, GetX(trigger), GetY(trigger), -1);
			bkhk->MGStartBlackhawk(Random(2), 100, 0);
		}
	}
	
	return FakeGoto(0);
}






////////////////////////////////////////////////////////////////////////////////







func CreateInterior()
{
  Log("$CreatingInterior$");

  //Aufzug
  CreateObject(LFTP, 1535, 1270, -1);

  //Leitern
  CreateObject(LADR, 1255, 1260, -1)->Set(14);
  CreateObject(LADR, 1480, 1260, -1)->Set(28);
  CreateObject(LADR, 1590, 1260, -1)->Set(28);
  CreateObject(LADR, 1720, 1250, -1)->Set(9);
  CreateObject(LADR, 1880, 1258, -1)->Set(10);
  CreateObject(LADR, 2040, 1258, -1)->Set(10);
  CreateObject(LADR, 2200, 1258, -1)->Set(10);
  CreateObject(LADR, 2395, 1260, -1)->Set(14);

  CreateObject(LADR, 3530, 1260, -1)->Set(17);
  CreateObject(LADR, 3635, 1055, -1)->Set(5);
  CreateObject(LADR, 3910, 640, -1)->Set(7);
  CreateObject(LADR, 4240, 795, -1)->Set(26);
  CreateObject(LADR, 4380, 805, -1)->Set(7);
  CreateObject(LADR, 4650, 1070, -1)->Set(19);
  CreateObject(LADR, 5425, 1267, -1)->Set(4);
  CreateObject(LADR, 5575, 1267, -1)->Set(4);

  //Bodenluken
  CreateObject(HA4K, 1480, 1033, -1);
  CreateObject(HA4K, 1480, 1113, -1);
  CreateObject(HA4K, 1480, 1173, -1);
  CreateObject(HA4K, 1590, 1033, -1);
  CreateObject(HA4K, 1590, 1113, -1);
  CreateObject(HA4K, 1590, 1173, -1);
  CreateObject(HA4K, 1720, 1173, -1);
  CreateObject(HA4K, 1880, 1173, -1);
  CreateObject(HA4K, 2040, 1173, -1);
  CreateObject(HA4K, 2200, 1173, -1);

  //Rampen
  DrawMaterialQuad("Wall-Plate",1300,1321,1300,1361,1260,1321,1300,1321,true);

  DrawMaterialQuad("Wall-PlateBlue",1341,1150,1341,1140,1371,1150,1341,1150,true);
  DrawMaterialQuad("Wall-Plate",1371,1160,1371,1150,1401,1160,1371,1160,true);
  DrawMaterialQuad("Wall-PlateBlue",1401,1171,1401,1161,1431,1171,1401,1171,true);

  DrawMaterialQuad("Wall-Metal2",1371,1300,1371,1290,1341,1300,1371,1300,true);
  DrawMaterialQuad("Wall-Metal2",1400,1290,1400,1280,1370,1290,1400,1290,true);
  DrawMaterialQuad("Wall-Plate",1430,1280,1430,1270,1400,1280,1430,1280,true);
  DrawMaterialQuad("Wall-Plate",1460,1270,1460,1260,1430,1270,1460,1270,true);

  DrawMaterialQuad("Wall-Plate",1731,1270,1731,1260,1761,1270,1731,1270,true);
  DrawMaterialQuad("Wall-Plate",1761,1281,1761,1271,1791,1281,1761,1281,true);
  DrawMaterialQuad("Wall-Metal2",1791,1291,1791,1281,1821,1291,1791,1291,true);
  DrawMaterialQuad("Wall-Metal2",1821,1301,1821,1291,1851,1301,1821,1301,true);

  DrawMaterialQuad("Wall-PlateBlue",2252,1170,2252,1160,2222,1170,2252,1170,true);
  DrawMaterialQuad("Wall-Plate",2280,1160,2280,1150,2250,1160,2280,1160,true);
  DrawMaterialQuad("Wall-PlateBlue",2311,1150,2311,1140,2281,1150,2311,1150,true);

  DrawMaterialQuad("Wall-Plate",2231,1321,2231,1361,2391,1321,2231,1321,true);

  //Glasscheiben
  CreateObject(_WIN, 1432, 1081, -1);
  CreateObject(_WIN, 1432, 1110, -1);
  CreateObject(_WIN, 1638, 1081, -1);
  CreateObject(_WIN, 1638, 1110, -1);

  //Kisten
  CreateObject(WCR2, 1690, 1170, -1)->AutoRespawn();
  CreateObject(WCR2, 1690, 1260, -1)->AutoRespawn();

  //Metallkisten
  CreateObject(MWCR, 4650, 1220, -1)->AutoRespawn();
  CreateObject(MWCR, 4050, 910, -1)->AutoRespawn();

  //Verbandskisten
  CreateObject(BECR, 1670, 1260, -1)->AutoRespawn();
  CreateObject(BECR, 4120, 580, -1)->AutoRespawn();
  CreateObject(BECR, 5190, 1240, -1)->AutoRespawn();

  //Explosive Kisten
  CreateObject(XWCR, 1845, 1170, -1)->AutoRespawn();
  CreateObject(XWCR, 3605, 868, -1)->AutoRespawn();

  //Benzinfässer
  CreateObject(PBRL, 1620, 1260, -1)->AutoRespawn();
  CreateObject(PBRL, 3660, 848, -1)->AutoRespawn();
  CreateObject(PBRL, 4505, 938, -1)->AutoRespawn();

  //Explosivfässer
  CreateObject(XBRL, 1600, 1260, -1)->AutoRespawn();
  CreateObject(XBRL, 3280, 1208, -1)->AutoRespawn();
  CreateObject(XBRL, 4605, 918, -1)->AutoRespawn();

  //Gasflaschen
  CreateObject(GSBL, 1635, 1030, -1)->AutoRespawn();
  CreateObject(GSBL, 4220, 818, -1)->AutoRespawn();
  CreateObject(GSBL, 5160, 1238, -1)->AutoRespawn();

  //Gerüste
  CreateObject(SFFG, 4715, 1240, -1)->Set(5);
  CreateObject(SFFG, 5470, 1280, -1)->Set(4);
  var plat = CreateObject(SFFG, 5470, 1330, -1);
  plat->Set(5);
  plat->SetClrModulation(RGB(125,125,250));
  CreateObject(SFFG, 5530, 1280, -1)->Set(4);
  plat = CreateObject(SFFG, 5530, 1330, -1);
  plat->Set(5);
  plat->SetClrModulation(RGB(125,125,250));

  //Haie
  CreateObject(SHRK, 400, 1500, -1)->AutoRespawn();
  CreateObject(SHRK, 1000, 1400, -1)->AutoRespawn();
  CreateObject(SHRK, 1200, 1400, -1)->AutoRespawn();
  CreateObject(SHRK, 2400, 1400, -1)->AutoRespawn();
  CreateObject(SHRK, 2700, 1400, -1)->AutoRespawn();
  CreateObject(SHRK, 5600, 1400, -1)->AutoRespawn();
  CreateObject(SHRK, 5700, 1400, -1)->AutoRespawn();
  CreateObject(SHRK, 6400, 1500, -1)->AutoRespawn();

  //Piranha
  CreateObject(PIRA, 990, 1440, -1)->AutoRespawn();
  CreateObject(PIRA, 1240, 1440, -1)->AutoRespawn();
  CreateObject(PIRA, 2420, 1440, -1)->AutoRespawn();
  CreateObject(PIRA, 2700, 1440, -1)->AutoRespawn();
  CreateObject(PIRA, 3910, 1290, -1)->AutoRespawn();
  CreateObject(PIRA, 5660, 1420, -1)->AutoRespawn();
  CreateObject(PIRA, 5810, 1420, -1)->AutoRespawn();

  //Container
  CreateObject(CON1, 1780, 1170, -1);
  CreateObject(CON1, 1820, 1171, -1)->SetPerspective(2);
  CreateObject(CON1, 4530, 940, -1)->SetPerspective(2);

  //Hydrauliktüren
  CreateObject(SEDR,1459, 1030,-1);
  CreateObject(SEDR,1459, 1170,-1);
  CreateObject(SEDR,1611, 1030,-1);
  CreateObject(SEDR,1611, 1170,-1);

  //!!!
/*
  //Sandsackbarrieren
  CreateObject(SBBA, 1380, 440, -1)->Right();
  CreateObject(SBBA, 1690, 440, -1)->Right();
  CreateObject(SBBA, 2660, 340, -1);
  CreateObject(SBBA, 3800, 410, -1)->Right();
  CreateObject(SBBA, 4204, 409, -1);
  CreateObject(SBBA, 5075, 400, -1);
  CreateObject(SBBA, 5165, 400, -1)->Right();
  CreateObject(SBBA, 5265, 400, -1);
  CreateObject(SBBA, 5355, 400, -1)->Right();
  CreateObject(SBBA, 5780, 350, -1)->Right();
  CreateObject(SBBA, 6340, 620, -1)->Right();
  CreateObject(SBBA, 6490, 340, -1)->Right();

*/

  //Explosivtanks
  CreateObject(XTNK, 1480, 1260, -1)->AutoRespawn();
  CreateObject(XTNK, 1590, 1260, -1)->AutoRespawn();

  //!!!
/*

  //Phosphortanks
  CreateObject(PTNK, 1635, 440, -1)->AutoRespawn();
  CreateObject(PTNK, 2855, 410, -1)->AutoRespawn();
  CreateObject(PTNK, 6295, 410, -1)->AutoRespawn();

*/

  //Tore und Konsolen
  var autod = CreateObject (HNG3, 1433, 1110, -1);
  autod->Open();
  CreateObject (CONS, 1500, 1105, -1)->Set(autod);

  autod = CreateObject (HNG3, 1637, 1110, -1);
  autod->Open();
  CreateObject (CONS, 1570, 1105, -1)->Set(autod);

  //Selbstschussanlagen
  var aSelfDefense = [];
  aSelfDefense[0] = CreateObject(SEGU, 1310, 1141, -1);
    aSelfDefense[0]->Arm(MISA);
    aSelfDefense[0]->TurnOn();
    aSelfDefense[0]->SetR(180);
    CreateObject(CONS, 1420, 1025, -1)->Set(aSelfDefense[0]);

  aSelfDefense[1] = CreateObject(SEGU, 2340, 1141, -1);
    aSelfDefense[1]->Arm(MISA);
    aSelfDefense[1]->TurnOn();
    aSelfDefense[1]->SetR(180);
    CreateObject(CONS, 1650, 1025, -1)->Set(aSelfDefense[1]);

  //Seegras und Muscheln entfernen
  for(var obj in FindObjects(Find_Or(Find_ID(SWD1), Find_ID(SWD2), Find_ID(SHEL)), Find_InRect(1250, 1270, 1150, 35)))
    RemoveObject(obj);

  //Sounds

  //Wind
  CreateObject(SE4K, 1100, 300, -1)->Set("WindSound*.ogg",3000,105);
  CreateObject(SE4K, 2100, 300, -1)->Set("WindSound*.ogg",3000,105);
  CreateObject(SE4K, 3100, 300, -1)->Set("WindSound*.ogg",3000,105);
  CreateObject(SE4K, 4100, 300, -1)->Set("WindSound*.ogg",3000,105);
  CreateObject(SE4K, 5100, 300, -1)->Set("WindSound*.ogg",3000,105);
  CreateObject(SE4K, 6100, 300, -1)->Set("WindSound*.ogg",3000,105);

  //Motor
  CreateObject(SE4K, 1400, 1360, -1)->Set("tank",0,0);

  //Hallen
  CreateObject(SE4K, 1535, 1150, -1)->Set("Interior*.ogg",665,105);
  CreateObject(SE4K, 2070, 1240, -1)->Set("Interior*.ogg",665,105);

  //Möven
  CreateObject(SE4K, 840, 1150, -1)->Set("SeaSounds*.ogg",140,35);
  CreateObject(SE4K, 3210, 1150, -1)->Set("SeaSounds*.ogg",140,35);
  CreateObject(SE4K, 3400, 750, -1)->Set("SeaSounds*.ogg",140,35);
  CreateObject(SE4K, 4670, 850, -1)->Set("SeaSounds*.ogg",140,35);
  CreateObject(SE4K, 5180, 1140, -1)->Set("SeaSounds*.ogg",140,35);
  CreateObject(SE4K, 6030, 1150, -1)->Set("SeaSounds*.ogg",140,35);

  //Vögel
  CreateObject(SE4K, 3600, 810, -1)->Set("BirdSong*",140,35);
  CreateObject(SE4K, 4410, 780, -1)->Set("BirdSong*",140,35);

  //Wipfe
  CreateObject(SE4K, 4940, 1150, -1)->Set("Snuff*",900,100);

  //Erdrutsche
  CreateObject(SE4K, 4000, 1480, -1)->Set("FallingDirt*.ogg",950,250);

  //Wellen
  CreateObject(SE4K, 400, 1270, -1)->Set("Wave*.ogg",105,35);
  CreateObject(SE4K, 1070, 1270, -1)->Set("Wave*.ogg",105,35);
  CreateObject(SE4K, 2780, 1270, -1)->Set("Wave*.ogg",105,35);
  CreateObject(SE4K, 5450, 1270, -1)->Set("Wave*.ogg",105,35);
  CreateObject(SE4K, 6530, 1270, -1)->Set("Wave*.ogg",105,35);

  //Jungel
  CreateObject(SE4K, 3290, 1140, -1)->Set("Jungle*.ogg",140,70);
  CreateObject(SE4K, 3660, 800, -1)->Set("Jungle*.ogg",140,70);
  CreateObject(SE4K, 4130, 550, -1)->Set("Jungle*.ogg",140,70);
  CreateObject(SE4K, 4900, 1100, -1)->Set("Jungle*.ogg",140,70);

  //Lautsprecher
  CreateObject(SE4K, 1800, 1060, -1)->Set("Announce*.ogg",3000,200);
}

func CreateEquipment()
{
  Log("$CreatingEquipment$");

  //Versorgungskisten (Kugeln)
  var crate = CreateObject(AMCT, 1770, 1142, -1);
  crate->Set(ABOX);
  crate = CreateObject(AMCT, 3790, 790, -1);
  crate->Set(ABOX);
  crate = CreateObject(AMCT, 4530, 1220, -1);
  crate->Set(ABOX);

  //Versorgungskisten (Gewehrgranaten)
  var crate = CreateObject(AMCT, 1820, 1142, -1);
  crate->Set(GBOX);
  crate = CreateObject(AMCT, 4140, 770, -1);
  crate->Set(GBOX);
  crate = CreateObject(AMCT, 4650, 1090, -1);
  crate->Set(GBOX);

  //Raketen
  PlaceSpawnpoint(MBOX, 3440, 1125);
  PlaceSpawnpoint(MBOX, 4560, 935);

  //Versorgungskisten (APW)
  var crate = CreateObject(AMCT, 3490, 1120, -1);
  crate->Set(ATWN);
  var crate = CreateObject(AMCT, 4360, 820, -1);
  crate->Set(ATWN);

  //!!!
/*

  //Artilleriebatterien
  aArtillery[0] = CreateObject(ATBY,1025,210,-1);
  aArtillery[0]->SetRotation(20);
  aArtillery[1] = CreateObject(ATBY,3640,330,-1);
  aArtillery[1]->SetRotation(20);
  aArtillery[2] = CreateObject(ATBY,5720,340,-1);
  aArtillery[2]->SetRotation(-30);
  aArtillery[3] = CreateObject(ATBY,7360,310,-1);
  aArtillery[3]->SetRotation(-30);

*/

}

func CreateDecoration()
{
  Log("$CreatingDecoration$");

  //Radio
  CreateObject(RDIO, 1460, 980, -1);

  //Labortische
  CreateObject(LTBL, 1450, 1110, -1);
  CreateObject(LTBL, 1620, 1110, -1);

  //Monitore
  CreateObject(MNI2, 1440, 1098, -1);
  CreateObject(MNI2, 1460, 1098, -1);
  CreateObject(MNI2, 1610, 1098, -1)->Off();
  CreateObject(MNI2, 1630, 1098, -1);

  //Bildschirm
  CreateObject(SCA2, 1535, 1095, -1);

  //Jetwrack
  CreateObject(_JW1, 5390, 1340, -1)->SetR(-20);
  CreateObject(_JW2, 5535, 1415, -1)->SetR(20);

  //Schiffswrack
  CreateObject(_WRK, 2670, 1450, -1);

  //Truhe
  CreateObject(CHST, 2715, 1450, -1);

  //Kaputtes Boot
  CreateObject(SLBB, 1025, 1485, -1)->SetR(40);

  //Holzhütten
  CreateObject(HTB2, 4610, 1075, -1)->SetR(2);
  CreateObject(HTB2, 4815, 1100, -1)->SetR(5);
  CreateObject(HTB2, 4885, 1170, -1)->SetR(-1);
  CreateObject(HTB2, 5065, 1170, -1)->SetR(5);
  CreateObject(HTB2, 5175, 1240, -1)->SetR(-2);

  //!!!
  /*

  //Regale
  CreateObject(FRAM, 4250, 580, -1);
  CreateObject(FRAM, 4270, 580, -1);

*/

  //Radare
  CreateObject(RADR, 1490, 980, -1);
  CreateObject(RADR, 1580, 980, -1)->SetPhase(10);
  CreateObject(RADR, 1758, 1030, -1)->SetClrModulation(RGB(125,125,125));
  CreateObject(RADR, 5535, 1230, -1);

  //!!!
/*

  //Schilder
  CreateObject(ESGN, 1065, 460, -1);
  CreateObject(MSGN, 2610, 410, -1);
  CreateObject(MSGN, 2620, 410, -1);
  CreateObject(MSGN, 2630, 410, -1);
  CreateObject(ESGN, 2960, 505, -1);
  CreateObject(ESGN, 5830, 505, -1);
  CreateObject(ESGN, 6200, 505, -1);
  CreateObject(MSGN, 6525, 410, -1);
  CreateObject(MSGN, 6535, 410, -1);
  CreateObject(MSGN, 6545, 410, -1);

  //Pflanzen
  CreateObject(PLNT, 1120, 525, -1);
  CreateObject(PLNT, 2715, 510, -1);

  //Apparaturen
  CreateObject(GADG, 1135, 540, -1)->Set(1);
  CreateObject(GADG, 4340, 580, -1)->Set(2);

*/

  //Orientierungslichter
  CreateObject(OLGH, 1310, 1140, -1)->Set(2, 10, 1, 1, 30);
  CreateObject(OLGH, 1540, 980, -1)->Set(4, 15, 1, 1, 30);
  CreateObject(OLGH, 1960, 1170, -1)->Set(4, 15, 1, 1, 30);
  CreateObject(OLGH, 2120, 1170, -1)->Set(4, 15, 1, 1, 30);
  CreateObject(OLGH, 2340, 1140, -1)->Set(2, 10, 1, 1, 30);

  //Büsche
  CreateObject(BSH2, 3430, 990, -1);
  CreateObject(BSH2, 3780, 1080, -1);
  CreateObject(BSH2, 3880, 1070, -1);
  CreateObject(BSH2, 3870, 720, -1);
  CreateObject(BSH2, 3980, 540, -1);
  CreateObject(BSH2, 4000, 540, -1);
  CreateObject(BSH2, 4020, 540, -1);
  CreateObject(BSH2, 4040, 540, -1);
  CreateObject(BSH2, 4300, 670, -1);
  CreateObject(BSH2, 4365, 1010, -1);
  CreateObject(BSH2, 4390, 1020, -1);
  CreateObject(BSH2, 4430, 1040, -1);
  CreateObject(BSH2, 4600, 1000, -1);
  CreateObject(BSH2, 4760, 1100, -1);

  //Steine
  CreateObject(STNE, 1140, 1420, -1);
  CreateObject(STNE, 1680, 1460, -1);
  CreateObject(STNE, 2895, 1370, -1);
  CreateObject(STNE, 3725, 820, -1)->Set(3);
  CreateObject(STNE, 3900, 980, -1);
  CreateObject(STNE, 4130, 1310, -1);
  CreateObject(STNE, 5770, 1480, -1);

  //!!!
/*

  //Monitor
  CreateObject(MNI2, 1095, 528, -1);

  //Spinde
  CreateObject(LCKR, 5705, 550, -1);
  CreateObject(LCKR, 5725, 550, -1);
  CreateObject(LCKR, 5745, 550, -1);

*/

  //Geländer
  CreateObject(RAI1, 1409, 1030, -1)->SetRail([3,3,3,3,3,3,3,3,1,3,1,3,1,3,1,3,2,2,2,3,1,3,1,3,1,3,1,3,3,3,3,3,3,3,3]);
  CreateObject(RAI1, 1265, 1140, -1)->SetRail([1,3,1,3,1]);
  CreateObject(RAI1, 2318, 1140, -1)->SetRail([1,3,1,3,1]);

  //!!!
/*

  //Zäune
  CreateObject(FENC, 2850, 410, -1);
  CreateObject(FENC, 4470, 390, -1)->Set(2);
  CreateObject(FENC, 4531, 390, -1)->Set(2);
  CreateObject(FENC, 6300, 410, -1);

  //Bildschirme
  CreateObject(SCR1, 2745, 470, -1);
  CreateObject(SCR3, 3555, 595, -1);
  CreateObject(SCA1, 4375, 480, -1);
  CreateObject(SCR1, 6405, 470, -1);
  CreateObject(SCR3, 7310, 410, -1);

  //Automaten
  CreateObject(VGMN, 1000, 540, -1);
  CreateObject(VGMN, 7210, 430, -1)->Set(3);

*/

  //Lüftungsgitter
  CreateObject(ENGT, 1360, 1270, -1);
  CreateObject(ENGT, 1800, 1270, -1);
  CreateObject(ENGT, 1955, 1270, -1);
  CreateObject(ENGT, 2115, 1270, -1);
  CreateObject(ENGT, 2295, 1270, -1);

  //Rohre
  CreateObject(PIPL, 1290, 1325, -1)->Up()->Up();
  CreateObject(PIPL, 1320, 1325, -1)->Up()->Up();
  CreateObject(PIPL, 1950, 1325, -1)->Up()->Up();
  CreateObject(PIPL, 1980, 1325, -1)->Up()->Up();
  CreateObject(PIPL, 2110, 1325, -1)->Up()->Up();
  CreateObject(PIPL, 2140, 1325, -1)->Up()->Up();

  //!!!
/*

  //Topfpflanze
  CreateObject(PLT2, 1030, 540, -1);

  //Tische
  CreateObject(GTBL, 1105, 540, -1);

*/

}

func CreateOptionalFeatures()
{
  //Hintergründe
  CreateObject(BD03,500,250,-1);
  CreateObject(BD03,2000,350,-1);
  CreateObject(BD03,3500,200,-1);
}










