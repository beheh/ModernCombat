/*-- Schwerverletzter --*/

#strict 2

static const FKDT_SuicideTime = 15; //Standardzeit bei Fake Death

local clonk,oldvisrange,oldvisstate,suicide,killmsg,szTipp,idTipp;

public func AimAngle()		{}
public func ReadyToFire()	{}
public func IsAiming()		{}
public func MenuQueryCancel()	{return true;}


/* Initialisierung */

protected func Initialize()
{
  //Anderer Todesschrei zur Unterscheidung zwischen Fake Death und "echtem" Ableben
  Sound("ClonkDie*.ogg");
  _inherited();
}

/* Erstellung */

public func Set(object pClonk)
{
  clonk = pClonk;
  SetPosition(GetX(pClonk),GetY(pClonk));
  SetXDir(GetXDir(pClonk));
  SetYDir(GetYDir(pClonk));

  //Reanimationszeichen erstellen
  CreateObject(SM01,0,0,GetOwner(pClonk))->Set(this);

  //CTF-Flagge entfernen
  for(var content in FindObjects(Find_ActionTarget(pClonk),Find_ID(FLA2)))
    if(GetID(content) == FLA2)
      content->~AttachTargetLost();

  //Clonk aufnehmen
  Enter(this, pClonk);
  //Evtl. Granaten holen
  pClonk->~GrabGrenades(this);
  //Objekte des Clonks aufnehmen
  GrabContents(pClonk);

  //Aussehen des Clonks imitieren
  SetGraphics(0,this,GetID(pClonk),1,GFXOV_MODE_Object,0,0,pClonk);

  //Sichtwerte speichern
  oldvisrange = GetObjPlrViewRange(pClonk);
  oldvisstate = GetPlrFogOfWar(GetOwner(pClonk));

  //Sichtwerte für den FakeDeath setzen
  SetPlrViewRange(100,pClonk);
  SetFoW(true,GetOwner(pClonk)); 

  //Lebenszeit setzen
  suicide = FKDT_SuicideTime + 1;

  //Etwas Lebensenergie
  DoEnergy(10, pClonk);

  //Soundloop starten
  Sound("FKDT_ClonkDown.ogg", false, pClonk, 100, GetOwner(pClonk)+1, +1);

  //Verzögert Auswahlmenü öffnen
  DoMenu();
  ScheduleCall(this,"DoMenu",35,suicide);
}

public func KillMessage(string msg)
{
  //Killnachricht setzen
  killmsg = msg;

  //Spieler hat Hilfen aktiviert: Quicktipp geben
  if (clonk && !GetPlrExtraData(GetOwner(clonk), "Hazard_NoHelpMsg"))
  {
    var array = GetQuickTipp(this);
      idTipp = array[0];
    szTipp = array[1];
  }

  DeathMenu();
}

/* Auswahlmenü */

func DoMenu()
{
  suicide--;
  DeathMenu();
}

