/*-- Schwerverletzter --*/

#strict

static const FKDT_SuicideTime = 15; //Standardzeit bei Fake Death

local clonk,oldvisrange,oldvisstate,suicide;

public func AimAngle()		{return();}
public func ReadyToFire()	{return();}
public func IsAiming()		{return();}
public func MenuQueryCancel()	{return(true);}

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
  CreateObject(_IN3,0,0,GetOwner(pClonk))->Set(this());
  
  //CTF-Flagge entfernen
  for(var content in FindObjects(Find_ActionTarget(pClonk),Find_ID(FLA2)))
   if(GetID(content) == FLA2)
    content->~AttachTargetLost();

  //Clonk aufnehmen
  pClonk->Enter(this());
  //Objekte des Clonks aufnehmen
  GrabContents(pClonk);
  
  //Aussehen des Clonks imitieren
  SetGraphics (0,this(),GetID(pClonk),1,GFXOV_MODE_Object,0,0,pClonk);

  //Sichtwerte speichern
  oldvisrange = GetObjPlrViewRange(pClonk);
  oldvisstate = GetPlrFogOfWar(GetOwner(pClonk));

  //Sichtwerte für den FakeDeath setzen
  SetPlrViewRange(100,pClonk);
  SetFoW(true,GetOwner(pClonk)); 

  //Lebenszeit setzen
  suicide = FKDT_SuicideTime;

  //Etwas Lebensenergie
  DoEnergy(10, pClonk);

  //Soundloop starten
  Sound("FKDT_ClonkDown.ogg", false, pClonk, 100, GetOwner(pClonk)+1, +1);

  //Verzögert Auswahlmenü öffnen
  ScheduleCall(this(),"DoMenu",35,suicide);
}

/* Auswahlmenü */

func DoMenu()
{
  suicide--;
  DeathMenu();
}

private func DeathMenu()
{
  var selection = GetMenuSelection(clonk);

  CloseMenu(clonk);

	if(GetMenu(clonk)) return;

  //Menü erstellen
  CreateMenu(FKDT, clonk, this(), 0, Format("$Title$"), C4MN_Style_Dialog, true);	//Titelzeile
  if(FindObject(SICD))
  {
   AddMenuItem("$Suicide$", "Suicide", ICN2, clonk, 0, 0, "$SuicideDesc$");		//Selbstmord
   AddMenuItem(" ","", NONE,clonk, 0, 0, "", 512, 0, 0);				//Leerzeile
   AddMenuItem(Format("$Info$", GetName(clonk)),"", NONE, clonk, 0, 0, "", 512, 0, 0);	//Hinweise
  }
  else
  {
   AddMenuItem(Format("$Info2$", GetName(clonk)),"", NONE, clonk, 0, 0, "", 512, 0, 0);	//Falls kein Selbstmord möglich
  }
  AddMenuItem(Format("$DeathCounter$", suicide),"", NONE, clonk, 0, 0, "", 512, 0, 0);	//Zeit bis zum Tod
  if(suicide <= 0)
   Suicide();
  
  if(selection >= 0) SelectMenuItem(selection, clonk);
  //SetMenuTextProgress(1, clonk); 
}

/* Selbstmord */

public func Suicide()
{
  //Clonkinventar löschen sofern Arenaregel aktiv
  if(FindObject(NODR))
   for(var item in FindObjects(Find_Container(this),Find_Not(Find_OCF(OCF_Alive))))
    RemoveObject(item);

	if(clonk) {
		//Ende im Gelände 
		clonk->Kill();
	 
		//Leiche "auswerfen" und ausfaden lassen 
		if(clonk) clonk->Exit(0,0,GetObjHeight(clonk)/2);
		if(clonk) clonk->FadeOut();
  }

  //Verschwinden
  RemoveObject(0,1);
}

public func GetClonk()	{return(clonk);}

/* Zerstörung */

public func Destruction() {
	while(Contents()) {
		RemoveObject(Contents(), false);
	}
}

/* Wiederbelebung */

public func Reanimation()
{
  //Kein Clonk?
  if(!clonk) return();

  //Soundloop beenden
  Sound("FKDT_ClonkDown.ogg", false, clonk, 100, GetOwner(clonk)+1, -1);

  //Clonk "auswerfen"
  if(Contained(clonk) == this())
   clonk->Exit(0,0,GetObjHeight(clonk)/2);

  //Besitztümer weitergeben
  if(GetAlive(clonk)) {
		clonk->GrabContents(this());
		clonk->RemoveEffect("NoAnnounce", clonk);
  }
  else {
		while(Contents()) {
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
  if(!clonk) return();
  var val = clonk->~RejectCollect(idObj,pObj);
  return(val);
}

public func ControlDig(object pCaller)
{
  if(pCaller == clonk) return(1);
  //Herausnehmen per Graben: Holen-Menü öffnen
  pCaller->SetCommand(0, "Get", this(), 0, 0, 0, 1);
}

/* Abfrage */

public func GetDeathCountdown()
{
  return(suicide);
}
