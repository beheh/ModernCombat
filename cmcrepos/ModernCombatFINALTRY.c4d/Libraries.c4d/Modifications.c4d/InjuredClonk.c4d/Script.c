/*-- Schwerverletzter --*/

#strict

static const FKDT_SuicideTime = 15; //Standardzeit bei Fake Death

local clonk,oldvisrange,oldvisstate,suicide, mediccalls;

public func AimAngle()     {return();}
public func ReadyToFire()  {return();}
public func IsAiming()     {return();}


/* Initalisierung */

protected func Initialize()
{
  //Anderer Todesschrei zur Unterscheidung zwischen Fake Death und "echtem" Ableben
  Sound("ClonkDie*.ogg");

  //Anzahl Rufe
  mediccalls = 2;
  _inherited();
}

/* Erstellung */

public func Set(object pClonk)
{
  clonk = pClonk;
  SetPosition(GetX(pClonk),GetY(pClonk)/*GetObjHeight(pClonk)/2*/);
  SetXDir(GetXDir(pClonk));
  SetYDir(GetYDir(pClonk));

  //Clonk aufnehmen
  pClonk->Enter(this());
  //Objekte des Clonks aufnehmen
  GrabContents(pClonk);
  //Aussehen des Clonks imitieren
  SetGraphics (0,this(),GetID(pClonk),1,GFXOV_MODE_Object,0,0,pClonk);

  //Sichtwerte speichern
  oldvisrange = GetObjPlrViewRange(pClonk);
  oldvisstate = GetPlrFogOfWar(GetOwner(pClonk));

  //Sichtwerte für den Fake Death setzen
  SetPlrViewRange(100,pClonk);
  SetFoW(true,GetOwner(pClonk)); 

  //Lebenszeit setzen
  suicide = FKDT_SuicideTime;

  //Etwas Lebensenergie
  DoEnergy(10, pClonk);

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

  //Menü erstellen
  CreateMenu (FKDT, clonk, this(), 0, Format("$Title$"), C4MN_Style_Dialog, true);//Titelzeile
  if(!FindObject(NOSC))
   AddMenuItem("$Suicide$", "Suicide", ICN2, clonk, 0, 0, "$SuicideDesc$");//Selbstmord
  //if(mediccalls)
  // AddMenuItem("$CallMedic$", "MedicCall", ICN3, clonk);//Nach Sanitäter rufen
  //else
  //{
   AddMenuItem(Format("$CantCallMedic$",RGB(128,128,128),suicide), 0, ICN3, clonk);
  //}
  AddMenuItem(" ","", NONE,clonk, 0, 0, "", 512, 0, 0);					//Leerzeile
  if(!FindObject(NOSC))
   AddMenuItem(Format("$Info$", GetName(clonk)),"", NONE, clonk, 0, 0, "", 512, 0, 0);	//Hinweise
  else
  {
   AddMenuItem(Format("$Info2$", GetName(clonk)),"", NONE, clonk, 0, 0, "", 512, 0, 0);
  }
  AddMenuItem(Format("$DeathCounter$", suicide),"", NONE, clonk, 0, 0, "", 512, 0, 0);	//Zeit bis zum Tod

  if(suicide <= 0)
    Suicide();

  SetMenuTextProgress(1, clonk); 
}

public func MenuQueryCancel() { return(true); }

/* Selbstmord */

public func Suicide()
{
  //Eventuelles Hinweisicon entfernen
  for(var icons in FindObjects(Find_ID(ICN3), Find_Owner(GetOwner(clonk))))
   RemoveObject(icons);

  //Ende im Gelände 
  clonk->Kill(); 
 
  //Leiche "auswerfen" und ausfaden lassen 
  clonk->Exit(0,0,GetObjHeight(clonk)/2);
  clonk -> FadeOut();

  //Clonkinventar löschen sofern Arenaregel aktiv
  if(FindObject(GCTF))
  for(var item in FindObjects(Find_Container(this)))
   RemoveObject(item);

  //Verschwinden
  RemoveObject();
}

public func GetClonk() {return(clonk);}

/* Nach Sanitäter rufen */

private func MedicCall()
{
  //LOL
  Sound("Lol.ogg");

  //Hinweisicon erstellen
  var icon = CreateObject(ICN2, 0, -10, GetOwner(this));
  FadeOutIcon(icon, 0);

  //Ein Ruf weniger
  return(mediccalls--);
}

private func FadeOutIcon(object pItem, int iRepeat)
{
  if(iRepeat >= 256)
  {
   RemoveObject(pItem);
   return;
  }
  SetClrModulation(RGBa(255,255,255,iRepeat), pItem);
  ScheduleCall(0, "FadeOutIcon", 1, 0, pItem, iRepeat+3);
}

/* Entfernen (bei Wiederbelebung) */

public func Destruction()
{
  //Kein Clonk?
  if(!clonk) return();

  //Clonk "auswerfen"
  if(Contained(clonk) == this())
   clonk->Exit(0,0,GetObjHeight(clonk)/2);

  //Besitztümer weitergeben
  if(GetAlive(clonk))
   clonk->GrabContents(this());
  else
   while(Contents())
    Exit(Contents(),0,+10);

  //Sichtdaten zurücksetzen
  SetFoW(oldvisstate,GetOwner(clonk));
  SetPlrViewRange(oldvisrange,clonk);
}

public func RejectCollect(id idObj, object pObj)
{
  if(!clonk) return();
  var val = clonk->~RejectCollect(idObj,pObj);
  if(val)  Message("RejectCollect",this());
  return(val);
}

public func ControlDig(object pCaller)
{
  if(pCaller == clonk) return(1);
  SetCommand(pCaller,"Context",0,0,0,this());
  return(1);
}