private func DeathMenu()
{
  Update();

  var selection = GetMenuSelection(clonk);

  CloseMenu(clonk);

  if(GetMenu(clonk)) return;

  //Menü erstellen
  CreateMenu(FKDT, clonk, this, 0, Format("$Title$"), C4MN_Style_Dialog, true);			//Titelzeile
  if(FindObject(SICD))										//Selbstmord möglich?
  {
    AddMenuItem(Format("$Info$", GetName(clonk)),"", NONE, clonk, 0, 0, "", 512, 0, 0);		//Hinweis
    AddMenuItem(Format("$DeathCounter$", suicide),"", NONE, clonk, 0, 0, "", 512, 0, 0);	//Counter
    if(suicide != FKDT_SuicideTime)
    {AddMenuItem("$Suicide$", "Suicide", SM06, clonk, 0, 0, "$SuicideDesc$");}			//Selbstmordbutton
  }
  else
  {
    AddMenuItem(Format("$Info2$", GetName(clonk)),"", NONE, clonk, 0, 0, "", 512, 0, 0);	//Alternativcounter
    AddMenuItem(Format("$DeathCounter$", suicide),"", NONE, clonk, 0, 0, "", 512, 0, 0);	//Counter
  }

  if (GetType(killmsg) == C4V_String)
  {
    AddMenuItem("", "", NONE, clonk, 0, 0, "", 512, 0, 0);		//Leerzeile
    AddMenuItem(Format("$Killer$", GetName(clonk)),"", NONE, clonk, 0, 0, "", 512, 0, 0);	//Titel
    AddMenuItem(killmsg, "", NONE, clonk, 0, 0, "", 512);					//Killerinformationen
  }

  if (szTipp)
  {
    AddMenuItem("", "", NONE, clonk, 0, 0, "", 512, 0, 0);					//Leerzeile
    AddMenuItem(Format("{{%i}} $Tip$", idTipp),"", NONE, clonk, 0, 0, "", 512, 0, 0);		//Zufälliger Tipp
    AddMenuItem(szTipp,"", NONE, clonk, 0, 0, "", 512, 0, 0);
  }

  var obj;
  if (obj = FindObject(RWDS))									//Punktestatistik erstellen
  {
    AddMenuItem("", "", NONE, clonk, 0, 0, "", 512, 0, 0);					//Leerzeile
    AddMenuItem(Format("$Points$", GetName(clonk)),"", NONE, clonk, 0, 0, "", 512, 0, 0);	//Titel
    //Einsortieren
    var aList = [], iPlr, aData = obj->~GetData(), szString = "";
    while(aData[iPlr] != 0)
    {
      var iTeam = obj->~GetPlayerData(RWDS_PlayerTeam, iPlr);
      if(!aList[iTeam]) aList[iTeam] = [];
      szString = Format("%s: %d", obj->~GetPlayerData(RWDS_PlayerName, iPlr), obj->~GetPlayerPoints(RWDS_TotalPoints, iPlr));
      aList[iTeam][GetLength(aList[iTeam])] = szString;
      iPlr++;
    }
    //Teamweise ausgeben
    for (var aTeam in aList)
      if (aTeam)
        for (var szString in aTeam)
          if (GetType(szString) == C4V_String)
            AddMenuItem(szString, "", NONE, clonk, 0, 0, "", 512);
  }

  if(suicide <= 0)
    Suicide();

  if(selection >= 0) SelectMenuItem(selection, clonk);
}

private func GetQuickTipp(object pFake)
{
  //Standard-Tipp
  if (!Random(8) || !ContentsCount(0, pFake))
    return GetGeneralTipp();
  //Sonst Tipps zu Inventarobjekten
  var array = [], id = [], tipp;
    for (var obj in FindObjects(Find_Container(pFake)))
    {
      //Hat schon so einen Tipp
      if (GetIndexOf(GetID(obj), id) != -1 || !(tipp = GetRandomTipp(0, GetID(obj))))
        continue;
      //Tipp hinzufügen
      id[GetLength(id)] = GetID(obj);
      array[GetLength(array)] = tipp;
    }
  //Keine Tipps
  if (!array || GetType(array) != C4V_Array || !GetLength(array))
    return GetGeneralTipp();
  return GetRandomTipp(array);
}

private func GetGeneralTipp()
{
  return GetRandomTipp([[FGRN, "$NONE0$"], [CSTR, "$NONE1$"], [SM05, "$NONE2$"], [SM04, "$NONE3$"], [XBRL, "$NONE4$"], [PCMK, "$NONE5$"]]);
}

