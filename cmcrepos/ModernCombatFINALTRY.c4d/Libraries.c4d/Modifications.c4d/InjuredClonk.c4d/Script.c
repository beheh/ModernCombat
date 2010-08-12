/*-- Schwerverletzter --*/

#strict 2

static const FKDT_SuicideTime = 15; //Standardzeit bei Fake Death

local clonk,oldvisrange,oldvisstate,suicide,killmsg;

public func AimAngle()		{}
public func ReadyToFire()	{}
public func IsAiming()		{}
public func MenuQueryCancel()	{return true;}


/* Initalisierung */

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
  CreateObject(_IN3,0,0,GetOwner(pClonk))->Set(this);
  
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

public func KillMessage(string msg) {
  killmsg = msg;
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
    {AddMenuItem("$Suicide$", "Suicide", ICN2, clonk, 0, 0, "$SuicideDesc$");}			//Selbstmordbutton
    else
    {AddMenuItem("<c 777777>$Suicide$</c>", "", ICN2, clonk, 0, 0, "$SuicideDesc$");}		//Kein Selbstmordbutton
  }
  else
  {
    AddMenuItem(Format("$Info2$", GetName(clonk)),"", NONE, clonk, 0, 0, "", 512, 0, 0);	//Alternativcounter
    AddMenuItem(Format("$DeathCounter$", suicide),"", NONE, clonk, 0, 0, "", 512, 0, 0);	//Counter
  }

  if (GetType(killmsg) == C4V_String)
    AddMenuItem(Format("", GetName(clonk)),"", NONE, clonk, 0, 0, "", 512, 0, 0);		//Leerzeile
    AddMenuItem(Format("$Killer$", GetName(clonk)),"", NONE, clonk, 0, 0, "", 512, 0, 0);	//Titel
    AddMenuItem(killmsg, "", NONE, clonk, 0, 0, "", 512);					//Killerinformationen

  var obj;
  if (obj = FindObject(RWDS))									//Punktestatistik erstellen
  {
   AddMenuItem(Format("", GetName(clonk)),"", NONE, clonk, 0, 0, "", 512, 0, 0);		//Leerzeile
   AddMenuItem(Format("$Points$", GetName(clonk)),"", NONE, clonk, 0, 0, "", 512, 0, 0);	//Titel
   //Einsortieren
   var aList = [], iPlr, aData = obj->~GetData(), szString = "";
   while(aData[iPlr] != 0) {
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
   for(var item in FindObjects(Find_Container(this),Find_Not(Find_OCF(OCF_Alive))))
    RemoveObject(item);

    if(clonk)
    {
      //Töten
      if(GetAlive(clonk)) Kill(clonk);

      //Leiche "auswerfen" und ausfaden lassen
      Exit(clonk,0,GetObjHeight(clonk)/2);
      clonk->FadeOut();
    }

  //Verschwinden
  RemoveObject();
}

public func GetClonk()	{return clonk;}

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
  	for(var item in FindObjects(Find_Container(clonk), Find_Func("IsGrenade"))) {
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