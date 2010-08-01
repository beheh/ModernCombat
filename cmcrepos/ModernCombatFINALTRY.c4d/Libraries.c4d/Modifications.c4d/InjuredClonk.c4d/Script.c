/*-- Schwerverletzter --*/

#strict

static const FKDT_SuicideTime = 15; //Standardzeit bei Fake Death

local clonk,oldvisrange,oldvisstate,suicide,szKillstr;

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
  //Evtl. Granaten holen
  pClonk->~GrabGrenades(this);
  //Objekte des Clonks aufnehmen
  GrabContents(pClonk);
  
  //Aussehen des Clonks imitieren
  SetGraphics(0,this(),GetID(pClonk),1,GFXOV_MODE_Object,0,0,pClonk);

  //Sichtwerte speichern
  oldvisrange = GetObjPlrViewRange(pClonk);
  oldvisstate = GetPlrFogOfWar(GetOwner(pClonk));

  //Sichtwerte f�r den FakeDeath setzen
  SetPlrViewRange(100,pClonk);
  SetFoW(true,GetOwner(pClonk)); 

  //Lebenszeit setzen
  suicide = FKDT_SuicideTime + 1;

  //Etwas Lebensenergie
  DoEnergy(10, pClonk);

  //Soundloop starten
  Sound("FKDT_ClonkDown.ogg", false, pClonk, 100, GetOwner(pClonk)+1, +1);

  //Verz�gert Auswahlmen� �ffnen
  DoMenu();
  ScheduleCall(this(),"DoMenu",35,suicide);
}

/* Auswahlmen� */

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

  //Men� erstellen
  CreateMenu(FKDT, clonk, this(), 0, Format("$Title$"), C4MN_Style_Dialog, true);	//Titelzeile
  if(FindObject(SICD))
  {
  	if(suicide != FKDT_SuicideTime)
    {
    	AddMenuItem("$Suicide$", "Suicide", ICN2, clonk, 0, 0, "$SuicideDesc$"); //Selbstmord
    }
    else
    {
    	AddMenuItem("<c 777777>$Suicide$</c>", "", ICN2, clonk, 0, 0, "$SuicideDesc$");	//Kein Selbstmord
    }
    AddMenuItem(" ","", NONE,clonk, 0, 0, "", 512, 0, 0);
 		//AddMenuItem(Format("$Info$", GetName(clonk)),"", NONE, clonk, 0, 0, "", 512, 0, 0);	//Hinweise
    AddMenuItem(Format("$DeathCounter$", suicide),"", NONE, clonk, 0, 0, "", 512, 0, 0);	//Zeit bis zum Tod
  }
  else
  {
    //AddMenuItem(Format("$Info2$", GetName(clonk)),"", NONE, clonk, 0, 0, "", 512, 0, 0);//Falls kein Selbstmord m�glich
    AddMenuItem(Format("$DeathCounter$", suicide),"", NONE, clonk, 0, 0, "", 512, 0, 0);	//Zeit bis zum Tod
  }
  AddMenuItem(" ","", NONE,clonk, 0, 0, "", 512, 0, 0);
  AddMenuItem("$Killer$", "", NONE, clonk, 0, 0, "", 512, 0, 0);
  AddMenuItem(Format("%s", szKillstr), "", NONE, clonk, 0, 0, "", 512, 0, 0);
  if(FindObject(RWDS)) {
	  AddMenuItem(" ","", NONE,clonk, 0, 0, "", 512, 0, 0);
	  AddMenuItem("$Points$", "", NONE, clonk, 0, 0, "", 512, 0, 0);
  }
  if(suicide <= 0)
   Suicide();
  
  if(selection >= 0) SelectMenuItem(selection, clonk);
}

/* Selbstmord */

public func Update() {
	if(!GetAlive(clonk)) {
		Suicide();
	}
}

public func Suicide()
{
  //Clonkinventar l�schen sofern Arenaregel aktiv
  if(FindObject(NODR))
   for(var item in FindObjects(Find_Container(this),Find_Not(Find_OCF(OCF_Alive))))
    RemoveObject(item);

    if(clonk)
    {
      //T�ten
      if(GetAlive(clonk)) clonk->Kill();

      //Leiche "auswerfen" und ausfaden lassen
      clonk->Exit(0,0,GetObjHeight(clonk)/2);
      clonk->FadeOut();
    }

  //Verschwinden
  RemoveObject();
}

public func GetClonk()	{return(clonk);}

/* Zerst�rung */

public func Destruction()
{
  while(Contents())
  {
    RemoveObject(Contents(), false);
  }

  //Soundloop beenden
  Sound("FKDT_ClonkDown.ogg", false, clonk, 100, GetOwner(clonk)+1, -1);
}

/* Wiederbelebung */

public func Reanimation()
{
  //Kein Clonk?
  if(!clonk) return();

  //Clonk "auswerfen"
  if(Contained(clonk) == this())
   clonk->Exit(0,0,GetObjHeight(clonk)/2);

  //Besitzt�mer weitergeben
  if(GetAlive(clonk))
  {
  	clonk->GrabContents(this);
  	for(var item in FindObjects(Find_Container(clonk), Find_Func("IsGrenade"))) {
	  	clonk->~StoreGrenade(item);
	  	if(!Contained(item)) RemoveObject(item);
    }
    clonk->RemoveEffect("NoAnnounce", clonk);
  }
  else
  {
    while(Contents())
    {
      Exit(Contents(),0,+10);
    }
  }

  //Sichtdaten zur�cksetzen
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
  //Herausnehmen per Graben: Holen-Men� �ffnen
  pCaller->SetCommand(0, "Get", this(), 0, 0, 0, 1);
}

/* Abfrage */

public func GetDeathCountdown()
{
  return(suicide);
}