private func GetRandomTipp(array a, id id)
{
  if (a)
    return a[Random(GetLength(a))];

  //Waffen
  if (id == ASTR) return GetRandomTipp([[ASTR, "$ASTR0$"], [ASTR, "$ASTR1$"]]);
  if (id == MNGN) return GetRandomTipp([[MNGN, "$MNGN0$"]]);
  if (id == PSTL) return GetRandomTipp([[PSTL, "$PSTL0$"], [PSTL, "$PSTL1$"]]);
  if (id == RTLR) return GetRandomTipp([[MISL, "$RTLR0$"], [RTLR, "$RTLR1$"], [MISL, "$RTLR2$"]]);
  if (id == PPGN) return GetRandomTipp([[PPGN, "$PPGN0$"]]);
  if (id == SGST) return GetRandomTipp([[SGST, "$SGST0$"]]);
  if (id == SMGN) return GetRandomTipp([[SMGN, "$SMGN0$"]]);

  //Granaten
  if (id->~IsGrenade() && !Random(6)) return GetRandomTipp([[BOOM, "$NADE0$"]]);
  if (id == FGRN) return GetRandomTipp([[FGRN, "$FGRN0$"]]);
  if (id == FRAG) return GetRandomTipp([[FRAG, "$FRAG0$"], [SHRP, "$FRAG1$"]]);
  if (id == PGRN) return GetRandomTipp([[PGRN, "$PGRN0$"]]);
  if (id == STUN) return GetRandomTipp([[STUN, "$STUN0$"], [STUN, "$STUN1$"]]);
  if (id == SGRN) return GetRandomTipp([[SGRN, "$SGRN0$"]]);

  //Equipment
  if (id == AMPK) return GetRandomTipp([[AMPK, "$AMPK0$"]]);
  if (id == BBTP) return GetRandomTipp([[BBTP, "$BBTP0$"], [BBTP, "$BBTP1$"], [BBTP, "$BBTP2$"]]);
  if (id == C4PA) return GetRandomTipp([[C4PA, "$C4PA0$"], [C4PA, "$C4PA1$"]]);
  if (id == DRGN) return GetRandomTipp([[DRGN, "$DRGN0$"], [DRGN, "$DRGN1$"]]);
  if (id == FAPK) return GetRandomTipp([[FAPK, "$FAPK0$"], [FAPK, "$FAPK1$"], [FAPK, "$FAPK2$"]]);
  if (id == RSHL) return GetRandomTipp([[RSHL, "$RSHL0$"], [RSHL, "$RSHL1$"], [RSHL, "$RSHL2$"], [RSHL, "$RSHL3$"]]);
  if (id == CDBT) return GetRandomTipp([[CDBT, "$CDBT0$"], [CDBT, "$CDBT1$"]]);
  if (id == CUAM) return GetRandomTipp([[CUAM, "$CUAM0$"]]);
}

/* Selbstmord */

public func Update()
{
  if(!GetAlive(clonk))
  {Suicide();}
}

public func Suicide()
{
  //Clonkinventar löschen sofern Arenaregel aktiv
  if(FindObject(NODR))
    for(var item in FindObjects(Find_Container(this),Find_Not(Find_OCF(OCF_Living))))
      RemoveObject(item);

  if(clonk)
  {
    //Töten
    if(GetAlive(clonk)) Kill(clonk);

    //Leiche "auswerfen" und ausfaden lassen
    Exit(clonk,0,GetObjHeight(clonk)/2);
    clonk->InstantDie();
    clonk->FadeOut();
  }

  //Verschwinden
  RemoveObject();
}

public func GetClonk()		{return clonk;}

/* Zerstörung */

public func Destruction()
{
  while(Contents())
  {
    RemoveObject(Contents());
  }

  //Soundloop beenden
  Sound("FKDT_ClonkDown.ogg", false, clonk, 100, GetOwner(clonk)+1, -1);
}

/* Wiederbelebung */

public func Reanimation()
{
  //Kein Clonk?
  if(!clonk) return;

  //Clonk "auswerfen"
  if(Contained(clonk) == this)
   Exit(clonk,0,GetObjHeight(clonk)/2);

  //Besitztümer weitergeben
  if(GetAlive(clonk))
  {
    clonk->GrabContents(this);
    for(var item in FindObjects(Find_Container(clonk), Find_Func("IsGrenade")))
    {
      clonk->~StoreGrenade(item);
      if(!Contained(item)) RemoveObject(item);
    }
    RemoveEffect("NoAnnounce", clonk);
  }
  else
  {
    while(Contents())
    {
      Exit(Contents(),0,+10);
    }
  }

  //Sichtdaten zurücksetzen
  SetFoW(oldvisstate,GetOwner(clonk));
  SetPlrViewRange(oldvisrange,clonk);

  RemoveObject();
}

public func RejectCollect(id idObj, object pObj)
{
  if(!clonk) return;
  var val = clonk->~RejectCollect(idObj,pObj);
  return val;
}

public func ControlDig(object pCaller)
{
  if(pCaller == clonk) return 1;
  //Herausnehmen per Graben: Holen-Menü öffnen
  SetCommand(pCaller, "Get", this, 0, 0, 0, 1);
}

/* Abfrage */

public func GetDeathCountdown()
{
  return suicide;
}